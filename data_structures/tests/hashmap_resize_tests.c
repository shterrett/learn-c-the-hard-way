#include "minunit.h"
#include <lcthw/hashmap.h>
#include <assert.h>
#include <lcthw/bstrlib.h>

char *test_resize()
{
  Hashmap *map = Hashmap_create(NULL, NULL);

  int i = 0;
  for (i = 0; i < 49; i++) {
    bstring key = bformat("key-%d", i);
    bstring value = bformat("value-%d", i);
    Hashmap_set(map, key, value);
  }

  bstring test_key = bformat("key-%d", 25);
  bstring test_value = bformat("value-%d", 25);
  mu_assert(bstrcmp(Hashmap_get(map, test_key), test_value) == 0,
            "Did not populate correctly"
           );
  mu_assert(map->count == 49, "hashmap count wrong");
  mu_assert(map->buckets->end == 100, "bucket length wrong");


  for (i = 49; i < 75; i++) {
    bstring key = bformat("key-%d", i);
    bstring value = bformat("value-%d", i);
    Hashmap_set(map, key, value);
  }

  bstring new_test_key = bformat("key-%d", 65);
  bstring new_test_value = bformat("value-%d", 65);
  mu_assert(bstrcmp(Hashmap_get(map, test_key), test_value) == 0,
            "existing data lost"
           );
  mu_assert(bstrcmp(Hashmap_get(map, new_test_key), new_test_value) == 0,
            "did not populate correctly"
           );


  mu_assert(map->count == 75, "hashmap count wrong");
  mu_assert(map->buckets->end == 200, "bucket length wrong");

  for (i = 74; i > 43; i--) {
    bstring key = bformat("key-%d", i);
    Hashmap_delete(map, key);
  }

  mu_assert(bstrcmp(Hashmap_get(map, test_key), test_value) == 0,
            "early data not retained"
           );
  mu_assert(Hashmap_get(map, new_test_key) == NULL,
            "new data not deleted"
           );
  mu_assert(map->count == 44, "hashmap count wrong");
  mu_assert(map->buckets->end == 100, "bucket length wrong");


  Hashmap_destroy(map);

  return NULL;
}

char *all_tests()
{
  mu_suite_start();
  mu_run_test(test_resize);
  return NULL;
}

RUN_TESTS(all_tests);
