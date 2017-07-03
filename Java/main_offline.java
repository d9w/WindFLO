public class main_offline {
  public static int nSc;

  public static void main(String argv[]) throws Exception {
	nSc = Integer.parseInt(argv[0]);
	KusiakLayoutEvaluator eval = new KusiakLayoutEvaluator();
	WindScenario sc = new WindScenario("../Wind Competition/2015/Scenarios/"+argv[0]+".xml");	  
	eval.initialize(sc);
	GA algorithm = new GA(eval);
	algorithm.run(); // optional, name of method 'run' provided on submission
  }
}
