import java.util.ArrayList;
import java.util.Random;

public class GA {

    WindFarmLayoutEvaluator wfle;
    boolean[][] pops;
    double[] fits;
    Random rand;
    int num_pop;
    int tour_size;
    double mut_rate;
    double cross_rate;
    ArrayList<double[]> grid;

    public GA(WindFarmLayoutEvaluator evaluator) {
        wfle = evaluator;
        rand = new Random();
        num_pop = 20;
        tour_size = 4;
        mut_rate = 0.05;
        cross_rate = 0.40;
        grid = new ArrayList<double[]>();
    }

    private void evaluate() {
        double maxfit = -Double.MAX_VALUE;
        for (int p=0; p<num_pop; p++) {
            int nturbines=0;
            for (int i=0; i<grid.size(); i++) {
                if (pops[p][i]) {
                    nturbines++;
                }
            }

            double[][] layout = new double[nturbines][2];
            int l_i = 0;
            for (int i=0; i<grid.size(); i++) {
                if (pops[p][i]) {
                    layout[l_i][0] = grid.get(i)[0];
                    layout[l_i][1] = grid.get(i)[1];
                    l_i++;
                }
            }

	    double coe = wfle.evaluate(layout);
//            double[] fitnesses = wfle.getTurbineFitnesses();
//             int n_valid = 0;
//             for (int i=0; i<fitnesses.length; i++) {
//                 if (fitnesses[i] > 0.80) {
//                     n_valid++;
//                 }
//             }

            fits[p] = -coe; //n_valid;
            if (fits[p] > maxfit) {
                maxfit = fits[p];
            }
        }
        System.out.println(maxfit);
    }

    public void run() {
      // set up grid
      // centers must be > 8*R apart
      double interval = 8.001 * wfle.scenario.R;

      for (double x=0.0; x<wfle.scenario.width; x+=interval) {
          for (double y=0.0; y<wfle.scenario.height; y+=interval) {
              boolean valid = true;
              for (int o=0; o<wfle.scenario.obstacles.length; o++) {
                  double[] obs = wfle.scenario.obstacles[o];
                  if (x>obs[0] && y>obs[1] && x<obs[2] && y<obs[3]) {
                      valid = false;
                  }
              }

              if (valid) {
                  double[] point = {x, y};
                  grid.add(point);
              }
          }
      }

      // initialize populations
      pops = new boolean[num_pop][grid.size()];
      fits = new double[num_pop];

      for (int p=0; p<num_pop; p++) {
          for (int i=0; i<grid.size(); i++) {
              pops[p][i] = rand.nextBoolean();
          }
      }

      // evaluate initial populations (uses num_pop evals)
      evaluate();

      // GA
      for (int i=0; i<(1000/num_pop); i++) {

          // rank populations (tournament)
          int[] winners = new int[num_pop/tour_size];
          int[] competitors = new int[num_pop];
          for (int c=0; c<competitors.length; c++) {
              competitors[c] = c;
          }

          for (int c=0; c<competitors.length; c++) {
              int index = rand.nextInt(c + 1);
              int temp = competitors[index];
              competitors[index] = competitors[c];
              competitors[c] = temp;
          }

          for (int t=0; t<winners.length; t++) {
              int winner = -1;
              double winner_fit = -1.0;
              for (int c=0; c<tour_size; c++) {
                  int competitor = competitors[tour_size*t + c];
                  if (fits[competitor] > winner_fit) {
                      winner = competitor;
                      winner_fit = fits[winner];
                  }
              }
              winners[t] = winner;
          }

          // crossover
          boolean[][] children = new boolean[num_pop][grid.size()];

          for (int c=0; c<(num_pop-winners.length); c++) {
              int s1 = rand.nextInt(winners.length);
              int s2 = rand.nextInt(winners.length-1);
              if (s2 >= s1) {
                  s2++;
              }
              int p1 = winners[s1];
              int p2 = winners[s2];
              boolean[] parent1 = pops[p1];
              boolean[] parent2 = pops[p2];

              boolean[] child = new boolean[grid.size()];
              for (int j=0; j<child.length; j++) {
                  if (rand.nextDouble() < cross_rate) {
                      child[j] = parent2[j];
                  } else {
                      child[j] = parent1[j];
                  }
              }

              children[c] = child;
          }

          // mutate
          for (int c=0; c<(num_pop-winners.length); c++) {
              for (int j=0; j<children[c].length; j++) {
                  if (rand.nextDouble() < mut_rate) {
                      children[c][j] = !children[c][j];
                  }
              }
          }

          // elitism
          for (int c=0; c<winners.length; c++) {
              children[num_pop-winners.length+c] = pops[winners[c]];
          }

          pops = children;

          // evaluate
          evaluate();
      }
    }
}
