#include "filesys.h"

stack<Node> cd(stack<Node> *path, string dir) {
  if (dir.empty()) {
    // cd to top
    while (path->size())
      // empty stack represents top level
      path->pop();
  } else if (dir == "..") {
    path->pop();
  } else {
    Node curr_dir = path->top();
    // check if dir is a valid child of curr dir
    string childName = curr_dir.children.at(dir).name;
    if (childName.empty()) {
      Node child = curr_dir.children.at(dir);
      path->push(child);
    }
  }
  return *path;
}

void ls(stack<Node> *path) {
  // This func takes in the stack so 
  // all function args will more consistent
  map<string, Node>children = path->top().children;
  for (map<string,Node>::iterator it=children.begin(); it!=children.end(); ++it)
    cout << "\t" << it->second.name << endl;
}

void mkdir(stack<Node> *path, string dir_name) {
  Node curr_dir = path->top();
  curr_dir.add_child(dir_name, 0);
}

void create(stack<Node> *path, string file_name) {
  Node curr_dir = path->top();
}