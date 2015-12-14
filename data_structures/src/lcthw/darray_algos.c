#include "darray_algos.h"
#include <stdlib.h>

void swap(DArray *array, int i, int j)
{
  void *tmp = DArray_get(array, i);
  DArray_set(array, i, DArray_get(array, j));
  DArray_set(array, j, tmp);
}

void qst(DArray *array, DArray_compare cmp, int low, int hi);
int DArray_qsort(DArray *array, DArray_compare cmp)
{
  qst(array, cmp, 0, array->end - 1);
  return 0;
}

int qs_partition(DArray *array, DArray_compare cmp, int low, int hi)
{
  void *pivot = DArray_get(array, low);
  int i = low - 1;
  int j = hi + 1;
  while (1) {
    do {
      j--;
    } while (cmp(DArray_get(array, j), pivot) > 0);
    do {
      i++;
    } while(cmp(DArray_get(array, i), pivot) < 0);
    if (i < j) {
      swap(array, i, j);
    } else {
      return j;
    }
  }
}

void qst(DArray *array, DArray_compare cmp, int low, int hi)
{
  if (low < hi) {
    int p = qs_partition(array, cmp, low, hi);
    qst(array, cmp, low, p);
    qst(array, cmp, p + 1, hi);
  }
}

void heapify(DArray *array, DArray_compare cmp);
void sift_down(DArray *array, int start, int end, DArray_compare cmp);
int DArray_heapsort(DArray *array, DArray_compare cmp)
{
  heapify(array, cmp);
  debug("MAX HEAP:");
  for (int i = 0; i < array->end; i++) {
    debug("Array i: %s", DArray_get(array, i));
  }

  int end = array->end - 1;
  while (end > 0) {
    swap(array, end, 0);
    end--;
    sift_down(array, 0, end, cmp);
  }

  return 0;
}

void heapify(DArray *array, DArray_compare cmp)
{
  int last_index = array->end - 1;
  int start = (last_index - 1) / 2;

  while (start >= 0) {
    sift_down(array, start, last_index, cmp);
    start--;
  }
}

void sift_down(DArray *array, int start, int end, DArray_compare cmp)
{
  int root = start;
  int child;
  int swp;

  while (root * 2 + 1 <= end) {
    child = root * 2 + 1;
    swp = root;

    if (cmp(DArray_get(array, swp), DArray_get(array, child)) < 0) {
      swp = child;
    }

    if (child + 1 <= end &&
         (cmp(DArray_get(array, swp), DArray_get(array, child + 1))) < 0) {
      swp = child + 1;
    }

    if (swp == root) {
      return;
    } else {
      swap(array, root, swp);
      root = swp;
    }
  }
}

int DArray_mergesort(DArray *array, DArray_compare cmp)
{
  int count = array->end;

  if (count > 1) {
    int pivot = count / 2;
    int after_pivot = count - pivot;
    DArray *left = DArray_create(array->element_size, pivot);
    DArray *right = DArray_create(array->element_size, after_pivot);

    for (int i = 0; i < pivot; i++) {
      DArray_push(left, DArray_get(array, i));
    }

    for (int i = pivot; i < count; i++) {
      DArray_push(right, DArray_get(array, i));
    }

    DArray_mergesort(left, cmp);
    DArray_mergesort(left, cmp);

    int i = 0;
    int l_ptr = 0;
    int r_ptr = 0;

    for (i = 0; i < count; i++) {
      if (l_ptr >= pivot) {
        DArray_set(array, i, DArray_get(right, r_ptr));
        r_ptr++;
      } else if (r_ptr >= after_pivot) {
        DArray_set(array, i, DArray_get(left, l_ptr));
        l_ptr++;
      } else if (cmp(DArray_get(left, l_ptr), DArray_get(right, r_ptr)) < 0) {
        DArray_set(array, i, DArray_get(left, l_ptr));
        l_ptr++;
      } else {
        DArray_set(array, i, DArray_get(right, r_ptr));
        r_ptr++;
      }
    }

    DArray_destroy(left);
    DArray_destroy(right);
  }
  return 0;
}
