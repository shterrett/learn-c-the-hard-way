#undef NDEBUG
#include "dbg.h"
#include <stdio.h>
#include <assert.h>

/*
 * naive copy that assumes valid input
 */
void copy(char to[], char from[])
{
  int i = 0;
  // will not terminate if from isn't null-terminated
  while ((to[i] = from[i]) != '\0') {
    ++i;
  }
}

/*
 * safer version with error checking
 */
int safercopy(int from_len, char *from, int to_len, char *to)
{
  assert(from != NULL && to != NULL && "from and to cannot be NULL");

  int i = 0;
  int max = from_len > to_len - 1 ? to_len - 1 : from_len;

  // to_len must have at least 1 byte

  check(from_len >= 0, "from length must be positive");
  check(to_len > 0, "to must have at least 1 byte");

  for (i = 0; i < max; i++) {
    to[i] = from[i];
  }

  to[to_len - 1] = '\0';

  return i;

error:
  return -1;
}

int main(int argc, char *argv[])
{
  char from[] = "0123456789";
  int from_len = sizeof(from);

  char to[] = "6543210";
  int to_len = sizeof(to);

  debug("Copying '%s':%d to '%s':%d", from, from_len, to, to_len);

  int rc = safercopy(from_len, from, to_len, to);
  check(rc > 0, "Failed to safercopy");
  check(to[to_len - 1] == '\0', "to not null terminated");
  debug("Result is: '%s':%d", to, to_len);

  rc = safercopy(from_len * -1, from, to_len, to);
  check(rc == -1, "safercopy should fail with negative length");
  check(to[to_len - 1] == '\0', "String not terminated");

  rc = safercopy(from_len, from, 0, to);
  check(rc == -1, "safercopy should fail with 0 to length");
  check(to[to_len - 1] == '\0', "String not terminated");

  return 0;

error:
  return -1;
}
