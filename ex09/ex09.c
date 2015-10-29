#include <stdio.h>

int main(int argc, char *argv[]) {
  int i = 0;
  while (i < 25) {
    printf("%d ", i);
    i++;
  }
  printf("\n");

  while (i > 0) {
    printf("%d ", i);
    i--;
  }

  do {
    printf("%d ", i);
    i++;
  } while (i < 25);
  printf("\n");

  int j = 1;
  while (j) {
    printf("%d ", j);
    if (j % 17 == 0) {
      j = 0;
    } else {
      j++;
    }
  };
  printf("\n");

  return 0;
}
