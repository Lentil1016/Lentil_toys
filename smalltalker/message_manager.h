#ifndef _MESSAGE_MANAGER_H_
#define _MESSAGE_MANAGER_H_

#include<pthread.h>
#include<unistd.h>
#include<list>
#include<stdlib.h>
#include<memory>
#include<log4cpp/Category.hh>

class message_manager
{
public:
	~message_manager();
	static message_manager* get_instance();//done

	int create_pipe();//done, will lock the mutex
	const int buffer_lenth;
	pthread_rwlock_t* get_subthread_lock();
	int return_till_exit();//done
private:
	//constructor
	message_manager();//done
	message_manager(const message_manager&);
	message_manager& operator = (const message_manager&);

	int broadcast(const char* buffer);//done, will lock the mutex
	int add_pipe_tolist(int pipe_fd);//done
	static void* watcher_thread(void*);//done

	//variable
	static std::auto_ptr<message_manager> m_instance;
	std::list<int> pipe_list;
	pthread_mutex_t pipe_list_mutex,main_run_flag;
	pthread_t stdin_watcher;
	pthread_rwlock_t subthread_run_flag;
	log4cpp::Category& log;
};


#endif
