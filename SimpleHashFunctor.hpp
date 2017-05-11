#ifndef _SIMPLEHASHFUNCTOR_HPP_
#define _SIMPLEHASHFUNCTOR_HPP_

#include <iostream>

using namespace std;

/**
 * @brief Hashes the input to provide a key. Currently only takes char, short, int, long (both signed
 * and unsigned), float and double.
 */
class SimpleHashFunctor
{
public:
    template <typename Type>
    unsigned long operator() (Type value);
    unsigned long operator() (const std::string& value);

private:

    template<typename Type>
    struct is_pointer { static const bool value = false; };

    template <typename Type>
    struct is_pointer<Type*> { static const bool value = true; };

};

template <typename Type>
unsigned long SimpleHashFunctor::operator() (Type value)
{
    if (is_pointer<Type>::value)
    {
	cerr << "Doesn't support any pointer types for hashing" << endl;
	return 0;
    }

    // do the simple hash of returning the value itself.
    return ((unsigned long) value);
}
    
#endif //_SIMPLEHASHFUNCTOR_HPP_
