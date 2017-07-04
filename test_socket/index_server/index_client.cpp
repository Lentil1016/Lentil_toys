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
	while(i<120)
	{
		oss.str("");
		oss<<"push_back  "<<rand()/19910<<"\n";
		send(conn,oss.str().c_str(),oss.str().size(),0);
		std::cout<<oss.str()<<std::endl;
		i++;
		oss.str("");
		oss<<"push_back  "<<rand()<<"\n";
		send(conn,oss.str().c_str(),oss.str().size(),0);
		std::cout<<oss.str()<<std::endl;
		i++;
	}
}

void* assign(void* a)
{
	sleep(10);
	int conn = *(int*)a, i;
	std::ostringstream oss;
	while(1)
	{
		oss.str("");
		oss<<"assign "<<rand()%300<<" "<<rand()<<"\n";
		send(conn, oss.str().c_str(),oss.str().size(),0);
		std::cout<<oss.str()<<std::endl;
		sleep(1);
	}
}

void* query(void* a)
{
	sleep(10);
	int conn = *(int*)a, i;
	std::ostringstream oss;
	while(1)
	{
		oss.str("");
		oss<<"query "<<rand()%100<<" \n";
		send(conn, oss.str().c_str(),oss.str().size(),0);
		std::cout<<oss.str()<<std::endl;
		i++;
		usleep(1000);
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
	server_addr.sin_addr.s_addr = inet_addr("10.130.73.37");	// 服务器ip

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

