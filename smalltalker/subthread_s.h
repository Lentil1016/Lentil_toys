#ifndef _SUBTHREAD_S_H_
#define _SUBTHREAD_S_H_

#include<pthread.h>
#include<semaphore.h>

#include<stdio.h>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<memory>

#include"socket"

#include<log4cpp/Category.hh>

struct thread_struct
{
	int conn;
	int pipe;
	sem_t sem;
};

class sub_thread
{
public:
	static sub_thread* get_instance();
	static void* rs_manager(void* conn_pipe);
private:
	//constructor
	sub_thread();
	sub_thread(const sub_thread&);
	sub_thread& operator=(const sub_thread&);

	//variable
	static std::auto_ptr<sub_thread> m_instance;
	const size_t buffer_size;
	sub_thread* m_sub_thread;
	log4cpp::Category& log;

	//method
	static void* sub_sender(void* a);
	static void* sub_receiver(void* a);
	static int help();
};

#endif
