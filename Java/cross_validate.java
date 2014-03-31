import java.io.BufferedReader;
import java.io.FileReader;
import java.util.StringTokenizer;

public class cross_validate {
    public static void main(String argv[]) {
	try {
	    for (int lay=0; lay<100; lay++) {
		for (int sc=0; sc<9; sc++) {
		    String currentLine;
		    BufferedReader br = new BufferedReader(new FileReader("../TestLayouts/layout"+lay+".txt"));
		    int i=0;
		    currentLine=br.readLine();
		    int n=Integer.parseInt(currentLine);
		    double layout[][]=new double[n][2];
		    while ((currentLine=br.readLine()) != null && i<n) {
			StringTokenizer tokenizer=new StringTokenizer(currentLine);
			int j=0;
			while (tokenizer.hasMoreElements() && j<2) {
			    layout[i][j]=Double.parseDouble(tokenizer.nextToken());
			    j++;
			}
			i++;
		    }
		    WindScenario ws = new WindScenario("../Scenarios/0"+sc+".xml");
		    KusiakLayoutEvaluator le = new KusiakLayoutEvaluator();
		    le.initialize(ws);
		    le.evaluate(layout);
		    //	    System.out.println("Energy output: "+le.getEnergyOutput());
		    System.out.println(le.getWakeFreeRatio());
		}
	    }
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }
}
