#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>

char *values[] = { "XXXX", "1234", "abcd", "xjvef", "NDSS" };

#define NUM_VALUES 5

List *create_words()
{
  int i = 0;
  List *words = List_create();

  for (i = 0; i < NUM_VALUES; i++) {
    char *word = malloc(sizeof(char) * 5);
    strncpy(word, values[i], 5);
    List_push(words, word);
  }

  return words;
}

int is_sorted(List * words)
{
  LIST_FOREACH(words, first, next, cur)
  {
    if (cur->next && strcmp(cur->value, cur->next->value) > 0) {
      debug("%s %s", (char *)cur->value, (char *)cur->next->value);
      return 0;
    }
  }

  return 1;
}

char *test_bubble_sort()
{
  List *words = create_words();

  // unsorted
  int rc = List_bubble_sort(words, (List_compare) strcmp);
  mu_assert(rc == 0, "Bubble sort failed");
  mu_assert(is_sorted(words), "Words not sorted after bubble sort");

  // sorted
  rc = List_bubble_sort(words, (List_compare) strcmp);
  mu_assert(rc == 0, "Bubble sort of already sorted failed");
  mu_assert(is_sorted(words), "Already bubble sorted list no longer sorted");

  List_clear_destroy(words);

  // empty
  words = List_create();
  rc = List_bubble_sort(words, (List_compare) strcmp);
  mu_assert(rc == 0, "Bubble sort failed on empty list.");
  mu_assert(is_sorted(words), "Words should be sorted if empty");

  List_clear_destroy(words);

  return NULL;
}

char *test_merge_sort()
{
  List *words = create_words();

  // unsorted
  List *res = List_merge_sort(words, (List_compare) strcmp);
  mu_assert(is_sorted(res), "Words not sorted after merge sort");

  // sorted
  List *res2 = List_merge_sort(res, (List_compare) strcmp);
  mu_assert(is_sorted(res), "Merge sorted list no longer sorted.");

  List_clear_destroy(res2);
  List_clear_destroy(res);
  List_clear_destroy(words);

  return NULL;
}

char *all_tests()
{
  mu_suite_start();
  mu_run_test(test_bubble_sort);
//  mu_run_test(test_merge_sort);

  return NULL;
}

RUN_TESTS(all_tests);
