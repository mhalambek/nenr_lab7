#pragma once
#include <cmath>
#include <vector>

class SigmoidNeuron {
  private:
  double* w;

  public:
  SigmoidNeuron(double* w)
      : w{ w } {};
  double sigmoid(double t) const
  {
    return 1 / (1 + exp(-t));
  };

  double operator()(const std::vector<double>& x) const
  {
    double sum = w[0];

    for (unsigned int i = 0; i < x.size(); ++i) {
      sum += x[i] * w[i + 1];
    }

    return sigmoid(sum);
  };
};
