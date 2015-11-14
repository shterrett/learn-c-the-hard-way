#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void die(const char *message)
{
  if (errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

// a typedef creates a fake type, in this
// case for a function pointer
typedef int (*compare_cb) (int a, int b);
typedef int *(*sorter) (int *numbers, int count, compare_cb cmp);

/**
 * Classic bubble sort
 * Uses compare_cb for the sorting
 * returns pointer to array of int.
 * NOT a function pointer
 */
int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
  int temp = 0;
  int i = 0;
  int j = 0;
  int *target = malloc(count * sizeof(int));

  if (!target) {
    die("Memory Error");
  }

  memcpy(target, numbers, count * sizeof(int));

  for (i = 0; i < count; i++) {
    for (j = 0; j < count - 1; j++) {
      if (cmp(target[j], target[j + 1]) > 0) {
        temp = target[j + 1];
        target[j+1] = target[j];
        target[j] = temp;
      }
    }
  }

  return target;
}

int *binary_sort(int *numbers, int count, compare_cb cmp)
{
  int *result = malloc(count * sizeof(int));

  if (count > 1) {
    int pivot = count / 2;
    int after_pivot = count - pivot;
    int *left = malloc(pivot * sizeof(int));
    int *right = malloc(after_pivot * sizeof(int));
    memcpy(left, numbers, pivot * sizeof(int));
    memcpy(right, numbers + pivot, after_pivot * sizeof(int));
    int *sorted_left = binary_sort(left, pivot, cmp);
    int *sorted_right = binary_sort(left, after_pivot, cmp);

    int i = 0;
    int *l_ptr = left;
    int *r_ptr = right;

    for (i = 0; i < count; i++) {
      if (l_ptr == left + pivot) {
        result[i] = *r_ptr;
      } else if (r_ptr == right + after_pivot) {
        result[i] = *l_ptr;
      } else if (*l_ptr < *r_ptr) {
        result[i] = *l_ptr;
        l_ptr++;
      } else {
        result[i] = *r_ptr;
        r_ptr++;
      }
    }

    free(left);
    free(right);
    free(sorted_left);
    free(sorted_right);

    return result;

  } else {
    return result;
  }
}

int sorted_order(int a, int b)
{
  return a - b;
}

int reverse_order(int a, int b)
{
  return b - a;
}

int strange_order(int a, int b)
{
  if (a == 0 || b == 0) {
    return 0;
  } else {
    return a % b;
  }
}

/**
 * Used to test that we are sorting things correctly
 * by doing the sort and printing it out.
 */

void test_sorting(int *numbers, int count, sorter sort, compare_cb cmp) {
  int *sorted = bubble_sort(numbers, count, cmp);

  if (!sorted) {
    die("Failed to sort as requested");
  }

  for(int i = 0; i < count; i++) {
    printf("%d ", sorted[i]);
  }
  printf("\n");

  free(sorted);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    die("USAGE: ex18 <list of numbers>");
  }

  int count = argc - 1;
  int i = 0;
  char **inputs = argv + 1;
  int *numbers = malloc(count * sizeof(int));
  if (!numbers) {
    die("Memory Error");
  }

  for(i = 0; i < count; i++) {
    numbers[i] = atoi(inputs[i]);
  }

  printf("Testing Bubble Sort\n");
  test_sorting(numbers, count, bubble_sort, sorted_order);
  test_sorting(numbers, count, bubble_sort, reverse_order);
  test_sorting(numbers, count, bubble_sort, strange_order);

  printf("Testing Binary Sort\n");
  test_sorting(numbers, count, binary_sort, sorted_order);
  test_sorting(numbers, count, binary_sort, reverse_order);
  test_sorting(numbers, count, binary_sort, strange_order);

  free(numbers);

  return 0;
}
