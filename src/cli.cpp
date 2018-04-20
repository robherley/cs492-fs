#include "filesys.h"

/**
 * Little prompt to grab the input. AND IT'S COLORFUL
 */
string prompt(stack<Node *> cwd) {
  cout << BLUE << getenv("USER") << YELLOW << " | " << GREEN;
  while (!cwd.empty()) {
    cout << cwd.top()->name << '/';
    cwd.pop();
  }
  cout << endl << MAGENTA << "$ " << RES;
  string input;
  cin >> input;
  return input;
}

/**
 * Our main program loop. Handles user input based on assignment commands.
 */
void start_cli(Node *root, tuple<string, string, int, int> args) {
  map<string, int> cmds = {{"cd", 0},      {"ls", 1},     {"mkdir", 2},
                           {"create", 3},  {"append", 4}, {"remove", 5},
                           {"delete", 6},  {"exit", 7},   {"dir", 8},
                           {"prfiles", 9}, {"prdisk", 10}};
  bool running = true;
  stack<Node *> cwd;
  cwd.push(root);
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