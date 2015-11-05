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

struct Person *Person_create(char *name, int age, int height, int weight)
{
  struct Person *who = malloc(sizeof(struct Person));
  assert(who != NULL);

  who->name = strdup(name);
  who->age = age;
  who->height = height;
  who->weight = weight;

  return who;
}

void Person_destroy(struct Person *who)
{
  assert(who != NULL);

  free(who->name);
  free(who);
}

void Person_print(struct Person *who)
{
  printf("Name: %s\n", who->name);
  printf("\tAge: %d\n", who->age);
  printf("\tHeight: %d\n", who->height);
  printf("\tWeight: %d\n", who->weight);
}

void Person_workout(struct Person *who)
{
  who->weight -= 10;
}

void Person_name_change(struct Person *who, char *new_name)
{
  free(who->name);
  who->name = strdup(new_name);
}

int main(int argc, char *argv[])
{
  char joes_name[] = "Joe Alex";
  struct Person *joe = Person_create(joes_name, 32, 64, 140);
  struct Person *frank = Person_create("Frank Blank", 20, 72, 180);

  printf("Joe is at memory location %p:\n", (void *) joe);
  Person_print(joe);
  printf("Frank is at memory location %p:\n", (void *) frank);
  Person_print(frank);

  printf("------------------------------\n");
  printf("Young, healthy Joe\n");
  Person_print(joe);

  joe->age += 20;
  joe->height -= 2;
  joe->weight += 40;
  printf("Joe gets old and fat\n");
  Person_print(joe);

  Person_workout(joe);
  printf("Joe goes to gym\n");
  Person_print(joe);

  Person_name_change(joe, "Joe Smith");
  Person_print(joe);

  Person_destroy(joe);
  Person_destroy(frank);

  return 0;
}
