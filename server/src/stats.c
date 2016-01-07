#include <math.h>
#include "stats.h"
#include <stdlib.h>
#include "dbg.h"

Stats *Stats_recreate(double sum,
                      double sumsq,
                      unsigned long n,
                      double min,
                      double max
                     )
{
  Stats *st = malloc(sizeof(Stats));
  check_mem(st);

  st->sum = sum;
  st->sumsq = sumsq;
  st->n = n;
  st->min = min;
  st->max = max;

  return st;

error:
  return NULL;
}

Stats *Stats_create()
{
  return Stats_recreate(0.0, 0.0, 0L, 0.0, 0.0);
}

double Stats_mean(Stats *st)
{
  return st->sum / st->n;
}

double Stats_stddev(Stats *st)
{
  return sqrt((st->sumsq - (st->sum * st->sum / st->n)) / (st->n - 1));
}

void Stats_sample(Stats *st, double s)
{
  st->sum += s;
  st->sumsq += s * s;

  if (st->n == 0) {
    st->min = s;
    st->max = s;
  } else {
    if (st->min > s) { st->min = s; }
    if (st->max < s) { st->max = s; }
  }

  st->n += 1;
}

char *Stats_dump(Stats *st)
{
  char *summary = NULL;
  int rc = asprintf(&summary,
                    "sum: %.2f\n"
                    "sumsq: %.2f\n"
                    "n: %ld\n"
                    "min: %.2f\n"
                    "max: %.2f\n"
                    "mean: %.2f\n"
                    "stddev: %.2f\n",
                    st->sum,
                    st->sumsq,
                    st->n,
                    st->min,
                    st->max,
                    Stats_mean(st),
                    Stats_stddev(st)
                   );

  check(rc > 0, "Failed to build summary");

  return summary;

error:
  return NULL;
}
