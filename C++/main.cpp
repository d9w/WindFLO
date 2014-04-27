//
//  main.cpp
//

#include <iostream>
#include <stdio.h>

#include "WindScenario.h"
#include "KusiakLayoutEvaluator.h"

int main(int argc, const char * argv[]) {

  WindScenario wsc("../Scenarios/00.xml");
  KusiakLayoutEvaluator wfle;
  wfle.initialize(wsc);
  double interval = 8.001 * wfle.scenario.R;

  int nx = (int) wfle.scenario.width / interval;
  int ny = (int) wfle.scenario.height / interval;
  Matrix<double> layout = new Matrix<double>(nx*ny, 2);

  int t=0;
  for (int x=0; x<nx; x++) {
    for (int y=0; y<ny; y++) {
      // TODO: add obstacles to wind scenario
      /*
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
      */

      layout.set(t, 0, x*interval);
      layout.set(t, 1, y*interval);
      printf("turbine %d/%d\t(%f, %f)\n", t, nx*ny, layout.get(t, 0), layout.get(t,1));
      t++;
    }
  }

  printf("wake free ratio: %f\n", wfle.evaluate(&layout));
  Matrix<double>* eouts = wfle.getEnergyOutputs();
  for (unsigned int i=0; i<eouts->rows; i++) {
    for (unsigned int j=0; j<eouts->cols; j++) {
      printf("eout(%d,%d)\t%f\t\t", i, j, eouts->get(i,j));
    }
    std::cout << std::endl;
  }

  printf("After %d evaluations\n", wfle.getNumberOfEvaluation());

  std::cout << "the future home of an algorithm" << std::endl;
}
