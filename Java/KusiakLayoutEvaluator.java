import java.io.BufferedReader;
import java.io.FileReader;
import java.util.StringTokenizer;

public class KusiakLayoutEvaluator extends WindFarmLayoutEvaluator {
	protected double tspe[][];
	protected double tpositions[][];
	protected double energyCapture;
	protected double wakeFreeRatio;
	protected double energyCost;

	protected WindScenario scenario;
	
	public static final double fac=Math.PI/180;

	public void initialize(WindScenario scenario) {
		tspe=null;
		tpositions=null;
		energyCapture=0;
		wakeFreeRatio=0;
		this.scenario=scenario;
                energyCost=Double.MAX_VALUE;
	}

        @Override
	public double evaluate(double[][] layout) {
	    final double ct  = 750000;
	    final double cs  = 8000000;
	    final double m   = 30;
	    final double r   = 0.03;
	    final double y   = 20;
	    final double com = 20000;

	    double wfr = evaluate_2014(layout);
	    if (wfr<=0) return Double.MAX_VALUE; 
	    int n = layout.length;
	    
	    energyCost = (((ct*n+cs*Math.floor(n/m))*(0.666667+0.333333*Math.exp(-0.00174*n*n))+com*n)/
		    ((1-Math.pow(1+r, -y))/r)/(8760.0*scenario.wakeFreeEnergy*wfr*n))+0.1/n;

	    //	    System.out.println(n+"\t"+wfr+"\t"+fit);

	    return energyCost;
	}

	public double evaluate_2014(double[][] layout) {
		WindFarmLayoutEvaluator.nEvals++;
		// Copying the layout
		tpositions=new double[layout.length][layout[0].length];
		for (int i=0; i<layout.length; i++) {
			for (int j=0; j<layout[i].length; j++) {
				tpositions[i][j]=layout[i][j];
			}
		}

		energyCapture=0;
		if (checkConstraint(tpositions)) {
			tspe=new double[scenario.thetas.length][tpositions.length];
			// wind resource per turbine => stored temporaly in tspe
			for (int turb=0; turb<tpositions.length; turb++) {
				// for each turbine
				for (int thets=0; thets<scenario.thetas.length; thets++) {
					// for each direction
					// calculate the wake
					double totalVdef=calculateWakeTurbine(turb, thets);
					double cTurb=scenario.c[thets]*(1.0-totalVdef);
					// annual power output per turbine and per direction
					double tint=scenario.thetas[thets][1]-scenario.thetas[thets][0];
					double w=scenario.omegas[thets];
					double ki=scenario.ks[thets];
					double totalPow=0;
					for (int ghh=1; ghh<scenario.vints.length; ghh++) {
						double v=(scenario.vints[ghh]+scenario.vints[ghh-1])/2.0;
						double P=powOutput(v);
						double prV=WindScenario.wblcdf(scenario.vints[ghh], cTurb, ki)-WindScenario.wblcdf(scenario.vints[ghh-1],cTurb,ki);
						totalPow+=prV*P;
					}
					totalPow+=scenario.PRated*(1.0-WindScenario.wblcdf(scenario.vRated, cTurb, ki));
					totalPow*=tint*w;
					tspe[thets][turb]=totalPow;
					energyCapture+=totalPow;
				}
			}
			wakeFreeRatio=energyCapture/(scenario.wakeFreeEnergy*tpositions.length);
			return wakeFreeRatio;
		} else {
			energyCapture=0;
			wakeFreeRatio=0;
			tspe=null;
			return 0;
		}
	}

	@Override
	public double[][] getEnergyOutputs() {
		return tspe;
	}

	@Override
	public double[] getTurbineFitnesses() {
		double res[]=new double[tspe[0].length];
		for (int i=0; i<res.length; i++) {
			res[i]=0;
			for (int j=0; j<tspe.length; j++) {
				res[i]+=tspe[j][i];
			}
			res[i]=res[i]/scenario.wakeFreeEnergy;
		}
		return res;
	}

	public boolean checkConstraint(double layout[][]) {
	    for (int i=0; i<layout.length; i++) {
		if (layout[i][0]!=layout[i][0] || layout[i][1]!=layout[i][1] || layout[i][0]<0.0 || layout[i][1]<0.0 || layout[i][0]>getFarmWidth() || layout[i][1]>getFarmHeight()) {
			System.out.println("Turbine "+i+"("+layout[i][0]+", "+layout[i][1]+") is invalid.");
			return false;
		}

		// checking obstacle constraints
		for (int j=0; j<scenario.obstacles.length; j++) {
		    if (layout[i][0] > scenario.obstacles[j][0] &&
			layout[i][0] < scenario.obstacles[j][2] &&
			layout[i][1] > scenario.obstacles[j][1] &&
			layout[i][1] < scenario.obstacles[j][3]) {
			System.out.println("Turbine "+i+"("+layout[i][0]+", "+layout[i][1]+") is in the obstacle "+j+" ["+scenario.obstacles[j][0]+", "+scenario.obstacles[j][1]+", "+scenario.obstacles[j][2]+", "+scenario.obstacles[j][3]+"].");
			return false;
		    }
		}
		// checking the security constraints
	        for (int j=0; j<layout.length; j++) {
	            if (i!=j) {
	                // calculate the sqared distance between both turb
	                double dist=(layout[i][0]-layout[j][0])*(layout[i][0]-layout[j][0])+
	                (layout[i][1]-layout[j][1])*(layout[i][1]-layout[j][1]);
	                if (dist<scenario.minDist) {
			    System.out.println("Security distance contraint violated between turbines "+i+" ("+layout[i][0]+", "+layout[i][1]+") and "+j+" ("+layout[j][0]+", "+layout[j][1]+"): "+Math.sqrt(dist)+" > "+Math.sqrt(scenario.minDist));
	                    return false;
	                }
	            }
	        }
	    }
	    return true;
	}

	protected double calculateWakeTurbine(int turb, int thetIndex) {
	    double x=tpositions[turb][0];
	    double y=tpositions[turb][1];
	    double velDef=0;
	    for (int oturb=0; oturb<tpositions.length; oturb++) {
	        if (oturb!=turb) {
	            double xo=tpositions[oturb][0];
	            double yo=tpositions[oturb][1];
	            double beta=calculateBeta(x, y, xo, yo, thetIndex);
	            if (beta<scenario.atan_k) {
	                double dij=calculateProjectedDistance(x, y, xo, yo, thetIndex);
	                double curDef=calculateVelocityDeficit(dij);
	                velDef+=curDef*curDef;
	            }
	        }
	    }
	    return Math.sqrt(velDef);
	}
	
	protected double calculateBeta(double xi, double yi, double xj, double yj, int thetIndex) {
	    double num=((xi-xj)*scenario.getCosMidThetas(thetIndex)+(yi-yj)*scenario.getSinMidThetas(thetIndex)+scenario.rkRatio);
	    double a=xi-xj+scenario.rkRatio*scenario.getCosMidThetas(thetIndex);
	    double b=yi-yj+scenario.rkRatio*scenario.getSinMidThetas(thetIndex);
	    double denom=Math.sqrt(a*a+b*b);
	    return Math.acos(num/denom);
	}
	
	protected double powOutput(double v) {
	    if (v<scenario.vCin) {
	        return 0;
	    } else if (v>=scenario.vCin && v<=scenario.vRated) {
	        return scenario.lambda*v+scenario.eta;
	    } else if (scenario.vCout>v && v>scenario.vRated) {
	        return scenario.PRated;
	    } else {
	        return 0;
	    }
	}
	
	protected double calculateProjectedDistance(double xi, double yi, double xj, double yj, int thetIndex) {
		return Math.abs((xi-xj)*scenario.getCosMidThetas(thetIndex)+(yi-yj)*scenario.getSinMidThetas(thetIndex));
	}

	double calculateVelocityDeficit(double dij) {
	    return scenario.trans_CT/((1.0+scenario.krRatio*dij)*(1.0+scenario.krRatio*dij));
	}

	@Override
	public double getEnergyOutput() {
		
		return energyCapture;
	}

	@Override
	public double getWakeFreeRatio() {
		return wakeFreeRatio;
	}
	
	@Override
	public double getEnergyCost() {
		return energyCost;
	}

    @Override
	public double getTurbineRadius() {
	return scenario.R;
    }

    @Override
	public double getFarmWidth() {
	return scenario.width;
    }

    @Override
	public double getFarmHeight() {
	return scenario.height;
    }

    @Override
	public double[][] getObstacles() {
	return scenario.obstacles;
    }
}
