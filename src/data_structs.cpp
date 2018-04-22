#include "filesys.h"

/**
 * ----------------------------------------------------------
 *                      NODE
 * ----------------------------------------------------------
 */

/**
 * Constructor
 */
Node::Node(string n_name) { name = n_name; }

/**
 * Destructor
 */
Node::~Node() {
  for (auto &child : dirs) {
    delete child.second;
  }
  for (auto &file : files) {
    delete file.second;
  }
}

/**
 * Adds a dir child to the node.
 */
void Node::add_dir(string n_name) {
  Node *node = new Node(n_name);
  pair<string, Node *> new_pair(n_name, node);
  dirs.insert(new_pair);
}

/**
 * Simple method to check if a node has a dir within it.
 */
bool Node::has_dir(string child_name) {
  return dirs.find(child_name) != dirs.end();
}

/**
 * Simple method to check if a node has a dir within it.
 */
bool Node::has_file(string child_name) {
  return files.find(child_name) != files.end();
}

/**
 * Adds a file child to the node.
 */
void Node::add_file(string f_name, int leftover, vector<int> alloc_blocks) {
  time_t f_timestamp;
  time(&f_timestamp);
  File *file = new File(f_name, leftover, f_timestamp, alloc_blocks);
  pair<string, File *> new_pair(f_name, file);
  files.insert(new_pair);
}

/**
 * Checks if Node has no files or folders
 */
bool Node::is_empty() { return !(files.size() + dirs.size()); }

/**
 * Helper to check a node's child_dirs and pretty print them.
 * Like the 'tree' command but better because it has colors and meta-data
 */
void print_child_dirs(ostream &os, Node *node, vector<bool> pipes) {
  unsigned counter = 0;
  unsigned f_counter = 0;
  for (auto &file : node->files) {
    bool last = f_counter + 1 == node->files.size();
    bool no_dirs = !(node->dirs.size());
    for (auto b : pipes)
      os << (b ? "    " : "│   ");
    if (last && no_dirs)
      os << "└── ";
    else
      os << "├── ";
    char human_ts[13];
    strftime(human_ts, 13, "%b %d %R", localtime(&(file.second)->timestamp));
    os << YELLOW << file.first << GREEN
       << " [Blocks: " << (file.second)->l_file.size() << ", TS: " << human_ts
       << "]" << RES << endl;
    f_counter++;
  }
  if (node->dirs.size() != 0) {
    for (auto &child : node->dirs) {
      vector<bool> pipes_copy = pipes;
      bool last = counter + 1 == node->dirs.size();
      for (auto b : pipes)
        os << (b ? "    " : "│   ");
      if (last)
        os << "└── ";
      else
        os << "├── ";
      os << BLUE << child.first << GREEN << " [Children: "
         << (child.second)->dirs.size() + (child.second)->files.size() << "]"
         << RES << endl;
      pipes_copy.push_back(last);
      print_child_dirs(os, child.second, pipes_copy);
      counter++;
    }
  }
}

/**
 * Overload to print node pointers
 */
ostream &operator<<(ostream &os, Node *node) {
  os << BLUE << node->name << GREEN << " [Children: " << node->dirs.size()
     << "]" << RES << endl;
  vector<bool> pipes;
  print_child_dirs(os, node, pipes);
  os.flush();
  return os;
}

/**
 * ----------------------------------------------------------
 *                     LDISK
 * ----------------------------------------------------------
 */

/**
 * Constructs a LDisk based on disk size and number of blocks
 */
LDisk::LDisk(int disk, int block) {
  disk_size = disk;
  block_size = block;
  num_blocks = disk / block;
  // Init the ldisk with false value (to represent empty)
  blocks.reserve(num_blocks);
  blocks.resize(num_blocks);
  fill(blocks.begin(), blocks.end(), false);
}

/**
 * Allocated memory in LDisk based on the blocks required. Returns a vector of
 * ints representing the blocks that were allocated.
 */
vector<int> LDisk::alloc(int blocks_wanted) {
  int curr_block = 0;
  vector<int> reserved_blocks;
  // Loop until we reserve all the blocks or we run out of blocks
  while ((curr_block != num_blocks) && (blocks_wanted != 0)) {
    // If the current block is open
    if (!blocks.at(curr_block)) {
      blocks.at(curr_block) = true;
      reserved_blocks.push_back(curr_block);
      blocks_wanted--;
    }
    curr_block++;
  }
  // If we didn't get to reserve all the blocks, and we ran out of memory
  if (blocks_wanted) {
    cout << "Out of memory." << endl;
    // Reset all the blocks we attempted to allocate
    for (int block : reserved_blocks)
      blocks.at(block) = false;
    reserved_blocks.clear();
    // Put in an invalid num to designate we could not allocate
    reserved_blocks.push_back(-1);
  }
  return reserved_blocks;
}

/**
 * Overload to print of ldisk nodes
 */
ostream &operator<<(ostream &os, LDisk disk) {
  // Cool Pretty Printing For Disk Allocations
  for (int i = 0; i < disk.blocks.size(); i++) {
    cout << (disk.blocks.at(i) ? RED : GREEN) << "◼";
    if (!((i + 1) % 80))
      cout << '\n';
  }
  cout << RES << endl;
  int rel = 0;
  for (int i = 0; i < disk.num_blocks; i++) {
    string status = disk.blocks.at(i) ? "In use: " : "Free: ";
    if (i == disk.num_blocks - 1) {
      if (rel == i)
        cout << status << rel << endl;
      else
        cout << status << rel << "-" << i << endl;
    } else if (disk.blocks.at(i) != disk.blocks.at(i + 1)) {
      if (rel == i)
        cout << status << rel << endl;
      else
        cout << status << rel << "-" << i << endl;
      rel = i + 1;
    }
  }
  os.flush();
  return os;
}

/**
 * ----------------------------------------------------------
 *                      FILE
 * ----------------------------------------------------------
 */

File::File(string f_name, int f_leftover, time_t f_timestamp, vector<int> ab) {
  name = f_name;
  leftover = f_leftover;
  timestamp = f_timestamp;
  l_file = ab;
}