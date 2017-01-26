#pragma once
#include "NeuronskaMreza.hpp"

struct Sol {
  NeuronskaMreza nen;
  double err;
  double fit;

  Sol(NeuronskaMreza n, const Dataset& set)
      : nen{ n }
  {
    err = n.calcError(set);
    fit = 1 / err;
  };

  Sol(NeuronskaMreza n, double err)
      : nen{ n }
      , err{ err }
      , fit{ 1 / err } {};
};
