#ifndef __LIBRARY_MALLOC_H
#define __LIBRARY_MALLOC_H

VOID *malloc(UINTN size);
VOID *malloc_rt(UINTN Size);
VOID *memalign(UINTN boundary, UINTN size);
VOID *memalign2(UINTN boundary, UINTN size, BOOLEAN runtime);
VOID *calloc(UINTN count, UINTN size);
VOID  free(VOID *ptr);

#endif
