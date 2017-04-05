#include"read_guard.h"

using namespace std;
using namespace boost;

read_guard::read_guard():_host_ptr(new vector<string>)
{
}

read_guard::~read_guard()
{
}

bool read_guard::verify(int index, const string& match)
{
	shared_ptr<vector<string> > data_ptr;
	reader_sign(data_ptr);
	if(check_bound(index) && (*data_ptr)[index] == match)
		return true;
	else
		return false;
}

int read_guard::find(const string& match)
{
	shared_ptr<vector<string> > data_ptr;
	reader_sign(data_ptr);
	for (vector<string>::iterator i = data_ptr->begin(); i < data_ptr->end(); i++)
		if(*i == match)
			return i-data_ptr->begin();
	return -1;
}

string read_guard::query(int index)
{
	shared_ptr<vector<string> > data_ptr;
	reader_sign(data_ptr);
	if(check_bound(index))
		return (*data_ptr)[index];
	else return string("NULL");//else will throw out a exception.
}

bool read_guard::push_back(const std::string& match)
{
	lock_guard<mutex> locker(_host_ptr_mutex);

	//push_back won't lead to a reallocation?
	//or no other reader or writer?
	if(is_not_full()||is_exclusive())
		_host_ptr->push_back(match);//is OK to do changes on present data
	else
	{
		//need to do changes on the copy
		lock_guard<mutex> copy_locker(_on_copy_mutex);

		//sign as a reader and copy data
		_host_ptr_mutex.unlock();
		shared_ptr<vector<string> >copy_ptr;
		copy_to(copy_ptr);
		copy_ptr->push_back(match);

		//update _host_ptr's reference to the copy
		_host_ptr_mutex.lock();
		update_host_ptr(copy_ptr);
		return true;
	}
	return false;
}

bool read_guard::assign(int index,const string& match)
{
	lock_guard<mutex> locker(_host_ptr_mutex);
	check_bound(index);
	if(is_exclusive())
		(*_host_ptr)[index].assign(match);//is OK to do changes on present data
	else
	{
		//need to do changes on the copy
		lock_guard<mutex> copy_locker(_on_copy_mutex);

		//sign as a reader and copy data
		_host_ptr_mutex.unlock();
		shared_ptr<vector<string> > copy_ptr;
		copy_to(copy_ptr);
		(*copy_ptr)[index].assign(match);

		//update _host_ptr's reference to the copy
		_host_ptr_mutex.lock();
		update_host_ptr(copy_ptr);
		return true;//copy happened
	}
	return false;
}

bool read_guard::erase(int index)
{
	lock_guard<mutex> locker(_host_ptr_mutex);
	check_bound(index);
	if(is_exclusive())
		_host_ptr->erase(_host_ptr->begin()+index);
	else
	{
		//need to do changes on the copy
		lock_guard<mutex> copy_locker(_on_copy_mutex);

		//sign as a reader and copy data
		_host_ptr_mutex.unlock();
		shared_ptr<vector<string> > copy_ptr;
		copy_to(copy_ptr);
		copy_ptr->erase(copy_ptr->begin()+index);

		//update _host_ptr's reference to the copy
		_host_ptr_mutex.lock();
		update_host_ptr(copy_ptr);
		return true;//copy happened
	}
	return false;
}

inline void read_guard::reader_sign(shared_ptr<vector<string> >& data_ptr)
{
	lock_guard<mutex> locker(_host_ptr_mutex);
	data_ptr = _host_ptr;
}

inline bool read_guard::is_not_full()
{
	if(_host_ptr->capacity() > _host_ptr->size())
		return true;
	else
		return false;
}

inline bool ::read_guard::is_exclusive()
{
	if(_host_ptr.use_count() == 1)
		return true;
	else
		return false;
}

inline void read_guard::copy_to(shared_ptr<vector<string> >& copy_ptr)
{
	//keep at least one reader reference to present data
	shared_ptr<vector<string> > data_ptr;
	reader_sign(data_ptr);
	copy_from_host(copy_ptr);
}

inline void read_guard::update_host_ptr(const shared_ptr<vector<string> >& data_ptr)
{
	_host_ptr = data_ptr;
}

inline void read_guard::copy_from_host(shared_ptr<vector<string> >& data_ptr)
{
	data_ptr.reset(new vector<string>(*_host_ptr));
}

inline bool read_guard::check_bound(int index)
{
	if(index < _host_ptr->size() && index >= 0)
		return true;
	else
	{
		throw "over bound";
		return false;
	}
}
