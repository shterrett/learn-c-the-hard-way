#include <stdio.h>

int main(int argc, char *argv[])
{
  int distance = 100;
  float power = 2.345f;
  double super_power = 56789.4532;
  char initial = 'H';
  char first_name[] = "Stuart";
  char last_name[] = "Terrett";

  printf("You are %d miles away.\n", distance);
  printf("You have %f levels of power.\n", power);
  printf("You have %f awesome super powers.\n", super_power);
  printf("I have an initial %c.\n", initial);
  printf("I have a first name %s.\n", first_name);
  printf("I have a last_name %s.\n", last_name);
  printf("My whole name is %s %c %s.\n",
         first_name,
         initial,
         last_name
        );

  int bugs = 100;
  double bug_rate = 1.2;

  printf("You have %d bugs at the imaginary rate of %f.\n",
         bugs,
         bug_rate
        );

  unsigned long universe_of_defects = 9223372036854775808UL * 2UL - 1UL;

  printf("The entire universe has %lu bugs.\n", universe_of_defects);

  double expected_bugs = bugs * bug_rate;
  printf("You are expected to have %f bugs.\n", expected_bugs);

  long double part_of_universe = expected_bugs / universe_of_defects;
  printf("That is only a %Lf portion of the universe.\n", part_of_universe);

  // this is just weird
  char nul_byte = '\0';

  int care_percentage = bugs * nul_byte;
  printf("Which means you should care %d%%.\n", care_percentage);

  return 0;
}
