public class main {

  public static void main(String argv[]) {
      try {
	  String userToken = new String("USER TOKEN GOES HERE");
	  String runToken = null;
	  System.out.println("Your user token is: "+userToken);
	  for (int sc = 0; sc < 5; sc++) {
	      // Create the competition evaluator
	      CompetitionEvaluator eval = new CompetitionEvaluator();
	      // initialize the evalutor with the scenario id (from 0 to 4), with you user token (see you account online) and the run token. For the first run, ommit the run token, the server will send you (and display) a new one. This run token can be recovered with the getRunToken() method from CompetitionEvaluator
	      if (sc == 0) {
		  eval.initialize(sc, userToken);
		  runToken = eval.getRunToken();
	      } else {
		  eval.initialize(sc, userToken, runToken);
	      }
	      GA algorithm = new GA(eval);
	      System.out.println("The current run token is: "+runToken);
	      algorithm.run(); // optional, name of method 'run' provided on submission
	      // algorithm can also just use constructor
	  }
      } catch (Exception e) {
          e.printStackTrace();
      }
  }
}
