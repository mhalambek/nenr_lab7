#include "Dataset.hpp"
#include "NeuronskaMreza.hpp"
#include "utils.hpp"
#include <cassert>
#include <iostream>
#include <random>

struct Sol {
  NeuronskaMreza nen;
  double fit;
  double err;
  Sol(NeuronskaMreza n, Dataset& set)
      : nen{ n }
  {
    err = n.calcError(set);
    fit = 1 / err;
  }
  Sol(NeuronskaMreza n, double _err)
      : nen{ n }
  {
    err = _err;
    fit = 1 / err;
  }
};

Sol* best = nullptr;

void scaleFitness(vector<Sol>& pop)
{
  double sum = 0,
         min = 100000;

  if (best == nullptr) {
    best = new Sol(NeuronskaMreza(pop.front().nen.params, pop.front().nen.layout), pop.front().err);
  }

  for (auto& s : pop) {
    if (s.err < best->err) {
      free(best);
      best = new Sol(s.nen, s.err);
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

Sol& select(vector<Sol>& pop)
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

void killHandler(int signum)
{
  cout << "received signal: " << signum << endl;
  cout << "best mean error: " << best->err << endl;
  for (auto d : best->nen.params) {
    cout << d << " ";
  }

  exit(signum);
}

int main(int argc, char** argv)
{
  signal(SIGINT, killHandler);
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  Dataset set("./dataset.txt");

  vector<unsigned int> layout({ 2, 8, 3 });

  const unsigned int populationSize = 20;
  const unsigned int maxIter = 100000;
  vector<Sol> population;
  for (unsigned int i = 0; i < populationSize; ++i) {
    population.push_back(Sol(NeuronskaMreza(layout), set));
  }

  // double sum = 0;
  // for (auto p : population) {
  //   sum += p.fit;
  // }
  // assert(sum > 0.999);
  Mutate mutate(0.1, 2, 0.95, 0.02);

  for (unsigned int currentIter = 0; currentIter < maxIter; ++currentIter) {
    scaleFitness(population);
    vector<Sol> nextGen;
    cout << "iter: " << currentIter << ", best mse: " << best->err << endl;
    nextGen.push_back(*best);

    while (nextGen.size() != populationSize) {
      auto mom = select(population);
      auto dad = select(population);
      auto childGenom = mom.nen.params * dad.nen.params;
      mutate(childGenom);
      Sol s(NeuronskaMreza(childGenom, layout), set);
      nextGen.push_back(s);
    }
    population = nextGen;
  }

  killHandler(0);

  return 0;
}
