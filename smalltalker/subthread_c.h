#include<pthread.h>
#include<semaphore.h>

#include<stdio.h>
#include<iostream>
#include<string.h>
#include<stdlib.h>

#include"socket"

const size_t buffer_size	=1024;

struct thread_struct
{
	int conn;
	sem_t sem;
	int flag;
};

void* sub_sender(void* a)
{
	//get init data
	thread_struct* asyner = (thread_struct*)a;

	//get sem
	sem_wait(&(asyner->sem));
	std::cout<< "sender get sem"<<std::endl;

	//init buffer
	char buffer[buffer_size];

	while(1)
	{
		//read buffer from stdin
		std::cin>>buffer;
		if(send(asyner->conn, buffer, strlen(buffer), 0) <= 0)
		{
			std::cout<<"[sending failed]"<<std::endl;
			//set reconnecting flag
			asyner->flag=-1;
			//release sem
			sem_post(&asyner->sem);
			break;
		}
		else if (strcmp(buffer,"exit")==0)
		{
			//on condition of exit from this side
			std::cout<<"[Get exit signal, client post the sem, quitting]"<<std::endl;
			sem_post(&asyner->sem);
			break;
		}
		else
		{
			//normal condition
			std::cout<<"[sent out message]"<<std::endl;
			//clear up the buffer
			memset(buffer,0,strlen(buffer));
		}
	}
	return NULL;
}

void* sub_receiver(void* a)
{
	//get init data
	thread_struct* asyner = (thread_struct*)a;

	//get sem
	sem_wait(&(asyner->sem));
	std::cout << "receiver get sem"<<std::endl;

	//init buffer
	char buffer[buffer_size];

	while(1)
	{
		//recv from client
		if((recv(asyner->conn, buffer, buffer_size,0) <= 0)||(strcmp(buffer,"exit")==0))
		{
			//on condition of other side quite or receive failed
			std::cout<<"[The connection of socket "<< asyner->conn <<" has down]"<<std::endl;
			//set reconnecting flag
			asyner->flag=-1;
			sem_post(&asyner->sem);
			break;
		}
		else
		{
			//on condition of received a normal message
			std::cout<<buffer<<"\n[receive a message]"<<std::endl;
			//clear up the buffer
			memset(buffer,0,strlen(buffer));
		}
	}
	return NULL;
}

int help()
{
	std::cout<<"=============help=============\n"\
		<<"x		--kill this side\n"
		<<"exit		--kill both side\n"
		<<"others	--send normal message\n"
		<<"==============================\n";
	return 0;
}

int start_cs(int conn,int flag)
{
	help();

	//create asyner
	struct thread_struct asyner;
	asyner.conn=conn;
	asyner.flag=flag;
	//std::cout<<"conn = "<<asyner.conn<<std::endl;

	//init sem
	sem_init(&asyner.sem, 0, 2);

	//create thread
	pthread_t sender,receiver;
	int ret;

	//int pthread_create(
	//		pthread_t *thread,					//thread id
	//		const pthread_attr_t *attr,			//thread attribute
	//		void *(*start_routine)(void*),		//thread entrance
	//		void *arg)							//thread parameter

	ret= pthread_create(&sender, NULL, sub_sender, &asyner);
	if(ret != 0)
	{
		perror("pthread_create error");
		exit(1);
	}
	ret= pthread_create(&receiver, NULL, sub_receiver, &asyner);
	if(ret != 0)
	{
		perror("pthread_create error");
		exit(1);
	}

	//detach threads
	pthread_detach(sender);
	pthread_detach(receiver);

	sleep(1);

	//std::cout<<"thread init done"<<std::endl;
	sem_wait(&asyner.sem);

	//get sem, time to destroy threads
	sem_destroy(&asyner.sem);
	std::cout<<"[killing threads]"<<std::endl;
	pthread_cancel(sender);
	pthread_cancel(receiver);
	return asyner.flag;
}
