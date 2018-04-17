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