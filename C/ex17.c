#include <assert.h>
#include <complex.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define MAX_DATA 512
#define MAX_ROWS 100

// 一个地址结构体
struct Address {
  int id;
  int set;
  char name[MAX_DATA];
  char email[MAX_DATA];
};

// 一个数据库结构体，里面有很多地址
struct Database {
  struct Address rows[MAX_ROWS];
};

// 一个连接结构体，用于连接文件和内存中的数据库
struct Connection {
  FILE *file;
  struct Database *db;
};

// 报错函数
void die(const char *messgae) {
  if (errno) {
    perror(messgae);
  } else {
    printf("ERROR: %s\n", messgae);
  }

  exit(1);
}

// 打印地址
void Address_print(struct Address *addr) {
  printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

// 读取数据
void Database_load(struct Connection *conn) {
  // 读取文件到内存
  int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
  if (rc != 1)
    die("Failed to load database");
}

// 打开数据库
struct Connection *Database_open(const char *filename, char mode) {
  // 申请内存
  struct Connection *conn = malloc(sizeof(struct Connection));
  if (!conn)
    die("Memory error");
  // 申请数据库内存
  conn->db = malloc(sizeof(struct Database));
  if (!conn->db)
    die("Memory error");
  if (mode == 'c') {
    // change 模式下设置为可写
    conn->file = fopen(filename, "w");
  } else {
    // 其他模式下可读
    conn->file = fopen(filename, "r+");
    if (conn->file) {
      Database_load(conn);
    }
  }
  if (!conn->file)
    die("Fail to open the file");

  return conn;
}

void Database_close(struct Connection *conn) {
  // 关闭数据库
  if (conn) {
    if (conn->file)
      fclose(conn->file);
    if (conn->db)
      free(conn->db);
    free(conn);
  }
}

void Database_write(struct Connection *conn) {
  // 将文件流指针指向文件开头
  rewind(conn->file);

  // 将数据库写到文件缓冲区中
  int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
  if (rc != 1)
    die("Failed to write database");
  // 文件写入磁盘
  rc = fflush(conn->file);
  if (rc == -1)
    die("Cannot flush database.");
}

void Database_create(struct Connection *conn) {
  int i = 0;
  // 创建数据库，为每条地址分配序号
  for (i = 0; i < MAX_ROWS; i++) {
    struct Address addr = {.id = i, .set = 0};
    conn->db->rows[i] = addr;
  }
}
// 设置第 id 条地址的内容
void Database_set(struct Connection *conn, int id, const char *name,
                  const char *email) {
  // 获取第 id 条数据的内存地址
  struct Address *addr = &conn->db->rows[id];
  // 如果没有被设置过
  if (addr->set)
    die("Already set, delete it first");
  // 标记为已经设置
  addr->set = 1;
  // 将字符串深度复制到地址中
  char *res = strncpy(addr->name, name, MAX_DATA);
  if (!res)
    die("Name copy failed");
  res = strncpy(addr->email, email, MAX_DATA);
  if (!res)
    die("Email copy failed");
}

// 获取第 id 条数据
void Database_get(struct Connection *conn, int id) {
  // 获取第 id 条数据
  struct Address *addr = &conn->db->rows[id];
  // 如果设置过则打印出来
  if (addr->set) {
    Address_print(addr);
  } else {
    die("ID is not set");
  }
}

// 删除第 id 条数据
void Database_delete(struct Connection *conn, int id) {
  // 将第 id 条数据标记为未设置
  struct Address addr = {.id = id, .set = 0};
  conn->db->rows[id] = addr;
}

// 打印出所有地址
void Database_list(struct Connection *conn) {
  int i = 0;
  // 获取数据库
  struct Database *db = conn->db;

  // 遍历数据库
  for (i = 0; i < MAX_ROWS; i++) {
    struct Address *cur = &db->rows[i];

    if (cur->set) {
      Address_print(cur);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3)
    die("Usage: ex17 <dbfile> <action> [action params]");

  char *filename = argv[1];
  char action = argv[2][0];
  struct Connection *conn = Database_open(filename, action);
  int id = 0;

  if (argc > 3)
    // 把字符串转为int
    id = atoi(argv[3]);
  if (id >= MAX_ROWS)
    die("There is not that many records");

  switch (action) {
  case 'c':
    Database_create(conn);
    Database_write(conn);
    break;
  case 'g':
    if (argc != 4)
      die("Need an id to get");
    Database_get(conn, id);
    break;
  case 's':
    if (argc != 6)
      die("Need id, name, email to set");
    Database_set(conn, id, argv[4], argv[5]);
    Database_write(conn);
    break;
  case 'd':
    if (argc != 4)
      die("Need id to delete");

    Database_delete(conn, id);
    Database_write(conn);
    break;
  case 'l':
    Database_list(conn);
    break;
  default:
    die("Invalid action, only: c=create, g=get, s=set, d=delete, l=list");
  }
  Database_close(conn);
  return 0;
}
