//#include "index_subthread_c.h"

#include"socket"
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<sstream>

const int myport		=11011;
const int queue_size	=20;
const int max_retry		=5;


void* push(void* a)
{
	int conn = *(int*)a, i=0;
	std::ostringstream oss;
	while(i<110)
	{
		oss.str("");
		oss<<"push_back  "<<rand();
		send(conn,oss.str().c_str(),oss.str().size(),0);
		std::cout<<oss.str()<<std::endl;
		i++;
		if(i>10)
			sleep(3);
		oss.str("");
		oss<<"push_back  "<<rand();
		send(conn,oss.str().c_str(),oss.str().size(),0);
		std::cout<<oss.str()<<std::endl;
		i++;
		if(i>10)
			sleep(17);
	}
}

void* assign(void* a)
{
	int conn = *(int*)a, i;
	std::ostringstream oss;
	while(i<100)
	{
		oss.str("");
		oss<<"assign "<<rand()%20<<" "<<rand();
		send(conn, oss.str().c_str(),oss.str().size(),0);
		std::cout<<oss.str()<<std::endl;
		i++;
		sleep(10);
	}
}

void* query(void* a)
{
	int conn = *(int*)a, i;
	std::ostringstream oss;
	while(1)
	{
		oss.str("");
		oss<<"query "<<rand()%100;
		send(conn, oss.str().c_str(),oss.str().size(),0);
		std::cout<<oss.str()<<std::endl;
		i++;
		sleep(1);
	}
}


int main()
{
	int retry=0, sock_client_fd = socket(AF_INET, SOCK_STREAM, 0);

	//define server socket
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(0));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(myport);
	server_addr.sin_addr.s_addr = inet_addr("10.130.73.37");    // 服务器ip

	connect(sock_client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	pthread_t pusher, assigner, querier;//, eraser, querier, verifier, finder;
	pthread_create(&pusher, NULL, push, &sock_client_fd);
	pthread_create(&assigner, NULL, assign, &sock_client_fd);
	pthread_create(&querier, NULL, query, &sock_client_fd);

	pthread_detach(pusher);
	pthread_detach(assigner);
	pthread_detach(querier);


	std::cin.get();
	std::cin.get();
	return 0;
}


