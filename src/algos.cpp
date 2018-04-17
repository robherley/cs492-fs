#include "paging.h"

// function that tells us if a page is inside of the data structure for the fifo
// algorithm
bool contains(int page_num, queue<int> memory) {
  queue<int> mem_copy(memory);
  while (mem_copy.size() > 1) {
    if (mem_copy.front() == page_num)
      return true;
    mem_copy.pop();
  }
  return false;
}

// function that tells us if a page is inside of the data structure for the lru
// algorithm
bool contains_lru(int page_num, vector<pair<int, unsigned long>> *memory) {
  for (unsigned i = 0; i < memory->size(); i++) {
    if ((*memory)[i].first == page_num)
      return true;
  }
  return false;
}

// function that tells us if a page is inside the memory data structure for
bool contains_clock(int page_num, queue<pair<int, int>> *memory) {
  queue<pair<int, int>> mem_copy(*memory);
  while (mem_copy.size() > 1) {
    if (mem_copy.front().first == page_num)
      return true;
    mem_copy.pop();
  }
  return false;
}

// translates memory location to a page id
int mem_to_page(int mem_loc, int page_size, vector<pair<int, int>> plist_vect) {
  int page_id = 0;
  int mem_copy = 0;
  for (unsigned i = 0; i < plist_vect.size(); i++) {
    if (mem_copy + plist_vect[i].second > mem_loc)
      break;
    else {
      mem_copy += plist_vect[i].second;
      page_id += (plist_vect[i].second / page_size);
    }
  }
  while (mem_copy <= mem_loc) {
    mem_copy += page_size;
    page_id++;
  }
  return page_id;
}

void lru(vector<pair<int, int>> ptrace_vect, vector<pair<int, int>> plist_vect,
         vector<pair<int, unsigned long>> *mem, int p_size, bool prepaging) {
  int p_faults = 0;
  unsigned long p_counter = 0;
  for (unsigned i = 0; i < ptrace_vect.size(); i++) {
    int curr_mem = ptrace_vect[i].second;
    // get a pair of <program#, pagenumber within program's page table>
    int curr_page = mem_to_page(curr_mem, p_size, plist_vect);
    // Not in memory
    if (!contains_lru(curr_page, mem)) {
      // increment page fault count
      p_faults++;
      int min_ele_index = 0;
      unsigned long min_time = ULONG_MAX;
      // find the minimum
      for (unsigned j = 0; j < mem->size(); j++) {
        if ((*mem)[j].second < min_time) {
          min_time = (*mem)[j].second;
          min_ele_index = j;
        }
      }
      pair<int, unsigned long> page(curr_page, p_counter);
      (*mem).at(min_ele_index) = page;
      p_counter++;
      if (prepaging) {
        int padding = 1;
        int next_page;
        // find the next invalid page
        while (1) {
          if (i + padding >= ptrace_vect.size()) break;
          int next_mem = ptrace_vect[i + padding].second;
          next_page = mem_to_page(next_mem, p_size, plist_vect);
          // if the mem doesnt contain this page, then break
          if (!contains_lru(next_page, mem))
            break;
          padding++;
        }
        // perform one more replacement
        int min_ele_index = 0;
        unsigned long min_time = ULONG_MAX;
        for (unsigned j = 0; j < mem->size(); j++) {
          if ((*mem)[j].second < min_time) {
            min_time = (*mem)[j].second;
            min_ele_index = j;
          }
        }
        pair<int, unsigned long> page(next_page, p_counter);
        (*mem).at(min_ele_index) = page;
        p_counter++;
      }
    }
  }
  cout << "Page faults for LRU " << p_faults << endl;
}

void fifo(vector<pair<int, int>> ptrace_vect, vector<pair<int, int>> plist_vect,
          queue<int> *mem, int p_size, vector<Program> programs,
          bool prepaging) {
  int p_faults = 0;
  for (unsigned i = 0; i < ptrace_vect.size(); i++) {
    int curr_mem = ptrace_vect[i].second;
    // get a pair of <program#, pagenumber within program's page table>
    int curr_page = mem_to_page(curr_mem, p_size, plist_vect);
    if (contains(curr_page, *mem) == false) {
      // Not in memory
      cout << "Page Fault" << endl;
      p_faults++;
      mem->pop();
      mem->push(curr_page);
      if (prepaging) {
        int padding = 1;
        int next_page;
        // find the next invalid page
        while (1) {
          if (i + padding >= ptrace_vect.size())
            break;
          int next_mem = ptrace_vect[i + padding].second;
          next_page = mem_to_page(next_mem, p_size, plist_vect);
          // if the mem doesnt contain this page, then break
          if (!contains(next_page, *mem))
            break;
          padding++;
        }
        // perform one more pop and push
        mem->pop();
        mem->push(next_page);
      }
    }
  }
  cout << "Page faults for FIFO " << p_faults << endl;
}

void clock(vector<pair<int, int>> ptrace_vect,
           vector<pair<int, int>> plist_vect, queue<pair<int, int>> *mem,
           int p_size, bool prepaging) {
  int p_faults = 0;
  for (unsigned i = 0; i < ptrace_vect.size(); i++) {
    int curr_mem = ptrace_vect[i].second;
    int curr_page_id = mem_to_page(curr_mem, p_size, plist_vect);
    if (contains_clock(curr_page_id, mem) == false) {
      cout << "[" << i << "]: Page Fault: " << p_faults << endl;
      p_faults++;
      pair<int, int> curr_page;
      while (1) {
        if (mem->front().second == 0) {
          curr_page.first = curr_page_id;
          curr_page.second = 1;
          mem->pop();
          mem->push(curr_page);
          break;
        }
        if (mem->front().second == 1) {
          curr_page.first = mem->front().first;
          curr_page.second = 0;
          mem->pop();
          mem->push(curr_page);
        }
      }
      if (prepaging) {
        int padding = 1;
        int next_page_id;
        // find the next invalid page
        while (1) {
          if (i + padding >= ptrace_vect.size())
            break;
          int next_mem = ptrace_vect[i + padding].second;
          next_page_id = mem_to_page(next_mem, p_size, plist_vect);
          // if the mem doesnt contain this page, then break
          if (!contains_clock(next_page_id, mem))
            break;
          padding++;
        }
        // perform one more pop and push
        pair<int, int> next_page;
        while (1) {
          if (mem->front().second == 0) {
            next_page.first = next_page_id;
            next_page.second = 1;
            mem->pop();
            mem->push(next_page);
            break;
          }
          if (mem->front().second == 1) {
            next_page.first = mem->front().first;
            next_page.second = 0;
            mem->pop();
            mem->push(curr_page);
          }
        }
      }
    }
  }
  cout << "Page faults for Clock: " << p_faults << endl;
}