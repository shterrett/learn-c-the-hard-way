#include "list.h"
#include "dbg.h"

List *List_create()
{
  return calloc(1, sizeof(List));
}

void List_destroy(List * list)
{
  LIST_FOREACH(list, first, next, cur) {
    if (cur->prev) {
      free(cur->prev);
    }
  }

  if (list->last) {
    free(list->last);
  }
  free(list);
}

void List_clear(List * list)
{
  LIST_FOREACH(list, first, next, cur) {
    free(cur->value);
  }
}

void List_clear_destroy(List * list)
{
  List_clear(list);
  List_destroy(list);
}

void List_push(List * list, void *value)
{
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;

  if (list->last == NULL) {
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    node->prev = list->last;
    list->last = node;
  }

  list->count++;

error:
  return;
}

void *List_pop(List * list)
{
  ListNode *node = list->last;
  return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List * list, void *value)
{
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;

  if (list->first == NULL) {
    list->first = node;
    list->last = node;
  } else {
    node->next = list->first;
    list->first->prev = node;
    list->first = node;
  }

  list->count++;

error:
  return;
}

void *List_shift(List * list)
{
  ListNode *node = list->first;
  return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List * list, ListNode * node)
{
  void *result = NULL;

  check(list->first && list->last, "List is empty");
  check(node, "node can't be NULL");

  if (node == list->first && node == list->last) {
    list->first = NULL;
    list->last = NULL;
  } else if (node == list->first) {
    list->first = node->next;
    check(list->first != NULL, "Invalid list; first is now NULL");
    list->first->prev = NULL;
  } else if (node == list->last) {
    list->last = node->prev;
    check(list->last != NULL, "Invalid list; last is now NULL");
    list->last->next = NULL;
  } else {
    ListNode *after = node->next;
    ListNode * before = node->prev;
    after->prev = before;
    before->next = after;
  }

  list->count--;

  result = node->value;
  free(node);
  return result;

error:
  return NULL;
}

List *List_copy(List * list)
{
  List *new_list = List_create();
  check_mem(new_list);

  LIST_FOREACH(list, first, next, cur) {
    List_push(new_list, cur->value);
  }

  return new_list;

error:
  if (new_list) {
    List_clear_destroy(new_list);
  }
  return NULL;
}

List *List_split(List * list, ListNode *node)
{
  List *second_list = List_create();

  second_list->first = node;
  second_list->last = list->last;
  if (node->prev) {
    list->last = node->prev;
    list->last->next = NULL;
  } else if  (list->first == node) {
    list->first = NULL;
    list->last = NULL;
  } else {
    sentinel("Invalid list");
  }

  node->prev = NULL;

  LIST_FOREACH(second_list, first, next, cur) {
    list->count--;
    second_list->count++;
  }

  return second_list;

error:
  if (second_list) {
    List_clear_destroy(second_list);
  }
  return NULL;
}

void List_join(List * first_list, List * second_list)
{
  check(first_list->last, "First list does not have tail");
  check(second_list->first, "Second list does not have head");
  first_list->last->next = second_list->first;
  second_list->first->prev = first_list->last;
  first_list->last = second_list->last;

error:
  return;
}
