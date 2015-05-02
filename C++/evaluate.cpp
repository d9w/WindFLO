//
//  main.cpp
//

#include <iostream>
#include <stdio.h>
#include <string>
#include "tinyxml2.h"

#include "WindScenario.h"
#include "KusiakLayoutEvaluator.h"
#include "Matrix.hpp"

int main(int argc, const char * argv[]) {

  WindScenario wsc(argv[1]);
  KusiakLayoutEvaluator wfle;
  wfle.initialize(wsc);
  tinyxml2::XMLDocument doc;
  std::string layout_file(argv[2]);
  doc.LoadFile(layout_file.c_str());
  tinyxml2::XMLNode* parent = doc.FirstChild();
  parent = parent->NextSibling();
  tinyxml2::XMLNode* child;
  int nturbines = 0;
  for( child = parent->FirstChild(); child; child = child->NextSibling() ) {
    nturbines++;
  }
  Matrix<double>* layout = new Matrix<double>(nturbines,2);
  int i = 0;
  for( child = parent->FirstChild(); child; child = child->NextSibling() ) {
    tinyxml2::XMLElement* elem = child->ToElement();
    layout->set(i, 0, elem->DoubleAttribute("x"));
    layout->set(i, 1, elem->DoubleAttribute("y"));
    i++;
  }

  wfle.evaluate(layout);
  double coe = wfle.getEnergyCost();
  printf("%f\n", coe);
}
