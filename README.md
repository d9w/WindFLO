WindFLO
=======

APIs for the Wind Farm Layout Optimization competition

## Intent

These APIs are for the GECCO 2014 [Wind Farm Layout Optimization Competition](http://www.irit.fr/wind-competition/). Competitors are expected to use these
functions to determine the fitness of the layouts they are optimizing. All APIs
keep a running count of the number of evaluations made, which will be used in
the comparison of competing algorithms. We hope competitors will find these
APIs useful and clear and will not try to subvert them.

## Basic Interface

### Methods

Between the C++, Java, and MATLAB implementations, the basic operation is the
same and consists of only two main methods:

* *Initialize/construct:* The evaluator class must be constructed and
  initialized with a wind scenario number. We have provided 10 sample
  Scenarios, which can be found in
  [Scenarios](https://github.com/d9w/WindFLO/tree/master/Scenarios).

* *Evaluate:* This method takes a layout as an n-by-2 matrix, where n is the
  number of turbines, and the columns represent the x and y coordinates of each
  turbine, respectively. This method returns the wake free ratio in Java and
  C++, sets all variables, and updates the evaluation count.

### Variables

Users have read access to the following variables, which

* *Wake free ratio:* the fitness of the entire layout. It is a value between 0
  and 1 representing the energy capture of the field over the theoretical
  energy capture of the same number of turbines without any inter-turbine wake.
  In C++ and Java, this value is returned by the evaluate method. If any of the
  turbines are within 8 radii of each other, given as R in the wind scenarios
  and here a set value of 38.5m, the layout will be considered invalid and the
  wake free ratio will be negative.

* *Energy output:* the energy capture of the entire field in kWh.

* *Turbine Fitnesses:* the wake free ratio for each turbine. This is an n-by-1
  matrix with the same turbine order as the input to the evaluate method.

* *Energy outputs:* the energy at each turbine location in 24 directional bins.
  As can be seen in the wind scenario files, the wind distribution is split up
  over 24 directional bins. This n-by-24 matrix gives the energy capture of
  each turbine location, in the same order as the input to the evaluate method,
  for each of the 24 directional bins.

* *Number of evaluations:* the number of times the evaluate method has been
  called so far.

## C++

The C++ interface is used as follows:

```C++
Matrix<double> tpos(n, 2);
stringstream scFileName;
scFileName << "../Scenarios/" << scnumber << ".xml";
WindScenario wsc(scFileName.str());
KusiakLayoutEvaluator simu;
simu.initialize(wsc);
double wake_free_ratio = simu.evaluate(&tpos);
wake_free_ratio = simu.getWakeFreeRatio();
double energy_output = simu.getEnergyOutput();
Matrix<double>* turbine_fitnesses = simu.getTurbineFitnesses();
Matrix<double>* energy_outputs = simu.getEnergyOutputs();
int n_evals = simu.getNumberOfEvaluations();
```

where `Matrix` is defined in `Matrix.hpp`, and the tpos `Matrix` needs to have
set values. A full example of use can be seen in `main.cpp`.

## Java

The Java interface is very similar to the C++ interface:

```Java
double tpos[][] = new double[n][2];
WindScenario ws = new WindScenario("../Scenarios/0"+sc+".xml");
KusiakLayoutEvaluator simu = new KusiakLayoutEvaluator();
simu.initialize(ws);
double wake_free_ratio = simu.evaluate(tpos);
wake_free_ratio = simu.getWakeFreeRaio();
double energy_output = simu.getEnergyOutput();
double[][] turbine_fitnesses = simu.getTurbineFitnesses();
double[][] energy_outputs = simu.getEnergyOutputs();
int n_evals = simu.getNumberOfEvaluations();
```

Again, tpos is empty here, but a more full example can be seen in `main.java`.

## MATLAB

The MATLAB interface uses a class which is returned by the method calls:

```Matlab
tpos = zeros(200,2);
wfle = WindFarmLayoutEvaluator('../Scenarios/00.xml');
wfle = wfle.evaluate(tpos);
wake_free_ratio = wfle.wfRatio;
energy_output = wfle.EnergyOutput;
turbine_fitnesses = wfle.TurbineFitnesses;
energy_outputs = wfle.EnergyOutputs;
n_evals = wfle.nEvals;
```

An example of use can be seen in `cross_validate.m`.

## Questions, comments, bugs

We are continuously working on the competition software as we work on the
automated submission and leaderboard system. If you would like to contact us
concerning the APIs, please email us. Our emails can be found on the
[competition site](http://www.irit.fr/wind-competition/).
