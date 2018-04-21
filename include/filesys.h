#ifndef FILESYS_H
#define FILESYS_H

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RES "\x1b[0m"

using namespace std;

/**
 * Holds meta data about files
 */
class File {
public:
  string name;
  int size;
  time_t timestamp;

  File(string, int, time_t);
};

/**
 * Nodes are folders, they have children of other folders and have files.
 */
class Node {
public:
  string name;
  map<string, Node *> dirs;
  map<string, File *> files;

  Node(string);
  ~Node();
  void add_dir(string);
  bool has_dir(string);
  void add_file(string, int, time_t);
  bool is_empty();
  friend ostream &operator<<(ostream &, Node *);
  void print_children(ostream &, Node *, int);
};

class LDisk {
public:
  int block_size;
  int disk_size;
  int num_blocks;
  vector<bool> blocks;

  LDisk(int, int);
  friend ostream &operator<<(ostream &, LDisk);
};

class LFile {
public:
};

// Setup
tuple<string, string, int, int> parse_args(int, char *const[]);
void print_usage();
void construct_fs(Node *, tuple<string, string, int, int>);

// Utils
queue<string> split(string, char);
void add_dir_from_root(Node *, queue<string>);
void add_file_from_root(Node *, queue<string>, int);
Node *get_dir_ptr(Node *, string);
LDisk *find_next_free(LDisk *);
vector<pair<int, int>> alloc(LDisk *, int);

// Cli
void start_cli(Node *, tuple<string, string, int, int>);

#endif
