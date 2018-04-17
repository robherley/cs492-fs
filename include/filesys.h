#ifndef FILESYS_H
#define FILESYS_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Node {
public:
  string _name;
  int _size;
  time_t _timestamp;
  bool _folder;
  map<string, Node> _children;

  Node(string, int, time_t, bool);
  bool add_child(string, int, time_t, bool);
};

class LDisk {
  public:
  int _b_size;
  int _max_size;

  // vector iterator that represents used blocks in the disk
  vector<bool> _blocks;
  LDisk(int, int);
  void set_avail(int, bool);
};

class LFile {
  public:
  // vector or linked link storing the disk blocks that make up a file
  vector<struct FileBlock> _blocks;

  LFile();
  void add_fb(int, void*);
};

struct FileBlock {
  int _b_size;
  void* _start;
  // We dont need next because we can use iterator
}

// Setup
tuple<string, string, int, int> parse_args(int argc, char *const argv[]);
void print_usage();

#endif
