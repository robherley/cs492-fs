#include "filesys.h"

int main(int argc, char *const argv[]) {
  auto args = parse_args(argc, argv);
  cout << "\nFile List: " << get<0>(args) << endl;
  cout << "Directory List: " << get<1>(args) << endl;
  cout << "Disk Size: " << get<2>(args) << endl;
  cout << "Block Size: " << get<3>(args) << endl << endl;

  // Read our input with file lists
  ifstream dir_list(get<1>(args));

  if (!dir_list.good()) {
    cerr << "File List not found '" << get<1>(args) << "'" << endl;
    return 1;
  }

  // Pointer to our root node for our entire file system.
  auto root = new Node("root", 0, 0);

  // Used for getline
  string line;

  while (getline(dir_list, line)) {
    add_dir_from_root(root, split(line, '/'));
  }

  cout << root << endl;

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
    /**
     * Indicies of file vector:
     * [0] : inode number
     * [1] : size in 512-byte blocks
     * [2] : file permissions
     * [3] : number of hard links
     * [4] : owner
     * [5] : group
     * [6] : size in bytes
     * [7] : Last Modified Month
     * [8] : Last Modified Day
     * [9] : Last Modified Time
     * [10] : Path from root
     */
    add_file_from_root(root, split(file_meta[10], '/'), stoi(file_meta[6]));
  }

  cout << '\n' << root << endl;

  delete root;
  return 0;
}
