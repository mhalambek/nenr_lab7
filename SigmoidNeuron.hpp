#pragma once
#include <cmath>
#include <vector>

class SigmoidNeuron {
  private:
  int startIndex;

  public:
  SigmoidNeuron(int startIndex)
      : startIndex{ startIndex } {};
  double sigmoid(double t) const
  {
    return 1 / (1 + exp(-t));
  };

  double operator()(const std::vector<double>& x, const std::vector<double>& w) const
  {
    double sum = w[startIndex];

    for (unsigned int i = 0; i < x.size(); ++i) {
      sum += x[i] * w[i + 1 + startIndex];
    }

    return sigmoid(sum);
  };
};
