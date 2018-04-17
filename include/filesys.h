#ifndef FILESYS_H
#define FILESYS_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Node {
  string _name;
  int _size;
  int _timestamp;
  bool _folder;
  map<string,Node> _children;

public:
  Node(string, int, int, bool);
};

#endif
