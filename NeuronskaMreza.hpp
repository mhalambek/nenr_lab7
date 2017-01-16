#pragma once
#include "Dataset.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

struct N1 {
  vector<double*> w;
  vector<double*> s;

  N1(vector<double*> _w, vector<double*> _s)
      : w{ _w }
      , s{ _s }
  {
  }

  double operator()(vector<double> x)
  {
    vector<double> ret;
    assert(x.size() == w.size());
    assert(x.size() == s.size());
    double sum = 0;

    for (unsigned int i = 0; i < x.size(); ++i) {
      sum += abs(x[i] - *w[i]) / abs(*s[i]);
    }

    return 1.0 / (1.0 + sum);
  };
};

struct N2 {
  vector<double*> w;

  N2(vector<double*> _w)
      : w{ _w } {};

  double sigmoid(double t)
  {
    return 1 / (1 + exp(-t));
  };

  double operator()(vector<double> x)
  {
    vector<double> ret;
    assert(x.size() == (w.size() - 1));
    double sum = *w[0];

    for (unsigned int i = 1; i < w.size(); ++i) {
      sum += x[i - 1] * *w[i];
    }

    return sigmoid(sum);
  };
};

random_device dev;

double getRandDouble()
{

  return uniform_real_distribution<>(-2.0, 2.0)(dev);
}

unsigned int totalSize(vector<unsigned int> vec)
{
  unsigned int ret = vec[0] * 2 * vec[1];

  for (unsigned int i = 2; i < vec.size(); ++i) {
    ret += (1 + vec[i - 1]) * vec[i];
  }

  return ret;
}

struct NeuronskaMreza {
  vector<unsigned int> layout;
  vector<N1> secondLayer;
  vector<vector<N2> > otherLayers;
  vector<double> params;
  NeuronskaMreza(const char* filePath)
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

    double d;
    while (!fin.eof()) {
      fin >> d;
      params.push_back(d);
    }

    unsigned int lastIndex = 0;
    for (unsigned int i = 0; i < layout[1]; ++i) {
      vector<double*> w;
      vector<double*> s;
      for (unsigned int j = 0; j < layout[0]; ++j) {
        w.push_back(&params[lastIndex++]);
      }
      for (unsigned int j = 0; j < layout[0]; ++j) {
        s.push_back(&params[lastIndex++]);
      }
      secondLayer.push_back(N1(w, s));
    }
    for (unsigned int i = 2; i < layout.size(); ++i) {
      otherLayers.push_back(vector<N2>());
      for (unsigned int k = 0; k < layout[i]; ++k) {
        vector<double*> w;
        //bias
        w.push_back(&params[lastIndex++]);
        for (unsigned int j = 0; j < layout[i - 1]; ++j) {
          w.push_back(&params[lastIndex++]);
        }
        otherLayers.back().push_back(N2(w));
      }
    }
  };
  NeuronskaMreza(vector<unsigned int> _layout)
      : layout{ _layout }
  {
    assert(layout.size() >= 3);
    assert(layout[0] == 2);
    assert(layout.back() == 3);
    //TODO hack
    params.reserve(totalSize(layout) * sizeof(double));
    for (unsigned int i = 0; i < layout[1]; ++i) {
      //TODO randomize vals
      vector<double*> w;
      vector<double*> s;
      for (unsigned int j = 0; j < layout[0]; ++j) {
        params.push_back(getRandDouble());
        w.push_back(&params[params.size() - 1]);
      }
      for (unsigned int j = 0; j < layout[0]; ++j) {
        params.push_back(getRandDouble());
        s.push_back(&params[params.size() - 1]);
      }
      secondLayer.push_back(N1(w, s));
    }
    for (unsigned int i = 2; i < layout.size(); ++i) {
      otherLayers.push_back(vector<N2>());
      for (unsigned int k = 0; k < layout[i]; ++k) {
        vector<double*> w;
        //bias
        params.push_back(getRandDouble());
        w.push_back(&params[params.size() - 1]);
        for (unsigned int j = 0; j < layout[i - 1]; ++j) {
          params.push_back(getRandDouble());
          w.push_back(&params[params.size() - 1]);
        }
        otherLayers.back().push_back(N2(w));
      }
    }
    //TODO hack doesn't work
    assert(params.size() < 1024);
  }
  NeuronskaMreza(vector<double> pars, vector<unsigned int> _lay)
      : layout{ _lay }
      , params{ pars }
  {
    unsigned int lastIndex = 0;
    for (unsigned int i = 0; i < layout[1]; ++i) {
      vector<double*> w;
      vector<double*> s;
      for (unsigned int j = 0; j < layout[0]; ++j) {
        w.push_back(&params[lastIndex++]);
      }
      for (unsigned int j = 0; j < layout[0]; ++j) {
        s.push_back(&params[lastIndex++]);
      }
      secondLayer.push_back(N1(w, s));
    }
    for (unsigned int i = 2; i < layout.size(); ++i) {
      otherLayers.push_back(vector<N2>());
      for (unsigned int k = 0; k < layout[i]; ++k) {
        vector<double*> w;
        //bias
        w.push_back(&params[lastIndex++]);
        for (unsigned int j = 0; j < layout[i - 1]; ++j) {
          w.push_back(&params[lastIndex++]);
        }
        otherLayers.back().push_back(N2(w));
      }
    }
  };
  vector<double> operator()(double x, double y)
  {
    vector<double> in({ x, y });

    vector<double> n;
    for (auto& neuron : secondLayer) {
      n.push_back(neuron(in));
    }
    in = n;
    n.clear();

    for (auto& layer : otherLayers) {
      for (auto& neuron : layer) {
        n.push_back(neuron(in));
      }
      in = n;
      n.clear();
    }

    return in;
  }
  double calcError(Dataset& set)
  {
    double ret = 0;

    for (const auto& s : set) {
      auto o = (*this)(s.x, s.y);

      ret += pow(((double)s.A - o[0]), 2);
      ret += pow(((double)s.B - o[1]), 2);
      ret += pow(((double)s.C - o[2]), 2);
    }

    return ret / set.data.size();
  }
};
