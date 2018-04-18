#ifndef FILESYS_H
#define FILESYS_H

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
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
 * Nodes are folders, they have children of other folders or have files.
 */
class Node {
public:
  string name;
  int size;
  time_t timestamp;
  map<string, Node *> children;
  // map<string, File> files;

  Node(string, int, time_t);
  void add_child(string, int, time_t);
  bool has_child(string);
  friend ostream &operator<<(ostream &, Node *);
  void print_children(ostream &, Node *, int);
};

class LDisk {
public:
  int b_size;
  int max_size;

  // vector iterator that represents used blocks in the disk
  vector<bool> blocks;
  LDisk(int, int);
  void set_avail(int, bool);
};

struct FileBlock {
  int b_size;
  void *start;
  // We dont need next because we can use iterator
};

class LFile {
public:
  // vector or linked link storing the disk blocks that make up a file
  vector<FileBlock> blocks;

  LFile();
  void add_fb(int, void *);
};

// Setup
tuple<string, string, int, int> parse_args(int, char *const[]);
void print_usage();

// Utils
queue<string> split(string, char);
void add_dir(Node *, queue<string>);

#endif
