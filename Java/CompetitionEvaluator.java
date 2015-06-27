import java.net.*;
import java.io.*;
import org.json.*;

/**
 * The class CompetitionEvaluator connects the evaluation cost function of the competition to the online 
 * server of the competition
 */
public class CompetitionEvaluator extends WindFarmLayoutEvaluator {
    protected int nScenario;        // scenario id (from 0 to 4)
    protected String user_token;
    protected String run_token;
    
    public double R;              // turbines size
    public double height;         // farm height
    public double width;          // farm width
    public double obstacles[][];  // rows of [xmin, ymin, xmax, ymax]

    protected double energyOutputs[][];
    protected double energyOutput;
    protected double turbineFitnesses[];
    protected double wakeFreeRatio;
    protected double energyCost;

    //protected String hostname = "http://windflo.com";
    protected String hostname = "http://52.28.81.122";
    /**
     * Initializes the evaluator with a scenario and a user token. This method generates a new run token.
     * This method doesn't increase the number of evaluations counter.
     * @param nScenario id of the scenario (from 0 to 4) to initialize the evaluator with
     * @param user_token the id of the user (see online leaderboard)
     */
    public void initialize(int nScenario, String user_token) {
	this.nScenario = nScenario;
	this.user_token = user_token;
	energyOutputs=null;
	turbineFitnesses=null;
	energyOutput=0;
	wakeFreeRatio=0;
	energyCost=Double.MAX_VALUE;

	try {
	    // getting scenario's data
	    URL targetURL = new URL(hostname+"/scenarios/"+nScenario);
	    URLConnection connection = targetURL.openConnection();
	    BufferedReader in = new BufferedReader(
				new InputStreamReader(
				connection.getInputStream()));
	    String inputLine;
	    String jsonString = new String();
	    while ((inputLine = in.readLine()) != null) {
		//System.out.println(inputLine);
		jsonString += inputLine;
	    }
	    
	    in.close();
	    JSONObject jsonObj = new JSONObject(jsonString);
	    height = jsonObj.getDouble("height");
	    width = jsonObj.getDouble("width");
	    R = jsonObj.getDouble("r");
	    JSONArray jsonObs = jsonObj.getJSONArray("obstacles");
	    obstacles = new double[jsonObs.length()][4];
	    for (int i=0; i<jsonObs.length(); i++) {
		obstacles[i][0] = jsonObs.getJSONObject(i).getDouble("xmin");
		obstacles[i][1] = jsonObs.getJSONObject(i).getDouble("ymin");
		obstacles[i][2] = jsonObs.getJSONObject(i).getDouble("xmax");
		obstacles[i][3] = jsonObs.getJSONObject(i).getDouble("ymax");
		//System.out.println(obstacles[i][0] + " " + obstacles[i][1] + " " + obstacles[i][2] + " " + obstacles[i][3] + " ");
	    }
	    //System.out.println(height+" "+width+" "+R);

	    // initializing a run
	    targetURL = new URL(hostname+"/runs/");
	    connection = targetURL.openConnection();
	    connection.setDoOutput(true);
	    connection.setRequestProperty("Content-Type", "application/json");
	    OutputStreamWriter out = new OutputStreamWriter(
							   connection.getOutputStream());
	    out.write("{\"api_token\":\""+user_token+"\"}");
	    out.close();
	    BufferedReader response = new BufferedReader(
				   new InputStreamReader(
							 connection.getInputStream()));
	    jsonString = "";
	    while ((inputLine = response.readLine()) != null) {
		//System.out.println(inputLine);
		jsonString+=inputLine;
	    }
	    jsonObj = new JSONObject(jsonString);
	    run_token = jsonObj.getString("token");
	    nEvals = jsonObj.getInt("evals");
	    System.out.println("Your run token is: "+run_token);
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }

    /**
     * Initializes the evaluator with a scenario, a user token and an existing run token
     * This method doesn't increase the number of evaluations counter.
     * @param nScenario id of the scenario (from 0 to 4) to initialize the evaluator with
     * @param user_token the id of the user (see online leaderboard)
     * @param run_token the id of the run (see online leaderboard)
     */
    public void initialize(int nScenario, String user_token, String run_token) {
		this.nScenario = nScenario;
	this.user_token = user_token;
	this.run_token = run_token;
	energyOutputs=null;
	turbineFitnesses=null;
	energyOutput=0;
	wakeFreeRatio=0;
	energyCost=Double.MAX_VALUE;

	try {
	    // getting scenario's data
	    URL targetURL = new URL(hostname+"/scenarios/"+nScenario);
	    URLConnection connection = targetURL.openConnection();
	    BufferedReader in = new BufferedReader(
				new InputStreamReader(
				connection.getInputStream()));
	    String inputLine;
	    String jsonString = new String();
	    while ((inputLine = in.readLine()) != null) {
		//System.out.println(inputLine);
		jsonString += inputLine;
	    }
	    
	    in.close();
	    JSONObject jsonObj = new JSONObject(jsonString);
	    height = jsonObj.getDouble("height");
	    width = jsonObj.getDouble("width");
	    R = jsonObj.getDouble("r");
	    JSONArray jsonObs = jsonObj.getJSONArray("obstacles");
	    obstacles = new double[jsonObs.length()][4];
	    for (int i=0; i<jsonObs.length(); i++) {
		obstacles[i][0] = jsonObs.getJSONObject(i).getDouble("xmin");
		obstacles[i][1] = jsonObs.getJSONObject(i).getDouble("ymin");
		obstacles[i][2] = jsonObs.getJSONObject(i).getDouble("xmax");
		obstacles[i][3] = jsonObs.getJSONObject(i).getDouble("ymax");
		//		System.out.println(obstacles[i][0] + " " + obstacles[i][1] + " " + obstacles[i][2] + " " + obstacles[i][3] + " ");
	    }
	    //	    System.out.println(height+" "+width+" "+R);

	} catch (Exception e) {
	    e.printStackTrace();
	}
    }

    /**
     * 2015 WIND FARM LAYOUT OPTIMIZATION EVALUATION FUNCTION
     *
     * Evaluates online a given layout and returns its cost of energy
     * Calling this method increases the number of evaluations counter.
     * @param layout The layout to evaluate
     * @return the cost of energy (positive) 
     * and max_double if the layout is invalid
     */
    public double evaluate(double[][] layout) {
	try {
	    // initializing a run
	    URL targetURL = new URL(hostname+"/evaluate/");
	    URLConnection connection = targetURL.openConnection();
	    connection.setDoOutput(true);
	    connection.setRequestProperty("Content-Type", "application/json");
	    OutputStreamWriter out = new OutputStreamWriter(
							   connection.getOutputStream());
	    String request = new String();
	    request += "{\"api_token\":\""+user_token+"\", \"run\":\""+run_token+"\", \"scenario\":"+nScenario+", ";
	    request += "\"xs\":["+layout[0][0];
	    for (int i=1; i<layout.length; i++) {
		request += ", "+layout[i][0];
	    }
	    request += "], \"ys\":[ "+layout[0][1];
	    for (int i=1; i<layout.length; i++) {
		request += ", "+layout[i][1];
	    }
	    request += "]}";
	    //System.out.println(request);
	    out.write(request);
	    out.close();
	    BufferedReader response = new BufferedReader(
				   new InputStreamReader(
							 connection.getInputStream()));
	    String inputLine;
	    String jsonString = "";
	    while ((inputLine = response.readLine()) != null) {
		//	System.out.println(inputLine);
		jsonString+=inputLine;
	    }
	    JSONObject jsonObj = new JSONObject(jsonString);
	    this.energyCost = jsonObj.getDouble("energy_cost");
	    this.energyOutput = jsonObj.getDouble("energy_output");
	    this.wakeFreeRatio = jsonObj.getDouble("wake_free_ratio");
	    this.nEvals = jsonObj.getInt("evals"); 
	    JSONArray jsonTurbFit = jsonObj.getJSONArray("turbine_fitnesses");
	    turbineFitnesses = new double[jsonTurbFit.length()];
	    for (int i=0; i<jsonTurbFit.length(); i++) {
		turbineFitnesses[i] = jsonTurbFit.getJSONArray(i).getDouble(0);
	    }
	    JSONArray jsonEnergyOutputs = jsonObj.getJSONArray("energy_outputs");
	    energyOutputs = new double[jsonEnergyOutputs.length()][jsonEnergyOutputs.getJSONArray(0).length()];
	    for (int i=0; i<jsonEnergyOutputs.length(); i++) {
		JSONArray jsonEnergy = jsonEnergyOutputs.getJSONArray(i);
		for (int j=0; j<jsonEnergy.length(); j++) {
		    energyOutputs[i][j] = jsonEnergy.getDouble(j);
		}
	    }
	    /*	    for (int i=0; i<energyOutputs.length; i++) {
		for (int j=0; j<energyOutputs[i].length; j++) {
		    System.out.print(energyOutputs[i][j]+" ");
		}
		System.out.println();
		}*/
	} catch (Exception e) {
	    e.printStackTrace();
	}
	return 0.0;
    }
    
    @Override
    public boolean checkConstraint(double layout[][]) {
	double minDist = 64.0*R*R; // squared minimum distance
	for (int i=0; i<layout.length; i++) {
	    // checking turbine position
	    if (layout[i][0]!=layout[i][0] || layout[i][1]!=layout[i][1] || layout[i][0]<0.0 || layout[i][1]<0.0 || layout[i][0]>width || layout[i][1]>height) {
		System.out.println("Turbine "+i+"("+layout[i][0]+", "+layout[i][1]+") is invalid.");
		return false;
	    }
	    // checking obstacle constraints
	    for (int j=0; j<obstacles.length; j++) {
		if (layout[i][0] > obstacles[j][0] &&
		    layout[i][0] < obstacles[j][2] &&
		    layout[i][1] > obstacles[j][1] &&
		    layout[i][1] < obstacles[j][3]) {
		    System.out.println("Turbine "+i+"("+layout[i][0]+", "+layout[i][1]+") is in the obstacle "+j+" ["+obstacles[j][0]+", "+obstacles[j][1]+", "+obstacles[j][2]+", "+obstacles[j][3]+"].");
		    return false;
		}
	    }
	    // checking the security constraints
	    for (int j=0; j<layout.length; j++) {
		if (i!=j) {
		    // calculate the sqared distance between both turb
		    double dist=(layout[i][0]-layout[j][0])*(layout[i][0]-layout[j][0])+
	                (layout[i][1]-layout[j][1])*(layout[i][1]-layout[j][1]);
		    if (dist<minDist) {
			System.out.println("Security distance contraint violated between turbines "+i+" ("+layout[i][0]+", "+layout[i][1]+") and "+j+" ("+layout[j][0]+", "+layout[j][1]+"): "+Math.sqrt(dist)+" > "+Math.sqrt(minDist));
			return false;
		    }
		}
	    }
	}
	return true;
    }

    @Override
    public double[][] getEnergyOutputs() {
	return this.energyOutputs;
    }
    
    @Override
    public double[] getTurbineFitnesses() {
	return this.turbineFitnesses;
    }
    
    @Override
    public double getEnergyOutput() {
	return energyOutput;
    }
    
    @Override
    public double getWakeFreeRatio() {
	return this.wakeFreeRatio;
    }
    
    @Override
    public double getEnergyCost() {
	return energyCost;
    }

    @Override
	public double getTurbineRadius() {
	return R;
    }

    @Override
	public double getFarmWidth() {
	return width;
    }
    
    @Override
	public double getFarmHeight() {
	return height;
    }

    @Override
	public double[][] getObstacles() {
	return obstacles;
    }
    
    /**
     * Returns the current run token
     * @return the run token
     */
    public String getRunToken() {
	return run_token;
    }
    
    /**
     * Test function with random layouts
     */
    public static void main(String args[]) {
	java.util.Random rng = new java.util.Random(System.currentTimeMillis());
	CompetitionEvaluator eval = new CompetitionEvaluator();
	eval.initialize(0, "USER TOKEN GOES HERE", "RUN TOKEN GOES HERE");
	double layout[][];
	//	do {
	    do {
		double minDist = 8.0*eval.R;
		int layoutWidth = rng.nextInt((int)(eval.width/minDist))+3;
		int layoutHeight = rng.nextInt((int)(eval.height/minDist))+3;
		double tileWidth = eval.width/layoutWidth;
		double tileHeight = eval.height/layoutHeight;
		layout = new double[layoutWidth*layoutHeight][2];
		for (int i=0; i<layoutWidth; i++) {
		    for (int j=0; j<layoutHeight; j++) {
			layout[i+j*layoutWidth][0] = ((double)i*tileWidth)+rng.nextDouble()*0.2*tileWidth;
			layout[i+j*layoutWidth][1] = ((double)j*tileHeight)+rng.nextDouble()*0.2*tileHeight;
		    }
		}
	    } while (!eval.checkConstraint(layout));
	    eval.evaluate(layout);
	    System.out.println("Energy cost: "+eval.getEnergyCost());
	    //	} while  (eval.getNumberOfEvaluation() < 8000 && eval.getEnergyCost()>0.001724);
    } 

}
