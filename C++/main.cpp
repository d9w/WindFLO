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

void process_mem_usage(double& vm_usage, double& resident_set)
{
    using std::ios_base;
    using std::ifstream;
    using std::string;
    
    vm_usage     = 0.0;
    resident_set = 0.0;
    
    // 'file' stat seems to give the most reliable results
    //
    ifstream stat_stream("/proc/self/stat",ios_base::in);
    
    // dummy vars for leading entries in stat that we don't care about
    //
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;
    
    // the two fields we want
    //
    unsigned long vsize;
    long rss;
    
    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
    >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
    >> utime >> stime >> cutime >> cstime >> priority >> nice
    >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest
    
    stat_stream.close();
    
    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage     = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}


int main(int argc, const char * argv[])
{
  /*  srand(time(NULL));

  for (int i=0; i<100; i++) {
    int n=((double)rand())/RAND_MAX*5+10;
    stringstream filename;
    filename << "../TestLayouts/layout" << i << ".txt";
    cout << "Generating " << filename.str() << endl;
    ofstream layoutFile(filename.str());
    layoutFile << n*n << endl;
    for (int t=0; t<n*n; t++) {
      double x=fabs(14000/n*(t%n)+((double)rand())/RAND_MAX*20-10);
      double y=fabs(7000/n*(int)(t/n)+((double)rand())/RAND_MAX*10-5);
      layoutFile << x << " " << y << endl;
    }
    layoutFile.close();
  }
  exit(0);*/

  // read a layout file
  for (int i=0; i<100; i++) {
    for (int sc=0; sc<9; sc++) {
      stringstream layoutFileName;
      stringstream scFileName;
      char scnumber[64];
      sprintf(scnumber, "%02d", sc);
      layoutFileName << "../TestLayouts/layout" << i << ".txt";
      scFileName << "../Scenarios/" << scnumber << ".xml";
      
      ifstream layoutFile(layoutFileName.str());
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
  exit(0);
    
    /*    srand(time(NULL));
    double vm, rss;
    while (true) {
        process_mem_usage(vm, rss);
        cout << "VM: " << vm << "; RSS: " << rss << endl;

        WindScenario wsc(0);
        Matrix<double> tpositions(rand()%500,2);
        for (unsigned i=0; i<tpositions.rows; i++) {
            tpositions.set(i,0,i*310);
            tpositions.set(i,1,i*310);
        }
        struct rusage start,end;
        getrusage(RUSAGE_SELF, &start);
        WindSimulator simu(wsc, &tpositions);
        getrusage(RUSAGE_SELF, &end);
        cerr << endl;
        cerr << simu.energyOutput(2) << " in " << ((double)end.ru_utime.tv_usec-(double)start.ru_utime.tv_usec) << "ms" << endl;
        //cerr << simu.tspe.toString() << endl;
	}*/
}

