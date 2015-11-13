#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
  int row_id;
  int set;
  int name_len;
  int email_len;
  char *name;
  char *email;
};

struct Address new_address(int row_id) {
  struct Address addr = { .row_id = row_id,
                          .set = 0,
                          .name_len = 1,
                          .email_len = 1,
                          .name = "\0",
                          .email = "\0"
                        };
  return addr;
}

struct Database {
  int max_data;
  int max_rows;
  struct Address *rows;
};

struct Connection {
  FILE *file;
  struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{
  if (errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }
  Database_close(conn);
  exit(1);
}

void Address_print(struct Address *addr)
{
  printf("%d %s %s\n", addr->row_id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
  int rc = fread(&conn->db->max_data, sizeof(int), 1, conn->file);
  rc += fread(&conn->db->max_rows, sizeof(int), 1, conn->file);

  if (rc != 2) {
    die("Failed to load database config", conn);
  }

  conn->db->rows = malloc(conn->db->max_rows * sizeof(struct Address));
  for(int i = 0; i < conn->db->max_rows; i++) {
    struct Address *row = &conn->db->rows[i];
    rc += fread(&row->row_id, sizeof(int), 1, conn->file);
    rc += fread(&row->set, sizeof(int), 1, conn->file);
    rc += fread(&row->name_len, sizeof(int), 1, conn->file);
    rc += fread(&row->email_len, sizeof(int), 1, conn->file);

    row->name = malloc(conn->db->max_data);
    row->email = malloc(conn->db->max_data);

    rc += fread(row->name, row->name_len, 1, conn->file);
    rc += fread(row->email, row->email_len, 1, conn->file);
  }

  if (rc != 2 + 6 * conn->db->max_rows) {
    die("Failed to load database", conn);
  }
}

struct Connection *Database_open(const char *filename, char mode)
{
  struct Connection *conn = malloc(sizeof(struct Connection));
  if (!conn) {
    die("Memory error", conn);
  }

  conn->db = malloc(sizeof(struct Database));
  if (!conn->db) {
    die("Memory error", conn);
  }

  if (mode == 'c') {
    conn->file = fopen(filename, "w");
  } else {
    conn->file = fopen(filename, "r+");

    if (conn->file) {
      Database_load(conn);
    }
  }

  if (!conn->file) {
    die("Failed to open the file", conn);
  }

  return conn;
}

void Database_close(struct Connection *conn)
{
  if (conn) {
    if (conn->file) {
      fclose(conn->file);
    }
    if (conn->db) {
      free(conn->db);
    }

    free(conn);
  }
}

void Database_write(struct Connection *conn)
{
  rewind(conn->file);

  int rc = fwrite(&conn->db->max_data, sizeof(int), 1, conn->file);
  rc += fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file);

  // write each address in the rows
  struct Address addr;
  for (int i = 0; i < conn->db->max_rows; i++) {
    addr = conn->db->rows[i];
    rc += fwrite(&addr.row_id, sizeof(int), 1, conn->file);
    rc += fwrite (&addr.set, sizeof(int), 1, conn->file);
    rc += fwrite(&addr.name_len, sizeof(int), 1, conn->file);
    rc += fwrite(&addr.email_len, sizeof(int), 1, conn->file);
    rc += fwrite(addr.name, addr.name_len, 1, conn->file);
    rc += fwrite(addr.email, addr.email_len, 1, conn->file);
  }

  if (rc != (6 * conn->db->max_rows + 2)) {
    die("Failed to write database", conn);
  }

  rc = fflush(conn->file);
  if (rc == -1) {
    die("Cannot flush database.", conn);
  }
}

void Database_create(struct Connection *conn, int max_rows, int max_data)
{
  conn->db->max_rows = max_rows;
  conn->db->max_data = max_data;
  conn->db->rows = malloc(max_rows * sizeof(struct Address));

  int i = 0;
  for (i = 0; i < max_rows; i++) {
    conn->db->rows[i] = new_address(i);
  }
}

void Database_set(struct Connection *conn,
                  int row_id,
                  const char *name,
                  const char *email
                 )
{
  struct Address *addr = &conn->db->rows[row_id];
  if (addr->set) {
    die("Already set; delete it first", conn);
  }

  addr->set = 1;

  int name_len = strlen(name);
  int email_len = strlen(email);

  if (name_len > conn->db->max_data ||
      email_len > conn->db->max_data
     ) {
    die("Exceeded data length limit", conn);
  }

  free(addr->name);
  free(addr->email);
  addr->name_len = name_len;
  addr->name = malloc(name_len);
  addr->email_len = email_len;
  addr->email = malloc(email_len);

  char *res = strncpy(addr->name, name, name_len);
  if (!res) {
    die("Name copy failed", conn);
  }

  res = strncpy(addr->email, email, email_len);
  if (!res) {
    die("Email copy failed", conn);
  }
}

void Database_get(struct Connection *conn, int row_id)
{
  struct Address *addr = &conn->db->rows[row_id];

  if (addr->set) {
    Address_print(addr);
  } else {
    die("ID is not set", conn);
  }
}

void Database_delete(struct Connection *conn, int row_id)
{
  struct Address curr = conn->db->rows[row_id];
  free(curr.email);
  free(curr.name);

  conn->db->rows[row_id] = new_address(row_id);
}

void Database_list(struct Connection *conn)
{
  struct Database *db = conn->db;

  for (int i = 0; i < conn->db->max_rows; i++) {
    struct Address *cur = &db->rows[i];

    if (cur->set) {
      Address_print(cur);
    }
  }
}

void Database_find(struct Connection *conn, const char *field, const char *query)
{
  struct Address *found;
  int matched = 0;
  int i = 0;
  while (!matched && i < conn->db->max_rows) {
    struct Address curr = conn->db->rows[i];
    char *query_result = "";
    if (strncmp(field, "name", conn->db->max_data) == 0) {
      query_result = curr.name;
    } else if (strncmp(field, "email", conn->db->max_data) == 0) {
      query_result = curr.email;
    } else {
      die("Field not present", conn);
    }

    if (strncmp(query, query_result, conn->db->max_data) == 0) {
      found = &curr;
      matched = 1;
    }
    i++;
  }

  if (matched) {
    Address_print(found);
  } else {
    printf("No record matching %s = %s\n", field, query);
  }
}

void guard_row_count(int row_id, struct Connection *conn) {
  if (row_id >= conn->db->max_rows) {
    die("There are not that many records", conn);
  }
}

int main(int argc, char *argv[])
{
  if (argc < 3) {
    die("USAGE: ex17 <dbfile> <action> [action params]", NULL);
  }

  char *filename = argv[1];
  char action = argv[2][0];
  struct Connection *conn = Database_open(filename, action);
  int row_id = 0;

  if (argc > 3) {
    row_id = atoi(argv[3]);
  }

  switch (action) {
    case 'c': {
      if (argc != 5) {
        die("Must provide max_rows and max_data", conn);
      }
      int max_rows = atoi(argv[3]);
      int max_data = atoi(argv[4]);
      Database_create(conn, max_rows, max_data);
      Database_write(conn);
      break;
    }
    case 'g':
      if (argc != 4) {
        die("Need an row_id to get", conn);
      }
      guard_row_count(row_id, conn);
      Database_get(conn, row_id);
      break;
    case 's':
      if (argc != 6) {
        die("Need row_id, name, email to set", conn);
      }
      guard_row_count(row_id, conn);
      Database_set(conn, row_id, argv[4], argv[5]);
      Database_write(conn);
      break;
    case 'd':
      if (argc != 4) {
        die("Need row_id to delete", conn);
      }

      Database_delete(conn, row_id);
      Database_write(conn);
      break;
    case 'l':
      Database_list(conn);
      break;
    case 'f': {
      if (argc != 5) {
        die("Need field and query", conn);
      }
      char *field = argv[3];
      char *query = argv[4];
      Database_find(conn, field, query);
      break;
    }
    default:
      die("Invalid action: c=create, g=get, s=set, d=delete, l=list", conn);
  }

  Database_close(conn);

  return 0;
}
