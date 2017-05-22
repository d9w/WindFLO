//
//  main.cpp
//

#include <iostream>
#include <stdio.h>

#include "CompetitionScenario.h"
#include "GA.h"
#include "CompetitionEvaluator.h"

int main(int argc, const char * argv[]) {

  CompetitionScenario wsc(0);
  CompetitionEvaluator wfle;
  // C++ GA user api_token. Change it to your private token
  wfle.initialize(wsc, "0FDCFH8MVQK2M1XGJCPJB6EMB50DF8");
  GA ga(wfle);
  ga.run();
  for (int i=1; i<5; ++i) {
    CompetitionScenario wsc(i);
    wfle.scenario = wsc;
    GA ga(wfle);
    ga.run();
  }
}

