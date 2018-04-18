#include "filesys.h"

int main(int argc, char *const argv[]) {
  auto args = parse_args(argc, argv);
  cout << "\nFile List: " << get<0>(args) << endl;
  cout << "Directory List: " << get<1>(args) << endl;
  cout << "Disk Size: " << get<2>(args) << endl;
  cout << "Block Size: " << get<3>(args) << endl << endl;

  string line;

  // Read our input with file lists
  ifstream file_list(get<0>(args));

  // Read our input with file lists
  ifstream dir_list(get<1>(args));

  if (!dir_list.good()) {
    cerr << "File List not found '" << get<1>(args) << "'" << endl;
    return 1;
  }

  Node root = Node("root", get<2>(args), 0); // Not sure what to do with ts yet

  root.add_child("test", 1337, 1);
  root.children.at("test").add_child("sub_test", 13, 2);
  root.add_child("empty_dir", 69, 2);

  cout << root << endl;

  // vector<string> temp;
  // while (getline(dir_list, line)) {
  //   temp = split(line, '/');
  //   for (int i = 0; i < temp.size(); i++) {
  //     cout << temp[i];
  //     if (i + 1 != temp.size()) {
  //       cout << " -> ";
  //     }
  //   }
  //   cout << endl;
  // }

  if (!file_list.good()) {
    cerr << "File List not found '" << get<0>(args) << "'" << endl;
    return 1;
  }

  // Build a vector of each line in the file
  vector<string> file_vect;
  while (getline(file_list, line))
    file_vect.push_back(line);

  return 0;
}
