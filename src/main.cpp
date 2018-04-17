#include "filesys.h"

int main(int argc, char *const argv[]) {
  auto args = parse_args(argc, argv);
  cout << "File List: " << get<0>(args) << endl;
  cout << "Directory List: " << get<1>(args) << endl;
  cout << "Disk Size: " << get<2>(args) << endl;
  cout << "Block Size: " << get<3>(args) << endl;

  string line;

  // Read our input with file lists
  ifstream file_list(get<0>(args));

  if (!file_list.good()) {
    cerr << "File List not found '" << get<0>(args) << "'" << endl;
    return 1;
  }

  // Build a vector of each line in the file
  vector<string> file_vect;
  while (getline(file_list, line))
    file_vect.push_back(line);

  // Testing Purposes
  cout << "----------------------File List:---------------------------" << endl;
  for (auto &s : file_vect)
    cout << s << endl;
  cout << "-----------------------------------------------------------" << endl;

  // Read our input with file lists
  ifstream dir_list(get<1>(args));

  if (!dir_list.good()) {
    cerr << "File List not found '" << get<1>(args) << "'" << endl;
    return 1;
  }

  // Build a vector of each line in the file
  vector<string> dir_vect;
  while (getline(dir_list, line))
    dir_vect.push_back(line);

  // Testing Purposes
  cout << "-----------------------Dir List:---------------------------" << endl;
  for (auto &s : dir_vect)
    cout << s << endl;
  cout << "-----------------------------------------------------------" << endl;

  return 0;
}
