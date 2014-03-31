//
//  main.cpp
//  TurbinesSimulatorTest
//
//  Created by Sylvain Cusat-Blanc on 04/12/12.
//  Copyright (c) 2012 Sylvain Cusat-Blanc. All rights reserved.
//

#include <iostream>
#include <time.h>
#include <unistd.h>
#include <ios>
#include <fstream>

#include "WindScenario.h"
#include "KusiakLayoutEvaluator.h"
#include "Matrix.hpp"


int main(int argc, const char * argv[]) {
  for (int i=0; i<100; i++) {
    for (int sc=0; sc<10; sc++) {
      stringstream layoutFileName;
      stringstream scFileName;
      char scnumber[64];
      sprintf(scnumber, "%02d", sc);
      layoutFileName << "../TestLayouts/layout" << i << ".txt";
      scFileName << "../Scenarios/" << scnumber << ".xml";

      ifstream layoutFile(layoutFileName.str().c_str());
      int n;
      layoutFile >> n;
      Matrix<double> tpos(n, 2);
      int current=0;
      if (layoutFile.is_open()) {
        double x, y;
        while (!layoutFile.eof()) {
          layoutFile >> x >> y;
          tpos.set(current, 0, x);
          tpos.set(current, 1, y);
          current++;
        }
      }
      layoutFile.close();
      WindScenario wsc(scFileName.str());
      KusiakLayoutEvaluator simu;
      simu.initialize(wsc);
      double wfr=simu.evaluate(&tpos);
      cout.precision(15);
      cout << wfr << endl;
    }
  }
}
