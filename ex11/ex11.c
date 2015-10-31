#include <stdio.h>

int main(int argc, char *argv[])
{
  int numbers[4] = { 97 };
  char name[4] = { 'a' };

  // print them raw
  printf("numbers: %c %c %c %c\n",
         numbers[0],
         numbers[1],
         numbers[2],
         numbers[3]
        );
  printf("name each: %d %d %d %d\n",
         name[0],
         name[1],
         name[2],
         name[3]
        );
  printf("name: %s\n", name);

  numbers[0] = 101;
  numbers[1] = 102;
  numbers[2] = 103;
  numbers[3] = 104;

  name[0] = 'S';
  name[1] = 't';
  name[2] = 'u';
  name[3] = '\0';

  // print initialized
  printf("numbers: %c %c %c %c\n",
         numbers[0],
         numbers[1],
         numbers[2],
         numbers[3]
        );
  printf("name each: %d %d %d %d\n",
         name[0],
         name[1],
         name[2],
         name[3]
        );
  printf("name: %s\n", name);

  // another way to use name
  char *another = "Stu";
  printf("another: %s\n", another);
  printf("another each: %c %c %c %c\n",
         another[0],
         another[1],
         another[2],
         another[3]
        );

  // string as int
  char *weird = "Zed";
  printf("string: %s\n", weird);
  printf("string char numbers: %d %d %d\n",
         weird[0],
         weird[1],
         weird[2]
        );

  int weird_int = 0;
  for (int j = 0; weird[j] != '\0'; j++) {
    weird_int += (10 * (3 - j) * weird[j]);
  }

  printf("String as an int: %d\n", weird_int);

  return 0;
}
