#ifndef __WIND_FARM_LAYOUT_EVALUATOR_H__
#define __WIND_FARM_LAYOUT_EVALUATOR_H__

#include "Matrix.hpp"
#include "WindScenario.h"

class WindFarmLayoutEvaluator {
 public:
  WindFarmLayoutEvaluator(){};
  virtual ~WindFarmLayoutEvaluator(){};
  virtual void initialize(WindScenario& scenario)=0;
  virtual double evaluate(Matrix<double>* layout)=0;
  virtual Matrix<double>* getEnergyOutputs()=0;
  virtual Matrix<double>* getTurbineFitnesses()=0;
  virtual double getEnergyOutput()=0;
  virtual double getWakeFreeRatio()=0;
  
  int getNumberOfEvaluation() {return nEvals;};
 
 protected:
  static int nEvals;
};

#endif /* defined(__WIND_FARM_LAYOUT_EVALUATOR_H__) */

