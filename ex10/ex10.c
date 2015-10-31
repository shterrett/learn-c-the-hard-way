#include <stdio.h>

void print_letter(int position, char letter)
{
  printf("%d: %c\n", position, letter);
}

char upcase(char letter)
{
  if (letter >= 'a') {
    int difference = 'a' - 'A';
    return letter - difference;
  } else {
    return letter;
  }
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("ERROR: You need one argument.\n");
    // abort with error
    return 1;
  }

  char *word = argv[1];
  char letter;

  for (int i = 0; letter = word[i], letter != '\0'; i++) {
    switch (upcase(letter)) {
      case 'A':
      case 'E':
      case 'I':
      case 'O':
      case 'U':
        print_letter(i, letter);
        break;
      case 'Y':
        if (i > 2) {
          print_letter(i, letter);
        }
        break;
      default:
        printf("%d: %c is not a vowel\n", i, letter);
    }
  }
}
