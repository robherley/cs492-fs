#include "filesys.h"

int main(int argc, char *const argv[]) {
  auto args = parse_args(argc, argv);

  // Pointer to our root node for our entire file system.
  Node *root = new Node("root");

  // Initialize our disk
  LDisk disk = LDisk(get<2>(args), get<3>(args));

  // Build our inital filesystem from the text files in args.
  construct_fs(root, args, disk);

  // Start our cli interface;
  start_cli(root, args, disk);

  delete root;
  return 0;
}
