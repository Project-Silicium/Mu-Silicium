#ifndef _MALLOC_LIB_H_
#define _MALLOC_LIB_H_

VOID *malloc    (UINTN size);
VOID *malloc_rt (UINTN Size);
VOID *memalign  (UINTN boundary, UINTN size);
VOID *memalign2 (UINTN boundary, UINTN size, BOOLEAN runtime);
VOID *calloc    (UINTN count, UINTN size);
VOID  free      (VOID *ptr);

#endif /* _MALLOC_LIB_H_ */