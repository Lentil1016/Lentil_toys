#include"message_manager.h"
#include<iostream>
#include<string.h>

std::auto_ptr<message_manager> message_manager::m_instance(new message_manager);

message_manager::message_manager():buffer_lenth(100)
{
	pthread_mutex_init(&pipe_list_mutex, NULL);
	pthread_mutex_init(&main_run_flag, NULL);
	pthread_mutex_lock(&main_run_flag);
	pthread_rwlock_init(&subthread_run_flag,NULL);
	pthread_create(&stdin_watcher, NULL, watcher_thread, NULL);
	pthread_detach(stdin_watcher);
}

message_manager* message_manager::get_instance()
{
	return m_instance.get();
}

int message_manager::create_pipe()
{
	int pipe_fd[2];
	if(pipe(pipe_fd)<0)
	{
		std::cout<<"[pipe create failed]"<<std::endl;
		exit(0);
	}
	add_pipe_tolist(pipe_fd[1]);
	return pipe_fd[0];
}

int message_manager::add_pipe_tolist(int pipefd)
{
	pthread_mutex_lock(&pipe_list_mutex);
	pipe_list.push_back(pipefd);
	pthread_mutex_unlock(&pipe_list_mutex);
	return 0;
}

int message_manager::broadcast(const char* buffer)
{
	pthread_mutex_lock(&pipe_list_mutex);
	for(std::list<int>::iterator i=pipe_list.begin(); i!=pipe_list.end();i++)
		write(*i,buffer,buffer_lenth);
	pthread_mutex_unlock(&pipe_list_mutex);
	return 0;
}

void* message_manager::watcher_thread(void* a)
{
	char buffer[message_manager::get_instance()->buffer_lenth];
	while(1)
	{
		std::cin>>buffer;
		message_manager::get_instance()->broadcast(buffer);
		if(strcmp(buffer,"exit")==0)
		{
			pthread_rwlock_wrlock(&message_manager::get_instance()->subthread_run_flag);
			std::cout<<"[connections all down, quiting from main thread]"<<std::endl;
			pthread_mutex_unlock(&message_manager::get_instance()->main_run_flag);
		}
	}
}

pthread_rwlock_t* message_manager::get_subthread_lock()
{
	return &subthread_run_flag;
}

int message_manager::return_till_exit()
{
	pthread_mutex_lock(&main_run_flag);
}

message_manager::~message_manager()
{
	pthread_rwlock_destroy(&subthread_run_flag);
	pthread_mutex_destroy(&main_run_flag);
	for(std::list<int>::iterator i=pipe_list.begin(); i!=pipe_list.end();i++)
		close(*i);
	pthread_mutex_destroy(&pipe_list_mutex);
}
