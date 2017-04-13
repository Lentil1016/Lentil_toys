#include<vector>
#include<iostream>

using namespace std;

class base
{
public:
	base(int a);
	virtual int getint();
	base* get_ptr();
private:
	int int1;
};

base* base::get_ptr()
{
	return this;
}

int base::getint()
{
	return int1;
}

base::base(int a):int1(a)
{
}

class derived:public base
{
public:
	derived(int a, int b);
	virtual int getint();
	int int2;//public member for observation
};

int derived::getint()
{
	return int2;
}

derived::derived(int a, int b):base(a),int2(b)
{
}

int main(int argc, char* argv[])
{
	cout<<sizeof(int)<<" "<<sizeof(base*)<<" "<<sizeof(base)<<endl;

	vector<base*> base_p;
	base_p.push_back(new derived(4, 5));
	base_p.push_back(new base(2));
	cout << base_p[0]->getint() << endl;

	vector<base> base_v;
	base_v.push_back(derived(4, 5));

	base* a = base_v[0].get_ptr();
	cout << base_v[0].getint()<< " " << a->getint() <<" "<< ((derived*) a)->getint() <<" "<< ((derived*)a)->int2 << endl;

	base_v.push_back(base(3));
	a = base_v[0].get_ptr();
	//every try
	cout << base_v[0].getint()<< " " << a->getint() <<" "<< ((derived*) a)->getint() <<" "<< ((derived*)a)->int2 << endl;
	return 0;
}
