#pragma once
#include <cassert>
#include <random>
#include <vector>

using namespace std;

vector<double> operator*(const vector<double>& mom, const vector<double>& dad)
{
  static random_device dev;
  vector<double> ret;
  ret.reserve(mom.size());

  assert(mom.size() == dad.size());

  static uniform_real_distribution<double> dis(0, 1);
  double num = dis(dev);

  if (num < 0.1) {
    for (unsigned int i = 0; i < mom.size(); ++i) {
      ret.push_back((mom[i] + dad[i]) / 2);
    }
  } else if (num < 0.3) {
    for (unsigned int i = 0; i < mom.size(); ++i) {
      if (dis(dev) > 0.5) {
        ret.push_back(mom[i]);
      } else {
        ret.push_back(dad[i]);
      }
    }
  } else {
    static uniform_int_distribution<unsigned int> posDis(0, mom.size() - 1);
    unsigned int crossPos = posDis(dev);

    for (unsigned int i = 0; i < crossPos; ++i) {
      ret.push_back(mom[i]);
    }
    for (unsigned int i = crossPos; i < dad.size(); ++i) {
      ret.push_back(dad[i]);
    }
  }

  assert(ret.size() == mom.size());
  return ret;
}
