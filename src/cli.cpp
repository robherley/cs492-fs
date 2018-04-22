#include "filesys.h"

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
  while (wanted_path.size()) {
    string to_dir = wanted_path.front();
    if (to_dir == "..") {
      if (cwd.size() == 1) {
        cout << "cd: error: root is the highest directory" << endl;
        break;
      } else {
        cwd.pop();
      }
    } else {
      if ((cwd.top())->has_dir(to_dir)) {
        cwd.push((cwd.top())->dirs.at(to_dir));
      } else {
        if (to_dir.size())
          cout << "cd: error: specifed folder '" << to_dir << "' does not exist"
               << endl;
        else
          cout << "cd: error: specified folder does not exist" << endl;
        break;
      }
    }
    wanted_path.pop();
  }
}

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
    // Exit
    case 7:
      running = false;
      cout << "Goodbye!" << endl;
      break;
    // Dir
    case 8:
      cout << cwd.top() << endl;
      break;
    default:
      cout << "Your token val was " << cmd << endl;
      break;
    }
  }
}