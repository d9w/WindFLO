#ifndef __GA_H__
#define __GA_H__

#include "CompetitionScenario.h"
#include "CompetitionEvaluator.h"
#include "Matrix.hpp"

class GA {
  public:
    CompetitionEvaluator wfle;
    Matrix<int>* pops;
    int num_pop;
    double* fits;
    int nt;
    int tour_size;
    double mut_rate;
    double cross_rate;
    Matrix<double>* grid;

    GA(CompetitionEvaluator evaluator);
    ~GA();
    void run();

  private:
    void evaluate();
};

#endif
