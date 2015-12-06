#include "list_algos.h"

void List_node_swap_adj(List * list, ListNode *first, ListNode *second)
{
  if (list->first == first) {
    list->first = second;
  }

  if (list->last == second) {
    list->last = first;
  }

  if (first->prev) {
    first->prev->next = second;
  }

  if (second->next) {
    second->next->prev = first;
  }
  second->prev = first->prev;
  first->next = second->next;
}

int List_bubble_sort(List * list, List_compare cmp)
{
  int swapped;
  do {
    swapped = 0;
    LIST_FOREACH(list, first, next, cur) {
      if (cur->next && cmp(cur->value, cur->next->value) > 0) {
        List_node_swap_adj(list, cur, cur->next);
        swapped = 1;
      }
    };
  } while (swapped);
  return 0;
}

List *List_merge(List *left, List *right, List_compare cmp)
{
  List *merged = List_create();

  while (left->count > 0 && right->count > 0) {
    if (cmp(List_first(left), List_first(right)) <= 0) {
      List_push(merged, List_shift(left));
    } else {
      List_push(merged, List_shift(right));
    }
  }

  while (left->count > 0) {
    List_push(merged, List_shift(left));
  }
  while (right->count > 0) {
    List_push(merged, List_shift(right));
  }

  return merged;
}

List *List_merge_sort(List * list, List_compare cmp)
{
  if (list->count <= 1) {
    return list;
  }

  ListNode *split = List_first(list);
  for (int i = 0; i <= list->count / 2 + 1; i++) {
    split = split->next;
  }
  List *left = list;
  List *right = List_split(list, split);

  left = List_merge_sort(left, cmp);
  right = List_merge_sort(right, cmp);

  List *sorted = List_merge(left, right, cmp);

  List_clear_destroy(left);
  List_clear_destroy(right);

  return sorted;
}
