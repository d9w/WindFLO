#ifndef __COMPETITION_EVALUATOR_H__
#define __COMPETITION_EVALUATOR_H__

#include "WindFarmLayoutEvaluator.h"
#include "WindScenario.h"
#include "CompetitionScenario.h"
#include "Matrix.hpp"
#include <limits>

class CompetitionEvaluator : public WindFarmLayoutEvaluator {
  public:
    CompetitionEvaluator();
    virtual ~CompetitionEvaluator();

    virtual void initialize(CompetitionScenario& scenario, const char* user_token);
    virtual void initialize(WindScenario& scenario) {};

    virtual double evaluate(Matrix<double>* layout);
    virtual double evaluate_2014(Matrix<double>* layout) {return wakeFreeRatio;};

    virtual Matrix<double>* getEnergyOutputs();
    virtual Matrix<double>* getTurbineFitnesses();
    virtual double getEnergyOutput() {return energyCapture;};
    virtual double getWakeFreeRatio() {return wakeFreeRatio;};
    virtual double getEnergyCost() {return energyCost;};
    CompetitionScenario scenario;
    bool checkConstraint(Matrix<double>* layout);

 protected:
    Matrix<double>* tspe;
    Matrix<double>* tpositions;
    Matrix<double>* tfitnesses;
    double energyCapture;
    double wakeFreeEnergy;
    double wakeFreeRatio;
    double energyCost;
    string user_token;
    string run_token;

};

#endif /* defined(__COMPETITION_EVALUATOR_H__) */
