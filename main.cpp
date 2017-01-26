#include "Dataset.hpp"
#include "NeuronskaMreza.hpp"
#include "Sol.hpp"
#include "crossing.hpp"
#include "utils.hpp"
#include <cassert>
#include <iostream>
#include <random>

Sol* best = nullptr;

void killHandler(int signum)
{
  cout << "received signal: " << signum << endl;
  cout << "best mean error: " << best->err << endl;
  for (auto t : best->nen.layout) {
    cout << t << " ";
  }
  cout << endl;
  for (auto d : best->nen.params) {
    cout << d << " ";
  }
  cout << endl;

  exit(signum);
}

int main(int argc, char** argv)
{
  signal(SIGINT, killHandler);
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  string datasetPath = "./dataset.txt";

  if (argc > 1) {
    datasetPath = argv[1];
  }

  Dataset set(datasetPath.c_str());

  vector<unsigned int> layout({ 2, 8, 4, 3 });

  const unsigned int populationSize = 20;
  const unsigned int maxIter = 100000;

  //initialize population
  vector<Sol> population;
  population.reserve(populationSize);

  for (unsigned int i = 0; i < populationSize; ++i) {
    population.push_back(Sol(NeuronskaMreza(layout), set));
  }

  //mutate operator
  Mutate mutate(0.1, 2, 0.95, 0.02);

  for (unsigned int currentIter = 0; currentIter < maxIter; ++currentIter) {

    scaleFitness(population, &best);
    vector<Sol> nextGen;
    nextGen.reserve(populationSize);

    cout << "iter: " << currentIter << ", best MSE : " << best->err << endl;

    nextGen.push_back(*best);

    while (nextGen.size() != populationSize) {
      auto mom = select(population);
      auto dad = select(population);
      //TODO
      auto childGenom = mom.nen.params * dad.nen.params;
      mutate(childGenom);
      Sol s(NeuronskaMreza(childGenom, layout), set);
      nextGen.push_back(s);
    }

    population = nextGen;
  }

  //log best to console on kill
  killHandler(0);

  return 0;
}
