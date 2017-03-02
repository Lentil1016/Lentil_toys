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

void* sub_client(void* a)
{
	//get init data
	thread_struct* asyner = (thread_struct*)a;

	//get sem
	sem_wait(&(asyner->sem));
	std::cout<< "client get sem"<<std::endl;

	//init buffer
	char buffer[buffer_size];

	while(1)
	{
		//read buffer from stdin
		std::cin>>buffer;

		if (strcmp(buffer,"x")==0){

			//on condition of kill client
			if(asyner->flag==0)
				std::cout<<"[Deny to quit, this is the original server]"<<std::endl;
			else{
				send(asyner->conn, buffer, strlen(buffer), 0);
				std::cout<<"[client post the sem, quitting from this side]"<<std::endl;
				sem_post(&asyner->sem);
			}

		}else if (strcmp(buffer,"exit")==0){

			//on condition of exit from both side
			send(asyner->conn, buffer, strlen(buffer), 0);
			std::cout<<"[client post the sem, quitting from both side]"<<std::endl;
			sem_post(&asyner->sem);

		}else {

			send(asyner->conn, buffer, strlen(buffer), 0);
			//normal condition
			std::cout<<"[sent out message]"<<std::endl;
			//clear up the buffer
			memset(buffer,0,strlen(buffer));
		}
	}
	return NULL;
}

void* sub_server(void* a)
{
	//get init data
	thread_struct* asyner = (thread_struct*)a;

	//get sem
	sem_wait(&(asyner->sem));
	std::cout << "server get sem"<<std::endl;

	//init buffer
	char buffer[buffer_size];

	while(1)
	{
		//recv from client
		recv(asyner->conn, buffer, buffer_size,0);

		if (strcmp(buffer,"x")==0){

			//on condition of losing connection
			std::cout<<"[losing connection, return to listen state]"<<std::endl;
			asyner->flag=-1;
			sem_post(&asyner->sem);

		}else if(strcmp(buffer,"exit")==0){

			//on condition of both side exit
			std::cout<<"[server post the sem, quitting from both side]"<<std::endl;
			sem_post(&asyner->sem);

		}else{

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
	pthread_t server,client;
	int ret;

	//int pthread_create(
	//		pthread_t *thread,					//thread id
	//		const pthread_attr_t *attr,			//thread attribute
	//		void *(*start_routine)(void*),		//thread entrance
	//		void *arg)							//thread parameter

	ret= pthread_create(&server, NULL, sub_server, &asyner);
	if(ret != 0)
	{
		perror("pthread_create");
		exit(1);
	}
	ret= pthread_create(&client, NULL, sub_client, &asyner);
	if(ret != 0)
	{
		perror("pthread_create");
		exit(1);
	}

	//detach threads
	pthread_detach(server);
	pthread_detach(client);

	sleep(1);

	//std::cout<<"thread init done"<<std::endl;
	sem_wait(&asyner.sem);

	//get sem, time to destroy threads
	sem_destroy(&asyner.sem);
	pthread_cancel(server);
	pthread_cancel(client);

	return asyner.flag;
}
