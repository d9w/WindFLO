import java.io.BufferedReader;
import java.io.FileReader;
import java.util.StringTokenizer;

public class main {

  public static void main(String argv[]) {
      try {
          WindScenario ws = new WindScenario("../Scenarios/obs_00.xml");
          KusiakLayoutEvaluator wfle = new KusiakLayoutEvaluator();
          wfle.initialize(ws);
          GA algorithm = new GA(wfle);
          algorithm.run(); // optional, name of method 'run' provided on submission
          // algorithm can also just use constructor
      } catch (Exception e) {
          e.printStackTrace();
      }
  }
}
