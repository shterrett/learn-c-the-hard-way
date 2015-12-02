#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>

char *test_create()
{
  size_t test_length = 16;

  char *test1 = malloc(test_length * sizeof(char));
  char *test2 = malloc(test_length * sizeof(char));
  char *test3 = malloc(test_length * sizeof(char));

  strncpy(test1, "Data for test 1", test_length);
  strncpy(test2, "Data for test 2", test_length);
  strncpy(test3, "Data for test 3", test_length);
  List *list = List_create();
  mu_assert(list != NULL, "Failed to create list");

  return NULL;
}

char *test_destroy()
{
  size_t test_length = 16;

  char *test1 = malloc(test_length * sizeof(char));
  char *test2 = malloc(test_length * sizeof(char));
  char *test3 = malloc(test_length * sizeof(char));

  strncpy(test1, "Data for test 1", test_length);
  strncpy(test2, "Data for test 2", test_length);
  strncpy(test3, "Data for test 3", test_length);
  List *list = List_create();
  List_push(list, test1);

  List_clear_destroy(list);

  return NULL;
}

char *test_push_pop()
{
  size_t test_length = 16;

  char *test1 = malloc(test_length * sizeof(char));
  char *test2 = malloc(test_length * sizeof(char));
  char *test3 = malloc(test_length * sizeof(char));

  strncpy(test1, "Data for test 1", test_length);
  strncpy(test2, "Data for test 2", test_length);
  strncpy(test3, "Data for test 3", test_length);
  List *list = List_create();

  List_push(list, test1);
  mu_assert(List_last(list) == test1, "Wrong last value");

  List_push(list, test2);
  mu_assert(List_last(list) == test2, "Wrong last value");

  List_push(list, test3);
  mu_assert(List_last(list) == test3, "Wrong last value");
  mu_assert(List_count(list) == 3, "Wrong count on push");

  char *val = List_pop(list);
  mu_assert(val == test3, "Wrong value on pop");

  val = List_pop(list);
  mu_assert(val == test2, "Wrong value on pop");

  val = List_pop(list);

  mu_assert(val == test1, "Wrong value on pop");
  mu_assert(List_count(list) == 0, "Wrong count after pop");

  List_clear_destroy(list);

  return NULL;
}

char *test_unshift()
{
  size_t test_length = 16;

  char *test1 = malloc(test_length * sizeof(char));
  char *test2 = malloc(test_length * sizeof(char));
  char *test3 = malloc(test_length * sizeof(char));

  strncpy(test1, "Data for test 1", test_length);
  strncpy(test2, "Data for test 2", test_length);
  strncpy(test3, "Data for test 3", test_length);
  List *list = List_create();

  List_unshift(list, test1);
  mu_assert(List_first(list) == test1, "Wrong first value");

  List_unshift(list, test2);
  mu_assert(List_first(list) == test2, "Wrong first value");

  List_unshift(list, test3);
  mu_assert(List_first(list) == test3, "Wrong last value");
  mu_assert(List_count(list) == 3, "Wrong count on unshift");

  List_clear_destroy(list);

  return NULL;
}

char *test_remove()
{
  size_t test_length = 16;

  char *test1 = malloc(test_length * sizeof(char));
  char *test2 = malloc(test_length * sizeof(char));
  char *test3 = malloc(test_length * sizeof(char));

  strncpy(test1, "Data for test 1", test_length);
  strncpy(test2, "Data for test 2", test_length);
  strncpy(test3, "Data for test 3", test_length);
  List *list = List_create();
  List_push(list, test1);
  List_push(list, test2);
  List_push(list, test3);

  char *val = List_remove(list, list->first->next);
  mu_assert(val == test2, "Wrong removed element");
  mu_assert(List_count(list) == 2, "Wrong count after remove");
  mu_assert(List_first(list) == test1, "Wrong first after remove");
  mu_assert(List_last(list) == test3, "Wrong last after remove");

  List_clear_destroy(list);

  return NULL;
}

char *test_shift()
{
  size_t test_length = 16;

  char *test1 = malloc(test_length * sizeof(char));
  char *test2 = malloc(test_length * sizeof(char));
  char *test3 = malloc(test_length * sizeof(char));

  strncpy(test1, "Data for test 1", test_length);
  strncpy(test2, "Data for test 2", test_length);
  strncpy(test3, "Data for test 3", test_length);
  List *list = List_create();
  List_push(list, test1);
  List_push(list, test3);

  mu_assert(List_count(list) != 0, "Wrong count before shift");

  char *val = List_shift(list);
  mu_assert(val == test1, "Wrong value on shift");

  val = List_shift(list);
  mu_assert(val == test3, "Wrong value on shift");
  mu_assert(List_count(list) == 0, "Wrong count after shift");

  List_clear_destroy(list);

  return NULL;
}

char *test_list_copy()
{
  size_t test_length = 16;

  char *test1 = malloc(test_length * sizeof(char));
  char *test2 = malloc(test_length * sizeof(char));
  char *test3 = malloc(test_length * sizeof(char));

  strncpy(test1, "Data for test 1", test_length);
  strncpy(test2, "Data for test 2", test_length);
  strncpy(test3, "Data for test 3", test_length);
  List *list = List_create();
  List_push(list, test1);
  List_push(list, test2);
  List_push(list, test3);

  List *new_list = List_copy(list);
  mu_assert(list != new_list, "Same list; not a copy");
  mu_assert(List_shift(new_list) == test1, "Wrong first value");
  mu_assert(List_shift(new_list) == test2, "Wrong second value");
  mu_assert(List_shift(new_list) == test3, "Wrong third value");

  List_clear_destroy(list);
  List_clear_destroy(new_list);
  return NULL;
}

char *test_list_split()
{
  size_t test_length = 16;

  char *test1 = malloc(test_length * sizeof(char));
  char *test2 = malloc(test_length * sizeof(char));
  char *test3 = malloc(test_length * sizeof(char));

  strncpy(test1, "Data for test 1", test_length);
  strncpy(test2, "Data for test 2", test_length);
  strncpy(test3, "Data for test 3", test_length);
  List *list = List_create();
  List_push(list, test1);
  List_push(list, test2);

  ListNode *second = list->last;
  List_push(list, test3);

  List *second_list = List_split(list, second);

  mu_assert(List_first(list) == test1, "Original head not preserved");
  mu_assert(List_last(list) == test1, "Original list not ended properly");
  mu_assert(list->count == 1, "List not split properly");
  mu_assert(List_first(second_list) == test2, "New list not started properly");
  mu_assert(List_last(second_list) == test3, "New list not ended properly");
  mu_assert(second_list->count == 2, "List not split properly");

  List_clear_destroy(list);
  List_clear_destroy(second_list);

  return NULL;
}

char *test_list_join()
{
  size_t test_length = 16;

  char *test1 = malloc(test_length * sizeof(char));
  char *test2 = malloc(test_length * sizeof(char));
  char *test3 = malloc(test_length * sizeof(char));

  strncpy(test1, "Data for test 1", test_length);
  strncpy(test2, "Data for test 2", test_length);
  strncpy(test3, "Data for test 3", test_length);
  List *list = List_create();
  List_push(list, test1);
  List_push(list, test2);

  List *second_list = List_create();
  List_push(second_list, test3);

  List_join(second_list, list);

  mu_assert(List_first(second_list) == test3, "Head not preserved");
  mu_assert(List_last(second_list) == test2, "Tail not added");

  List_shift(second_list);

  mu_assert(List_first(second_list) == test1, "Middle not added");

  List_clear_destroy(second_list);
  free(list);
  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_push_pop);
  mu_run_test(test_unshift);
  mu_run_test(test_remove);
  mu_run_test(test_shift);
  mu_run_test(test_destroy);
  mu_run_test(test_list_copy);
  mu_run_test(test_list_split);
  mu_run_test(test_list_join);

  return NULL;
}

RUN_TESTS(all_tests);
