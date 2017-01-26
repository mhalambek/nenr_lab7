#pragma once
#include "NeuralNetwork.hpp"

struct Solution {
  NeuralNetwork nen;
  double err;
  double fit;

  Solution(NeuralNetwork n, const Dataset& set)
      : nen{ n }
  {
    err = n.calcError(set);
    fit = 1 / err;
  };

  Solution(NeuralNetwork n, double err)
      : nen{ n }
      , err{ err }
      , fit{ 1 / err } {};
};
