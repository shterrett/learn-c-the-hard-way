#include <stdio.h>

int main(int argc, char *argv[])
{
  // create two arrays we care about
  int ages[] = { 23, 43, 12, 89, 2 };
  char *names[] = { "Alan",
                    "Frank",
                    "Mary",
                    "John",
                    "Lisa"
                  };

  // safely get the size of ages
  int count = sizeof(ages) / sizeof(int);

  // first way using indexing
  for (int i = 0; i < count; i++) {
    printf("%s has %d years alive.\n", names[i], ages[i]);
  }

  printf("---------------\n");

  // set up the pointers to the start of the arrays
  int *cur_age = &ages[0];
  char **cur_name = &names[0];
  char **args = argv;

  // second way using pointers;
  for (int i = 0; i < count; i++) {
    printf("%s is %d years old.\n",
           *(cur_name + 1),
           *(cur_age + i)
          );
  }

  printf("---------------\n");

  // third way, pointers are just arrays
  for (int i = 0; i < count; i++) {
    printf("%s is %d years old again.\n", cur_name[i], cur_age[i]);
  }

  printf("---------------\n");

  // fourth way with pointers in a stupid complex way
  for (cur_name = names, cur_age = ages;
       (cur_age - ages) < count;
       cur_name++, cur_age++
      ) {
    printf("%s lived %d years so far.\n", *cur_name, *cur_age);
  }

  printf("---------------\n");

  // command line args as pointers
  for (int i = 1; i < argc; i++) {
    printf("%s is temporarily stored in %p\n", *(args + i), (void *) (args + i));
  }

  // Some random pointers

  int j = 5;
  int nums[] = { j, 10, 12 };

  int *point_to_j = &j;
  int *point_to_nums = nums;

  printf("j is stored at %p\n", (void *) point_to_j);
  printf("j has value %d\n", *point_to_j);
  printf("nums starts at %p\n", (void *) point_to_nums);
  printf("the first value of nums is %d\n", *point_to_nums);
  printf("the first index of nums is %d\n", nums[0]);
  printf("the second value of nums is %d\n", *(point_to_nums + 1));
  printf("the third value of nums is %d\n", *(point_to_nums + 2));
  printf("the value after nums is %d\n", *(point_to_nums + 3));

  return 0;
}
