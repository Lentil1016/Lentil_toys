#ifndef _THREAD_MANAGER_H_
#define _THREAD_MANAGER_H_

#include<list>
#include<unistd.h>
#include<memory>

class thread_manager
{
public:
	//methods
	static thread_manager* get_instance();

	//destructor
	~thread_manager();
private:
	//constructors
	thread_manager();
	thread_manager(const thread_manager&);
	thread_manager& operator=(const thread_manager&);

	static void* listener(void*);

	//variable
	static std::auto_ptr<thread_manager> m_instance;
	int listen_sock;
	pthread_t listener_thread;

	//constants
	const int myport;
	const int queue_size;
};

#endif
