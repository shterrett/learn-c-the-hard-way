#include "bstrlib.h"
#include "server.h"

Hashmap *Server_initialize()
{
  Hashmap *store = Hashmap_create(NULL, NULL);
  check(store != NULL, "Could not create hashmap");
  return store;

error:
  return NULL;
}

int Server_create_stat(Hashmap *store, char *name)
{
  bstring b_name = bfromcstr(name);
  Stats *new_stat = Stats_create();
  check(new_stat != NULL, "Failed to create stat");
  int rc = Hashmap_set(store, b_name, new_stat);

  return rc;

error:
  if (b_name) { free(b_name); }
  return -1;
}

int Server_add_sample(Hashmap *store, char *name, double sample)
{
  bstring b_name = bfromcstr(name);
  Stats *stat = Hashmap_get(store, b_name);
  check(stat != NULL, "Could not find stat");
  Stats_sample(stat, sample);
  free(b_name);

  return 0;

error:
  if (b_name) { free(b_name); }
  return -1;
}

double Server_return_mean(Hashmap *store, char *name)
{
  bstring b_name = bfromcstr(name);
  Stats *stat = Hashmap_get(store, b_name);
  check(stat != NULL, "Could not find stat");

  free(b_name);

  return Stats_mean(stat);

error:
  if (b_name) { free(b_name); }
  return -1;
}

double Server_return_stddev(Hashmap *store, char *name)
{
  bstring b_name = bfromcstr(name);
  Stats *stat = Hashmap_get(store, b_name);
  check(stat != NULL, "Could not find stat");

  free(b_name);

  return Stats_stddev(stat);

error:
  if (b_name) { free(b_name); }
  return -1;
}

char *Server_dump_stats(Hashmap *store, char *name)
{
  bstring b_name = bfromcstr(name);
  Stats *stat = Hashmap_get(store, b_name);
  check(stat != NULL, "Could not find stat");

  free(b_name);

  return Stats_dump(stat);

error:
  if (b_name) { free(b_name); }
  return NULL;
}
