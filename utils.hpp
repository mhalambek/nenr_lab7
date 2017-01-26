#pragma once
#include "Solution.hpp"
#include <cassert>
#include <limits>
#include <random>

using namespace std;

ostream& operator<<(ostream& os, const vector<double>& obj)
{
  for (double d : obj) {
    os << d << " ";
  }

  return os;
}

class Mutate {
  public:
  double v1, mutationChance;
  random_device dev;
  uniform_real_distribution<double> chooser;
  normal_distribution<double> dis1, dis2;
  Mutate(double s1, double s2, double v1, double mutate)
      : v1{ v1 }
      , mutationChance{ mutate }
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

void scaleFitness(vector<Solution>& pop, Solution** best)
{
  double sum = 0,
         min = std::numeric_limits<double>::max();

  if (*best == nullptr) {
    *best = new Solution(NeuralNetwork(pop.front().nen.params, pop.front().nen.layout), pop.front().err);
  }

  for (auto& s : pop) {
    if (s.err < (*best)->err) {
      free(*best);
      *best = new Solution(s.nen, s.err);
    }
    sum += s.fit;
    if (s.fit < min) {
      min = s.fit;
    }
  }

  sum -= min * pop.size();
  for (auto& s : pop) {
    s.fit -= min;
    s.fit /= sum;
  }
}

const Solution& select(const vector<Solution>& pop)
{
  static random_device dev;
  static auto dis = uniform_real_distribution<double>(0, 1);
  double ch = dis(dev);

  for (auto& p : pop) {
    ch -= p.fit;
    if (ch < 0) {
      return p;
    }
  }

  return pop.back();
}
