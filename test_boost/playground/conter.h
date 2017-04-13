#include<boost/utility.hpp>
#include<stdlib.h>
#include<boost/thread.hpp>

class Counter: boost::noncopyable
{
public:
	Counter():value_(0){}
	int64_t value() const;
	int64_t getAndIncrease();

private:
	int64_t value_;
	mutable boost::mutex mutex_;
};
