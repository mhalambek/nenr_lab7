#pragma once
#include "NeuralNetwork.hpp"

template <typename S>
struct Solution {
  S nen;
  double err;
  double fit;

  Solution(S n, const Dataset& set)
      : nen{ n }
  {
    err = n.calcError(set);
    fit = 1 / err;
  };

  Solution(S n, double err)
      : nen{ n }
      , err{ err }
      , fit{ 1 / err } {};
};
