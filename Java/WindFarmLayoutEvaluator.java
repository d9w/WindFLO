/**
 * The class WindFarmLayoutEvaluator is an interface to easily exchange the
 * evaluation function of the wind farm layouts. The evaluator has to be initialized
 * with a wind scenario before being used to evaluate any layouts with the
 * evaluation function. After evaluation, the output data (energy output per 
 * turbine, per direction, etc.) are available by the means of the corresponding
 * getters. Each time the evaluation function is used, a global counter is 
 * increased. This counter is available with the function getNumberOfEvaluation.
 */
public abstract class WindFarmLayoutEvaluator {
	protected static int nEvals=0;

	/**
	 * 2015 WIND FARM LAYOUT OPTIMIZATION EVALUATION FUNCTION
         *
	 * Evaluates a given layout and returns its cost of energy
	 * Calling this method increases the number of evaluations counter.
	 * @param layout The layout to evaluate
	 * @return the cost of energy (positive) 
	 * and max_double if the layout is invalid
	 */
	public abstract double evaluate(double[][] layout);

	/**
	 * Returns the energy outputs per wind turbine and per direction of the last
	 * layout evaluated, ordered as in the layout vector provided to the
	 * evaluation method and the wind scenario wind directions.
	 * A layout must have been evaluated before this method is called.  This
	 * method doesn't increase the number of evaluation counter.
	 * @return The energy outputs; null if no layout have been evaluated
	 */
	public abstract double[][] getEnergyOutputs();

	/**
	 * Returns the wake free ratio per wind turbine of the last layout
	 * evaluated, ordered as in the layout vector provided in the evaluation
	 * method.
	 * A layout must have been evaluated before this method is called.  This
	 * method doesn't increase the number of evaluation counter.
	 * @return The wake free ratio per turbine
	 */
	public abstract double[] getTurbineFitnesses();

	/**
	 * Returns the global energy output of the last layout evaluated.
	 * A layout must have been evaluated before this method is called.
	 * This method doesn't increase the number of evaluation counter. 
	 * @return The global energy output
	 */
	public abstract double getEnergyOutput();

	/**
	 * Returns the global wake free ratio of the last layout evaluated.
	 * A layout must have been evaluated before this method is called.
	 * This method doesn't increase the number of evaluation counter. 
	 * @return The global wake free ratio
	 */
	public abstract double getWakeFreeRatio();

	/**
	 * Returns the energy cost of the last layout evaluated.
	 * A layout must have been evaluated before this method is called.
	 * This method doesn't increase the number of evaluation counter. 
	 * @return The energy cost
	 */
	public abstract double getEnergyCost();


	/**
	 * Returns the global number of time the evaluation function has been called.
	 */
	public static int getNumberOfEvaluation() {
		return nEvals;
	}

    /**
     * Check if a layout violate or not the constraints of a wind farm. It checks:
     *     - no turbine violates the security distance
     *     - no turbine is out of the farm
     *     - no turbine is inside an obstacle
     * This function does not increase the evaluation counter.
     * @param layout The layout to check
     * @result true if the layout is valid, false otherwise
     */
        public abstract boolean checkConstraint(double layout[][]);

    /**
     * Returns the radius of one turbine
     */
    public abstract double getTurbineRadius();

    /**
     * Returns the farm width
     */
    public abstract double getFarmWidth();
    
    /**
     * Returns the farm heigth
     */
    public abstract double getFarmHeight();

    /**
     * Return the obstacles of the farm
     * @return an array of [xmin, ymin, xmax, ymax] for each obstacle.
     */
    public abstract double[][] getObstacles();
}
