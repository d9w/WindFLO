
// Author: Ahmed Kheiri
// Method: Sequence-based Selection Hyper-heuristic

import java.util.ArrayList;
import java.util.Random;


public class SSHH {
	WindFarmLayoutEvaluator wfle;
	Random rand;
	int noIterations,getNumberOfEvaluation;
	int[] utilisation;
	boolean[] solution;
	boolean[] best_solution;
	boolean[] old_solution;
	double oldObj,bestObj;
	int[] solutionInR;
	int[] solutionInC;
	int step_x, step_y, old_step_x, old_step_y;
	int best_step_x, best_step_y;
	int shift, old_shift, best_shift;
	int minRadCell;
	ArrayList<double[]> grid;
	Heuristic[] heuristic;
	int[] heuristic_id;
	int[] move_acceptance;
	int[] parameter;
	int current_history_index;
	ArrayList<int[]> solutionsRecord;
	ArrayList<Double> objsRecord;
	int noSolutionsAlreadyEvaluated;	
	int[][] indexInSolution;
	int rows, cols;	
	int getNumberOfLowLevelHeuristics() {
		return 5;
	}
	
	// return the number of rows
	int getNumberOfRows() {
		return rows;
	}
	
	// return the number of cols
	int getNumberOfCols() {
		return cols;
	}
	
	// make sure that the turbine can be inserted at location "index" without violating any constraint
	void FixPosition(int index) {
		if (minRadCell > 0) {
			if (solution[index]) {
				int currentRow=solutionInR[index], currentCol=solutionInC[index];		
				for (int i=currentRow-minRadCell; i<=currentRow+minRadCell; ++i) {
					for (int j=currentCol-minRadCell; j<=currentCol+minRadCell; ++j) {
						if ( (i >= 0) && (i < getNumberOfRows()) && (j >= 0) && (j < getNumberOfCols()) ) {
							if (!( (i==currentRow) && (j==currentCol) )) {
								if (indexInSolution[i][j] != -1) {
									if (solution[indexInSolution[i][j]]) {
										solution[index] = false;
										return;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	// mutational operator
	void increaseDecreaseStepX(int p) {
		if (rand.nextBoolean()) p*=-1;
		step_x += p;
		if (step_x < 0) step_x = 0;
		
	}
	
	// mutational operator
	void increaseDecreaseStepY(int p) {
		if (rand.nextBoolean()) p*=-1;
		step_y += p;
		if (step_y < 0) step_y = 0;
	}
	
	// mutational operator
	void increaseDecreaseShift(int p) {
		if (rand.nextBoolean()) p*=-1;
		shift += p;
		if (shift < 0) shift = 0;
	}

	// apply low level heuristic with a given parameter
	void applyLLH(int h, int p) {
		if (p == 0) p = 1;
		else if (p == 1) p = 1+rand.nextInt(10);
		else if (p == 2) p = 10+rand.nextInt(minRadCell-9);
		
		double deleteProb = 0.30;
		
		if (h == 0) {
			if (rand.nextDouble() < deleteProb) {shift = 0;}
			increaseDecreaseStepX(p);
		}
		else if (h == 1) {
			if (rand.nextDouble() < deleteProb) {shift = 0;}
			increaseDecreaseStepY(p);
		}
		else if (h == 2) {
			if (rand.nextDouble() < deleteProb) {step_x = 0;}
			if (rand.nextDouble() < deleteProb) {step_y = 0;}
			increaseDecreaseShift(p);
		}
		else if (h == 3) {
			if (rand.nextDouble() < deleteProb) {shift = 0;}
			increaseDecreaseStepX(p);
			if (p == 0) p = 1;
			else if (p == 1) p = 1+rand.nextInt(10);
			else if (p == 2) p = 10+rand.nextInt(minRadCell-9);
			increaseDecreaseStepY(p);
		}
		else if (h == 4) {
			increaseDecreaseStepX(p);
			if (p == 0) p = 1;
			else if (p == 1) p = 1+rand.nextInt(10);
			else if (p == 2) p = 10+rand.nextInt(minRadCell-9);
			increaseDecreaseStepY(p);
			if (p == 0) p = 1;
			else if (p == 1) p = 1+rand.nextInt(10);
			else if (p == 2) p = 10+rand.nextInt(minRadCell-9);
			increaseDecreaseShift(p);
		}
		
		ConstructSolution();
	}
	
	// construct a solution using X (step_x), Y (step_y) and S (shift)
	void ConstructSolution() {
		for (int i=0; i<grid.size(); i++) {
			solution[i] = false;
		}
		for (int i=0; i<getNumberOfRows(); i+=(minRadCell + 1 + step_x)) {
			for (int j=0; j<getNumberOfCols(); j+=(minRadCell + 1 + step_y)) {
				int current_shift = (j/(minRadCell + 1 + step_y))*shift;
				boolean needFix = false;
				current_shift = i+current_shift;
				if ( current_shift >= getNumberOfRows()) {
					needFix = true;
					current_shift = current_shift % getNumberOfRows();
				}
				if (indexInSolution[current_shift][j] != -1) {
					solution[indexInSolution[current_shift][j]] = true;
					if (needFix) {
						FixPosition(indexInSolution[current_shift][j]);
					}
				}
			}
		}
	}
	
	// generate an initial solution
	void InitialSolution() {
		oldObj = Double.MAX_VALUE;
		step_x = 0; step_y = 0; shift = 0;
		ConstructSolution();
		evaluate();
	}
	
	// copy a solution from src to dest
	void CopySolution(boolean[] src, boolean[] dest) {
		for (int i=0; i<grid.size(); i++) {
			dest[i] = src[i];
		}
	}
	
	// sequence-based selection hyper-heuristic
	public SSHH(WindFarmLayoutEvaluator evaluator) {
		noIterations = 1800;
		long seed = 10;
		
		
		getNumberOfEvaluation = 0;
		bestObj = Double.MAX_VALUE;
		wfle = evaluator;
		rand = new Random(seed);
		grid = new ArrayList<double[]>();
		
		solutionsRecord = new ArrayList<int[]>();
		objsRecord = new ArrayList<Double>();
		noSolutionsAlreadyEvaluated = 0;
		
		heuristic = new Heuristic[getNumberOfLowLevelHeuristics()];
		for (int i=0; i<getNumberOfLowLevelHeuristics(); ++i) {
			heuristic[i] = new Heuristic(getNumberOfLowLevelHeuristics());
		}
		heuristic_id = new int[heuristic[0].maximum_size_array+1];
		move_acceptance = new int[heuristic[0].maximum_size_array+1];
		parameter = new int[heuristic[0].maximum_size_array+1];
		current_history_index = 1;
	}

	// cache solutions
	private int evaluatedBefore() {
		if (grid.size() < 15) return -1;
		
		int nturbines = 0;
		for (int i=0; i<grid.size(); i++) {
			if (solution[i]) {
				nturbines++;
			}
		}

		for (int i=0; i<solutionsRecord.size(); i++) {
			boolean found = true;
			if (solutionsRecord.get(i).length != nturbines) {found = false; continue;}
			int n=-1;
			for (int j=0; j<solutionsRecord.get(i).length; j++) {
				++n;
				for (; n<grid.size(); n++) {
					if (solution[n]) {
						if (solutionsRecord.get(i)[j]!=n) {
							found = false;
						}
						break;
					}
				}
				if (!found) break;
			}
			if (found) return i;
		}

		int[] temp_solution = new int[nturbines];
		int n=0;
		for (int i=0; i<grid.size(); i++) {
			if (solution[i]) {
				temp_solution[n++] = i;
			}
		}
		solutionsRecord.add(temp_solution);
		return -1;
	}
	
	// apply evaluate + move acceptance method and return true if the best solution in hand is improved
	private boolean evaluate() {
		//check all zeros;
		boolean allZeros = true;
		for (int i=0; i<grid.size(); i++) {
			if (solution[i]) {
				allZeros = false;
				break;
			}
		}
		if (allZeros) solution[rand.nextInt(solution.length)] = true;
	
		boolean is_best_solution_improved = false;
		int nturbines=0;
		for (int i=0; i<grid.size(); i++) {
			if (solution[i]) {
				nturbines++;
			}
		}

		double[][] layout = new double[nturbines][2];
		int l_i = 0;
		for (int i=0; i<grid.size(); i++) {
			if (solution[i]) {
				layout[l_i][0] = grid.get(i)[0];
				layout[l_i][1] = grid.get(i)[1];
				l_i++;
			}
		}

		int no_eval_before = wfle.getNumberOfEvaluation();
		
		double obj;
		int ind = evaluatedBefore();
		if (ind>=0) {
			obj = objsRecord.get(ind);
			++noSolutionsAlreadyEvaluated;
		}
		else {
		
			if (wfle.checkConstraint(layout)) {
				wfle.evaluate(layout);
				obj = calculateCostManually(layout.length); // wfle.getEnergyCost();
				if (wfle.getNumberOfEvaluation() > no_eval_before) {
					++getNumberOfEvaluation;
				}
			} else {
				obj = Double.MAX_VALUE;
			}
			double temp_obj = obj;
			objsRecord.add(temp_obj);
		}
		
		if (obj <= oldObj) {
			oldObj = obj;
			CopySolution(solution,old_solution);
			old_step_x = step_x;
			old_step_y = step_y;
			old_shift = shift;

			if (obj < bestObj) {
				bestObj = obj;
				CopySolution(solution,best_solution);
				best_step_x = step_x;
				best_step_y = step_y;
				best_shift = shift;
				is_best_solution_improved = true;
			}
		}
		else if (obj <= bestObj+0.01*bestObj) {
			oldObj = obj;
			CopySolution(solution,old_solution);
			old_step_x = step_x;
			old_step_y = step_y;
			old_shift = shift;
		}
		else {
			obj = oldObj;
			CopySolution(old_solution,solution);
			step_x = old_step_x;
			step_y = old_step_y;
			shift = old_shift;
		}
		return is_best_solution_improved;
	}

	// calculate objective function value
	public double calculateCostManually(int n) {
		double ct  = 750000.0;
		double cs  = 8000000.0;
		double m   = 30.0;
		double r   = 0.03;
		double y   = 20.0;
		double com = 20000.0;
		double energyCost = (((ct*n+cs*Math.floor(n/m))*(0.666667+0.333333*Math.exp(-0.00174*n*n))+com*n)/
	    ((1.0-Math.pow(1.0+r, -y))/r)/(8760.0*wfle.getEnergyOutput()))+0.1/n;
		
		return energyCost;
	}

	public void run() {
		minRadCell = 79;
		double interval = 0.10001 * wfle.getTurbineRadius();		
		rows = 0; cols = 0;
		for (double x=0.0; x<wfle.getFarmWidth(); x+=interval) ++rows;
		for (double y=0.0; y<wfle.getFarmHeight(); y+=interval) ++cols;
		indexInSolution = new int[rows][cols];
		int index_in_grid = 0;
		
		int current_row, current_col;
		current_row = -1;
		for (double x=0.0; x<wfle.getFarmWidth(); x+=interval) {
			current_col = -1;
			++current_row;
			for (double y=0.0; y<wfle.getFarmHeight(); y+=interval) {
				++current_col;
				boolean valid = true;
				for (int o=0; o<wfle.getObstacles().length; o++) {
					double[] obs = wfle.getObstacles()[o];
					if (x>obs[0] && y>obs[1] && x<obs[2] && y<obs[3]) {
						valid = false;
					}
				}

				if (valid) {
					double[] point = {x, y};
					grid.add(point);
					indexInSolution[current_row][current_col] = index_in_grid++;
				}
				else {
					indexInSolution[current_row][current_col] = -1;
				}
			}
		}

		// To speed-up search
		solutionInR = new int[grid.size()];
		solutionInC = new int[grid.size()];
		index_in_grid = 0;
		current_row = -1;
		for (double x=0.0; x<wfle.getFarmWidth(); x+=interval) {
			current_col = -1;
			++current_row;
			for (double y=0.0; y<wfle.getFarmHeight(); y+=interval) {
				++current_col;
				boolean valid = true;
				for (int o=0; o<wfle.getObstacles().length; o++) {
					double[] obs = wfle.getObstacles()[o];
					if (x>obs[0] && y>obs[1] && x<obs[2] && y<obs[3]) {
						valid = false;
					}
				}

				if (valid) {
					solutionInR[index_in_grid] = current_row;
					solutionInC[index_in_grid++] = current_col;
				}
			}
		}

		utilisation = new int[getNumberOfLowLevelHeuristics()];
		for (int i=0; i<getNumberOfLowLevelHeuristics(); ++i) {
			utilisation[i] = 0;
		}
		solution = new boolean[grid.size()];
		best_solution = new boolean[grid.size()];
		old_solution = new boolean[grid.size()];
		
		InitialSolution();
		
		heuristic_id[0] = rand.nextInt(getNumberOfLowLevelHeuristics());
		
		// SSHH
		while (getNumberOfEvaluation < noIterations) {

			heuristic_id[current_history_index] = heuristic[heuristic_id[current_history_index-1]].nextHeuristicSelection[rand.nextInt(heuristic[heuristic_id[current_history_index-1]].no_added_next_heuristics)];
			move_acceptance[current_history_index] = heuristic[heuristic_id[current_history_index]].sequenceBasedAcceptanceStrategySelection[rand.nextInt(heuristic[heuristic_id[current_history_index]].no_added_move_acceptances)];
			parameter[current_history_index] = heuristic[heuristic_id[current_history_index]].parameterSelection[rand.nextInt(heuristic[heuristic_id[current_history_index]].no_added_parameters)];
			
			if (current_history_index >= heuristic[0].maximum_size_array-1) {
				move_acceptance[current_history_index] = 0;
			}

			applyLLH(heuristic_id[current_history_index], parameter[current_history_index]);
			if (move_acceptance[current_history_index] == 0) {
				if (evaluate()) {
					utilisation[heuristic_id[current_history_index]]++;
					for (int i=current_history_index; i>0; --i) {
						if (heuristic[heuristic_id[i-1]].no_added_next_heuristics < heuristic[heuristic_id[i-1]].maximum_size_array - 1)
							heuristic[heuristic_id[i-1]].nextHeuristicSelection[heuristic[heuristic_id[i-1]].no_added_next_heuristics++] = heuristic_id[i];
						if (heuristic[heuristic_id[i]].no_added_move_acceptances < heuristic[heuristic_id[i]].maximum_size_array - 1)
							heuristic[heuristic_id[i]].sequenceBasedAcceptanceStrategySelection[heuristic[heuristic_id[i]].no_added_move_acceptances++] = move_acceptance[i];
						if (heuristic[heuristic_id[i]].no_added_parameters < heuristic[heuristic_id[i]].maximum_size_array - 1)
							heuristic[heuristic_id[i]].parameterSelection[heuristic[heuristic_id[i]].no_added_parameters++] = parameter[i];
					}
				}
				heuristic_id[0] = heuristic_id[current_history_index];
				current_history_index = 1;
			} else {
				current_history_index++;
			}		
		}
		
		CopySolution(best_solution,solution);
	}
}