#include "MyDistribution.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>
#include <gsl/gsl_cdf.h>

using namespace DNest3;

MyDistribution::MyDistribution()
{

}

// Sample prior hyperparameters from their respective priors
void MyDistribution::fromPrior()
{
	// Median orbital period, mu_P
	// Cauchy prior centered on 5.901 = log(365 days), scale = 1
	// general form for location=x0, scale=c
	// cauchy_rand = c * tan(M_PI * (randomU()-0.5)) + x0
	// the distribution is truncated to the interval -15.3, 27.1, hence
	// the 0.97 and 0.485 values
	center = 5.901 + tan(M_PI*(0.97*randomU() - 0.485));
	// Diversity of orbital periods, w_P
	// uniform prior between 0.1 and 3
	width = 0.1 + 2.9*randomU();
	// Mean amplitude (metres per second) 
	// standard Cauchy prior 
	// again truncated to the interval -21.2, 21.2
	mu = exp(tan(M_PI*(0.97*randomU() - 0.485)));
}


double MyDistribution::perturb_parameters()
{
	double logH = 0.;

	int which = randInt(3);

	if(which == 0)
	{
		center = (atan(center - 5.901)/M_PI + 0.485)/0.97;
		center += randh();
		wrap(center, 0., 1.);
		center = 5.901 + tan(M_PI*(0.97*center - 0.485));
	}
	else if(which == 1)
	{
		width += 2.9*randh();
		wrap(width, 0.1, 3.);
	}
	else
	{
		mu = log(mu);
		mu = (atan(mu)/M_PI + 0.485)/0.97;
		mu += randh();
		wrap(mu, 0., 1.);
		mu = tan(M_PI*(0.97*mu - 0.485));
		mu = exp(mu);
	}
	return logH;
}

// vec[0] = "position" (log-period)
// vec[1] = amplitude
// vec[2] = phase
// vec[3] = eccentricity
// vec[4] = viewing angle

double MyDistribution::log_pdf(const std::vector<double>& vec) const
{
	if(vec[1] < 0. ||
			vec[2] < 0. || vec[2] > 2.*M_PI ||
			vec[3] < 0. || vec[3] > 0.8189776 ||
			vec[4] < 0. || vec[4] > 2.*M_PI)
		return -1E300;

	return  -log(2.*width) - abs(vec[0] - center)/width
		    -log(mu) - vec[1]/mu
		    + 2.1*log(1. - vec[3]/0.995);
}

void MyDistribution::from_uniform(std::vector<double>& vec) const
{
	// inverse CDF of the Laplace distribution
	// http://en.wikipedia.org/wiki/Laplace_distribution#Cumulative_distribution_function
	if(vec[0] < 0.5)
		vec[0] = center + width*log(2.*vec[0]);
	else
		vec[0] = center - width*log(2. - 2.*vec[0]);

	// inverse CDF of the Exponential distribution
	vec[1] = -mu*log(1. - vec[1]);

	// inverse CDF of the Uniform distribution [0, 2pi]
	vec[2] = 2.*M_PI*vec[2];

	// inverse CDF of the Beta distribution
	vec[3] = 1. - pow(1. - 0.995*vec[3], 1./3.1);  // original (approximation?)
	//vec[3] = gsl_cdf_beta_Pinv(vec[3], 0.867, 3.03);  // gsl function with parameters from Kipping (2013)

	// inverse CDF of the Uniform distribution [0, 2pi]
	vec[4] = 2.*M_PI*vec[4];
}

void MyDistribution::to_uniform(std::vector<double>& vec) const
{
	// CDF of the Laplace distribution
	// http://en.wikipedia.org/wiki/Laplace_distribution#Cumulative_distribution_function
	if(vec[0] < center)
		vec[0] = 0.5*exp((vec[0] - center)/width);
	else
		vec[0] = 1. - 0.5*exp((center - vec[0])/width);

	// CDF of the Exponential distribution
	vec[1] = 1. - exp(-vec[1]/mu);

	// CDF of the Uniform distribution [0, 2pi]
	vec[2] = vec[2]/(2.*M_PI);

	// CDF of the Beta distribution
	vec[3] = 1. - pow(1. - vec[3]/0.995, 3.1);  // original (approximation?)
	//vec[3] = gsl_cdf_beta_P(vec[3], 0.867, 3.03);  // gsl function with parameters from Kipping (2013)
	
	// CDF of the Uniform distribution [0, 2pi]
	vec[4] = vec[4]/(2.*M_PI);
}

void MyDistribution::print(std::ostream& out) const
{
	out<<center<<' '<<width<<' '<<mu<<' ';
}

