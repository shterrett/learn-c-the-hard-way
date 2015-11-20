#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dbg.h"

#define MAX_DATA 100

typedef enum EyeColor {
  BLUE_EYES,
  GREEN_EYES,
  BROWN_EYES,
  BLACK_EYES,
  OTHER_EYES
} EyeColor;

const char *EYE_COLOR_NAMES[] = {
  "Blue", "Green", "Brown", "Black", "Other"
};

typedef struct Person {
  int age;
  char *first_name;
  char last_name[MAX_DATA];
  EyeColor eyes;
  float income;
  char terrorist[3];
} Person;

void terrorism_probability(char *percent) {
  time_t t;
  srand((unsigned) time(&t));
  int prob = (int) ((rand() / (double) RAND_MAX) * (100 + 1));
  sprintf(percent, "%d%%", prob);
}

int main(int argc, char *argv[])
{
  Person you = { .age = 0 };
  int i = 0;
  char *in = NULL;

  printf("Welcome to the NSA data entry portal!\n");

  printf("What's your First Name? ");
  scanf("%ms", &you.first_name);
  check(you.first_name != NULL, "Failed to read first name.");

  printf("What's your last name? ");
  in = fgets(you.last_name, MAX_DATA - 1, stdin);
  check(in != NULL, "Failed to read last name.");

  printf("How old are you? ");
  char *age;
  fgets(age, MAX_DATA - 1, stdin);
  you.age = atoi(age);
  check(you.age > 0, "You have to enter a number");

  printf("What color are your eyes?\n");
  for (i = 0; i <= OTHER_EYES; i++)
  {
    printf("%d) %s\n", i + 1, EYE_COLOR_NAMES[i]);
  }
  printf("> ");

  int eyes = -1;
  int rc;
  rc = fscanf(stdin, "%d", &eyes);
  check(rc > 0, "You have to enter a number.");

  you.eyes = eyes - 1;
  check(you.eyes <= OTHER_EYES && you.eyes >= 0, "Eyes not included in the list");

  printf("How much do you make per hour? ");
  rc = scanf("%f", &you.income);
  check(rc > 0, "Enter a floating point number.");

  terrorism_probability(you.terrorist);

  printf("---------- RESULTS ----------\n");
  printf("First Name: %s\n", you.first_name);
  printf("Last Name: %s\n", you.last_name);
  printf("Age: %d\n", you.age);
  printf("Eye color: %s\n", EYE_COLOR_NAMES[you.eyes]);
  printf("Income: %.2f\n", you.income);
  printf("Terrorist-ness: %s\n", you.terrorist);
  printf("Thank you for registering with the NSA!\n");

  return 0;

error:
  return -1;

}
