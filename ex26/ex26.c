#include <stdlib.h>
#include <stdio.h>
#include <wordexp.h>
#include "dbg.h"

#define MAX_DATA 512

void print_match(int line_no, char *file_name, char *line, int match_count)
{
  if (match_count <= 1) {
    printf("\e[33m1m%s\n\e[0m", file_name);
  }
  printf("  \e[33m%d:\e[0m %s", line_no, line);
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

  char *current_line = malloc(MAX_DATA * sizeof(char));
  int line_no = 0;
  int match_count = 0;

  while (fgets(current_line, MAX_DATA, file)) {
    line_no++;
    if (strstr(current_line, test)) {
      match_count++;
      print_match(line_no, file_name, current_line, match_count);
    }
  }

  free(current_line);
  fclose(file);

  printf("\n");
error:
  printf("\n");
  return;
}

void process_files(FILE *file_list, char *test)
{
  wordexp_t paths;

  char *current_file = (char *)malloc(MAX_DATA * sizeof(char));
  while (fgets(current_file, MAX_DATA, file_list)) {
    strip_newline(current_file);
    wordexp(current_file, &paths, 0);
    for (int i = 0; i < paths.we_wordc; i++) {
      search_file(paths.we_wordv[i], test);
    }
  }
  free(current_file);
  wordfree(&paths);
}

int main(int argc, char *argv[])
{
  check(argc > 1, "Search string must be provided");
  char *test = argv[1];
  FILE *file_list = fopen("./.logfindc", "r");

  process_files(file_list, test);

  fclose(file_list);

  return 0;
error:
  return -1;
}
