//
//  GA.cpp
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

#include "GA.h"
#include "WindScenario.h"
#include "KusiakLayoutEvaluator.h"

GA::GA(KusiakLayoutEvaluator evaluator) {
  wfle = evaluator;
  num_pop = 20;
  nt = 0;
  tour_size = 4;
  mut_rate = 0.05;
  cross_rate = 0.40;
  grid = new Matrix<double>(0,2);
  srand(0);
}

void GA::evaluate() {
  double maxfit = 0.0;
  for (int p=0; p<num_pop; p++) {
    int nturbines=0;
    for (int i=0; i<nt; i++) {
      if (pops.get(i,p)) {
        nturbines++;
      }
    }

    Matrix<double> layout = new Matrix<double>(nturbines,2);
    int l_i = 0;
    for (int i=0; i<nt; i++) {
      if (pops.get(i,p)) {
        layout.set(l_i, 0, grid.get(i,0));
        layout.set(l_i, 1, grid.get(i,1));
        l_i++;
      }
    }

    wfle.evaluate(&layout);
    Matrix<double> fitnesses = wfle.getTurbineFitnesses();

    int n_valid = 0;
    for (int i=0; i<nturbines; i++) {
      if (fitnesses.get(i,0) > 0.80) {
        n_valid++;
      }
    }

    fits[p] = n_valid;
    if (fits[p] > maxfit) {
        maxfit = fits[p];
    }
  }

  printf("Fitness: %f", maxfit);
}

void GA::run() {
  // set up grid
  // centers must be > 8*R apart
  double interval = 8.001 * wfle.scenario.R;

  int nx = (int) wfle.scenario.width / interval;
  int ny = (int) wfle.scenario.height / interval;

  // get number of valid grid spots
  nt=0;
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

  Matrix<double> grid = new Matrix<double>(nt, 2);
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
        grid.set(t, 0, x*interval);
        grid.set(t, 1, y*interval);
        t++;
      }
    }
  }

  // initialize populations
  pops = new Matrix<bool>(nt, num_pop);
  double fits [num_pop];

  for (int p=0; p<num_pop; p++) {
    for (int i=0; i<nt; i++) {
      bool turb = true;
      if ((double)rand()/RAND_MAX > 0.5) {
        turb = false;
      }
      pops.set(i, p, turb);
    }
  }

  std::cout << "here" << std::endl;
  // evaluate initial populations (uses num_pop evals)
  evaluate();
  std::cout << "done evaluating" << std::endl;

  // GA
  for (int i=0; i<(1000/num_pop); i++) {

    // rank populations (tournament)
    int num_winners = num_pop/tour_size;
    printf("num winners: %d", num_winners);
    int winners [num_winners];
    int competitors [num_pop];
    for (int c=0; c<num_pop; c++) {
      competitors[c] = c;
    }

    for (int c=0; c<num_pop; c++) {
      int index = rand() % (c+1);
      int temp = competitors[index];
      competitors[index] = competitors[c];
      competitors[c] = temp;
    }

    for (int t=0; t<num_winners; t++) {
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
    // check row,col
    Matrix<bool> children = new Matrix<bool>(nt, num_pop);

    for (int c=0; c<(num_pop-num_winners); c++) {
      int p1 = winners[rand() % num_winners];
      int p2 = winners[(p1+1+(rand() % (num_winners-2)))%num_winners];

      for (int j=0; j<nt; j++) {
        if ((double)rand()/RAND_MAX < cross_rate) {
          children.set(j,c,pops.get(j,p1));
        } else {
          children.set(j,c,pops.get(j,p2));
        }
      }
    }

    // mutate
    for (int c=0; c<(num_pop-num_winners); c++) {
      for (int j=0; j<nt; j++) {
        if ((double)rand()/RAND_MAX < mut_rate) {
          children.set(j,c,~children.get(j,c));
        }
      }
    }

    // elitism
    for (int c=0; c<num_winners; c++) {
      for (int j=0; j<nt; j++) {
        children.set(j, num_pop-num_winners+c, pops.get(j,winners[c]));
      }
    }

    pops = children;

    // evaluate
    evaluate();
  }
}
