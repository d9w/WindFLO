import java.io.BufferedReader;
import java.io.FileReader;
import java.util.StringTokenizer;

public class main {
    public static void main(String argv[]) {
	try {
		WindScenario ws = new WindScenario("../Scenarios/obs_00.xml");
		KusiakLayoutEvaluator le = new KusiakLayoutEvaluator();
		le.initialize(ws);
		System.out.println("The future home of an algorithm");
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }
}
