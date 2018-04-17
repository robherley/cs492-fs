#include "filesys.h"

/**
 * ----------------------------------------------------------
 *                      NODE 
 * ----------------------------------------------------------
 */

Node :: Node(string name, int size, time_t ts, bool folder) {
   _name = name; 
   _size = size;
   _timestamp = ts;
   _folder = folder;
   map<string, Node> children;
   _children = children;
}

bool Node :: add_child(string name, int size, time_t ts, bool folder) {
    Node new_node(name, size, ts, folder);
    pair<string, Node>new_pair(name,new_node);
    _children.insert(new_pair);
    return true;
}

/**
 * ----------------------------------------------------------
 *                     LDISK 
 * ----------------------------------------------------------
 */

LDisk :: LDisk(int block_size, int max_size) {
    _b_size = block_size;
    _max_size = max_size;
    vector<bool> blocks;
    _blocks = blocks;
}

void LDisk :: set_avail(int loc, bool availability) {
        _vector[loc] = availability;
}

/**
 * ----------------------------------------------------------
 *                    LFILE 
 * ----------------------------------------------------------
 */
LFile :: LFile() {
    vector<struct FileBlock> file_linked_list;
    _blocks = file_linked_list;
}


void LFile :: add_fb(int block_size, void* ptr) {
    struct FileBlock fb = {
        .b_size = block_size;
        ._start = ptr;
    }
    _blocks.push_back(fb);
}