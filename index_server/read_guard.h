#ifndef _READGUARD_H_
#define _READGUARD_H_

#include<boost/shared_ptr.hpp>
#include<boost/thread.hpp>
#include<string>
#include<vector>
#include<log4cpp/Category.hh>

#ifdef LENTIL_DEBUG
#include<sstream>
#endif

class read_guard
{
public:
	read_guard();
	~read_guard();

	//read only methods
	bool verify(int, const std::string&);
	int find(const std::string&);
	std::string query(int);

	//write methods
	bool push_back(const std::string&);
	//the following methods possibly need to copy on write
	bool assign(int, const std::string&);
	bool erase(int);

private:
	read_guard(const read_guard&);
	read_guard& operator= (const read_guard&);

	//variables
	boost::shared_ptr<std::vector<std::string> >		_host_ptr;
	boost::mutex										_host_ptr_mutex;
	boost::mutex										_on_copy_mutex;
	log4cpp::Category& log;
#ifdef DEBUG_LENTIL
	//debug
	std::ostringstream oss;
#endif

	//reader methods, inside auto lock
	inline void reader_sign(boost::shared_ptr<std::vector<std::string> >&);

	//writer methods, outside manual locker
	inline bool is_not_full();
	inline bool is_exclusive();
	inline void copy_to(boost::shared_ptr<std::vector<std::string> >&);
	inline void update_host_ptr(const boost::shared_ptr<std::vector<std::string> >&);
	inline void copy_from_host(boost::shared_ptr<std::vector<std::string> >&);

#ifdef DEBUG_LENTIL
	int query_times;
	int copy_times;
	int push_back_times;
#endif
	//other methods, no need to lock
	inline bool check_bound(int);
};

#endif
