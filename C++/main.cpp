//
//  main.cpp
//

#include <iostream>
#include <stdio.h>

#include "WindScenario.h"
#include "KusiakLayoutEvaluator.h"

int main(int argc, const char * argv[]) {

  WindScenario wsc("../Scenarios/obs_00.xml");
  KusiakLayoutEvaluator wfle;
  wfle.initialize(wsc);
  double interval = 8.001 * wfle.scenario.R;

  int nx = (int) wfle.scenario.width / interval;
  int ny = (int) wfle.scenario.height / interval;

  // get number of valid grid spots
  int nt=0;
  for (int x=0; x<nx; x++) {
    for (int y=0; y<ny; y++) {
      double xpos = x*interval;
      double ypos = y*interval;
      bool valid = true;

      for (unsigned int o=0; o<wfle.scenario.obstacles.rows; o++) {
        Matrix<double> obs = wfle.scenario.obstacles.getRow(o);
        if (xpos>obs.get(0,0) && ypos>obs.get(0,1) && xpos<obs.get(0,2) && ypos<obs.get(0,3)) {
          valid = false;
        }
      }
      if (valid) {
        nt++;
      }
    }
  }

  Matrix<double> layout = new Matrix<double>(nt, 2);
  int t = 0;
  for (int x=0; x<nx; x++) {
    for (int y=0; y<ny; y++) {
      double xpos = x*interval;
      double ypos = y*interval;
      bool valid = true;

      for (unsigned int o=0; o<wfle.scenario.obstacles.rows; o++) {
        Matrix<double> obs = wfle.scenario.obstacles.getRow(o);
        if (xpos>obs.get(0,0) && ypos>obs.get(0,1) && xpos<obs.get(0,2) && ypos<obs.get(0,3)) {
          valid = false;
        }
      }
      if (valid) {
        layout.set(t, 0, x*interval);
        layout.set(t, 1, y*interval);
        printf("turbine %d/%d\t(%f, %f)\n", t, nt, layout.get(t, 0), layout.get(t,1));
        t++;
      }
    }
  }

  printf("wake free ratio: %f\n", wfle.evaluate(&layout));
  /*
  Matrix<double>* eouts = wfle.getEnergyOutputs();
  for (unsigned int i=0; i<eouts->rows; i++) {
    for (unsigned int j=0; j<eouts->cols; j++) {
      printf("eout(%d,%d)\t%f\t\t", i, j, eouts->get(i,j));
    }
    std::cout << std::endl;
  }
  */

  printf("After %d evaluations\n", wfle.getNumberOfEvaluation());
}
