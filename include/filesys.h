#ifndef FILESYS_H
#define FILESYS_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/**
 * Nodes are folders, they have children of other folders or have files.
 */
class Node {
public:
  string name;
  int size;
  time_t timestamp;
  map<string, Node> children;
  // map<string, File> files;

  Node(string, int, time_t);
  bool add_child(string, int, time_t);
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
tuple<string, string, int, int> parse_args(int argc, char *const argv[]);
void print_usage();

// Commands
stack<Node> cd(stack<Node> *path, string dir);

#endif
