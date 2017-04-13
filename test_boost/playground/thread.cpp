#include<boost/thread.hpp>
#include<iostream>

#include"cat.h"

using namespace boost;

cat::cat()
{

}

void cat::meow()
{
	sleep(1);
	std::cout<<"meow"<<std::endl;
}

cat::~cat()
{

}

int main(int argc, char* argv[])
{
	cat c;
	thread t(c.meow);
	std::cout<< "hello" <<std::endl;
	return 0;
}
