#include "SimpleHashFunctor.hpp"

#define STRING_HASHING_METHOD(hashValue, character) (hashValue<<5) + hashValue + (character)


unsigned long SimpleHashFunctor::operator() (const std::string& value)
{
    const char* key = value.c_str();
    unsigned long hashValue = 0;

    while (*key)
    {
	hashValue = STRING_HASHING_METHOD(hashValue, *key);
	++key;
    }

    return hashValue;
}
