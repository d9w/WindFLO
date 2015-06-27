#include "KusiakLayoutEvaluator.h"

KusiakLayoutEvaluator::KusiakLayoutEvaluator() {
  tspe=NULL;
  tpositions=NULL;
}

void KusiakLayoutEvaluator::initialize(WindScenario& sc) {
  scenario=WindScenario(sc);
  wakeFreeEnergy=sc.wakeFreeEnergy;
  energyCapture=0;
  if (tspe) delete tspe;
  if (tpositions) delete tpositions;
  tspe=NULL;
  tpositions=NULL;
  nEvals=0;
  energyCost = std::numeric_limits<double>::max();
}

KusiakLayoutEvaluator::~KusiakLayoutEvaluator() {
  if (tspe) delete tspe;
  if (tpositions) delete tpositions;
}

double KusiakLayoutEvaluator::evaluate(Matrix<double>* layout) {
  static double ct  = 750000;
  static double cs  = 8000000;
  static double m   = 30;
  static double r   = 0.03;
  static double y   = 20;
  static double com = 20000;
  
  double wfr = evaluate_2014(layout);
  if (wfr <= 0) return std::numeric_limits<double>::max();
  int n = layout->rows;
  
  energyCost = (((ct*n+cs*std::floor(n/m))*(0.666667+0.333333*std::exp(-0.00174*n*n))+com*n)/
	  ((1.0-std::pow(1.0+r, -y))/r)/(8760.0*scenario.wakeFreeEnergy*wfr*n))+0.1/n;
  return energyCost;
}

double KusiakLayoutEvaluator::evaluate_2014(Matrix<double>* layout) {
  nEvals++;
  if (tpositions) delete tpositions;
  tpositions=new Matrix<double>(layout);
  if (tspe) delete tspe;
  energyCapture=0;
  wakeFreeRatio=0;
  if (checkConstraint()) {
    tspe=new Matrix<double>(scenario.thetas.rows, tpositions->rows);
    // Wind resource per turbine => stored temporaly in tspe
    for (int turb=0; turb<tpositions->rows; turb++) {
      // for each turbine
      for (int thets=0; thets<scenario.thetas.rows; thets++) {
	// for each direction
	// double theta=(scenario.thetas.get(thets, 0)+scenario.thetas.get(thets, 1))/2.0;
	// calculate wake
	// double totalVdef=calculateWakeTurbine(turb, theta);
	double totalVdef=calculateWakeTurbine(turb, thets);
	double cTurb=scenario.c.get(0,thets)*(1.0-totalVdef);
	// annual power output per turbine and per direction
	double tint=scenario.thetas.get(thets, 1)-scenario.thetas.get(thets, 0);
	double w=scenario.omegas.get(0, thets);
	double ki=scenario.ks.get(0, thets);
	double totalPow=0;
	for (int ghh=1; ghh<scenario.vints.cols; ghh++) {
	  double v=(scenario.vints.get(0, ghh)+scenario.vints.get(0, ghh-1))/2.0;
	  double P=powOutput(v);
	  double prV=wblcdf(scenario.vints.get(0, ghh), cTurb, ki)-wblcdf(scenario.vints.get(0, ghh-1),cTurb,ki);
	  totalPow+=prV*P;
	}
	totalPow+=scenario.PRated*(1.0-wblcdf(scenario.vRated, cTurb, ki));
	totalPow*=tint*w;
	tspe->set(thets, turb, totalPow);
	energyCapture+=totalPow;
      }
    }
    wakeFreeRatio=energyCapture/(wakeFreeEnergy*tpositions->rows);
    return wakeFreeRatio;
  } else {
    return 0;
  }
}

Matrix<double>* KusiakLayoutEvaluator::getEnergyOutputs() {
  if (!tspe) return NULL;
  Matrix<double>* res = new Matrix<double>(tspe);
  return res;
}

Matrix<double>* KusiakLayoutEvaluator::getTurbineFitnesses() {
  if (!tspe) return NULL;
  Matrix<double>* res = new Matrix<double>(tpositions->rows, 1);
  for (int i=0; i<res->rows; i++) {
    double val=0.0;
    for (int j=0; j<tspe->rows; j++) {
      val+=tspe->get(j,i);
    }
    res->set(i,0,val/scenario.wakeFreeEnergy);
  }
  return res;
}


double KusiakLayoutEvaluator::powOutput(double v) {
  if (v<scenario.vCin) {
    return 0;
  } else if (v>=scenario.vCin && v<=scenario.vRated) {
    return scenario.lambda*v+scenario.eta;
  } else if (scenario.vCout>v && v>scenario.vRated) {
    return scenario.PRated;
  } else {
    return 0;
  }
}

double KusiakLayoutEvaluator::calculateWakeTurbine(int turb, double theta) {
  double x=tpositions->get(turb, 0);
  double y=tpositions->get(turb, 1);
  static const double alpha=atan(scenario.k);
  double velDef=0;
  for (int oturb=0; oturb<tpositions->rows; oturb++) {
    if (oturb!=turb) {
      double xo=tpositions->get(oturb, 0);
      double yo=tpositions->get(oturb, 1);
      double beta=calculateBeta(x, y, xo, yo, theta);
      if (beta<alpha) {
	double dij=calculateProjectedDistance(x, y, xo, yo, theta);
	double curDef=calculateVelocityDeficit(dij);
	velDef+=curDef*curDef;
      }
    }
  }
  return sqrt(velDef);
}

double KusiakLayoutEvaluator::calculateWakeTurbine(int turb, int thetIndex) {
  double x=tpositions->get(turb, 0);
  double y=tpositions->get(turb, 1);
  static const double alpha=atan(scenario.k);
  double velDef=0;
  for (int oturb=0; oturb<tpositions->rows; oturb++) {
    if (oturb!=turb) {
      double xo=tpositions->get(oturb, 0);
      double yo=tpositions->get(oturb, 1);
      double beta=calculateBeta(x, y, xo, yo, thetIndex);
      if (beta<alpha) {
	double dij=calculateProjectedDistance(x, y, xo, yo, thetIndex);
	double curDef=calculateVelocityDeficit(dij);
	velDef+=curDef*curDef;
      }
    }
  }
  return sqrt(velDef);
}

double KusiakLayoutEvaluator::calculateVelocityDeficit(double dij) {
  static const double a=1.0-sqrt(1.0-scenario.CT);
  static const double rkRatio=scenario.k/scenario.R;
  return a/((1.0+rkRatio*dij)*(1.0+rkRatio*dij));
}

double KusiakLayoutEvaluator::calculateProjectedDistance(double xi, double yi, double xj, double yj, double theta) {
  static const double fac=M_PI/180.0;
  return abs((xi-xj)*cos(fac*theta)+(yi-yj)*sin(fac*theta));
}

double KusiakLayoutEvaluator::calculateProjectedDistance(double xi, double yi, double xj, double yj, int thetIndex) {
  return abs((xi-xj)*scenario.getCosMidThetas(thetIndex)+(yi-yj)*scenario.getSinMidThetas(thetIndex));
}

// calculate the angle between to turbines using xi, xj, yi, yj, R, k and theta
double KusiakLayoutEvaluator::calculateBeta(double xi, double yi, double xj, double yj, double theta) {
  static const double fac=M_PI/180.0;
  static const double rkRatio=scenario.R/scenario.k;
  double num=((xi-xj)*cos(fac*theta)+(yi-yj)*sin(fac*theta)+rkRatio);
  double a=xi-xj+rkRatio*cos(fac*theta);
  double b=yi-yj+rkRatio*sin(fac*theta);
  double denom=sqrt(a*a+b*b);
  return acos(num/denom);
}

double KusiakLayoutEvaluator::calculateBeta(double xi, double yi, double xj, double yj, int thetIndex) {
  static const double rkRatio=scenario.R/scenario.k;
  double num=((xi-xj)*scenario.getCosMidThetas(thetIndex)+(yi-yj)*scenario.getSinMidThetas(thetIndex)+rkRatio);
  double a=xi-xj+rkRatio*scenario.getCosMidThetas(thetIndex);
  double b=yi-yj+rkRatio*scenario.getSinMidThetas(thetIndex);
  double denom=sqrt(a*a+b*b);
  return acos(num/denom);
}

bool KusiakLayoutEvaluator::checkConstraint() {
  static const double minDist=64.0*scenario.R*scenario.R;
  for (int i=0; i<tpositions->rows; i++) {
    // check boundaries
    if (tpositions->get(i, 0) < 0 || tpositions->get(i, 0) > scenario.width ||
        tpositions->get(i, 1) < 0 || tpositions->get(i, 1) > scenario.height) {
      return false;
    }
    // check for obstacles
    for (int j=0; j<scenario.obstacles.rows; j++) {
      if (tpositions->get(i, 0) > scenario.obstacles.get(j, 0) &&
	  tpositions->get(i, 0) < scenario.obstacles.get(j, 2) &&
	  tpositions->get(i, 1) > scenario.obstacles.get(j, 1) &&
	  tpositions->get(i, 1) < scenario.obstacles.get(j, 3)) {
	printf("Obstacle %d [%f, %f, %f, %f] violated by turbine %d (%f, %f)\n", 
	       j, scenario.obstacles.get(j, 0), scenario.obstacles.get(j, 1),
	       scenario.obstacles.get(j, 2), scenario.obstacles.get(j, 3), i);
	return false;
      }
    }
    // checking security distance constraint
    for (int j=0; j<tpositions->rows; j++) {
      if (i!=j) {
	// calculate the sqared distance between both turbs
	double dist=(tpositions->get(i, 0)-tpositions->get(j, 0))*(tpositions->get(i, 0)-tpositions->get(j, 0))+
	  (tpositions->get(i, 1)-tpositions->get(j, 1))*(tpositions->get(i, 1)-tpositions->get(j, 1));
	if (dist<minDist) {
          //printf("dist:\t%f\t<\t%f\t(%d,%d)\n",dist,minDist,i,j);
	  return false;
	}
      }
    }
  }
  return true;
}

