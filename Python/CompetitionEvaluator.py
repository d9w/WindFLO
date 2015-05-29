import numpy as np
import scipy
import requests
import json
import xml.etree.ElementTree as ET

class WindScenario:

    def __init__(self, number):
        self.number = number
        r = requests.get('http://127.0.0.1:5000/scenarios/%d' % (number))
        results = r.json()
        self.R = results['r']
        self.height = results['height']
        self.width = results['width']
        self.obstacles = results['obstacles']

class WindFarmLayoutEvaluator:

    def __init__(self, ws, user_token):
        self.ws = ws
        self.user_token = user_token
        self.energy_outpus = np.zeros(0)
        self.turbine_fitnesses = np.zeros(0)
        self.energy_output = 0
        self.wake_free_ratio = 0
        self.energy_cost = 0
        self._evals = 0

        r = requests.post('http://127.0.0.1:5000/runs/',
                json=dict(api_token=self.user_token),
                headers={'Content-Type':'application/json'})
        self.run_token = r.json()['token']

    def evaluate(self, layout):
        r = requests.post('http://127.0.0.1:5000/evaluate/',
                json=dict(api_token=self.user_token,
                    run=self.run_token,
                    scenario=self.ws.number,
                    xs=layout[:,0].tolist()[0:10],
                    ys=layout[:,1].tolist()[0:10]))
        results = r.json()
        self.energy_cost = results['energy_cost']
        self.energy_outputs = results['energy_outputs']
        self.energy_output = results['energy_output']
        self._evals = results['evals']
        self.turbine_fitnesses = results['turbine_fitnesses']
        self.wake_free_ratio = results['wake_free_ratio']
        return self.energy_cost
