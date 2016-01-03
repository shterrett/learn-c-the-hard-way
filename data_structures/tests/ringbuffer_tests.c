#include "minunit.h"
#include <assert.h>
#include <lcthw/ringbuffer.h>
#include <lcthw/bstrlib.h>

RingBuffer *rbuf = NULL;
struct tagbstring test_data_0 = bsStatic("test 1");
struct tagbstring test_data_1 = bsStatic("test 2 is longer than test 1");
struct tagbstring test_data_2 = bsStatic("t");
struct tagbstring test_data_3 = bsStatic("gffhnqu38r98tqyt0n9843ytvbq0g7h-jqc98tq09yvr8nvqt98-r0vnqo9fhqi");

char *test_create()
{
  rbuf = RingBuffer_create(64);
  mu_assert(rbuf != NULL, "Failed to create RingBuffer");
  mu_assert(RingBuffer_empty(rbuf) == 1, "RingBuffer not empty");
  mu_assert(rbuf->start == 0, "RingBuffer start is not 0");
  mu_assert(rbuf->end == 0, "RingBuffer end is not 0");
  mu_assert(RingBuffer_available_space(rbuf) == 64, "RingBuffer space not correct");

  return NULL;
}

char *test_read_write()
{
  RingBuffer_puts(rbuf, &test_data_0);
  mu_assert(rbuf->end == 6, "ends at not reset correctly");
  mu_assert(RingBuffer_empty(rbuf) == 0, "not actually empty");

  RingBuffer_puts(rbuf, &test_data_1);
  mu_assert(rbuf->end == 34, "ends at not reset correctly");

  char *data_1 = malloc(sizeof(char) * 6);
  RingBuffer_read(rbuf, data_1, 6);
  mu_assert(bstrcmp(bfromcstr(data_1), &test_data_0) == 0,
            "read did not return proper string"
           );
  mu_assert(rbuf->start == 6, "did not reset start correctly");

  char *data_2 = malloc(sizeof(char) * 28);
  RingBuffer_read(rbuf, data_2, 28);
  mu_assert(bstrcmp(bfromcstr(data_2), &test_data_1) == 0,
            "read did not return proper string"
           );
  mu_assert(rbuf->start == 0, "did not reset start correctly");
  mu_assert(rbuf->end == 0, "did not reset end correctly");


  RingBuffer_puts(rbuf, &test_data_2);
  RingBuffer_puts(rbuf, &test_data_3);
  mu_assert(rbuf->start == 0, "start not maintained");
  mu_assert(rbuf->end == 64, "end not set correctly");
  mu_assert(RingBuffer_full(rbuf) == 1, "RingBuffer actually full");
  mu_assert(RingBuffer_available_space(rbuf) == 0, "RingBuffer has no space");
  mu_assert(RingBuffer_available_data(rbuf) == 64, "RingBuffer has wrong available data");

  bstring data_3 = RingBuffer_get_all(rbuf);
  bstring expected = bformat("%s%s", bdata(&test_data_2), bdata(&test_data_3));
  mu_assert(bstrcmp(data_3, expected) == 0, "data not returned correctly");
  mu_assert(RingBuffer_empty(rbuf) == 1, "RingBuffer should be empty");

  return NULL;
}

char *test_destroy()
{
  RingBuffer_destroy(rbuf);

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_read_write);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
