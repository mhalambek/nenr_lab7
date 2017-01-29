#pragma once
#include "Dataset.hpp"
#include "RBFNeuron.hpp"
#include "SigmoidNeuron.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

class NeuralNetwork {
  private:
  void makeNetwork()
  {
    unsigned int lastIndex = 0;
    for (unsigned int i = 0; i < layout[1]; ++i) {
      secondLayer.push_back(RBFNeuron(lastIndex));
      lastIndex += 2 * layout[0];
    }
    for (unsigned int i = 2; i < layout.size(); ++i) {
      otherLayers.push_back(vector<SigmoidNeuron>());
      otherLayers.back().reserve(100);
      for (unsigned int k = 0; k < layout[i]; ++k) {
        otherLayers.back().push_back(SigmoidNeuron(lastIndex));
        lastIndex += layout[i - 1] + 1;
      }
    }

    size = totalSize();
  };

  unsigned int totalSize() const
  {
    unsigned int ret = layout[0] * 2 * layout[1];

    for (unsigned int i = 2; i < layout.size(); ++i) {
      ret += (1 + layout[i - 1]) * layout[i];
    }

    return ret;
  };

  public:
  vector<unsigned int> layout;
  vector<RBFNeuron> secondLayer;
  vector<vector<SigmoidNeuron> > otherLayers;
  unsigned int size;

  NeuralNetwork(const char* filePath)
  {
    ifstream fileStream(filePath);
    string line;
    getline(fileStream, line);
    istringstream fin(line);
    unsigned int t;
    while (!fin.eof()) {
      fin >> t;
      layout.push_back(t);
    }

    getline(fileStream, line);
    fin = istringstream(line);

    makeNetwork();
  };

  NeuralNetwork(const NeuralNetwork& ann)
      : layout{ ann.layout }
  {
    makeNetwork();
  };

  NeuralNetwork(vector<unsigned int> layout)
      : layout{ layout }
  {
    assert(layout.size() >= 3);
    assert(layout[0] == 2);
    assert(layout.back() == 3);

    makeNetwork();
  };

  vector<double> operator()(double x, double y, const vector<double>& params) const
  {
    vector<double> in({ x, y });

    vector<double> n;
    n.reserve(secondLayer.size());

    for (auto& neuron : secondLayer) {
      n.push_back(neuron(in, params));
    }

    in = n;
    n.clear();

    for (auto& layer : otherLayers) {
      n.reserve(layer.size());
      for (auto& neuron : layer) {
        n.push_back(neuron(in, params));
      }
      in = n;
      n.clear();
    }

    return in;
  };

  double calcError(const Dataset& set, const vector<double>& params) const
  {
    double ret = 0;

    for (const auto& s : set) {
      auto o = (*this)(s.x, s.y, params);

      ret += pow(((double)s.A - o[0]), 2);
      ret += pow(((double)s.B - o[1]), 2);
      ret += pow(((double)s.C - o[2]), 2);
    }

    return ret / set.size();
  };
};
