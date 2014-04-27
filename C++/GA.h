#ifndef __GA_H__
#define __GA_H__

#include "WindScenario.h"
#include "KusiakLayoutEvaluator.h"
#include "Matrix.hpp"

class GA {
  public:
    KusiakLayoutEvaluator wfle;
    Matrix<bool> pops;
    double fits[];
    int num_pop;
    int nt;
    int tour_size;
    double mut_rate;
    double cross_rate;
    Matrix<double> grid;

    GA(KusiakLayoutEvaluator evaluator);
    //~GA();
    void run();

  private:
    void evaluate();
};

#endif
