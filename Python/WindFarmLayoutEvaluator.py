import numpy as np
import scipy
import xml.etree.ElementTree as ET

class WindScenario:

    def __init__(self, xml):
        pass

class WindFarmLayoutEvaluator:

    def __init__(self, ws):
        self.ws = ws
        self.energy_outpus = numpy.zeros(0)
        self.turbine_fitnesses = numpy.zeros(0)
        self.energy_output = 0
        self.wake_free_ratio = 0
        self.energy_cost = 0
        self._evals = 0

    def evaluate(self, layout):
        self._evals += 1
        return 0

    def get_number_of_evaluations(self):
        return self._evals

    def check_constraint(self, layout):
        return True
