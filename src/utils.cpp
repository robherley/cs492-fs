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
void add_file_from_root(Node *root, queue<string> path, int n_blocks,
                        int leftover, LDisk &ldisk) {
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
  if (curr->has_file(path.front())) {
    cout << "create: error: the file '" << path.front() << "' already exists!";
  } else {
    vector<int> alloc_blocks = ldisk.alloc(n_blocks);
    if (alloc_blocks.size()) {
      if (alloc_blocks.at(0) == -1) {
        cout << "create: error: " << path.front()
             << " could not be added. (Out of Space)" << endl;
      }
      curr->add_file(path.front(), leftover, alloc_blocks);
    } else {
      // We are creating an empty file of size 0
      curr->add_file(path.front(), leftover, alloc_blocks);
    }
  }
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

/**
 * Given the root node, prints all the meta data about the files in the system.
 */
void print_file_info(Node *node, tuple<string, string, int, int> args) {
  for (auto &file : node->files) {
    cout << YELLOW << left << setw(25) << file.first << RES;
    if ((file.second)->leftover == 0) {
      cout << GREEN << right << setw(5)
           << (file.second)->l_file.size() * get<3>(args) << "B  " << RES;
    } else if ((file.second)->l_file.size()) {
      cout << GREEN << right << setw(5)
           << (((file.second)->l_file.size() - 1) * get<3>(args)) +
                  (file.second)->leftover
           << "B  " << RES;
    } else {
      cout << GREEN << right << setw(5) << "0"
           << "B  " << RES;
    }
    char human_ts[16];
    strftime(human_ts, 16, "%b %d %T", localtime(&(file.second)->timestamp));
    cout << MAGENTA << left << setw(17) << human_ts << RES;
    if ((file.second)->l_file.size()) {
      int edge = (file.second)->l_file.at(0);
      for (int i = 0; i < (file.second)->l_file.size(); i++) {
        int block = (file.second)->l_file.at(i);
        // Last index
        if (i == (file.second)->l_file.size() - 1)
          cout << '(' << edge << " -> " << block << ") ";
        // Block->block is not continuous
        else if (((file.second)->l_file.at(i + 1) - block) != 1) {
          cout << '(' << edge << " -> " << block << ") ";
          edge = (file.second)->l_file.at(i + 1);
        }
      }
      cout << endl;
    } else {
      cout << "(No Allocations)" << endl;
    }
  }
  for (auto &dir : node->dirs) {
    print_file_info(dir.second, args);
  }
}

/**
 * Given a node, find the sum of all the other node files that have leftover
 * bytes minus the block size.
 */
int get_fragmentation(Node *node, tuple<string, string, int, int> args) {
  int total_frag = 0;
  for (auto &file : node->files) {
    if ((file.second)->leftover)
      total_frag += (get<3>(args) - (file.second)->leftover);
  }
  for (auto &dir : node->dirs) {
    total_frag += get_fragmentation(dir.second, args);
  }
  return total_frag;
}