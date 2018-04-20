#include "filesys.h"

/**
 * Little prompt to grab the input. AND IT'S COLORFUL
 */
string prompt() {
  cout << BLUE << getenv("USER") << YELLOW << "@" << GREEN
       << "fake-filesystem\n"
       << MAGENTA << "$ " << RES;
  string input;
  cin >> input;
  return input;
}

void start_cli(Node *root, tuple<string, string, int, int> args) {
  map<string, int> cmds = {{"cd", 0},      {"ls", 1},     {"mkdir", 2},
                           {"create", 3},  {"append", 4}, {"remove", 5},
                           {"delete", 6},  {"exit", 7},   {"dir", 8},
                           {"prfiles", 9}, {"prdisk", 10}};
  bool running = true;
  while (running) {
    // Grab our input, split it into a queue of tokens
    queue<string> tokens = split(prompt(), ' ');
    if (cmds.find(tokens.front()) == cmds.end()) {
      cout << "fs: command not found: " << tokens.front() << endl;
      continue;
    }
    int cmd = cmds.at(tokens.front());
    tokens.pop();
    switch (cmd) {
    case 7:
      running = false;
      cout << "Goodbye!" << endl;
      break;
    default:
      cout << "Your token val was " << cmd << endl;
      break;
    }
  }
}