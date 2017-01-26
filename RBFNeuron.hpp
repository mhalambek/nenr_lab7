#pragma once
#include <cmath>
#include <vector>

class RBFNeuron {
  private:
  double* w;
  double* s;

  public:
  RBFNeuron(double* w, double* s)
      : w{ w }
      , s{ s } {};

  double operator()(const std::vector<double>& x) const
  {
    double sum = 0;

    for (int i = 0; i < (int)x.size(); ++i) {
      sum += std::abs(x[i] - w[i]) / std::abs(s[i]);
    }

    return 1.0 / (1.0 + sum);
  };
};
