#include<boost/bind.hpp>
#include<boost/function.hpp>
#include<iostream>

using namespace boost;
using namespace std;

void callback(int a, int b)
{
	cout<<"random "<< a <<" make a callback on "<< b <<endl;
}

void useless(boost::function<void(int)> fun)
{
	fun(random());
}

int main(int argc, char* argv[])
{
	boost::function<void(int)> fun1 = bind(callback, _1, 1);
	boost::function<void(int)> fun2 = bind(callback, _1, 2);
	useless(fun1);
	useless(fun2);
	return 0;
}
