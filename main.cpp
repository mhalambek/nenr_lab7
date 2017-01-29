#include "Dataset.hpp"
#include "NeuralNetwork.hpp"
#include "Solution.hpp"
#include "crossing.hpp"
#include "utils.hpp"
#include <cassert>
#include <iostream>
#include <random>

Solution best;

void killHandler(int signum)
{
  cout << "received signal: " << signum << endl;
  cout << "best mean error: " << best.err << endl;
  cout << endl;
  for (auto d : best.sol) {
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
  NeuralNetwork ann(layout);

  const unsigned int populationSize = 5;
  const unsigned int maxIter = 100000;

  //initialize population
  vector<Solution> population;
  population.reserve(populationSize);

  for (unsigned int i = 0; i < populationSize; ++i) {
    population.push_back(Solution(ann.size));
    population.back().err = ann.calcError(set, population.back().sol);
    population.back().fit = 1 / population.back().err;
  }

  //mutate operator
  Mutate mutate(0.1, 2, 0.95, 0.02);

  for (unsigned int currentIter = 0; currentIter < maxIter; ++currentIter) {

    scaleFitness(population, best);
    vector<Solution> nextGen;
    nextGen.reserve(populationSize);

    cout << "iter: " << currentIter << ", best MSE : " << best.err << endl;

    nextGen.push_back(best);

    while (nextGen.size() != populationSize) {
      auto mom = select(population);
      auto dad = select(population);
      //TODO
      auto child = mom * dad;
      mutate(child.sol);
      child.err = ann.calcError(set, child.sol);
      child.fit = 1 / child.err;
      nextGen.push_back(child);
    }

    population = nextGen;
  }

  //log best to console on kill
  killHandler(0);

  return 0;
}
