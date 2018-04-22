#ifndef FILESYS_H
#define FILESYS_H

#include <algorithm>
#include <csignal>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unistd.h>
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
  time_t timestamp;
  vector<int> l_file;
  int leftover; // this will represent the bytes in the last node block

  File(string, int, time_t, vector<int>);
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
  void add_file(string, int, vector<int>);
  bool has_file(string);
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
  vector<int> alloc(int);
  friend ostream &operator<<(ostream &, LDisk);
};

// Setup
tuple<string, string, int, int> parse_args(int, char *const[]);
void print_usage();
void construct_fs(Node *, tuple<string, string, int, int>, LDisk &);

// Utils
queue<string> split(string, char);
void add_dir_from_root(Node *, queue<string>);
void add_file_from_root(Node *, queue<string>, int, int, LDisk &);
Node *get_dir_ptr(Node *, string);
void print_file_info(Node *, tuple<string, string, int, int>);
int get_fragmentation(Node *, tuple<string, string, int, int>);

// Cli
void start_cli(Node *, tuple<string, string, int, int>, LDisk &);

#endif
