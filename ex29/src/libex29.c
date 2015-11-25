#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dbg.h"

void uppercase(char *out, const char *in, int len)
{
  int i = 0;
  for (i = 0; i < len; i++) {
    out[i] = toupper(in[i]);
  }
}

void lowercase(char *out, const char *in, int len)
{
  int i = 0;
  for(i = 0; i < len; i++) {
    out[i] = tolower(in[i]);
  }
}
