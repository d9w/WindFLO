public abstract class WindFarmLayoutEvaluator {
	protected static int nEvals=0;
	
	/**
	 * Initializes the evaluator with a wind scenario
	 * This method doesn't increase the number of evaluations counter.
	 * @param scenario
	 */
	public abstract void initialize(WindScenario scenario);

	/**
	 * Evaluates a given layout and returns its wake free ratio
	 * This method increases the number of evaluations counter.
	 * @param layout The layout to evaluate
	 * @return The wake free ratio of the layout 
	 * or a negative value is the layout is invalid
	 */
	public abstract double evaluate(double[][] layout);
	
	/**
	 * Returns the energy outputs per wind turbine and per direction of the last layout evaluated, 
	 * ordered as in the layout vector provided to the evaluation method and the wind scenario wind directions.
	 * A layout must have been evaluated before this method is called.
	 * This method doesn't increase the number of evaluation counter.
	 * @return The energy outputs; null if no layout have been evaluated
	 */
	public abstract double[] getEnergyOutputs();
	
	/**
	 * Returns the wake free ratio per wind turbine of the last layout evaluated, ordered as in the layout vector
	 * provided in the evaluation method. A layout must have been evaluated before this method is called.
	 * This method doesn't increase the number of evaluation counter. 
	 * @return The wake free ratio per turbine
	 */
	public abstract double[][] getTurbineFitnesses();
	
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
	
	public static int getNumberOfEvaluation() {
		return nEvals;
	}	
	
}
