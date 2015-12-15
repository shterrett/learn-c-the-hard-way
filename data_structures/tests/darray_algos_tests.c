#include "minunit.h"
#include <lcthw/darray_algos.h>

int testcmp(char **a, char **b)
{
  int cmp =  strcmp(*a, *b);
  return cmp;
}

DArray *create_words()
{
  DArray *result = DArray_create(0, 5);
  char *words[] = { "asdfasfd",
                    "werwar",
                    "13234",
                    "asdfasfd",
                    "oioj"
                  };

  int i = 0;
  for (i = 0; i < 5; i++)
  {
    DArray_push(result, words[i]);
  }

  return result;
}

int is_sorted(DArray *array)
{
  int i = 0;

  for (i = 0; i < DArray_count(array) - 1; i++) {
    if (strcmp(DArray_get(array, i), DArray_get(array, i + 1)) > 0) {
      return 0;
    }
  }

  return 1;
}

char *run_sort_test(int (*func) (DArray *, DArray_compare), const char *name)
{
  DArray *words = create_words();
  mu_assert(!is_sorted(words), "Words should not start sorted");

  debug("--- Testing %s sorting algorithm", name);
  int rc = func(words, (DArray_compare) strcmp);
  mu_assert(rc == 0, "sort failed");
  mu_assert(is_sorted(words), "Not sorted");

  DArray_destroy(words);

  return NULL;
}

char *test_qsort()
{
  return run_sort_test(DArray_qsort, "qsort");
}

char *test_heapsort()
{
  return run_sort_test(DArray_heapsort, "heapsort");
}

char *test_mergesort()
{
  return run_sort_test(DArray_mergesort, "mergesort");
}

int intcmp(const void *a, const void *b)
{
  return ((int)a > (int)b) - ((int)b > (int)a);
}

char *test_find()
{
  DArray *array = DArray_create(0, 5);
  char *words[] = { "hell",
                    "hello",
                    "july",
                    "light",
                    "terror"
                  };
  for(int i = 0; i < 5; i++) {
    DArray_push(array, words[i]);
  }

  char *hell = "hell";
  mu_assert(DArray_find(array, hell, (DArray_compare) strcmp) == 0,
            "word not found"
           );
  char *light = "light";
  mu_assert(DArray_find(array, light, (DArray_compare) strcmp) == 3,
            "word not found"
           );
  char *nonexistent = "nonexistent";
  mu_assert(DArray_find(array, nonexistent, (DArray_compare) strcmp) == -1,
            "missing word found"
           );

  DArray_destroy(array);
  array = DArray_create(0, 5);

  for (int i = 0; i < 5; i++) {
    int j = i * 3;
    DArray_push(array, (void *)j);
  }

  int two = 2;
  mu_assert(DArray_find(array, (void *)two, intcmp) == -1,
            "missing number found"
           );
  int three = 3;
  mu_assert(DArray_find(array, (void *)three, intcmp) == 1,
            "number not found"
           );

  DArray_destroy(array);

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_qsort);
  mu_run_test(test_heapsort);
  mu_run_test(test_mergesort);
  mu_run_test(test_find);

  return NULL;
}

RUN_TESTS(all_tests);
