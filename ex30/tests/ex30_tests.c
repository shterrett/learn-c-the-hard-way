#include "minunit.h"
#include <dlfcn.h>

void uppercase(char *out, const char *in, int len);
void lowercase(char *out, const char *in, int len);
char *lib_file = "build/libex30.so";
void *lib = NULL;

char *test_dlopen()
{
  lib = dlopen(lib_file, RTLD_NOW);

  mu_assert(lib != NULL, "Failed to open the library to test.");
  return NULL;
}

char *test_uppercase()
{
  char *test_string = "HeLlO wOrLd";
  int length = strlen(test_string);
  char upper_string[length];

  uppercase(upper_string, test_string, length);

  int cmp = strncmp(upper_string, "HELLO WORLD", length);
  mu_assert(cmp == 0, "Uppercase failed");
  return NULL;
}

char *test_lowercase()
{
  char *test_string = "HeLlO wOrLd";
  int length = strlen(test_string);
  char lower_string[length];

  lowercase(lower_string, test_string, length);

  int cmp = strncmp(lower_string, "hello world", length);
  mu_assert(cmp == 0, "lowercase failed");
  return NULL;
}

char *test_dlclose()
{
  int rc = dlclose(lib);

  mu_assert(rc == 0, "Failed to close lib.");
  return NULL;
}

char *test_fail()
{
  mu_assert(2 + 2 == 5, "Big Brother is watching you");
  return NULL;
}

char *all_tests()
{
  mu_suite_start();
  mu_run_test(test_dlopen);
  mu_run_test(test_uppercase);
  mu_run_test(test_lowercase);
  mu_run_test(test_dlclose);
  mu_run_test(test_fail);

  return NULL;
}

RUN_TESTS(all_tests);
