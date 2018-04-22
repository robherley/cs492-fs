#include "filesys.h"

/**
 * Simple usage message
 */
void print_usage() {
  cerr << "Usage: ./fs <args>\n"
       << "\t-f [input files storing information on files]\n"
       << "\t-d [input files storing information on directories]\n"
       << "\t-s [disk size]\n"
       << "\t-b [block size]" << endl;
}

/**
 * Checks the input to the main program and returns a tuple of the parsed
 * arguments in the order (file_list, dir_list, disk_size, block_size)
 */
tuple<string, string, int, int> parse_args(int argc, char *const argv[]) {
  if (argc != 9) {
    print_usage();
    exit(1);
  }
  string file_list, dir_list;
  int disk_size, block_size;
  // Loop over args
  for (int i = 1; i < argc; i += 2) {
    string arg = argv[i];
    if (!arg.compare("-f")) {
      file_list = argv[i + 1];
    } else if (!arg.compare("-d")) {
      dir_list = argv[i + 1];
    } else if (!arg.compare("-s")) {
      disk_size = atoi(argv[i + 1]);
    } else if (!arg.compare("-b")) {
      block_size = atoi(argv[i + 1]);
    } else {
      print_usage();
      exit(1);
    }
  }
  // Return our parsed args in a tuple
  tuple<string, string, int, int> args =
      make_tuple(file_list, dir_list, disk_size, block_size);
  return args;
}

/**
 * Given the root pointer and args, construct our file system based on the text
 * files specified in the arguments.
 */
void construct_fs(Node *root, tuple<string, string, int, int> args,
                  LDisk &ldisk) {
  // Read our input with file lists
  ifstream dir_list(get<1>(args));

  if (!dir_list.good()) {
    cerr << "File List not found '" << get<1>(args) << "'" << endl;
    exit(1);
  }

  // Used for getline
  string line;

  while (getline(dir_list, line)) {
    add_dir_from_root(root, split(line, '/'));
  }

  // Read our input with file lists
  ifstream file_list(get<0>(args));

  if (!file_list.good()) {
    cerr << "File List not found '" << get<0>(args) << "'" << endl;
    exit(1);
  }

  // Build a vector of each line in the file
  while (getline(file_list, line)) {
    vector<string> file_meta;
    istringstream iss(line);
    string str;
    while (iss >> str)
      file_meta.push_back(str);
    int bn = ceil((float)stoi(file_meta[6]) / (float)get<3>(args));
    int leftover = stoi(file_meta[6]) % get<3>(args);
    add_file_from_root(root, split(file_meta[10], '/'), bn, leftover, ldisk);
  }
}