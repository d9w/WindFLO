import numpy
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
        self.thetas = numpy.array(scenario['theta'])
        self.obstacles = scenario['obstacle']
        self.width = scenario['Width']
        self.height = scenario['Height']
        self.nturbines = scenario['NTurbines']
        self.energy = scenario['WakeFreeEnergy']
