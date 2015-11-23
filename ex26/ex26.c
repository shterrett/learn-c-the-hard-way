#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <wordexp.h>
#include "dbg.h"

#define MAX_DATA 512

void print_match(int line_no, char *file_name, char *line, int match_count)
{
  if (match_count <= 1) {
    printf("\e[33;1m%s\n\e[0m", file_name);
  }
  printf("  \e[33m%d:\e[0m %s", line_no, line);
}

void strip_newline(char *str)
{
  int newline_pos = strcspn(str, "\n");
  str[newline_pos] = '\0';
}

typedef char *(*test_str)(const char *haystack, const char *needle);

void search_file(char *file_name, char *test, test_str cmp_fn)
{
  FILE *file = fopen(file_name, "r");
  check(file, "File not found: %s", file_name);

  char *current_line = malloc(MAX_DATA * sizeof(char));
  int line_no = 0;
  int match_count = 0;

  while (fgets(current_line, MAX_DATA, file)) {
    line_no++;
    if (cmp_fn(current_line, test)) {
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

void process_files(FILE *file_list, char *test, test_str cmp_fn)
{
  wordexp_t paths;

  char *current_file = (char *)malloc(MAX_DATA * sizeof(char));
  while (fgets(current_file, MAX_DATA, file_list)) {
    strip_newline(current_file);
    wordexp(current_file, &paths, 0);
    for (int i = 0; i < paths.we_wordc; i++) {
      search_file(paths.we_wordv[i], test, cmp_fn);
    }
  }
  free(current_file);
  wordfree(&paths);
}

int main(int argc, char *argv[])
{
  FILE *file_list = fopen("./.logfindc", "r");

  int is_case_insensitive = 0;
  char *option_strings = "i";
  int opt;

  while ((opt = getopt(argc, argv, option_strings)) != -1) {
    switch (opt) {
    case 'i':
      is_case_insensitive = 1;
      break;
    default:
      sentinel("Usage: -i case insensitivity");
    }
  }

  check(optind <= argc, "Search string must be provided");
  char *test = argv[optind];

  test_str cmp_fn = NULL;
  if (is_case_insensitive) {
    cmp_fn = strcasestr;
  } else {
    cmp_fn = strstr;
  }

  process_files(file_list, test, cmp_fn);

  fclose(file_list);

  return 0;
error:
  return -1;
}
