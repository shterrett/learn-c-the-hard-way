#include <stdio.h>

int main(int arg, char *argv[])
{
  int nums[] = { 10, 11, 12, 13, 14 };
  int *num_start = nums;
  int *num_ptr = nums;
  int num_size = sizeof(nums) / sizeof(int);

  printf("num_size = %d\n", num_size);
  printf("start_position = %p\n", (void *) num_start);
  printf("\n");
  while ((num_ptr - num_start) < num_size) {
    printf("Value = %d\n", *num_ptr);
    printf("Address = %p\n", (void *) num_ptr);
    printf("position difference = %d\n", (int) (num_ptr - num_start));
    printf("\n");
    num_ptr++;
  }

  return 0;
}
