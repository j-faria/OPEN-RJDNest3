#ifndef _RJObject_
#define _RJObject_

#include <vector>
#include <ostream>

/*
* A class that implements basic birth-death Metropolis-Hastings
* proposals using an exponential prior on the masses and a
* uniform prior on the positions. This is designed for 1, 2,
* or 3-dimensional objects. This class stores masses for
* each of the components it has, but it does not store
* any other properties. If your components have more properties
* you may derive from this class or store them externally.
*/

template<class Distribution>
class RJObject
{
	protected:
		// How many parameters for each component
		int num_dimensions;

		// Maximum number of components allowed (minimum is zero)
		int max_num_components;

		// fixed num_components = max_num_components?
		bool fixed;

		// The hyperparameters that specify the conditional prior
		// for the components
		Distribution dist;

		// The components
		int num_components;
		std::vector< std::vector<double> > components;

		// Transformed into iid U(0, 1) priors
		std::vector< std::vector<double> > u_components;

		// Added/removed components
		std::vector< std::vector<double> > added;
		std::vector< std::vector<double> > removed;

		// Helper methods -- these do one thing at a time
		double perturb_num_components(double scale);
		double perturb_components(double chance);

		// Helper methods -- add or remove single component
		double add_component();
		double remove_component();

	public:
		/*
		* num_dimensions: number of dimensions for each object.
		* "mass" etc count. E.g. in StarField problem (x, y, flux)
		* specifies a star, so num_dimensions = 3.
		*
		* max_num_components: obvious
		* fixed: if true, doesn't do RJ steps. N will be fixeded at
		* max_num_components
		*/
		RJObject(int num_dimensions, int max_num_components, bool fixed,
				const Distribution& dist);

		// Generate everything from the prior
		void fromPrior();

		// The top-level perturb method
		double perturb();

		// Take "removed" components and put them into "added" with
		// negative amplitude. Only applicable to certain models!
		void consolidate_diff();

		// For output
		void print(std::ostream& out) const;

		// Getter for components
		const std::vector< std::vector<double> >& get_components() const
		{ return components; }

		// Getters for the diff
		const std::vector< std::vector<double> >& get_added() const
		{ return added; }

		// Getters for the diff
		const std::vector< std::vector<double> >& get_removed() const
		{ return removed; }

		// Getter for hyperparameters
		const Distribution& get_dist() const
		{ return dist; }
};

#include "RJObjectImpl.h"

#endif

