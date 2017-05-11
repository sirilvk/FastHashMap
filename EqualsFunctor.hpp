#ifndef _EQUALSFUNCTOR_HPP_
#define _EQUALSFUNCTOR_HPP_

class EqualsFunctor
{
public:
    template<class FirstInput, class SecondInput>
    bool operator() (const FirstInput& first, const SecondInput& second) const
    {
	return first == second;
    }
};

#endif // _EQUALSFUNCTOR_HPP_
