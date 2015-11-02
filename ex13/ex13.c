#include<stdio.h>

int main(int argc, char *argv[])
{
  char *states[] = {
    "California",
    "Oregon",
    "Washington",
    "Texas"
  };

  int num_states = 4;

  argv[1] = states[1];
  states[2] = argv[2];

  // print each string in argv
  // skipping argv[0] because name of binary
  for (int i = 1; i < argc; i++) {
    printf("arg %d: %s\n", i, argv[i]);
  }

  for (int i = 0, j = 0; j = i + 1, i < num_states; i++) {
    printf("state %d: %s\n", j, states[i]);
  }

  return 0;
}
