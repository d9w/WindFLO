//
//  main.cpp
//

#include <iostream>
#include <stdio.h>

#include "WindScenario.h"
#include "GA.h"
#include "KusiakLayoutEvaluator.h"

int main(int argc, const char * argv[]) {

  WindScenario wsc("../Scenarios/obs_00.xml");
  KusiakLayoutEvaluator wfle;
  wfle.initialize(wsc);
  GA ga(wfle);
  ga.run();
  printf("After %d evaluations\n", wfle.getNumberOfEvaluation());
}
