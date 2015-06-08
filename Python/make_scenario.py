import numpy as np

with open('scenario.xml', 'w') as f:
    f.write('<?xml version="1.0" encoding="utf-8"?>\n')
    f.write('<WindField><!--Randomly generated wind-->\n')
    f.write('  <Angles>\n')
    cs = abs(np.random.normal(np.random.normal(8,2), max(np.random.normal(1, 0.5),0.1), 24))
    ks = abs(np.random.normal(max(np.random.normal(2,0.5),0.5), max(np.random.normal(0.5,0.25),0.1), 24))
    omegas = 1+np.sin((np.random.random()*np.pi)+(4*np.random.random()*np.pi)/24 * np.arange(24))
    omegas = np.roll(omegas/sum(omegas), np.random.randint(24))
    for i in range(24):
        f.write('    <angle c="%f" k="%f" omega="%f" theta="%d"/>\n' % (cs[i], ks[i], omegas[i], i*15))
    f.write('  </Angles>\n')
    r = 385
    width = np.random.randint(10, 30) * r
    height = np.random.randint(10, 30) * r
    obsx = np.random.randint(5, 10)
    obsy = np.random.randint(5, 10)
    obs = np.random.random([obsx, obsy]) > np.random.uniform(0.9)
    wstep = width/obsx
    hstep = height/obsy
    f.write('  <Obstacles>\n')
    for x in range(obsx):
        for y in range(obsy):
            if obs[x][y]:
                xmin = x*wstep
                ymin = y*hstep
                xmax = (x+1)*wstep
                ymax = (y+1)*hstep
                f.write('    <obstacle xmin="%d" ymin="%d" xmax="%d" ymax="%d"/>\n' % (int(xmin),int(ymin),int(xmax),int(ymax)))
    f.write('  </Obstacles>\n')
    f.write('  <Parameters>\n')
    f.write('     <Width>%d</Width>\n' % (int(width)))
    f.write('     <Height>%d</Height>\n' % (int(height)))
    f.write('     <NTurbines>%d</NTurbines>\n' % (int((width*height)/(r**2))))
    f.write('     <WakeFreeEnergy>0.0</WakeFreeEnergy>\n')
    f.write('  </Parameters>\n')
    f.write('</WindField>')
