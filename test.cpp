#include "Dataset.hpp"
#include <cassert>
#include <iostream>

int main()
{
  Dataset set("./dataset.txt");
  assert(set.data.size() == 64);

  return 0;
}
