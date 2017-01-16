#include "Dataset.hpp"
#include "NeuronskaMreza.hpp"
#include "utils.hpp"
#include <cassert>
#include <iostream>

int main()
{
  Dataset set("./dataset.txt");
  assert(set.data.size() == 64);

  vector<unsigned int> layout({ 2, 3, 3 });

  NeuronskaMreza nen(layout);
  auto s = set.data[0];
  assert(totalSize(layout) == nen.params.size());

  for (unsigned int i = 0; i < nen.params.size(); ++i) {
    nen.params[i] = 0;
  }

  auto o = nen(s.x, s.y);

  assert(o == vector<double>(3, 0.5));
  // for (double d : o) {
  //   cout << d << " ";
  // }

  NeuronskaMreza nenFromVec(nen.params, nen.layout);
  assert(nenFromVec(s.x, s.y) == vector<double>(3, 0.5));

  assert(nen.calcError(set) == 0.75);

  auto child = nen.params * nenFromVec.params;

  auto before = child;

  Mutate m(0.2, 2, 0.8, 0.05);
  m(child);
  // mutate(child, 1, 2);

  assert(child != before);
  NeuronskaMreza ann("./ann.txt");
  cout << ann.calcError(set) << endl;
  cout << ann(set.data[0].x, set.data[0].y) << endl;
  cout << set.data[0].A << " " << set.data[0].B << " " << set.data[0].C << " " << endl;

  return 0;
}
