#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Person {
  char *name;
  int age;
  int height;
  int weight;
};

typedef struct Pet {
  char *name;
  char *species;
} best_friend;

void print_person(struct Person who)
{
  printf("Name: %s\n", who.name);
  printf("Age: %d\n", who.age);
  printf("Height: %d\n", who.height);
  printf("Weight: %d\n", who.weight);
}

struct Person change_name(struct Person who, char *name)
{
  who.name = name;
  return who;
}

void mutate_in_place(struct Person *who, int new_age)
{
  who->age = new_age;
}

int main(int argc, char *argv[])
{
  struct Person joe;
  joe.name = "Joe Alex";
  joe.age = 32;
  joe.height = 64;
  joe.weight = 140;

  best_friend houdini;
  houdini.name = "Houdini";
  houdini.species = "Felis Cattus";

  printf("-------------------------\n");
  printf("Initialized on Stack\n\n");

  print_person(joe);
  printf("Joe's pet %s is a %s\n", houdini.name, houdini.species);

  struct Person new_joe = change_name(joe, "Joe Doe");

  printf("-------------------------\n");
  printf("Mutated in function\n\n");

  print_person(joe);
  print_person(new_joe);

  mutate_in_place(&new_joe, 55);

  printf("-------------------------\n");
  printf("Mutated via pointer\n\n");
  print_person(new_joe);

  return 0;
}
