#include "list.h"

typedef int (*List_compare)(const void *a, const void *b);

extern int List_bubble_sort(List * list, List_compare cmp);
extern List *List_merge_sort(List * list, List_compare cmp);
