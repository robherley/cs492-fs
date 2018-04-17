#ifndef PAGING_H
#define PAGING_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <queue>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>
#include <climits>
#define NUM_LOCATIONS 512;

using namespace std;

enum Algo { FIFO, LRU, CLOCK };

class Page {
  int _id;
  bool _valid;
  // Don't use clock between the exact clock time doesnt matter.
  // it's relative time
  double _last_ref;

public:
  Page(int);
  int get_id(void);
  bool get_valid(void);
  double get_last_ref(void);

  void set_valid(int);
};

class Program {
  int _id;
  int _mem_size;
  vector<Page> _page_table;

public:
  Program(int, int, int, unsigned int);
  int get_id(void);
  int get_mem_size(void);
  vector<Page> get_page_table(void);
};

// Setup Functions
vector<pair<int, int>> build_pair_vector(ifstream &file);
vector<Program> init_programs(vector<pair<int, int>> plist_vect, int);
void prefill_mem_lru(vector<pair<int,unsigned long>> *main_mem, vector<Program> programs,
                         int page_size);
void prefill_mem_fifo(queue<int> *main_mem, vector<Program> programs,
                      int page_size);
void prefill_mem_clock(queue<pair<int, int>> *main_mem,
                       vector<Program> programs, int page_size);

// Paging Algorithms
void fifo(vector<pair<int, int>> ptrace_vect, vector<pair<int, int>> plist_vect,
          queue<int> *mem, int p_size, vector<Program> programs, bool prepaging);

void clock(vector<pair<int, int>> ptrace_vect,
           vector<pair<int, int>> plist_vect, queue<pair<int, int>> *mem,
           int p_size, bool prepaging);

void lru(vector<pair<int, int>> ptrace_vect, vector<pair<int, int>> plist_vect,
         vector<pair<int, unsigned long>> *mem, int p_size, bool prepaging);

#endif
