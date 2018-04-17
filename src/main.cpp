#include "paging.h"

int main(int argc, char *const argv[]) {
  if (argc != 6) {
    cerr << "Usage: ./VMsimulator <plist> <ptrace> <page size> <replacement "
            "type> <prepaging>\n"
         << "  Valid Replacement Types: FIFO, LRU, Clock\n"
         << "  Prepaging: + (on) or - (off)" << endl;
    return 1;
  }

  // Handling 'FIFO' or 'fifo' or any other case
  string input_algo = argv[4];
  for (auto &c : input_algo)
    c = tolower(c);

  Algo replacement_type;
  if (input_algo == "fifo") {
    replacement_type = FIFO;
  } else if (input_algo == "lru") {
    replacement_type = LRU;
  } else if (input_algo == "clock") {
    replacement_type = CLOCK;
  } else {
    cerr << "Invalid replacement algorithm '" << argv[4] << "' entered."
         << endl;
    return 1;
  }

  bool pre_paging;
  if (argv[5][0] == '+') {
    pre_paging = true;
  } else if (argv[5][0] == '-') {
    pre_paging = false;
  } else {
    cerr << "Invalid prepaging option '" << argv[5] << "' entered." << endl;
    return 1;
  }

  // (pID, Total#MemoryLocation)
  ifstream plist((string)argv[1] + ".txt");
  // (pID, ReferencedMemoryLocation)
  ifstream ptrace((string)argv[2] + ".txt");

  if (!plist.good()) {
    cerr << "plist file not found '" << argv[1] << "'\n";
    return 1;
  }

  if (!ptrace.good()) {
    cerr << "ptrace file not found '" << argv[2] << "'\n";
    return 1;
  }

  int p_size = atoi(argv[3]);

  // plist: parse & store vector of tuples (pID, Total#MemoryLocation)
  vector<pair<int, int>> plist_vector = build_pair_vector(plist);

  // ptrace: parse & store vector of tuples (pID, ReferencedMemoryLocation)
  vector<pair<int, int>> ptrace_vector = build_pair_vector(ptrace);

  // Construct our vector of programs w/ page tables
  vector<Program> program_vect = init_programs(plist_vector, atoi(argv[3]));

  if (replacement_type == FIFO) {
    queue<int> main_mem;
    prefill_mem_fifo(&main_mem, program_vect, p_size);
    fifo(ptrace_vector, plist_vector, &main_mem, p_size, program_vect, pre_paging);
  }

  if (replacement_type == LRU) {
    vector<pair<int,unsigned long>> main_mem;
    prefill_mem_lru(&main_mem, program_vect, p_size);
    lru(ptrace_vector, plist_vector, &main_mem, p_size, pre_paging);
  }

  if (replacement_type == CLOCK) {
    queue<pair<int,int>> main_mem;
    prefill_mem_clock(&main_mem, program_vect, p_size);
    clock(ptrace_vector, plist_vector, &main_mem, p_size, pre_paging);
  }

  cout << "Page Size: " << argv[3] << '\n';
  cout << "Algorithm: " << argv[4] << '\n';
  cout << "Prepaging: " << (pre_paging ? "yes" : "no") << '\n';

  return 0;
}
