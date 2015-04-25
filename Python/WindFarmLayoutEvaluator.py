import numpy as np
import scipy
import xml.etree.ElementTree as ET

class WindScenario:

    def __init__(self, xml):
        scenario = {'c':[],
                'k':[],
                'omega':[],
                'theta':[],
                'obstacle': []}
        root = ET.parse(xml).getroot()
        for child in root.getchildren():
            if child.tag == 'Angles':
                for angle in child.getchildren():
                    for param in angle.items():
                        scenario[param[0]].append(eval(param[1]))
            if child.tag == 'Obstacles':
                for obstacle in child.getchildren():
                    obsdict = {}
                    for param in obstacle.items():
                        obsdict[param[0]]=eval(param[1])
                    scenario['obstacle'].append(obsdict)
            if child.tag == 'Parameters':
                for param in child.getchildren():
                    scenario[param.tag] = eval(param.text)
        self.cs = np.array(scenario['c'])
        self.ks = np.array(scenario['k'])
        self.omegas = np.array(scenario['omega'])
        thetas = np.array(scenario['theta'])
        self.thetas = np.vstack((thetas, thetas+15)).T
        self.obstacles = scenario['obstacle']
        self.width = scenario['Width']
        self.height = scenario['Height']
        self.nturbines = scenario['NTurbines']
        self.energy = scenario['WakeFreeEnergy']
        self.CT=0.8;
        self.PRated=1500.0;
        self.R=38.5;
        self.eta=-500.0;
        self.k=0.0750;
        self.slambda=140.86;
        self.vCin=3.5;
        self.vCout=20;
        self.vRated=14;

class WindFarmLayoutEvaluator:

    def __init__(self, ws):
        self.ws = ws
        self._evals = 0

    def settings(self, nturb):
        # set dcos, esin, v1large, v2large, Plarge
        fac = np.pi/180
        ntheta = np.size(self.ws.thetas)
        self._dcos = np.tile(np.reshape(np.tile(np.cos(np.mean(
            self.ws.thetas*fac,1)), (nturb, 1)).T,
            ntheta*nturb/2, 1), (nturb, 1))
        self._esin = np.tile(np.reshape(np.tile(np.sin(np.mean(
            self.ws.thetas*fac,1)), (nturb, 1)).T,
            ntheta*nturb/2, 1), (nturb, 1))
        vints = numpy.arange(3.5, self.ws.vRated+0.5, 0.5)
        self._v1large = np.tile(np.array(vints[1:-1]), (ntheta, nturb))
        self._v2large = np.tile(np.array(vints[0:-2]), (ntheta, nturb))

    def evaluate(self, layout):
        self._evals += 1
        if not self.check_constraint(layout):
            return float('infinity')
        return 0

    def check_constraint(self, layout):
        dists = scipy.spatial.distance.pdist(layout)
        if any(dists <= 8*self.ws.R):
            return False
        for obs in self.ws.obstacles:
            bpositions = np.multiply(
                    layout <= [obs['xmax'], obs['ymax']],
                    layout >= [obs['xmin'], obs['ymin']])
            if any(np.multiply(bpositions[:,0], bpositions[:,1])):
                return False
        return True
