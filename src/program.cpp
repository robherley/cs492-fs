#include "paging.h"

Program ::Program(int id, int mem_size, int p_size, unsigned int page_id) {
  _id = id;
  _mem_size = mem_size;
  unsigned int num_pages = ceil(mem_size / p_size); // size page table
  // Add the pages for the calculated page table size
  vector<Page> page_table;
  for(unsigned int i=0; i < num_pages; i++){ 
    page_table.push_back(Page(page_id));
    page_id++;
  }
  _page_table = page_table;
}

int Program :: get_id(void) { return _id; }
int Program :: get_mem_size(void) { return _mem_size; }
vector<Page> Program :: get_page_table(void) { return _page_table; }
