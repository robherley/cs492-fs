#include "filesys.h"

/**
 * Simple function to split a string by a delimiting character and return a
 * vector of strings.
 */
vector<string> split(string to_split, char delim) {
  stringstream full(to_split);
  vector<string> string_list;

  string part;
  while (getline(full, part, delim))
    string_list.push_back(part);

  return string_list;
}
