#include "minunit.h"
#include <lcthw/tstree.h>
#include <string.h>
#include <assert.h>
#include <lcthw/bstrlib.h>
#include <lcthw/darray.h>

TSTree *node = NULL;
char *valueA = "VALUEA";
char *valueB = "VALUEB";
char *value2 = "VALUE2";
char *value4 = "VALUE4";
char *reverse = "VALUER";
int traverse_count = 0;

struct tagbstring test1 = bsStatic("TEST");
struct tagbstring test2 = bsStatic("TEST2");
struct tagbstring test3 = bsStatic("TSET");
struct tagbstring test4 = bsStatic("T");

char *test_insert()
{
  node = TSTree_insert(node, bdata(&test1), blength(&test1), valueA);
  mu_assert(node != NULL, "Failed to insert into tst");

  node = TSTree_insert(node, bdata(&test2), blength(&test2), value2);
  mu_assert(node != NULL, "Failed to insert second item into tst");

  node = TSTree_insert(node, bdata(&test3), blength(&test3), reverse);
  mu_assert(node != NULL, "Failed to insert reverse name into tst");

  node = TSTree_insert(node, bdata(&test4), blength(&test4), value4);
  mu_assert(node != NULL, "Failed to insert single char name into tst");

  return NULL;
}

char *test_search_exact()
{
  void *res = TSTree_search(node, bdata(&test1), blength(&test1));
  mu_assert(res == valueA, "Got wrong value back; should get A");

  res = TSTree_search(node, "TESTNO", strlen("TESTNO"));
  mu_assert(res == NULL, "Should not find inexact match");

  return NULL;
}

char *test_search_prefix()
{
  void *res = TSTree_search_prefix(node, bdata(&test1), blength(&test1));
  mu_assert(res == valueA, "Got wrong valueA by prefix");

  res = TSTree_search_prefix(node, bdata(&test1), 1);
  mu_assert(res == value4, "Got wrong value4 for prefix of 1");

  res = TSTree_search_prefix(node, "TE", strlen("TE"));
  mu_assert(res != NULL, "Should find for short prefix");

  res = TSTree_search_prefix(node, "TE--", strlen("TE--"));
  mu_assert(res != NULL, "Should find for partial prefix");

  return NULL;
}

char *test_collect()
{
  DArray *matches = TSTree_collect(node, "TES", 3);
  mu_assert(DArray_count(matches) == 2, "Did not return the right number of matches");
  int match_forwards = (strncmp("TEST", DArray_get(matches, 0), 4) == 0 &&
                       strncmp("TEST2", DArray_get(matches, 1), 5) == 0);
  int match_backwards = (strncmp("TEST2", DArray_get(matches, 0), 5) == 0 &&
                        strncmp("TEST", DArray_get(matches, 1), 4) == 0);
  mu_assert(match_forwards || match_backwards, "matches not found");

  return NULL;
}

void TSTree_traverse_test_cb(void *value, void *data)
{
  assert(value != NULL && "Should not get NULL");
  assert(data == valueA && "Expecting valueA as the data");
  traverse_count++;
}

char *test_traverse()
{
  traverse_count = 0;
  TSTree_traverse(node, TSTree_traverse_test_cb, valueA);
  mu_assert(traverse_count == 4, "Didn't find 4 keys");

  return NULL;
}

char *test_destroy() {
  TSTree_destroy(node);

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_insert);
  mu_run_test(test_search_exact);
  mu_run_test(test_search_prefix);
  mu_run_test(test_collect);
  mu_run_test(test_traverse);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
