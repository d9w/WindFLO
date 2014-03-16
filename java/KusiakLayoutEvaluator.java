import java.io.BufferedReader;
import java.io.FileReader;
import java.util.StringTokenizer;

public class KusiakLayoutEvaluator extends WindFarmLayoutEvaluator {
	protected double tspe[][];
	protected double tpositions[][];
	protected double energyCapture;
	protected double wakeFreeRatio;
	protected WindScenario scenario;
	
	public static final double fac=Math.PI/180;

	@Override
	public void initialize(WindScenario scenario) {
		tspe=null;
		tpositions=null;
		energyCapture=0;
		wakeFreeRatio=0;
		this.scenario=scenario;
	}

	@Override
	public double evaluate(double[][] layout) {
		WindFarmLayoutEvaluator.nEvals++;
		// Copying the layout
		tpositions=new double[layout.length][layout[0].length];
		for (int i=0; i<layout.length; i++) {
			for (int j=0; j<layout[i].length; j++) {
				tpositions[i][j]=layout[i][j];
			}
		}

		energyCapture=0;
		if (checkConstraint()) {
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
	public double[] getEnergyOutputs() {
		double res[]=new double[tspe[0].length];
		for (int i=0; i<res.length; i++) {
			res[i]=0;
			for (int j=0; j<tspe[i].length; j++) {
				res[i]+=tspe[i][j];
			}
		}
		return res;
	}

	@Override
	public double[][] getTurbineFitnesses() {
		return tspe;
	}

	protected boolean checkConstraint() {
	    for (int i=0; i<tpositions.length; i++) {
	        for (int j=0; j<tpositions.length; j++) {
	            if (i!=j) {
	                // calculate the sqared distance between both turb
	                double dist=(tpositions[i][0]-tpositions[j][0])*(tpositions[i][0]-tpositions[j][0])+
	                (tpositions[i][1]-tpositions[j][1])*(tpositions[i][1]-tpositions[j][1]);
	                if (dist<scenario.minDist) {
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
	
	public static void main(String argv[]) {
		try {
			int n=144;
			double layout[][]=new double[n][2];
			String currentLine;
			BufferedReader br = new BufferedReader(new FileReader("layout2.txt"));
			int i=0;
			while ((currentLine=br.readLine()) != null && i<n) {
				StringTokenizer tokenizer=new StringTokenizer(currentLine);
				int j=0;
				while (tokenizer.hasMoreElements() && j<2) {
					layout[i][j]=Double.parseDouble(tokenizer.nextToken());
					j++;
				}
				i++;
			}
			WindScenario ws = new WindScenario("01.xml");
			KusiakLayoutEvaluator le = new KusiakLayoutEvaluator();
			le.initialize(ws);
			le.evaluate(layout);
			System.out.println("Energy output: "+le.getEnergyOutput());
			System.out.println("Wake free: "+le.getWakeFreeRatio());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}


}
