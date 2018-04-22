#include "filesys.h"

int main(int argc, char *const argv[]) {
  auto args = parse_args(argc, argv);

  // Pointer to our root node for our entire file system.
  Node *root = new Node("root");

  LDisk disk = LDisk(get<2>(args), get<3>(args));

  // Test Allocations on LDisk
  auto allocated = disk.alloc(20);

  cout << "\nAllocated Addresses: " << endl;
  for (auto n : allocated) {
    cout << n * get<3>(args) << ' ';
  }

  // Cool Pretty Printing For Disk Allocations
  cout << endl;
  for (int i = 0; i < disk.blocks.size(); i++) {
    cout << (disk.blocks.at(i) ? RED : GREEN) << "◼";
    if (!((i + 1) % 80))
      cout << '\n';
  }
  cout << RES << endl;

  cout << "\nNumber of blocks: " << disk.blocks.size() << endl;
  cout << disk << endl;
  // Build our inital filesystem from the text files in args.
  construct_fs(root, args);

  // Start our cli interface;
  start_cli(root, args);

  delete root;
  return 0;
}
