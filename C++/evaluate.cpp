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
  if (argc==4) {
    double wfr = wfle.getWakeFreeRatio();
    if (wfr <= 0) {
      printf("{\n  \"energy_cost\": %.9f,\n", coe);
      printf("  \"energy_output\": %.9f,\n", wfle.getEnergyOutput());
      printf("  \"wake_free_ratio\": %.9f,\n", wfle.getWakeFreeRatio());
      printf("  \"turbine_fitnesses\": [],\n");
      printf("  \"energy_outputs\": []\n");
      printf("}");
    } else {
      printf("{\n  \"energy_cost\": %.9f,\n", coe);
      printf("  \"energy_output\": %.9f,\n", wfle.getEnergyOutput());
      printf("  \"wake_free_ratio\": %.9f,\n", wfle.getWakeFreeRatio());
      Matrix<double>* fitnesses = wfle.getTurbineFitnesses();
      printf("  \"turbine_fitnesses\": [");
      for (unsigned int i=0; i<fitnesses->rows; i++) {
        printf("[");
        for (unsigned int j=0; j<fitnesses->cols; j++) {
          printf("%.9f", fitnesses->get(i, j));
          if (j < fitnesses->cols-1) {
            printf(",");
          }
        }
        printf("]");
        if (i < fitnesses->rows-1) {
          printf(",");
        }
      }
      printf("],\n");
      Matrix<double>* energy = wfle.getEnergyOutputs();
      printf("  \"energy_outputs\": [");
      for (unsigned int i=0; i<energy->rows; i++) {
        printf("[");
        for (unsigned int j=0; j<energy->cols; j++) {
          printf("%.9f", energy->get(i, j));
          if (j < energy->cols-1) {
            printf(",");
          }
        }
        printf("]");
        if (i < energy->rows-1) {
          printf(",");
        }
      }
      printf("]\n");
      printf("}");
    }
  } else {
    printf("%.9f\n", coe);
  }
}
