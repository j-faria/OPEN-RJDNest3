from pylab import *
from scipy import stats
import numpy.ma as ma

max_planets = 3

sample_data = loadtxt('sample.txt', skiprows=2)
posterior_data = loadtxt('posterior_sample.txt')

## periods
periods_sample = []
periods_posterior = []
for i in range(max_planets):
	periods_sample.append(sample_data[:,10+i])
	periods_posterior.append(posterior_data[:,10+i])

p_s = np.concatenate(periods_sample)
p_p = np.concatenate(periods_posterior)
# mask 0 values, should use these arrays in plots
P_sample = np.ma.masked_equal(p_s, 0)
P_posterior = np.ma.masked_equal(p_p, 0)


## amplitudes
amplitudes_sample = []
amplitudes_posterior = []
for i in range(max_planets):
	amplitudes_sample.append(sample_data[:,10+i])
	amplitudes_posterior.append(posterior_data[:,10+i])

k_s = np.concatenate(amplitudes_sample)
k_p = np.concatenate(amplitudes_posterior)
# mask 0 values, should use these arrays in plots
K_sample = np.ma.masked_equal(k_s, 0)
K_posterior = np.ma.masked_equal(k_p, 0)



## eccentricities
eccentricities_sample = []
eccentricities_posterior = []
for i in range(max_planets):
	eccentricities_sample.append(sample_data[:,10+3*max_planets+i])
	eccentricities_posterior.append(posterior_data[:,10+3*max_planets+i])

ecc_s = np.concatenate(eccentricities_sample)
ecc_p = np.concatenate(eccentricities_posterior)
# mask 0 values, should use these arrays in plots
ECC_sample = np.ma.masked_equal(ecc_s, 0)
ECC_posterior = np.ma.masked_equal(ecc_p, 0)


posterior = np.zeros((len(P_posterior), 3))
posterior[:,0] = ma.log(P_posterior)
posterior[:,1] = ma.log(K_posterior)
posterior[:,2] = ECC_posterior