#pragma once
#include <cassert>
#include <iostream>
#include <random>

using namespace std;

ostream& operator<<(ostream& os, const vector<double>& obj)
{
  // os << obj.getXYZ() << obj.getABC() << '\n';
  for (double d : obj) {
    os << d << " ";
  }

  return os;
}

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

class Mutate {
  public:
  double v1, mutationChance;
  random_device dev;
  uniform_real_distribution<double> chooser;
  normal_distribution<double> dis1, dis2;
  Mutate(double s1, double s2, double _v1, double _mutate)
      : v1{ _v1 }
      , mutationChance{ _mutate }
  {
    chooser = uniform_real_distribution<double>(0, 1);
    dis1 = normal_distribution<double>(0, s1);
    dis2 = normal_distribution<double>(0, s2);
  };
  void operator()(vector<double>& c)
  {
    if (chooser(dev) < v1) {
      for (unsigned int i = 0; i < c.size(); ++i) {
        if (chooser(dev) < mutationChance) {
          c[i] += dis1(dev);
        }
      }
    } else {
      for (unsigned int i = 0; i < c.size(); ++i) {
        if (chooser(dev) < mutationChance) {
          c[i] = dis2(dev);
        }
      }
    }
  };
};
