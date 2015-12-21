#undef NDEBUG
#include <stdint.h>
#include "hashmap.h"
#include "dbg.h"
#include "bstrlib.h"

static int default_compare(void *a, void *b)
{
  return bstrcmp((bstring) a, (bstring) b);
}

/**
 * Bob Jenkin's hash algorithm
 */
static uint32_t default_hash(void *a)
{
  size_t len = blength((bstring) a);
  char *key = bdata((bstring) a);
  uint32_t hash = 0;
  uint32_t i = 0;

  for (hash = i = 0; i < len; ++i) {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash << 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash)
{
  Hashmap *map = calloc(1, sizeof(Hashmap));
  check_mem(map);

  map->compare = compare == NULL ? default_compare : compare;
  map->hash = hash == NULL ? default_hash : hash;
  map->buckets = DArray_create(sizeof(DArray *), DEFAULT_NUMBER_OF_BUCKETS);
  map->buckets->end = map->buckets->max;
  map->count = 0;
  check_mem(map->buckets);

  return map;

error:
  if(map) {
    Hashmap_destroy(map);
  }
  return NULL;
}

void Hashmap_destroy(Hashmap *map)
{
  int i = 0;
  int j = 0;

  if (map) {
    if (map->buckets) {
      for (i = 0; i < DArray_count(map->buckets); i++) {
        DArray *bucket = DArray_get(map->buckets, i);
        if (bucket) {
          for (j = 0; j < DArray_count(bucket); j++) {
            free(DArray_get(bucket, j));
          }
          DArray_destroy(bucket);
        }
      }
      DArray_destroy(map->buckets);
    }
    free(map);
  }
}

static inline HashmapNode *Hashmap_node_create(int hash, void *key, void *data)
{
  HashmapNode *node = calloc(1, sizeof(HashmapNode));
  check_mem(node);

  node->key = key;
  node->data = data;
  node->hash = hash;

  return node;

error:
  return NULL;
}

static inline List *Hashmap_find_bucket(Hashmap *map, void *key, int create, uint32_t *hash_out)
{
  uint32_t hash = map->hash(key);
  int bucket_n = hash % DArray_count(map->buckets);
  check(bucket_n >= 0, "Invalid bucket found: %d", bucket_n);
  *hash_out = hash;

  List *bucket = DArray_get(map->buckets, bucket_n);

  if (!bucket && create) {
    bucket = List_create();
    check_mem(bucket);
    DArray_set(map->buckets, bucket_n, bucket);
  }

  return bucket;

error:
  return NULL;
}

static inline HashmapNode *Hashmap_get_node(Hashmap *map, uint32_t hash, List *bucket, void *key);
void Hashmap_check_size_and_resize(Hashmap *map);
int Hashmap_set(Hashmap *map, void *key, void *data)
{
  uint32_t hash = 0;
  List *bucket = Hashmap_find_bucket(map, key, 1, &hash);
  check(bucket, "Error can't create bucket.");

  HashmapNode *node = Hashmap_get_node(map, hash, bucket, key);
  if (node) {
    node->data = data;
  } else {
    node = Hashmap_node_create(hash, key, data);
    check_mem(node);
    List_push(bucket, node);
    map->count++;
    Hashmap_check_size_and_resize(map);
  }

  return 0;

error:
  return -1;
}

static inline ListNode *Hashmap_bucket_list_node(Hashmap *map,
                                                 uint32_t hash,
                                                 List *bucket,
                                                 void *key
                                                )
{
  LIST_FOREACH(bucket, first, next, cur) {
    HashmapNode *node = cur->value;
    if (node->hash == hash && map->compare(node->key, key) == 0) {
      return cur;
    }
  }
  return NULL;
}

static inline HashmapNode *Hashmap_get_node(Hashmap *map, uint32_t hash, List *bucket, void *key)
{
  ListNode *list_node = Hashmap_bucket_list_node(map, hash, bucket, key);
  if (list_node) {
    return list_node->value;
  } else {
    return NULL;
  }
}

void *Hashmap_get(Hashmap *map, void *key)
{
  uint32_t hash = 0;
  List *bucket = Hashmap_find_bucket(map, key, 0, &hash);
  if (!bucket) { return NULL; }

  HashmapNode *node = Hashmap_get_node(map, hash, bucket, key);
  check(node != NULL, "Failed to get node from bucket when it should exist");

  return node->data;

error:
  return NULL;
}

int Hashmap_traverse(Hashmap *map, Hashmap_traverse_cb traverse_cb)
{
  int i = 0;
  int rc = 0;

  for (i = 0; i < DArray_count(map->buckets); i++) {
    List *bucket = DArray_get(map->buckets, i);
    if (bucket) {
      LIST_FOREACH(bucket, first, next, cur) {
        HashmapNode *node = cur->value;
        rc = traverse_cb(node);
        if (rc != 0) {
          return rc;
        }
      }
    }
  }

  return 0;
}

void *Hashmap_delete(Hashmap *map, void *key)
{
  uint32_t hash = 0;
  List *bucket = Hashmap_find_bucket(map,key, 0, &hash);
  if (!bucket) {
    return NULL;
  }
  ListNode *list_node = Hashmap_bucket_list_node(map, hash, bucket, key);

  if (list_node) {
    HashmapNode *node = List_remove(bucket, list_node);
    void *data = node->data;
    free(node);
    map->count--;
    Hashmap_check_size_and_resize(map);
    return data;
  } else {
    return NULL;
  }
}

int Hashmap_resize(Hashmap *map, int new_size)
{
  DArray *new_buckets = DArray_create(sizeof(DArray *), new_size);
  new_buckets->end = new_buckets->max;
  DArray *old_buckets = map->buckets;
  map->buckets = new_buckets;

  for (int i = 0; i < DArray_count(old_buckets); i++) {
    List *bucket = DArray_get(old_buckets, i);
    if (bucket) {
      LIST_FOREACH(bucket, first, next, cur) {
        HashmapNode *node = cur->value;
        map->count--;
        Hashmap_set(map, node->key, node->data);
      }
    }
  }
  DArray_clear_destroy(old_buckets);

  return new_size;
}

void Hashmap_check_size_and_resize(Hashmap *map)
{
  int original_size = DArray_count(map->buckets);
  int rc = 0;
  if (map->count > original_size / 2) {
    rc = Hashmap_resize(map, original_size * 2);
  } else if (map->count < original_size / 4 &&
             map->count > DEFAULT_NUMBER_OF_BUCKETS / 4
            ) {
    rc = Hashmap_resize(map, original_size / 2);
  }
}
