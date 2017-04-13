#include"conter.h"
#include<boost/thread/locks.hpp>

int64_t Counter::value()  const
{
	boost::lock_guard<boost::mutex> lock(mutex_);
	return value_;
}

int64_t Counter::getAndIncrease()
{
	boost::lock_guard<boost::mutex> lock(mutex_);
	int64_t ret = value_++;
	return ret;
}
