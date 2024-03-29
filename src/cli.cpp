#include "filesys.h"

/**
 * Converts our stack into a string.
 * (Very lazily)
 */
string cwd_to_string(stack<Node *> cwd) {
  stack<Node *> cwd_reversed;
  while (!cwd.empty()) {
    cwd_reversed.push(cwd.top());
    cwd.pop();
  }
  stringstream ss;
  while (!cwd_reversed.empty()) {
    ss << (cwd_reversed.top())->name << '/';
    cwd_reversed.pop();
  }
  return ss.str();
}

/**
 * Implementation of cd using a stack as the current directory.
 */
void cd(stack<Node *> &cwd, queue<string> &tokens) {
  if (tokens.empty()) { // Just cd is given, go to root
    while (cwd.size() != 1)
      cwd.pop();
    return;
  }
  queue<string> wanted_path = split(tokens.front(), '/');
  if (wanted_path.front() == "root") { // absolute path
    while (cwd.size() != 1)            // move to root
      cwd.pop();
    wanted_path.pop();
  }
  while (wanted_path.size()) {
    string to_dir = wanted_path.front();
    if (to_dir == "..") { // if we should move up a folder, pop the cwd stack
      if (cwd.size() == 1) {
        cout << "cd: error: root is the highest directory" << endl;
        break;
      } else {
        cwd.pop();
      }
    } else { // we need to go deeper
      if ((cwd.top())->has_dir(to_dir)) {
        cwd.push((cwd.top())->dirs.at(to_dir));
      } else { // if the folder we want doesn't exist
        cout << "cd: error: specified folder does not exist" << endl;
        break;
      }
    }
    wanted_path.pop();
  }
}

/**
 * Makes a directory node given either the relative path or absolute path
 */
void mkdir(Node *root, stack<Node *> &cwd, queue<string> &tokens) {
  // No input after mkdir
  if (tokens.empty()) {
    cout << "mkdir: error: no directory specified" << endl;
    return;
  }
  queue<string> wanted_path;
  queue<string> rel_path = split(tokens.front(), '/');
  if ((tokens.front() == "root") && (tokens.size() == 1)) {
    cout << "mkdir: error: folder already exists" << endl;
    return;
  }
  if (!rel_path.front().size()) {
    cout << "mkdir: invalid path specified" << endl;
    return;
  }
  if (rel_path.front() != "root") { // if we have a relative path
    queue<string> partial_path = split(cwd_to_string(cwd), '/');
    while (partial_path.size()) { // push all dirs of our cwd
      wanted_path.push(partial_path.front());
      partial_path.pop();
    }
    while (rel_path.size()) { // push all dirs of the relative path
      wanted_path.push(rel_path.front());
      rel_path.pop();
    }
  } else {
    wanted_path = rel_path;
  }
  // Remove root directory from path
  wanted_path.pop();

  Node *curr = root;
  // Loop over rest of queue, add dirs for each
  while (!wanted_path.empty()) {
    if (!curr->has_dir(wanted_path.front())) {
      // If the folder does not exist, add it.
      curr->add_dir(wanted_path.front());
    } else if (wanted_path.size() == 1) {
      // If we are at the folder we are trying to add and it already exists,
      // this is bad an should error
      cout << "mkdir: error: folder already exists" << endl;
    }
    // Set new curr to the next node
    curr = curr->dirs.at(wanted_path.front());
    // Pop the used dir
    wanted_path.pop();
  }
  return;
}

/**
 * Makes a file given either the relative path or absolute path
 */
void create(Node *root, stack<Node *> &cwd, queue<string> &tokens,
            LDisk &disk) {
  // No input after create
  if (tokens.empty()) {
    cout << "create: error: no file specified" << endl;
    return;
  }
  queue<string> wanted_path;
  queue<string> rel_path = split(tokens.front(), '/');
  if (!rel_path.front().size()) {
    cout << "create: invalid path specified" << endl;
    return;
  }
  if (rel_path.front() != "root") { // if we have a relative path
    queue<string> partial_path = split(cwd_to_string(cwd), '/');
    while (partial_path.size()) { // push all dirs of our cwd
      wanted_path.push(partial_path.front());
      partial_path.pop();
    }
    while (rel_path.size()) { // push all dirs of the relative path
      wanted_path.push(rel_path.front());
      rel_path.pop();
    }
  } else {
    wanted_path = rel_path;
  }
  add_file_from_root(root, wanted_path, 0, 0, disk);
}

/**
 * Deletes a file or folder given an absolute or relative path
 */
void delete_thing(Node *root, stack<Node *> &cwd, queue<string> &tokens,
                  LDisk &disk) {
  // No input after create
  if (tokens.empty()) {
    cout << "delete: error: no file/folder specified" << endl;
    return;
  }
  queue<string> wanted_path;
  queue<string> rel_path = split(tokens.front(), '/');
  if ((tokens.front() == "root") && (tokens.size() == 1)) {
    cout << "delete: error: you can't delete the root bc you don't have root "
         << "access and you never will. nice try, " << getenv("USER") << endl;
    return;
  }
  if (!rel_path.front().size()) {
    cout << "delete: invalid path specified" << endl;
    return;
  }
  if (rel_path.front() != "root") { // if we have a relative path
    queue<string> partial_path = split(cwd_to_string(cwd), '/');
    while (partial_path.size()) { // push all dirs of our cwd
      wanted_path.push(partial_path.front());
      partial_path.pop();
    }
    while (rel_path.size()) { // push all dirs of the relative path
      wanted_path.push(rel_path.front());
      rel_path.pop();
    }
  } else {
    wanted_path = rel_path;
  }
  Node *curr = root;
  wanted_path.pop(); // remove root
  while (wanted_path.size() != 1) {
    // if we can't get down the current folder
    if (!curr->has_dir(wanted_path.front())) {
      cout << "delete: error: invalid path specified" << endl;
      return;
    }
    // Set new curr to the next node
    curr = curr->dirs.at(wanted_path.front());
    // Pop the used dir
    wanted_path.pop();
  }
  // At this point, curr is equal to the directory containing the deletion

  // check for files first
  if (curr->has_file(wanted_path.front())) {
    auto blocks_to_free = (curr->files).at(wanted_path.front())->l_file;
    for (auto block : blocks_to_free)
      disk.blocks.at(block) = false;
    (curr->files).erase(wanted_path.front());
    return;
  }

  // check for folders next
  if (curr->has_dir(wanted_path.front())) {
    if ((curr->dirs).at(wanted_path.front())->is_empty()) {
      (curr->dirs).erase(wanted_path.front());
    } else {
      cout << "delete: error: the specified folder is not empty" << endl;
    }
    return;
  }
  cout << "delete: error: unable to delete specified paramater" << endl;
}

/**
 * Appends bytes to a given a file name
 */
void append(Node *root, stack<Node *> &cwd, queue<string> &tokens, LDisk &disk,
            tuple<string, string, int, int> args) {
  // No input after append
  if (tokens.empty()) {
    cout << "append: error: no file specified" << endl;
    return;
  }
  queue<string> wanted_path;
  queue<string> rel_path = split(tokens.front(), '/');

  tokens.pop();
  if (tokens.empty()) {
    cout << "append: error: no bytes specified" << endl;
    return;
  }
  int bytes_wanted;
  try {
    bytes_wanted = stoi(tokens.front());
  } catch (...) {
    cout << "append: error: invalid value for bytes" << endl;
    return;
  }

  if (!rel_path.front().size()) {
    cout << "append: error: invalid path specified" << endl;
    return;
  }
  if (rel_path.front() != "root") { // if we have a relative path
    queue<string> partial_path = split(cwd_to_string(cwd), '/');
    while (partial_path.size()) { // push all dirs of our cwd
      wanted_path.push(partial_path.front());
      partial_path.pop();
    }
  }
  // push all dirs of the path (besides the file)
  while (rel_path.size() - 1) {
    wanted_path.push(rel_path.front());
    rel_path.pop();
  }

  // Remove root
  wanted_path.pop();

  Node *curr = root;
  while (!wanted_path.empty()) {
    if (!curr->has_dir(wanted_path.front())) {
      cout << "append: error: specified file does not exist" << endl;
      return;
    }
    curr = (curr->dirs).at(wanted_path.front());
    wanted_path.pop();
  }

  if (!curr->has_file(rel_path.front())) {
    cout << "append: error: specified file does not exist" << endl;
    return;
  }

  File *wanted_file = curr->files.at(rel_path.front());
  int leftover_bac = wanted_file->leftover;
  int overflow = bytes_wanted % get<3>(args);
  int blocks_to_add = 0;
  if (wanted_file->leftover) { // if our file has leftover bytes
    // Find if have room for it in our last block
    int total_over = (overflow + wanted_file->leftover);
    if (total_over < get<3>(args)) {
      // If it fits in our block size
      wanted_file->leftover = total_over;
    } else if (total_over == get<3>(args)) {
      // If it's a perfect fit
      wanted_file->leftover = 0;
    } else {
      // It's greater than our block size, so we need to add a block
      blocks_to_add++;
      wanted_file->leftover = total_over % get<3>(args);
    }
  } else {
    wanted_file->leftover = overflow;
  }

  if (bytes_wanted < get<3>(args)) {
    // If the amount of bytes we are trying to add is less than our block size
    blocks_to_add++;
  } else {
    // Add the blocks we need
    blocks_to_add += ceil((float)(bytes_wanted) / (float)get<3>(args));
  }

  vector<int> added_blocks = disk.alloc(blocks_to_add);
  if (added_blocks.at(0) == -1) {
    // Our blocks could not be added (reset the leftover)
    wanted_file->leftover = leftover_bac;
    return;
  }

  // If everything worked out okay, add the blocks to our lfile
  for (auto ind : added_blocks)
    wanted_file->l_file.push_back(ind);

  // Update timestamp
  time(&wanted_file->timestamp);
}

/**
 * Remove a certain number of bytes from a given file
 */
void remove_thing(Node *root, stack<Node *> &cwd, queue<string> &tokens,
                  LDisk &disk, tuple<string, string, int, int> args) {
  // No input after remove
  if (tokens.empty()) {
    cout << "remove: error: no file specified" << endl;
    return;
  }
  queue<string> wanted_path;
  queue<string> rel_path = split(tokens.front(), '/');

  tokens.pop();
  if (tokens.empty()) {
    cout << "remove: error: no bytes specified" << endl;
    return;
  }
  int bytes_to_remove;
  try {
    bytes_to_remove = stoi(tokens.front());
  } catch (...) {
    cout << "remove: error: invalid value for bytes" << endl;
    return;
  }

  if (!rel_path.front().size()) {
    cout << "remove: error: invalid path specified" << endl;
    return;
  }
  if (rel_path.front() != "root") { // if we have a relative path
    queue<string> partial_path = split(cwd_to_string(cwd), '/');
    while (partial_path.size()) { // push all dirs of our cwd
      wanted_path.push(partial_path.front());
      partial_path.pop();
    }
  }
  // push all dirs of the path (besides the file)
  while (rel_path.size() - 1) {
    wanted_path.push(rel_path.front());
    rel_path.pop();
  }

  // Remove root
  wanted_path.pop();

  Node *curr = root;
  while (!wanted_path.empty()) {
    if (!curr->has_dir(wanted_path.front())) {
      cout << "remove: error: specified file does not exist" << endl;
      return;
    }
    curr = (curr->dirs).at(wanted_path.front());
    wanted_path.pop();
  }

  if (!curr->has_file(rel_path.front())) {
    cout << "remove: error: specified file does not exist" << endl;
    return;
  }

  File *wanted_file = curr->files.at(rel_path.front());

  int file_size;
  if (wanted_file->leftover == 0) {
    file_size = wanted_file->l_file.size() * get<3>(args);
  } else if (wanted_file->l_file.size()) {
    file_size = ((wanted_file->l_file.size() - 1) * get<3>(args)) +
                wanted_file->leftover;
  } else {
    file_size = 0;
  }

  // If we are trying to remove more than we have
  if (file_size < bytes_to_remove) {
    cout << "remove: error: removing more bytes than present in file" << endl;
    return;
  }

  // If we remove the entire thing, dealloc everything
  if (file_size == bytes_to_remove) {
    for (auto n : wanted_file->l_file)
      disk.blocks.at(n) = false;
    wanted_file->l_file.clear();
    wanted_file->leftover = 0;
    // Update timestamp
    time(&wanted_file->timestamp);
    return;
  }

  if (bytes_to_remove <= wanted_file->leftover) {
    // If the amount to remove is less than our overflow in the current block
    wanted_file->leftover = (wanted_file->leftover - bytes_to_remove);
    if (bytes_to_remove == wanted_file->leftover) {
      // If it's equal, delete the end block
      disk.blocks.at(wanted_file->l_file.back()) = false;
      wanted_file->l_file.pop_back();
    }
    // Update timestamp
    time(&wanted_file->timestamp);
    return;
  } else if ((bytes_to_remove < get<3>(args)) && !wanted_file->leftover) {
    // We have a perfect block (end) and want to remove from it
    wanted_file->leftover = get<3>(args) - bytes_to_remove;
    if (bytes_to_remove == get<3>(args)) {
      // If it's equal, delete the end block
      disk.blocks.at(wanted_file->l_file.back()) = false;
      wanted_file->l_file.pop_back();
    }
    // Update timestamp
    time(&wanted_file->timestamp);
    return;
  } else {
    // We need to remove the overflow and blocks with it.
    bytes_to_remove += wanted_file->leftover; // add our leftover
    // Set our new leftover value
    wanted_file->leftover = bytes_to_remove % get<3>(args);
    unsigned blocks_to_remove =
        (bytes_to_remove - wanted_file->leftover) / get<3>(args);
    if (wanted_file->leftover) {
      // If we have a leftover, we need an extra block for it
      blocks_to_remove--;
    }
    while (blocks_to_remove) {
      // Del the last block, free the location in ldisk, and move to the next
      disk.blocks.at(wanted_file->l_file.back()) = false;
      wanted_file->l_file.pop_back();
      blocks_to_remove--;
    }
    // Update timestamp
    time(&wanted_file->timestamp);
  }
}

/**
 * Little prompt to grab the input. AND IT'S COLORFUL
 */
string prompt(stack<Node *> cwd) {
  cout << '\n' << BLUE << getenv("USER") << YELLOW << " | " << GREEN;
  cout << cwd_to_string(cwd) << endl;
  cout << MAGENTA << "$ " << RES;
  string input;
  getline(cin, input);
  return input;
}

/**
 * Our main program loop. Handles user input based on assignment commands.
 */
void start_cli(Node *root, tuple<string, string, int, int> args, LDisk &disk) {
  signal(SIGINT, SIG_IGN);
  map<string, int> cmds = {{"cd", 0},      {"ls", 1},     {"mkdir", 2},
                           {"create", 3},  {"append", 4}, {"remove", 5},
                           {"delete", 6},  {"exit", 7},   {"dir", 8},
                           {"prfiles", 9}, {"prdisk", 10}};
  stack<Node *> cwd;
  cwd.push(root);
  bool running = true;
  while (running) {
    // Grab our input, split it into a queue of tokens
    queue<string> tokens = split(prompt(cwd), ' ');
    if (cmds.find(tokens.front()) == cmds.end()) {
      cout << "fs: command not found: " << tokens.front() << endl;
      continue;
    }
    int cmd = cmds.at(tokens.front());
    tokens.pop();
    switch (cmd) {
    // cd
    case 0:
      cd(cwd, tokens);
      break;
    // ls
    case 1:
      for (auto &dir : (cwd.top())->dirs)
        cout << GREEN << dir.first << RES << endl;
      for (auto &file : (cwd.top())->files)
        cout << YELLOW << file.first << RES << endl;
      break;
    // mkdir
    case 2:
      mkdir(root, cwd, tokens);
      break;
    // create
    case 3:
      create(root, cwd, tokens, disk);
      break;
    // append
    case 4:
      append(root, cwd, tokens, disk, args);
      break;
    // remove
    case 5:
      remove_thing(root, cwd, tokens, disk, args);
      break;
    // delete
    case 6:
      delete_thing(root, cwd, tokens, disk);
      break;
    // Exit
    case 7:
      running = false;
      cout << "Goodbye!" << endl;
      break;
    // Dir
    case 8:
      cout << root << endl;
      break;
    // prfiles
    case 9:
      print_file_info(root, args);
      break;
    // prdisk
    case 10:
      cout << disk << endl;
      cout << "Fragmentation: " << get_fragmentation(root, args) << " bytes"
           << endl;
      break;
    default:
      cout << "Your token val was " << cmd << endl;
      break;
    }
  }
}