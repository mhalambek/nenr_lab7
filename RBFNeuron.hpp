#pragma once
#include <cmath>
#include <vector>

class RBFNeuron {
  private:
  int startIndex;

  public:
  RBFNeuron(int startIndex)
      : startIndex{ startIndex } {};

  double operator()(const std::vector<double>& x, const std::vector<double>& w) const
  {
    double sum = 0;

    for (int i = 0; i < (int)x.size(); ++i) {
      sum += std::abs(x[i] - w[startIndex + i]) / std::abs(w[startIndex + i + x.size()]);
    }

    return 1.0 / (1.0 + sum);
  };
};
