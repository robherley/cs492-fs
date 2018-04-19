#include "filesys.h"

/**
 * ----------------------------------------------------------
 *                      NODE
 * ----------------------------------------------------------
 */

Node::Node(string n_name, int n_size, time_t n_timestamp) {
  name = n_name;
  size = n_size;
  timestamp = n_timestamp;
}

void Node::add_child(string n_name, int n_size, time_t n_timestamp) {
  auto node = new Node(n_name, n_size, n_timestamp);
  pair<string, Node *> new_pair(n_name, node);
  children.insert(new_pair);
}

bool Node::has_child(string child_name) {
  return children.find(child_name) != children.end();
}

void print_children(ostream &os, Node *node, int level) {
  if (node->children.size() != 0) {
    unsigned counter = 0;
    for (auto child : node->children) {
      Node curr = *child.second;
      os << setw(level * 4) << setfill(' ') << "";
      if (counter + 1 == node->children.size())
        os << "└── ";
      else
        os << "├── ";
      os << &curr;
      if (curr.children.size() != 0) {
        cout << curr.children.size() << endl;
        // print_children(os, child.second, level + 1);
      }
      counter++;
    }
  }
}

ostream &operator<<(ostream &os, Node *node) {
  os << BLUE << node->name << GREEN << " [Size: " << node->size
     << ", TS: " << node->timestamp << "]" << RES << endl;
  print_children(os, node, 0);
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