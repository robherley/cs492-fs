#include "filesys.h"

int main(int argc, char *const argv[]) {
  auto args = parse_args(argc, argv);

  // Pointer to our root node for our entire file system.
  auto root = new Node("root");

  // Build our inital filesystem from the text files in args.
  construct_fs(root, args);

  // int num_blocks = get<2>(args) / get<3>(args);
  // cout << RED << "Disk Size: " << RES << get<2>(args) << endl;
  // cout << RED << "Block Size: " << RES << get<3>(args) << endl;
  // cout << RED << "Total Blocks: " << RES << num_blocks << endl;
  // cout << RED << "\nFS Tree: " << RES << endl;
  // cout << root << endl;

  // Start our cli interface;
  start_cli(root, args);

  delete root;
  return 0;
}
