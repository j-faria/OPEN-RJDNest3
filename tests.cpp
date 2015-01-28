#include <iostream>
#include "Lookup.h"
#include "Start.h"
#include "MyModel.h"
#include "Data.h"
#include <gsl/gsl_cdf.h>


using namespace std;
using namespace DNest3;

typedef unsigned long timestamp_t;

static timestamp_t
get_timestamp ()
{
  struct timeval now;
  gettimeofday (&now, NULL);
  return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

int main(void)
{
	int i;
	double result1, result2;

	timestamp_t t10 = get_timestamp();
	for (i=0; i<10000; i++)
		result1 = gsl_cdf_beta_P(0.1, 0.867, 3.03);
	timestamp_t t11 = get_timestamp();
	double secs1 = (t11 - t10) / 1000000.0L;
	cout << secs1 << '\n';

	timestamp_t t20 = get_timestamp();
	for (i=0; i<10000; i++)
		result2 = 1. - pow(1. - 0.1/0.995, 3.03);
	timestamp_t t21 = get_timestamp();
	double secs2 = (t21 - t20) / 1000000.0L;
	cout << secs2 << '\n';

	cout << result1 << '\n';
	cout << result2 << '\n';

	return 0;
}