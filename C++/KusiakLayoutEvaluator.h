#ifndef __KUSIAK_LAYOUT_EVALUATOR_H__
#define __KUSIAK_LAYOUT_EVALUATOR_H__

#include "WindFarmLayoutEvaluator.h"
#include "WindScenario.h"
#include "Matrix.hpp"
#include <limits>

class KusiakLayoutEvaluator : public WindFarmLayoutEvaluator {
  public:
    KusiakLayoutEvaluator();
    virtual ~KusiakLayoutEvaluator();

    virtual void initialize(WindScenario& scenario);

    virtual double evaluate(Matrix<double>* layout);
    virtual double evaluate_2014(Matrix<double>* layout);

    virtual Matrix<double>* getEnergyOutputs();
    virtual Matrix<double>* getTurbineFitnesses();
    virtual double getEnergyOutput() {return energyCapture;};
    virtual double getWakeFreeRatio() {return wakeFreeRatio;};
    WindScenario scenario;

 protected:
    Matrix<double>* tspe;
    Matrix<double>* tpositions;
    double energyCapture;
    double wakeFreeEnergy;
    double wakeFreeRatio;

    bool checkConstraint();
    double calculateWakeTurbine(int index, double theta);
    double calculateWakeTurbine(int index, int thetindex);
    double calculateBeta(double xi, double yi, double xj, double yj, double theta);
    double calculateBeta(double xi, double yi, double xj, double yj, int thetIndex);
    double calculateVelocityDeficit(double dij);
    double calculateProjectedDistance(double xi, double yi, double xj, double yj, double theta);
    double calculateProjectedDistance(double xi, double yi, double xj, double yj, int thetIndex);
    double powOutput(double v);

    inline double wblcdf(double x, double sc=1.0, double sh=1.0) {return 1.0-exp(-fastPow(x/sc,sh));};

    // faster because b is usually equal to 2!
    inline static double fastPow(double a, double b) {
        if (abs(b-2)<0.0001) {
            return a*a;
        } if (abs(b-1)<0.0001) {
            return a;
        } if (abs(b)<0.0001) {
            return 1;
        } else {
            return pow(a,b);
        }
    }

};

#endif /* defined(__KUSIAK_LAYOUT_EVALUATOR_H__) */
