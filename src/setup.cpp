#include "paging.h"

/**
 * Parse plist or ptrace and turn them into a usable vector of integer pairs
 */
vector<pair<int, int>> build_pair_vector(ifstream &file) {
  string line;
  vector<pair<int, int>> pair_vect;
  while (getline(file, line)) {
    istringstream iss(line);
    string e1, e2;
    iss >> e1;
    iss >> e2;
    pair_vect.push_back(pair<int, int>(stoi(e1), stoi(e2)));
  }
  return pair_vect;
}

/**
 * Using the data from plist, constructs a vector of programs with initialized
 * page tables for the Program constructor.
 */
vector<Program> init_programs(vector<pair<int, int>> plist_vect, int p_size) {
  vector<Program> program_vect;
  unsigned int page_id = 1;
  for (auto &program : plist_vect) {
    // Constructor creates page and fills in page table
    Program curr = Program(program.first, program.second, p_size, page_id);
    // Up the page counter by the number of programs we added
    page_id += ceil(program.second / p_size);
    // Add our program to the vector of all programs
    program_vect.push_back(curr);
  }
  return program_vect;
}

// fill and split the main memory evenly with all 10 programs
// We waste mem spaces tho...
void prefill_mem_fifo(queue<int> *main_mem, vector<Program> programs,
                         int page_size) {
  int mem_per_prog = 512 / programs.size();
  int num_pages = mem_per_prog / page_size;
  for (unsigned int j = 0; j < programs.size(); j++) {
    Program program = programs[j];
    for (int i = 0; i < num_pages; i++) {
      vector<Page> curr_p_table = program.get_page_table();
      main_mem->push(curr_p_table.at(i).get_id());
      curr_p_table[i].set_valid(1);
    }
  }
}

void prefill_mem_lru(vector<pair<int,unsigned long>> *main_mem, vector<Program> programs,
                         int page_size) {
  int mem_per_prog = 512 / programs.size();
  int num_pages = mem_per_prog / page_size;
  for (auto &program : programs) {
    for (int i = 0; i < num_pages; i++) {
      vector<Page> curr_p_table = program.get_page_table();
      pair<int, int> page(curr_p_table.at(i).get_id(), 0);
      (*main_mem).push_back(page);
    }
  }
}

void prefill_mem_clock(queue<pair<int,int>> *main_mem, vector<Program> programs,
                         int page_size) {
  int mem_per_prog = 512 / programs.size();
  int num_pages = mem_per_prog / page_size;
  for (auto &program : programs) {
    for (int i = 0; i < num_pages; i++) {
      vector<Page> curr_p_table = program.get_page_table();
      pair<int, int> page(curr_p_table.at(i).get_id(), 1);
      main_mem->push(page);
      curr_p_table[i].set_valid(1);
    }
  }
}
