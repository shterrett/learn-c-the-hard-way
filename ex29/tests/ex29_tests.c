#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"
#include <dlfcn.h>

typedef int (*lib_function) (char *out, const char *in, int len);
void uppercase(char *out, const char *in, int len);
void lowercase(char *out, const char *in, int len);

int main()
{
  char *lib_file = "./build/libex29.so";
  void *lib = dlopen(lib_file, RTLD_NOW);
  check(lib != NULL, "Failed to open library %s: %s", lib_file, dlerror());

  lib_function func;
  int success;
  char *test_string = "Hello! This is Mission Control!";
  int test_string_length = strlen(test_string);

  char *lower_string = malloc(sizeof(char) * test_string_length);
  func = dlsym(lib, "lowercase");
  func(lower_string, test_string, test_string_length);
  success = strncmp(lower_string, "hello! this is mission control!", test_string_length);
  check(success == 0,
        "(dynamic) Lower Case failed: %s -> %s",
        test_string,
        lower_string
       );

  char *upper_string = malloc(sizeof(char) * test_string_length);
  func = dlsym(lib, "uppercase");
  func(upper_string, test_string, test_string_length);
  success = strncmp(upper_string, "HELLO! THIS IS MISSION CONTROL!", test_string_length);
  check(success == 0,
        "(dynamic) Upper Case failed: %s -> %s",
        test_string,
        upper_string
       );

  free(lower_string);
  free(upper_string);

  lower_string = malloc(sizeof(char) * test_string_length);
  lowercase(lower_string, test_string, test_string_length);
  success = strncmp(lower_string, "hello! this is mission control!", test_string_length);
  check(success == 0,
        "(static) Lower Case failed: %s -> %s",
        test_string,
        lower_string
       );

  upper_string = malloc(sizeof(char) * test_string_length);
  uppercase(upper_string, test_string, test_string_length);
  success = strncmp(upper_string, "HELLO! THIS IS MISSION CONTROL!", test_string_length);
  check(success == 0,
        "(static) Upper Case failed: %s -> %s",
        test_string,
        upper_string
       );

  return 0;
error:
  return 1;
}
