#ifndef _EQUALSFUNCTOR_HPP_
#define _EQUALSFUNCTOR_HPP_

#include <cstdlib>
#include <cmath>
#include <climits>

class EqualsFunctor
{
public:
    template<class FirstInput, class SecondInput>
    bool operator() (const FirstInput& first, const SecondInput& second) const
    {
	return first == second;
    }

    bool operator() (const double first, const double second) const
    {
	return (std::abs(first - second) < std::numeric_limits<double>::epsilon());
    }
};

#endif // _EQUALSFUNCTOR_HPP_
