#include "filesys.h"

/**
 * Simple function to split a string by a delimiting character and return a
 * vector of strings.
 */
queue<string> split(string to_split, char delim) {
  stringstream full(to_split);
  queue<string> string_queue;

  string part;
  while (getline(full, part, delim))
    string_queue.push(part);

  return string_queue;
}

/**
 * Given a path, traverse the tree and add folders where necessary.
 */
void add_dir(Node *root, queue<string> path) {
  if (path.front().compare(".")) {
    cerr << "Error: Invalid Directory in dir_list.txt!" << endl;
    exit(1);
  }

  // Remove root directory from path
  path.pop();

  Node *curr = root;
  // Loop over rest of queue, add dirs for each
  while (!path.empty()) {
    if (!curr->has_child(path.front())) {
      // If the folder does not exist, add it.
      curr->add_child(path.front(), 0, 0);
    }
    // Set new curr to the next node
    curr = curr->child_dirs.at(path.front());
    // Pop the used dir
    path.pop();
  }
}