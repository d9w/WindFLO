//
//  CompetitionScenario.h
//

#ifndef __CompetitionScenario__
#define __CompetitionScenario__

#include <iostream>
#include "Matrix.hpp"
#include <cmath>
#include <vector>

using namespace std;

class CompetitionScenario {
public:
    static void NbActiveScenario(int delta=0, bool disp=false) {
        static int nbActive=0;
        nbActive+=delta;
        if (disp) std::cerr << "# Active Scenarii : " << nbActive << std::endl;
    }

    CompetitionScenario() {};
    CompetitionScenario(int sc);
    CompetitionScenario(const CompetitionScenario& sc);
    ~CompetitionScenario() {NbActiveScenario(-1);};

    int num;
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
    double width;
    double height;
};

#endif /* defined(__CompetitionScenario__) */
