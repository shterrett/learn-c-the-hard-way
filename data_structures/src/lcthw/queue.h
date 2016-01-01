#ifndef _queue_
#define _queue_

#include "list.h"

typedef List Queue;

#define Queue_create() List_create()
#define Queue_destroy(q) List_clear_destroy((q))
#define Queue_send(q, v) List_push((q), (v))
#define Queue_recv(q) List_shift((q))
#define Queue_peek(q) List_first((q))
#define Queue_count(q) List_count((q))

#define QUEUE_FOREACH(q, cur) LIST_FOREACH((q), first, next, (cur))

#endif
