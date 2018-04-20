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
void add_dir_from_root(Node *root, queue<string> path) {
  if (path.front().compare(".")) {
    cerr << "Error: Invalid Directory in dir_list.txt!" << endl;
    exit(1);
  }

  // Remove root directory from path
  path.pop();

  Node *curr = root;
  // Loop over rest of queue, add dirs for each
  while (!path.empty()) {
    if (!curr->has_dir(path.front())) {
      // If the folder does not exist, add it.
      curr->add_dir(path.front());
    }
    // Set new curr to the next node
    curr = curr->dirs.at(path.front());
    // Pop the used dir
    path.pop();
  }
}

/**
 * Given a path, traverse the tree and add files.
 */
void add_file_from_root(Node *root, queue<string> path, int size) {
  if (path.front().compare(".")) {
    cerr << "Error: Invalid Directory in file_list.txt!" << endl;
    exit(1);
  }

  // Remove root directory from path
  path.pop();

  Node *curr = root;
  // Loop over rest of queue, add dirs for each
  while (path.size() != 1) {
    // Set new curr to the next node
    curr = curr->dirs.at(path.front());
    // Pop the used dir
    path.pop();
  }
  curr->add_file(path.front(), size, 0);
}

/**
 * Get's the pointer of a directory given the absolute path
 * Make sure to enter in the right path or it will break
 */
Node *get_dir_ptr(Node *root, string path) {
  queue<string> q_path = split(path, '/');
  Node *curr = root;
  q_path.pop();
  while (!q_path.empty()) {
    curr = curr->dirs.at(q_path.front());
    q_path.pop();
  }
  return curr;
}