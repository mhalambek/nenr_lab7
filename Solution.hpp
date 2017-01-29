#pragma once
#include "NeuralNetwork.hpp"
#include "crossing.hpp"
#include <iostream>
#include <vector>

double getRandDouble()
{
  static random_device dev;
  static auto dis = uniform_real_distribution<>(-2.0, 2.0);

  return dis(dev);
}

struct Solution {
  vector<double> sol;
  double err;
  double fit;

  Solution(){};

  Solution(const Solution& obj)
      : sol{ obj.sol }
      , err{ obj.err }
      , fit{ obj.fit } {};

  Solution(unsigned int size)
  {
    sol = vector<double>(size);
    for (auto& d : sol) {
      d = getRandDouble();
    }
  };

  Solution operator*(const Solution& obj) const
  {
    Solution ret;

    ret.sol = obj.sol * sol;

    return ret;
  };
};
