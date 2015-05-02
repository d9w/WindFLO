#ifndef __WIND_FARM_LAYOUT_EVALUATOR_H__
#define __WIND_FARM_LAYOUT_EVALUATOR_H__

#include "Matrix.hpp"
#include "WindScenario.h"

/**
 * The class WindFarmLayoutEvaluator is an interface to easily exchange the
 * evaluation function of the wind farm layouts. The evaluator has to be initialized
 * with a wind scenario before being used to evaluate any layouts with the
 * evaluation function. After evaluation, the output data (energy output per
 * turbine, per direction, etc.) are available by the means of the corresponding
 * getters. Each time the evaluation function is used, a global counter is
 * increased. This counter is available with the function getNumberOfEvaluation.
 */
class WindFarmLayoutEvaluator {
 public:
  WindFarmLayoutEvaluator(){};
  virtual ~WindFarmLayoutEvaluator(){};

    /**
	 * Initializes the evaluator with a wind scenario
	 * This method doesn't increase the number of evaluations counter.
	 * @param scenario
	 */
  virtual void initialize(WindScenario& scenario)=0;
    
    /**
     * 2015 WIND FARM LAYOUT OPTIMIZATION EVALUATION FUNCTION
     *
     * Evaluates a given layout and returns its cost of energy.
     * This method increases the number of evaluations counter.
     * @param layout The layout to evaluate
     * @return The cost cost of energy (positive)
     * and max_double if the layout is invalid
     */
  virtual double evaluate(Matrix<double>* layout)=0;

    /**
     * 2014 WIND FARM LAYOUT OPTIMIZATION EVALUATION FUNCTION
     *
     * Evaluates a given layout and returns its wake free ratio
     * This method increases the number of evaluations counter.
     * @param layout The layout to evaluate
     * @return The wake free ratio of the layout
     * or a negative value is the layout is invalid
     */
  virtual double evaluate_2014(Matrix<double>* layout)=0;

        /**
         * Returns the energy outputs per wind turbine and per direction of the
         * last layout evaluated, ordered as in the layout vector provided to
         * the evaluation method and the wind scenario wind directions.
         * A layout must have been evaluated before this method is called.
         * This method doesn't increase the number of evaluation counter.
         * @return The energy outputs; null if no layout have been evaluated
	 */

  virtual Matrix<double>* getEnergyOutputs()=0;

        /**
         * Returns the wake free ratio per wind turbine of the last layout
         * evaluated, ordered as in the layout vector provided in the
         * evaluation method.
         * A layout must have been evaluated before this method is called.
         * This method doesn't increase the number of evaluation counter.
         * @return The wake free ratio per turbine
	 */
    virtual Matrix<double>* getTurbineFitnesses()=0;

    /**
	 * Returns the global energy output of the last layout evaluated.
	 * A layout must have been evaluated before this method is called.
	 * This method doesn't increase the number of evaluation counter.
	 * @return The global energy output
	 */
    virtual double getEnergyOutput()=0;

    /**
	 * Returns the global wake free ratio of the last layout evaluated.
	 * A layout must have been evaluated before this method is called.
	 * This method doesn't increase the number of evaluation counter.
	 * @return The global wake free ratio
	 */
    virtual double getWakeFreeRatio()=0;

 	/**
	 * Returns the energy cost of the last layout evaluated.
	 * A layout must have been evaluated before this method is called.
	 * This method doesn't increase the number of evaluation counter. 
	 * @return The energy cost
	 */
    virtual double getEnergyCost()=0;

 
    /**
     * Returns the global number of time the evaluation function has been called.
     */
  int getNumberOfEvaluation() {return nEvals;};
 
 protected:
  static int nEvals;
};

#endif /* defined(__WIND_FARM_LAYOUT_EVALUATOR_H__) */

