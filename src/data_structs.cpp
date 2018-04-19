#include "filesys.h"

/**
 * ----------------------------------------------------------
 *                      NODE
 * ----------------------------------------------------------
 */

/**
 * Constructor
 */
Node::Node(string n_name, int n_size, time_t n_timestamp) {
  name = n_name;
  size = n_size;
  timestamp = n_timestamp;
}

/**
 * Destructor
 */
Node::~Node() {
  for (auto &child : child_dirs) {
    delete child.second;
  }
}

/**
 * Adds a dir child to the node.
 */
void Node::add_child(string n_name, int n_size, time_t n_timestamp) {
  Node *node = new Node(n_name, n_size, n_timestamp);
  pair<string, Node *> new_pair(n_name, node);
  child_dirs.insert(new_pair);
}

/**
 * Simple method to check if a node has a dir within it.
 */
bool Node::has_child(string child_name) {
  return child_dirs.find(child_name) != child_dirs.end();
}

/**
 * Helper to check a node's child_dirs and pretty print them.
 */
void print_child_dirs(ostream &os, Node *node, vector<bool> pipes) {
  unsigned counter = 0;
  for (auto &child : node->child_dirs) {
    vector<bool> pipes_copy = pipes;
    bool last = counter + 1 == node->child_dirs.size();
    for (auto b : pipes)
      os << (b ? "    " : "│   ");
    if (last)
      os << "└── ";
    else
      os << "├── ";
    os << BLUE << (child.second)->name << GREEN
       << " [Size: " << (child.second)->size
       << ", TS: " << (child.second)->timestamp
       << ", Children: " << (child.second)->child_dirs.size() << "]" << RES
       << endl;
    if ((child.second)->child_dirs.size() != 0) {
      pipes_copy.push_back(last);
      print_child_dirs(os, child.second, pipes_copy);
    }
    counter++;
  }
}

/**
 * Overload to print node pointers
 */
ostream &operator<<(ostream &os, Node *node) {
  os << BLUE << node->name << GREEN << " [Size: " << node->size
     << ", TS: " << node->timestamp << ", Children: " << node->child_dirs.size()
     << "]" << RES << endl;
  if (node->child_dirs.size() != 0) {
    vector<bool> pipes;
    print_child_dirs(os, node, pipes);
  }
  os.flush();
  return os;
}

/**
 * ----------------------------------------------------------
 *                     LDISK
 * ----------------------------------------------------------
 */

LDisk::LDisk(int n_block_size, int n_max_size) {
  b_size = n_block_size;
  max_size = n_max_size;
  vector<bool> n_blocks;
  blocks = n_blocks;
}

void LDisk::set_avail(int loc, bool availability) {
  blocks[loc] = availability;
}

/**
 * ----------------------------------------------------------
 *                    LFILE
 * ----------------------------------------------------------
 */

LFile::LFile() {
  vector<FileBlock> n_blocks;
  blocks = n_blocks;
}

void LFile::add_fb(int block_size, void *ptr) {
  FileBlock fb = {block_size, ptr};
  blocks.push_back(fb);
}