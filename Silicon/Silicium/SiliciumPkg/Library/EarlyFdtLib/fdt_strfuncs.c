#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>

unsigned long
strtoul (
  const char  *nptr,
  char        **endptr,
  int         base
  )
{
  RETURN_STATUS  Status;
  UINTN          ReturnValue;

  ASSERT (base == 10 || base == 16);

  if (base == 10) {
    Status = AsciiStrDecimalToUintnS (nptr, endptr, &ReturnValue);
  } else if (base == 16) {
    Status = AsciiStrHexToUintnS (nptr, endptr, &ReturnValue);
  } else {
    Status = RETURN_INVALID_PARAMETER;
  }

  if (RETURN_ERROR (Status)) {
    return MAX_UINTN;
  }

  return ReturnValue;
}

char *
strrchr (
  const char  *Str,
  int         Char
  )
{
  char  *S, *last;

  S    = (char *)Str;
  last = NULL;

  for ( ; ; S++) {
    if (*S == Char) {
      last = S;
    }

    if (*S == '\0') {
      return last;
    }
  }
}

