import numpy as np
from CompetitionEvaluator import WindScenario, WindFarmLayoutEvaluator

num_pop = 20;       # populations
tour_size = 4;      # tournament size
mut_rate = 0.05;    # mutation rate
cross_rate = 0.40;  # crossover rate - uniform crossover
max_evals = 2000;

def run_ga(wfle):
    xs = np.arange(0, ws.width, 8.001*ws.R)
    ys = np.arange(0, ws.height, 8.001*ws.R)
    grid = np.array([
            np.reshape(np.tile(xs,(np.size(ys),1)),
                np.size(xs)*np.size(ys),1),
            np.reshape(np.tile(ys,(np.size(xs),1)).T,
                np.size(xs)*np.size(ys),1)])
    grid = grid.T
    for obs in wfle.ws.obstacles:
        bpositions = np.multiply(
                grid <= [obs['xmax'], obs['ymax']],
                grid >= [obs['xmin'], obs['ymin']])
        bina = -np.multiply(bpositions[:,0], bpositions[:,1])
        grid = grid[bina,:]
    max_turbs = grid.shape[0]
    evals = int(max_evals/num_pop)

    pops = np.zeros([max_turbs, num_pop])
    fits = np.ones(num_pop)*np.inf

    for p in range(num_pop):
        bins = np.random.rand(max_turbs) > 0.5
        pops[:,p] = bins
        layout = grid[bins, :]
        fits[p] = wfle.evaluate(layout)

    for i in range(1,evals):
        # rank
        winners = np.zeros(num_pop/tour_size)
        competitors = np.arange(num_pop)
        np.random.shuffle(competitors)
        competitors = np.reshape(competitors, [len(winners), tour_size])
        for t in range(len(winners)):
            winners[t] = competitors[t,np.argmin(fits[competitors[t,:]])]

        # crossover
        children = np.zeros([max_turbs, num_pop])
        for c in range(len(winners), num_pop):
            np.random.shuffle(winners)
            parents = winners[0:2]
            child = pops[:, parents[0]]
            crossed = np.random.rand(max_turbs) < cross_rate
            child[crossed] = pops[crossed,parents[1]]
            children[:,c] = child

        # mutate
        opposite = -children
        mutate = np.random.rand(max_turbs, num_pop) < mut_rate
        children[mutate] = opposite[mutate]

        # elitism
        for c in range(len(winners)):
            children[:,c] = pops[:,winners[c]]

        # evaluate
        pops = children
        fits = np.ones(num_pop)*np.inf

        for p in range(num_pop):
            bins = pops[:,p] == 1
            layout = grid[bins,:]
            fits[p] = wfle.evaluate(layout)

        print('%d\t%f' % (i,min(fits)))

if __name__=='__main__':
    ws = WindScenario(0)
    # This is the 'Python GA' user token. Substitute your own user token in the evaluator construction
    wfle = WindFarmLayoutEvaluator(ws, 'AAVPD1LQTRFRT1F6IOQH9VK6IKP3TK')
    run_ga(wfle)
    for i in range(1,5):
        ws = WindScenario(i)
        wfle.ws = ws
        run_ga(wfle)
