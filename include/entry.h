#pragma once

#include <iostream>

struct Entry {
  char key[16];
  char value[40];
  Entry* next;
};
