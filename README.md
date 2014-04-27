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
  initialized with a wind scenario number. We have provided 20 sample
  Scenarios, which can be found in
  [Scenarios](https://github.com/d9w/WindFLO/tree/master/Scenarios), the latter
  10 of which are the same as the first 10 with the addition of two obstacles.

* *Evaluate:* This method takes a layout as an n-by-2 matrix, where n is the
  number of turbines, and the columns represent the x and y coordinates of each
  turbine, respectively. This method returns the wake free ratio in Java and
  C++, sets all variables, and updates the evaluation count.

### Variables

Users have read access to the following variables without increasing the
evaluation count. All variables are updated by the evaluator class:

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

## Submission expectations

We expect an algorithm class or function, depending on language, that takes in
an evaluator class as its only argument. We have provided an example binary GA
working on the second track problem, the optimization of the number of turbines
given a minimum wake free ratio per turbine, to show the expected format of
submission. Note that we do not expect to receive the contents of the
corresponding "main" file - we will use a similar script to run submissions and
provide this only as an example.

## Running the examples

All examples expect the scenario files in the current directory structure, and
while you can change this, to make the examples work, you must

```
$ git clone https://github.com/d9w/WindFLO
$ cd WindFLO
```

## C++

The C++ interface is displayed in `main.cpp` and `GA.cpp`. To compile and run
using gcc, a makefile has been provided. We are still working on the
expectations for C++ submissions, but expect to build your algorithm with a
makefile similar to this one. The example GA can be run using (from the WindFLO
directory):

```Bash
$ cd c++
$ make
$ ./eval.o
```

## Java

The Java interface is displayed in `main.java` and `GA.java`. To compile and
run using Java v6, use (from WindFLO directory):

```Bash
$ cd Java
$ javac main.java
$ java main
```

## MATLAB

The MATLAB interface is displayed in `main.m` and `GA.m`. `main.m` can be
executed in the MATLAB environment if the current directory is
`WindFLO/MATLAB`:

```Matlab
>> cd MATLAB
>> main
```

## Questions, comments, bugs

We are continuously working on the competition software as we work on the
automated submission and leaderboard system. If you would like to contact us
concerning the APIs, please email us. Our emails can be found on the
[competition site](http://www.irit.fr/wind-competition/).
