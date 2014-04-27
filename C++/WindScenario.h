//
//  WindScenario.h
//  TurbinesSimulator
//
//  Created by Sylvain Cusat-Blanc on 26/11/12.
//  Copyright (c) 2012 Sylvain Cusat-Blanc. All rights reserved.
//

#ifndef __TurbinesSimulator__WindScenario__
#define __TurbinesSimulator__WindScenario__

#include <iostream>
#include "Matrix.hpp"
#include <cmath>
#include <vector>

using namespace std;

class WindScenario {
private:
    void generateScenario(string);

public:
    static void NbActiveScenario(int delta=0, bool disp=false) {
        static int nbActive=0;
        nbActive+=delta;
        if (disp) std::cerr << "# Active Scenarii : " << nbActive << std::endl;
    }

    WindScenario() {};
    WindScenario(int sc);
    WindScenario(string fileName);
    WindScenario(const WindScenario& wsc);
    ~WindScenario() {NbActiveScenario(-1);};

    // wind resources
    Matrix<double> ks;
    Matrix<double> c;
    Matrix<double> omegas;
    Matrix<double> thetas;

    // Farm parameters
    Matrix<double> obstacles; //nobstacles * 4, rows of [xmin, ymin, xmax, ymax]
    double CT;
    double PRated;
    double R;
    double eta;
    double k;
    double lambda;
    double vCin;
    double vCout;
    double vRated;
    double wakeFreeEnergy;
    double width;
    double height;
    int nturbines;

    // optimization parameters
    static double fac;
    void initOptimizationParameters();
    Matrix<double> coSinMidThetas;
    inline double getCosMidThetas(int thetIndex) {
      return coSinMidThetas.get(thetIndex, 0);};
    inline double getSinMidThetas(int thetIndex) {
      return coSinMidThetas.get(thetIndex, 1);};
    double rkRatio;
    Matrix<double> vints;
    vector<double> wblcdfValues;
    double wblcdfAccuracy;
    double cMax, cMin;
    inline double getWblcdfVints(double c, int vintIndex, int ksIndex) {
      return wblcdfValues[(int)((c-cMin)/wblcdfAccuracy)*(vints.cols+1)*ks.cols+vintIndex*ks.cols+ksIndex];};
    inline double getWblcdfVrated(double c, int ksIndex) {
      return wblcdfValues[(int)((c-cMin)/wblcdfAccuracy)*(vints.cols+1)*ks.cols+vints.cols*ks.cols+ksIndex];};
    double getSinTheta(double theta);

    inline double wblcdf(double x, double sc=1.0, double sh=1.0) {
      return 1.0-exp(-fastPow(x/sc,sh));};
    inline static double fastPow(double a, double b) {
      if (abs(b-2)<0.0001) {
        return a*a;
      } if (abs(b-1)<0.0001) {
        return a;
      } if (abs(b)<0.0001) {
        return 1;
      } else {
        return pow(a,b);
      }
    };

};

#endif /* defined(__TurbinesSimulator__WindScenario__) */
