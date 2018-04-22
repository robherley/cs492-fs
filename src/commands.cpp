#include "filesys.h"

stack<Node> Node ::cd(stack<Node> *path, string dir) {
  if (dir.empty()) {
    // cd to top
    while (path->size())
      // empty stack represents top level
      path->pop();
  } else if (dir == "..") {
    path->pop();
  }
  else {
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