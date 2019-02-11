#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

wave = np.loadtxt('wavefunction.dat')
potential = np.loadtxt('potential.dat')

plt.plot(wave[:,0], wave[:,1], 'r') 
plt.plot(potential[:,0], potential[:,1], 'g')
plt.savefig('graph.png')
