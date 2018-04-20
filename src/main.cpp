#include "filesys.h"

int main(int argc, char *const argv[]) {
  auto args = parse_args(argc, argv);
  int num_blocks = get<2>(args) / get<3>(args);

  // Read our input with file lists
  ifstream dir_list(get<1>(args));

  if (!dir_list.good()) {
    cerr << "File List not found '" << get<1>(args) << "'" << endl;
    return 1;
  }

  // Pointer to our root node for our entire file system.
  auto root = new Node("root");

  // Used for getline
  string line;

  while (getline(dir_list, line)) {
    add_dir_from_root(root, split(line, '/'));
  }

  // Read our input with file lists
  ifstream file_list(get<0>(args));

  if (!file_list.good()) {
    cerr << "File List not found '" << get<0>(args) << "'" << endl;
    return 1;
  }

  // Build a vector of each line in the file
  while (getline(file_list, line)) {
    vector<string> file_meta;
    istringstream iss(line);
    string str;
    while (iss >> str)
      file_meta.push_back(str);
    add_file_from_root(root, split(file_meta[10], '/'), stoi(file_meta[6]));
  }

  cout << RED << "Disk Size: " << RES << get<2>(args) << endl;
  cout << RED << "Block Size: " << RES << get<3>(args) << endl;
  cout << RED << "Total Blocks: " << RES << num_blocks << endl;
  cout << RED << "\nFS Tree: " << RES << endl;
  cout << root << endl;

  delete root;
  return 0;
}
