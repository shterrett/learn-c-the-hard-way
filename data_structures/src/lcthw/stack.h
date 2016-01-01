#ifndef _stack_
#define _stack_

#include "darray.h"

typedef DArray Stack;

#define Stack_create() DArray_create(sizeof(void *), 100)
#define Stack_destroy(s) DArray_destroy((s))
#define Stack_push(s, v) DArray_push((s), (v))
#define Stack_pop(s) DArray_pop((s))
#define Stack_peek(s) DArray_last((s))
#define Stack_count(s) DArray_count((s))

#define STACK_FOREACH(stack, cur) void *cur = NULL; int _s_idx = 0; \
  for(_s_idx = Stack_count(stack), cur = DArray_get(stack, _s_idx); \
      _s_idx >= 0; \
      _s_idx--)

#endif
