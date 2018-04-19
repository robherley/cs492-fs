#include "filesys.h"

/**
 * ----------------------------------------------------------
 *                      NODE
 * ----------------------------------------------------------
 */

Node ::Node(string n_name, int n_size) {
  name = n_name;
  size = n_size;
  timestamp = time(NULL);
  map<string, Node> n_children;
  children = n_children;
}

bool Node ::add_file(string n_name, int n_size) {
  File new_file(n_name);
  pair<string, File> new_pair(n_name, new_file);
  files.insert(new_pair);
  return true;
}

bool Node ::add_dir(string n_name, int n_size) {
  Node new_node(n_name, n_size);
  pair<string, Node> new_pair(n_name, new_node);
  dirs.insert(new_pair);
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

File ::File(string n_name) {
  vector<FileBlock> n_blocks;
  blocks = n_blocks;
  name = n_name;
}

void LFile ::add_fb(int block_size, void *ptr) {
  FileBlock fb = {block_size, ptr};
  blocks.push_back(fb);
}