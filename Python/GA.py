import numpy as np
from WindFarmLayoutEvaluator import WindScenario, WindFarmLayoutEvaluator

num_pop = 20;       # populations
tour_size = 4;      # tournament size
mut_rate = 0.05;    # mutation rate
cross_rate = 0.40;  # crossover rate - uniform crossover

def run_ga(wfle):
    xs = np.arange(0, ws.width, 8.001*ws.R)
    ys = np.arange(0, ws.height, 8.001*ws.R)
    grid = np.array([
            numpy.reshape(numpy.tile(xs,(numpy.size(ys),1)),
                numpy.size(xs)*numpy.size(ys),1),
            numpy.reshape(numpy.tile(ys,(numpy.size(xs),1)).T,
                numpy.size(xs)*numpy.size(ys),1)])
    for obstacle in wfle.ws.obstacles:
        bpositions = np.multiply(
                grid <= [obs['xmax'], obs['ymax']],
                grid >= [obs['xmin'], obs['ymin']])

if __name__=='__main__':
    ws = WindScenario('../Scenarios/obs_00.xml')
    wfle = WindFarmLayoutEvaluator(ws)
    run_ga(wfle)
