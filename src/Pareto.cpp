#include "Pareto.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>

using namespace DNest3;

Pareto::Pareto(double x_min, double x_max,
					double y_min, double y_max,
					double f0_min, double f0_max)
:x_min(x_min)
,x_max(x_max)
,y_min(y_min)
,y_max(y_max)
,f0_min(f0_min)
,f0_max(f0_max)
{

}

void Pareto::fromPrior()
{
	f0 = exp(log(f0_min) + log(f0_max/f0_min)*randomU());
	alpha = 1. + 4.*randomU();
}

double Pareto::perturb_parameters()
{
	double logH = 0.;

	int which = randInt(2);

	if(which == 0)
	{
		f0 = log(f0);
		f0 += log(f0_max/f0_min)*pow(10., 1.5 - 6.*randomU())*randn();
		f0 = mod(f0 - log(f0_min), log(f0_max/f0_min)) + log(f0_min);
		f0 = exp(f0);
	}
	else
	{
		alpha += 4.*pow(10., 1.5 - 6.*randomU())*randn();
		alpha = mod(alpha - 1., 4.) + 1.;
	}

	return logH;
}

double Pareto::log_pdf(const std::vector<double>& vec) const
{
	if(vec[0] < x_min || vec[0] > x_max ||
		vec[1] < y_min || vec[1] > y_max ||
		vec[2] < f0)
		return -1E300;

	return log(alpha) - alpha*log(f0) - (alpha + 1.)*log(vec[2]);
}

void Pareto::from_uniform(std::vector<double>& vec) const
{
	vec[0] = x_min + (x_max - x_min)*vec[0];
	vec[1] = y_min + (y_max - y_min)*vec[1];
	vec[2] = f0*pow(1. - vec[2], -1./alpha);
}

void Pareto::to_uniform(std::vector<double>& vec) const
{
	vec[0] = (vec[0] - x_min)/(x_max - x_min);
	vec[1] = (vec[1] - y_min)/(y_max - y_min);
	vec[2] = 1. - pow(f0/vec[2], alpha);
}

void Pareto::print(std::ostream& out) const
{
	out<<f0<<' '<<alpha<<' ';
}

