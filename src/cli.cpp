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
 *
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
 * Little prompt to grab the input. AND IT'S COLORFUL
 */
string prompt(stack<Node *> cwd) {
  cout << BLUE << getenv("USER") << YELLOW << " | " << GREEN;
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
    // Exit
    case 7:
      running = false;
      cout << "Goodbye!" << endl;
      break;
    // Dir
    case 8:
      cout << cwd.top() << endl;
      break;
    case 10:
      cout << disk << endl;
      break;
    default:
      cout << "Your token val was " << cmd << endl;
      break;
    }
  }
}