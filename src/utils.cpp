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
 * I put so much work into the code below, to only succumb to the complexity of
 * all the edge cases of the collapseable linked list. May this wall of
 * commented code be a memorial to my fruitless effort.
 */

// /**
//  * Given a node in our LDisk list, find the next available node.
//  * Or, return the nullptr
//  */
// LDisk *find_next_free(LDisk *node) {
//   LDisk *curr = node;
//   for (; curr && !(curr->is_free); curr = curr->next)
//     ;
//   return curr;
// }

// void delete_from_addr(LDisk *head, int addr) {}

// /**
//  * Search over all the nodes, alloc memory when can available.
//  * Returns the vector of int of blocks that were allocated for that file
//  */
// vector<pair<int, int>> alloc(LDisk *head, int blocks_needed) {
//   vector<pair<int, int>> alloc_blocks;
//   while (blocks_needed != 0) {
//     LDisk *curr = find_next_free(head);
//     if (curr == nullptr) {
//       cout << "Out of space" << endl;
//       for (auto &addr : alloc_blocks) {
//         delete_from_addr(head, addr);
//       }
//       return alloc_blocks;
//     }
//     // See how many available blocks are in the node (add one bc zero
//     indexed) int blocks_available = (curr->end - curr->begin) + 1;
//     // Find how much we will allocated (if we have enough in our current
//     block) int will_alloc = min(blocks_available, blocks_needed); cout <<
//     "Blocks available: " << blocks_available << endl; cout << "Blocks needed:
//     " << blocks_needed << endl; blocks_needed -= will_alloc; cout << "Next
//     Needed: " << blocks_needed << endl; if (curr->begin == 0) { // Node is at
//     front (we are only singly linked)
//       if (will_alloc == blocks_available) { // If our node is the perfect
//       size
//         curr->is_free = false;
//       } else { // If we need to slice our node
//         // Make a new pointer starting at the allocation block size
//         LDisk *new_block = new LDisk(will_alloc, curr->end, true,
//         curr->next);
//         // Change the beginning node to end our allocation size
//         curr->end = will_alloc - 1; // Again, minus 1 bc zero indexed
//         // Show that this memory is now used
//         curr->is_free = false;
//         // Set the next node to our newly created leftover block
//         curr->next = new_block;
//       }
//       alloc_blocks.push_back(curr->begin, curr->end);
//     } else {
//       // TODO
//       cout << "Something went wrong" << endl;
//     }
//   }
//   return alloc_blocks;
// }
