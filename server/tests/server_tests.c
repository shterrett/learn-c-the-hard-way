#include "minunit.h"
#include "../src/bstrlib.h"
#include "../src/server.h"
#include "../src/stats.h"
#include "../src/hashmap.h"
#include <math.h>

#define EQ(X, Y, N) (round((X) * pow(10, N)) == round((Y) * pow(10, N)))

Hashmap *stat_store = NULL;
char *name_1 = "my_stat";
char *name_2 = "your_stat";
double samples_1[] = { 2, 4, 6, 8 };
double samples_2[] = { 3, 5, 7, 9 };

char *test_init()
{
  stat_store = Server_initialize();

  mu_assert(stat_store != NULL, "Failed to create hashmap");

  return NULL;
}

char *test_create_stat()
{
  int rc = 0;
  rc = Server_create_stat(stat_store, name_1);
  mu_assert(rc == 0, "Error creating stat store");

  rc = Server_create_stat(stat_store, name_2);
  mu_assert(rc == 0, "Error creating stat store");

  bstring b_name_1 = bfromcstr(name_1);
  bstring b_name_2 = bfromcstr(name_2);

  Stats *tmp_store = Hashmap_get(stat_store, b_name_1);
  mu_assert(tmp_store != NULL, "stat not saved");

  tmp_store = Hashmap_get(stat_store, b_name_2);
  mu_assert(tmp_store != NULL, "stat not saved");

  return NULL;
}

char *test_sample()
{
  bstring b_name_1 = bfromcstr(name_1);
  bstring b_name_2 = bfromcstr(name_2);

  int i = 0;
  for (i = 0; i < 4; i++) {
    Server_add_sample(stat_store, name_1, samples_1[i]);
  }
  Stats *tmp_stat = Hashmap_get(stat_store, b_name_1);
  mu_assert(tmp_stat->n == 4, "Samples not added");

  for (i = 0; i < 4; i++) {
    Server_add_sample(stat_store, name_2, samples_2[i]);
  }

  tmp_stat = Hashmap_get(stat_store, b_name_2);
  mu_assert(tmp_stat->n == 4, "Samples not added");

  return NULL;
}

char *test_return_mean()
{
  mu_assert(EQ(Server_return_mean(stat_store, name_1), 5, 0), "mean not correct");
  mu_assert(EQ(Server_return_mean(stat_store, name_2), 6, 0), "mean not correct");

  return NULL;
}

char *test_return_stddev()
{
  mu_assert(EQ(Server_return_stddev(stat_store, name_1), 2.58, 2),
            "stddev not correct"
           );
  mu_assert(EQ(Server_return_stddev(stat_store, name_2), 2.58, 2),
            "stddev not correct"
           );

  return NULL;
}

char *test_dump()
{
  char *summary = Server_dump_stats(stat_store, name_1);
  mu_assert(summary != NULL, "summary not created");
  debug("SUMMARY 1: %s", summary);

  summary = Server_dump_stats(stat_store, name_2);
  mu_assert(summary != NULL, "summary not created");
  debug("SUMMARY 2: %s", summary);

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_init);
  mu_run_test(test_create_stat);
  mu_run_test(test_sample);
  mu_run_test(test_return_mean);
  mu_run_test(test_return_stddev);
  mu_run_test(test_dump);

  return NULL;
}

RUN_TESTS(all_tests);
