#include "paging.h"

Page ::Page(int id) {
  _id = id;
  _valid = false;
  _last_ref = 0;
}

int Page ::get_id(void) { return _id; }
bool Page ::get_valid(void) { return _valid; }
double Page ::get_last_ref(void) { return _last_ref; }

void Page ::set_valid(int bit) { _valid = bit; }