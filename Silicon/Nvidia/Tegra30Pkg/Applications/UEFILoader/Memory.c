// Copyright (c) 2019 - 2020, Bingxing Wang and other project authors. All rights reserved.<BR>
// Copyright (c) 2021 - 2021, Leander Wollersberger. All rights reserved.<BR>

// TODO: Rewrite this mess

#include "UEFILoader.h"

#include <Library/BaseMemoryLib.h>

#define memchr(buf, ch, count) ScanMem8(buf, (UINTN)(count), (UINT8)ch)
#define memcmp(buf1, buf2, count) (int)(CompareMem(buf1, buf2, (UINTN)(count)))

#define _MAX(a, b) ((a) > (b) ? (a) : (b))
#define _MIN(a, b) ((a) < (b) ? (a) : (b))

#define BITOP(a, b, op)                                                        \
  ((a)[(UINTN)(b) / (8 * sizeof *(a))] op(UINTN) 1                           \
   << ((UINTN)(b) % (8 * sizeof *(a))))

VOID
memcpy_usr(
  VOID       *dest,
  CONST VOID *src,
  UINTN       n)
{
  //cast src and dest to char*
  char *src_char = (char *)src;
  char *dest_char = (char *)dest;
  for (int i=0; i<n; i++) {
    dest_char[i] = src_char[i]; //copy contents byte by byte
  }
}

static char *
twobyte_memmem(const unsigned char *h, UINTN k, const unsigned char *n)
{
  UINT16 nw = n[0] << 8 | n[1], hw = h[0] << 8 | h[1];
  for (h += 2, k -= 2; k; k--, hw = hw << 8 | *h++)
    if (hw == nw)
      return (char *)h - 2;
  return hw == nw ? (char *)h - 2 : 0;
}

static char *
threebyte_memmem(const unsigned char *h, UINTN k, const unsigned char *n)
{
  UINT32 nw = n[0] << 24 | n[1] << 16 | n[2] << 8;
  UINT32 hw = h[0] << 24 | h[1] << 16 | h[2] << 8;
  for (h += 3, k -= 3; k; k--, hw = (hw | *h++) << 8)
    if (hw == nw)
      return (char *)h - 3;
  return hw == nw ? (char *)h - 3 : 0;
}

static char *
fourbyte_memmem(const unsigned char *h, UINTN k, const unsigned char *n)
{
  UINT32 nw = n[0] << 24 | n[1] << 16 | n[2] << 8 | n[3];
  UINT32 hw = h[0] << 24 | h[1] << 16 | h[2] << 8 | h[3];
  for (h += 4, k -= 4; k; k--, hw = hw << 8 | *h++)
    if (hw == nw)
      return (char *)h - 4;
  return hw == nw ? (char *)h - 4 : 0;
}

//
// Two Way string search algorithm, with a bad shift table applied to the last
// byte of the window. A bit array marks which entries in the shift table are
// initialized to avoid fully initializing a 1kb/2kb table.
//
// Reference: CROCHEMORE M., PERRIN D., 1991, Two-way string-matching,
// Journal of the ACM 38(3):651-675
//
static char *twoway_memmem(
    const unsigned char *h, const unsigned char *z, const unsigned char *n,
    UINTN l)
{
  UINTN i, ip, jp, k, p, ms, p0, mem, mem0;
  UINTN byteset[32 / sizeof(UINTN)] = {0};
  UINTN shift[256];

  // Computing length of needle and fill shift table 
  for (i = 0; i < l; i++)
    BITOP(byteset, n[i], |=), shift[n[i]] = i + 1;

  // Compute maximal suffix 
  ip = -1;
  jp = 0;
  k = p = 1;
  while (jp + k < l) {
    if (n[ip + k] == n[jp + k]) {
      if (k == p) {
        jp += p;
        k = 1;
      }
      else
        k++;
    }
    else if (n[ip + k] > n[jp + k]) {
      jp += k;
      k = 1;
      p = jp - ip;
    }
    else {
      ip = jp++;
      k = p = 1;
    }
  }
  ms = ip;
  p0 = p;

  // And with the opposite comparison 
  ip = -1;
  jp = 0;
  k = p = 1;
  while (jp + k < l) {
    if (n[ip + k] == n[jp + k]) {
      if (k == p) {
        jp += p;
        k = 1;
      }
      else
        k++;
    }
    else if (n[ip + k] < n[jp + k]) {
      jp += k;
      k = 1;
      p = jp - ip;
    }
    else {
      ip = jp++;
      k = p = 1;
    }
  }
  if (ip + 1 > ms + 1)
    ms = ip;
  else
    p = p0;

  // Periodic needle? 
  if (memcmp(n, n + p, ms + 1)) {
    mem0 = 0;
    p    = _MAX(ms, l - ms - 1) + 1;
  }
  else
    mem0 = l - p;
  mem = 0;

  // Search loop 
  for (;;) {
    // If remainder of haystack is shorter than needle, done 
    if (z - h < l)
      return 0;

    // Check last byte first; advance by shift on mismatch 
    if (BITOP(byteset, h[l - 1], &)) {
      k = l - shift[h[l - 1]];
      if (k) {
        if (mem0 && mem && k < p)
          k = l - p;
        h += k;
        mem = 0;
        continue;
      }
    }
    else {
      h += l;
      mem = 0;
      continue;
    }

    // Compare right half 
    for (k = _MAX(ms + 1, mem); k < l && n[k] == h[k]; k++)
      ;
    if (k < l) {
      h += k - ms;
      mem = 0;
      continue;
    }
    // Compare left half 
    for (k = ms + 1; k > mem && n[k - 1] == h[k - 1]; k--)
      ;
    if (k <= mem)
      return (char *)h;
    h += p;
    mem = mem0;
  }
}

void *memmem(const void *h0, UINTN k, const void *n0, UINTN l)
{
  const unsigned char *h = h0, *n = n0;

  // Return immediately on empty needle 
  if (!l)
    return (void *)h;

  // Return immediately when needle is longer than haystack 
  if (k < l)
    return 0;

  // Use faster algorithms for short needles 
  h = memchr(h0, *n, k);
  if (!h || l == 1)
    return (void *)h;
  k -= h - (const unsigned char *)h0;
  if (k < l)
    return 0;
  if (l == 2)
    return twobyte_memmem(h, k, n);
  if (l == 3)
    return threebyte_memmem(h, k, n);
  if (l == 4)
    return fourbyte_memmem(h, k, n);

  return twoway_memmem(h, h + k, n, l);
}
