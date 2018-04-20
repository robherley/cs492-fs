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
 * Adds a file child to the node.
 */
void Node::add_file(string f_name, int f_size, time_t f_timestamp) {
  time(&f_timestamp);
  File *file = new File(f_name, f_size, f_timestamp);
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
  for (auto &file : node->files) {
    bool last = counter + 1 == node->files.size();
    bool no_dirs = !(node->dirs.size());
    for (auto b : pipes)
      os << (b ? "    " : "│   ");
    if (last && no_dirs)
      os << "└── ";
    else
      os << "├── ";
    char human_ts[13];
    strftime(human_ts, 13, "%b %d %R", localtime(&(file.second)->timestamp));
    os << YELLOW << file.first << GREEN << " [Size: " << (file.second)->size
       << ", TS: " << human_ts << "]" << RES << endl;
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

/**
 * ----------------------------------------------------------
 *                      FILE
 * ----------------------------------------------------------
 */

File::File(string f_name, int f_size, time_t f_timestamp) {
  name = f_name;
  size = f_size;
  timestamp = f_timestamp;
}