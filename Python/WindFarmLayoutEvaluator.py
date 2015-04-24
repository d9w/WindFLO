import numpy
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
        self.cs = numpy.array(scenario['c'])
        self.ks = numpy.array(scenario['k'])
        self.omegas = numpy.array(scenario['omega'])
        thetas = numpy.array(scenario['theta'])
        self.thetas = numpy.asmatrix(numpy.vstack((thetas, thetas+15)).T)
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

    def evaluate(self, scenario):
        pass

    def check_constraint(self, layout):
        dists = scipy.spatial.distance.pdist(layout)
        if any(dists <= 8*self.ws.R):
            return False
        for obs in self.ws.obstacles:
            bpositions = numpy.multiply(
                    layout <= [obs['xmax'], obs['ymax']],
                    layout >= [obs['xmin'], obs['ymin']])
            if any(numpy.multiply(bpositions[:,0], bpositions[:,1])):
                return False
        return True
