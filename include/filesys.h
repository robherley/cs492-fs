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
  string _name;
  int _size;
  int _timestamp;
  bool _folder;
  map<string, Node> _children;

public:
  Node(string, int, int, bool);
};

// Setup
tuple<string, string, int, int> parse_args(int argc, char *const argv[]);
void print_usage();

#endif
