//
//  main.cpp
//

#include <iostream>

#include "WindScenario.h"
#include "KusiakLayoutEvaluator.h"

int main(int argc, const char * argv[]) {

  WindScenario wsc("../Scenarios/00.xml");
  KusiakLayoutEvaluator simu;
  simu.initialize(wsc);
  std::cout << "the future home of an algorithm" << std::endl;
}
