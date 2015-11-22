#include <stdlib.h>
#include <stdio.h>
#include "dbg.h"

#define MAX_DATA 512

void print_match(int line_no, char *file_name, char *line)
{
  printf("%s: %d\n", file_name, line_no);
  printf("%s\n", line);
}

void strip_newline(char *str)
{
  int newline_pos = strcspn(str, "\n");
  str[newline_pos] = '\0';
}

void search_file(char *file_name, char *test)
{
  FILE *file = fopen(file_name, "r");
  check(file, "File not found: %s", file_name);
  char *current_line = malloc(MAX_DATA);
  int line_no = 0;

  while (fgets(current_line, MAX_DATA, file)) {
    line_no++;
    if (strstr(current_line, test)) {
      print_match(line_no, file_name, current_line);
    }
  }

  free(current_line);
error:
  return;
}

int main(int argc, char *argv[])
{
  check(argc > 1, "Search string must be provided");

  FILE *file_list = fopen("./.logfindc", "r");
  char *test = argv[1];

  char *current_file = (char *)malloc(MAX_DATA * sizeof(char));
  while (fgets(current_file, MAX_DATA, file_list)) {
    strip_newline(current_file);
    search_file(current_file, test);
  }

  free(current_file);
  fclose(file_list);

  return 0;
error:
  return -1;
}
