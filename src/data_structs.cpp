#include "filesys.h"

/**
 * ----------------------------------------------------------
 *                      NODE
 * ----------------------------------------------------------
 */

Node ::Node(string n_name, int n_size, time_t n_timestamp) {
  name = n_name;
  size = n_size;
  timestamp = n_timestamp;
  map<string, Node> n_children;
  children = n_children;
}

bool Node ::add_child(string n_name, int n_size, time_t n_timestamp) {
  Node new_node(n_name, n_size, n_timestamp);
  pair<string, Node> new_pair(name, new_node);
  children.insert(new_pair);
  return true;
}

/**
 * ----------------------------------------------------------
 *                     LDISK
 * ----------------------------------------------------------
 */

LDisk ::LDisk(int n_block_size, int n_max_size) {
  b_size = n_block_size;
  max_size = n_max_size;
  vector<bool> n_blocks;
  blocks = n_blocks;
}

void LDisk ::set_avail(int loc, bool availability) {
  blocks[loc] = availability;
}

/**
 * ----------------------------------------------------------
 *                    LFILE
 * ----------------------------------------------------------
 */

LFile ::LFile() {
  vector<FileBlock> n_blocks;
  blocks = n_blocks;
}

void LFile ::add_fb(int block_size, void *ptr) {
  FileBlock fb = {block_size, ptr};
  blocks.push_back(fb);
}