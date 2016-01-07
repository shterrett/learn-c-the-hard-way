#ifndef _server_h
#define _server_h

#include "stats.h"
#include "hashmap.h"

Hashmap *Server_initialize();
int Server_create_stat(Hashmap *store, char *name);
int Server_add_sample(Hashmap *store, char *name, double sample);
double Server_return_mean(Hashmap *store, char *name);
double Server_return_stddev(Hashmap *store, char *name);
char *Server_dump_stats(Hashmap *store, char *name);

#endif
