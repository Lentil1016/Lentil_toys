#include"read_guard.h"

using namespace std;
using namespace boost;

read_guard::read_guard():_host_ptr(new vector<string>),log(log4cpp::Category::getInstance(std::string("index")))
{
}

read_guard::~read_guard()
{
}

bool read_guard::verify(int index, const string& match)
{
	shared_ptr<vector<string> > data_ptr;
	reader_sign(data_ptr);
	log.info("verify come");
	if(check_bound(index) && (*data_ptr)[index] == match)
	{
#ifdef DEBUG_LENTIL
		oss.str("");
		oss<<"verify True string \""<<match.c_str()<<"\" at "<<index;
		log.debug(oss.str().c_str());
#endif
		return true;
	}
	else
	{
#ifdef DEBUG_LENTIL
		oss.str("");
		oss<<"verify False string \""<<match.c_str()<<"\" at "<<index;
		log.debug(oss.str().c_str());
#endif
		return false;
	}
}

int read_guard::find(const string& match)
{
	shared_ptr<vector<string> > data_ptr;
	reader_sign(data_ptr);
	log.info("find come");
	for (vector<string>::iterator i = data_ptr->begin(); i < data_ptr->end(); i++)
		if(*i == match)
		{
#ifdef DEBUG_LENTIL
			oss.str("");
			oss<<"find a string \""<<match.c_str()<<"\" at "<<i-data_ptr->begin()<<endl;
			log.debug(oss.str().c_str());
#endif
			return i-data_ptr->begin();
		}
#ifdef DEBUG_LENTIL
	oss.str("");
	oss<<"find no string fit \""<<match.c_str()<<"\"";
	log.debug(oss.str().c_str());
#endif
	return -1;
}

string read_guard::query(int index)
{
	shared_ptr<vector<string> > data_ptr;
	reader_sign(data_ptr);
	//log.info("query come");
	if(check_bound(index))
	{
#ifdef DEBUG_LENTIL
		oss.str("");
		oss<<"query a string \""<<(*data_ptr)[index].c_str()<<"\" at "<<index<<endl;
		log.debug(oss.str().c_str());
#endif
		return (*data_ptr)[index];
	}
	else return string("NULL");//else will throw out a exception.
}

bool read_guard::push_back(const std::string& match)
{
	lock_guard<mutex> locker(_host_ptr_mutex);

	//push_back won't lead to a reallocation?
	//or no other reader or writer?

	log.info("push_back come");
	if(is_not_full()||is_exclusive())
	{
		_host_ptr->push_back(match);//is OK to do changes on present data
#ifdef DEBUG_LENTIL
		oss.str("");
		oss<<"push_back \""<<match.c_str()<<"\" directly, size is "<<_host_ptr->size()<<\
			", capacity is "<<_host_ptr->capacity()<<", use count is "<< _host_ptr.use_count();
		log.debug(oss.str().c_str());
#endif
	}
	else
	{
		//need to do changes on the copy
		lock_guard<mutex> copy_locker(_on_copy_mutex);

		//sign as a reader and copy data
		_host_ptr_mutex.unlock();
		shared_ptr<vector<string> >copy_ptr;
		copy_to(copy_ptr);

#ifdef DEBUG_LENTIL
		oss.str("");
		oss<<"push_back \""<<match.c_str()<<"\" on COPY, size is "<<copy_ptr->size()<<\
			", capacity is "<<copy_ptr->capacity()<<", old use count is "<< _host_ptr.use_count();
		log.debug(oss.str().c_str());
#endif
		copy_ptr->push_back(match);

		//update _host_ptr's reference to the copy
		_host_ptr_mutex.lock();
		update_host_ptr(copy_ptr);
#ifdef DEBUG_LENTIL
		oss.str("");
		oss<<"push_back \""<<match.c_str()<<"\" done, size is "<<_host_ptr->size()<<\
			", capacity is "<<_host_ptr->capacity()<<", use count is "<< _host_ptr.use_count();
		log.debug(oss.str().c_str());
#endif
		return true;
	}
	return false;
}

bool read_guard::assign(int index,const string& match)
{
	lock_guard<mutex> locker(_host_ptr_mutex);
	check_bound(index);
	if(is_exclusive())
	{
		(*_host_ptr)[index].assign(match);//is OK to do changes on present data
#ifdef DEBUG_LENTIL
		oss.str("");
		oss<<"assign \""<<match.c_str()<<"\" on "<<index<<" directly, size is "<<_host_ptr->size()<<\
			", capacity is "<<_host_ptr->capacity()<<", use count is "<< _host_ptr.use_count();
		log.debug(oss.str().c_str());
#endif
	}
	else
	{
		//need to do changes on the copy
		lock_guard<mutex> copy_locker(_on_copy_mutex);

		//sign as a reader and copy data
		_host_ptr_mutex.unlock();
		shared_ptr<vector<string> > copy_ptr;
		copy_to(copy_ptr);
		(*copy_ptr)[index].assign(match);
#ifdef DEBUG_LENTIL
		oss.str("");
		oss<<"assign \""<<match.c_str()<<"\" on "<<index<<" COPY, size is "<<_host_ptr->size()<<\
			", capacity is "<<_host_ptr->capacity()<<", use count is "<< _host_ptr.use_count();
		log.debug(oss.str().c_str());
#endif
		//update _host_ptr's reference to the copy
		_host_ptr_mutex.lock();
		update_host_ptr(copy_ptr);
#ifdef DEBUG_LENTIL
		oss.str("");
		oss<<"assign \""<<match.c_str()<<"\" on "<<index<<" done, size is "<<_host_ptr->size()<<\
			", capacity is "<<_host_ptr->capacity()<<", use count is "<< _host_ptr.use_count();
		log.debug(oss.str().c_str());
#endif
		return true;//copy happened
	}
	return false;
}

bool read_guard::erase(int index)
{
	lock_guard<mutex> locker(_host_ptr_mutex);
	check_bound(index);
	if(is_exclusive())
	{
		_host_ptr->erase(_host_ptr->begin()+index);
#ifdef DEBUG_LENTIL
		oss.str("");
		oss<<"erase \""<<(*_host_ptr)[index].c_str()<<"\" on "<<index<<" directly, size is "<<_host_ptr->size()<<\
			", capacity is "<<_host_ptr->capacity()<<", use count is "<< _host_ptr.use_count();
		log.debug(oss.str().c_str());
#endif
	}
	else
	{
		//need to do changes on the copy
		lock_guard<mutex> copy_locker(_on_copy_mutex);

		//sign as a reader and copy data
		_host_ptr_mutex.unlock();
		shared_ptr<vector<string> > copy_ptr;
		copy_to(copy_ptr);
		copy_ptr->erase(copy_ptr->begin()+index);
#ifdef DEBUG_LENTIL
		oss.str("");
		oss<<"erase \""<<(*copy_ptr)[index].c_str()<<"\" on "<<index<<" COPY, size is "<<_host_ptr->size()<<\
			", capacity is "<<_host_ptr->capacity()<<", use count is "<< _host_ptr.use_count();
		log.debug(oss.str().c_str());
#endif
		//update _host_ptr's reference to the copy
		_host_ptr_mutex.lock();
		update_host_ptr(copy_ptr);
#ifdef DEBUG_LENTIL
		oss.str("");
		oss<<"erase \""<<(*copy_ptr)[index].c_str()<<"\" on "<<index<<" DONE, size is "<<_host_ptr->size()<<\
			", capacity is "<<_host_ptr->capacity()<<", use count is "<< _host_ptr.use_count();
		log.debug(oss.str().c_str());
#endif
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

