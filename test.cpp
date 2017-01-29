#include "Dataset.hpp"
#include "NeuralNetwork.hpp"
#include "Solution.hpp"
#include "utils.hpp"
#include <cassert>
#include <iostream>

int main()
{
  Dataset set("./dataset.txt");
  assert(set.size() == 64);
  vector<double> a = { 1, 2, 3 }, b = { 4, 5, 6 };

  auto c = a * b;

  vector<unsigned int> layout({ 2, 3, 3 });

  NeuralNetwork nen(layout);

  Solution s(nen.size);
  auto sample = set.begin();

  for (unsigned int i = 0; i < s.sol.size(); ++i) {
    s.sol[i] = 0;
  }

  auto o = nen(sample->x, sample->y, s.sol);

  assert(o == vector<double>(3, 0.5));

  return 0;
}
