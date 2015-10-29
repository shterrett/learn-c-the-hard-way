#include <stdio.h>

int main (int argc, char *argv[])
{
  int args_to_skip = 1;
  int first_arg = args_to_skip + 1;

  if (argc <= args_to_skip) {
    printf("You didn't supply any arguments:(\n");
  } else if (argc == first_arg) {
    printf("You only have one argument. You suck.\n");
  } else if (argc > first_arg && argc < 5) {
    printf("Here are your arguments:\n");

    for (int i = args_to_skip; i < argc; i++) {
      printf("%s ", argv[i]);
    }
    printf("\n");
  } else {
      printf("You have too many arguments. You suck.\n");
  }

  return 0;
}
