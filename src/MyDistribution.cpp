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
	// no hyperparameters
}


double MyDistribution::perturb_parameters()
{
	return 0.;
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

	return 0.;
}

void MyDistribution::from_uniform(std::vector<double>& vec) const
{
	// inverse CDF of the Jeffreys distribution [0.2d - 365000d]
	vec[0] = 0.2*pow(365000./0.2, vec[0]);

	// inverse CDF of the Modified Jeffreys distribution [1, 2129]
	vec[1] = 1.*( pow(1.+2129./1., vec[1]) - 1.);

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
	// CDF of the Jeffreys distribution [0.2d - 365000d]
	vec[0] = log(vec[0]/0.2) / log(365000./0.2);

	// CDF of the Modified Jeffreys distribution [1, 2129]
	vec[1] = log(1.+vec[1]/1.) / log(1.+2129./1.);

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

