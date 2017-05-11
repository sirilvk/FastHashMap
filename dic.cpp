#include <chrono>
#include <iostream>
#include "HashMap.hpp"
using std::string;


typedef std::chrono::high_resolution_clock::time_point TimePoint;
typedef std::chrono::high_resolution_clock SteadyClock;

template <typename precision = std::chrono::microseconds>
class EventTimerWithPrecision
{
public:
    EventTimerWithPrecision() { _beg = SteadyClock::now(); }

    long long elapsed() {
	return  std::chrono::duration_cast<precision>(SteadyClock::now() - _beg).count();
    }

    void reset() { _beg = SteadyClock::now(); }

private:
    TimePoint _beg;
};

typedef EventTimerWithPrecision<std::chrono::milliseconds> EventTimer;

int main()
{
    EventTimer tm;
    string strList[1000000];
    for (int i = 0; i < 1000000; ++i)
	strList[i] = std::to_string(i);
    
    HashMap<string, int> strMap;
//    boost::unordered_map<string, int> strMap;
    for (int i = 0; i < 1000000; ++i)
    {
	strMap[strList[i]] = i;
    }
    std::cout << "Total time taken " << tm.elapsed() << std::endl;
}
