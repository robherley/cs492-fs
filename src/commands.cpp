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
    string childName = curr_dir.dirs.at(dir).name;
    if (childName.empty()) {
      Node child = curr_dir.dirs.at(dir);
      path->push(child);
    }
  }
  return *path;
}

void ls(stack<Node> *path) {
  // This func takes in the stack so
  // all function args will more consistent
  map<string, Node> dirs = path->top().dirs;
  map<string, File> files = path->top().files;
  for (map<string, Node>::iterator it = dirs.begin(); it != dirs.end(); ++it)
    cout << "\t" << it->second.name << endl;
  for (map<string, File>::iterator it = files.begin(); it != files.end(); ++it)
    cout << "\t" << it->second.name << endl;
}

void mkdir(stack<Node> *path, string dir_name) {
  // Create a new node and append it to the .dirs member of Node
  Node curr_dir = path->top();
  curr_dir.add_dir(dir_name, 0);
}

void create(stack<Node> *path, string file_name) {
  // Create a new File and append it to the .files member of Node
  Node curr_dir = path->top();
  curr_dir.add_file(file_name, 0)
}

int append(stack<Node> *path, string file_name, int num_bits, int b_size) {
  Node curr_dir = path->top();
  File desired_file = curr_dir.files.at(file_name);
  // Check if this file exists
  if (!desired_file)
    cout << "This file doesn't exist in this folder" << endl;
  else {
    vector<FileBlock> fbs = desired_file.blocks;
    int last_fb_size = fbs[fbs.size()].size;
    // If there's enough space in the last file block
    if (b_size - last_fb_size >= num_bits) {
      last_fb_size += num_bits;
      // TODO: append here. Not really sure what to do yet
      return last_fb_size;
    }
  }
}

int remove(stack<Node> *path, string file_name, int num_bits) {
  Node curr_dir = path->top();
  File desired_file = curr_dir.files.at(file_name);
  // Check if this file exists
  if (!desired_file)
    cout << "This file doesn't exist in this folder" << endl;
  else {
    vector<FileBlock> fbs = desired_file.blocks;
    int last_fb_size = fbs[fbs.size()].size;
    // If there's enough space in the last file block
    if (last_fb_size >= num_bits) {
      last_fb_size -= num_bits;
      // TODO: append here. Not really sure what to do yet
      return last_fb_size;
    }
  }
}

void delete (stack<Node> *path, string name) {
  Node curr_dir = path->top();
  File desired_file = curr_dir.files.at(name);
  File desired_dir = curr_dir.dirs.at(name);
  // Check if this file exists
  if (desired_dir || desired_file) {
    if (desired_file)
      curr_dir.files.erase(name);
    else if (desired_dir)
      curr_dir.dirs.erase(name);
    curr_dir.timestamp = time(NULL);
  } else {
    cout << "No files or directories called '" << name
         << "'exist in this folder" << endl;
  }
}

vector<string> bfs(Node root) {

}