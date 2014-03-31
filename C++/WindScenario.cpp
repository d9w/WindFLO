//
//  WindScenario.cpp
//  TurbinesSimulator
//
//  Created by Sylvain Cusat-Blanc on 26/11/12.
//  Copyright (c) 2012 Sylvain Cusat-Blanc. All rights reserved.
//

#include "WindScenario.h"
#include "tinyxml2.h"
#include <cstring>
#include <sstream>
#include <cstdio>
#include <string>
#include <cmath>

double WindScenario::fac=M_PI/180;

WindScenario::WindScenario(int sc) {
    NbActiveScenario(1);
    stringstream fileName;
    char scnumber[64];
    sprintf(scnumber, "%02d", sc);
    fileName << "TurbinesSimulator/" << scnumber << ".xml";
    generateScenario(fileName.str());
    initOptimizationParameters();
}

WindScenario::WindScenario(string fileName) {
    NbActiveScenario(1);
    generateScenario(fileName);
    initOptimizationParameters();
}

WindScenario::WindScenario(const WindScenario& wsc):
c(wsc.c),
ks(wsc.ks),
omegas(wsc.omegas),
thetas(wsc.thetas),
CT(wsc.CT),
PRated(wsc.PRated),
R(wsc.R),
eta(wsc.eta),
k(wsc.k),
lambda(wsc.lambda),
vCin(wsc.vCin),
vCout(wsc.vCout),
vRated(wsc.vRated),
wakeFreeEnergy(wsc.wakeFreeEnergy),
width(wsc.width),
height(wsc.height),
nturbines(wsc.nturbines),
coSinMidThetas(wsc.coSinMidThetas),
rkRatio(wsc.rkRatio),
vints(wsc.vints),
wblcdfAccuracy(wsc.wblcdfAccuracy),
cMax(wsc.cMax),
cMin(wsc.cMin),
wblcdfValues(wsc.wblcdfValues) {
  NbActiveScenario(1);
}

void WindScenario::generateScenario(string fileName) {

    c=Matrix<double>(1,24);
    ks=Matrix<double>(1,24);
    omegas=Matrix<double>(1,24);
    thetas=Matrix<double>(24,2);

    for (unsigned i=0; i<24; i++) {
        thetas.set(i,0,i*15.0);
        thetas.set(i,1,(i+1)*15.0);
    }


    tinyxml2::XMLDocument doc;
    doc.LoadFile(fileName.c_str());
    tinyxml2::XMLNode* field = doc.FirstChild()->NextSibling();
    tinyxml2::XMLNode* angles = field->FirstChild()->NextSibling();
    tinyxml2::XMLNode* angle = angles->FirstChild();
    tinyxml2::XMLElement* elem = angle->ToElement();
    c.set(0,0,elem->DoubleAttribute("c"));
    ks.set(0,0,elem->DoubleAttribute("k"));
    omegas.set(0,0,elem->DoubleAttribute("omega"));
    for (int i=1; i<24; i++) {
      angle = angle->NextSibling();
      elem = angle->ToElement();
      c.set(0,i,elem->DoubleAttribute("c"));
      ks.set(0,i,elem->DoubleAttribute("k"));
      omegas.set(0,i,elem->DoubleAttribute("omega"));
    }
    tinyxml2::XMLNode* obstacles = angles->NextSibling(); // not used right now
    tinyxml2::XMLNode* parameters = obstacles->NextSibling();
    width = atof(parameters->FirstChildElement("Width")->GetText());
    height = atof(parameters->FirstChildElement("Height")->GetText());
    nturbines = atoi(parameters->FirstChildElement("NTurbines")->GetText());
    wakeFreeEnergy = atof(parameters->FirstChildElement("WakeFreeEnergy")->GetText());

    // these are currently all the same across scenarios, so aren't in the xml files
    CT=0.8;
    PRated=1500.0;
    R=38.5;
    eta=-500.0;
    k=0.0750;
    lambda=140.86;
    vCin=3.5;
    vCout=20;
    vRated=14;
}

void WindScenario::initOptimizationParameters() {
    coSinMidThetas=Matrix<double>(thetas.rows, 2);
    for (int thets=0; thets<thetas.rows; thets++) {
        double theta=(thetas.get(thets, 0)+thetas.get(thets, 1))/2.0*fac;
        coSinMidThetas.set(thets, 0, cos(theta));
        coSinMidThetas.set(thets, 1, sin(theta));
    }
    rkRatio=R/k;
    vints=Matrix<double>(1,2*vRated-7+1);
    for (double i=0; i<vints.cols; i++) {
      vints.set(0,i,3.5+i*0.5);
    }
}

