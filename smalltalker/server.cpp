#include "thread.h"

const int myport		=11010;
const int queue_size	=20;

void* socket_manager(void* conn)
{
	int sock  = *(int*)conn;
	start_cs(sock,0);
	close(sock);
	return NULL;
}

int main(int argc, char* argv[])
{
	int server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in sock_server;
	sock_server.sin_addr.s_addr=htonl(INADDR_ANY);
	sock_server.sin_family=AF_INET;
	sock_server.sin_port=htons(myport);

	//create socket struct
	struct sockaddr_in client_addr;
	socklen_t lenth=sizeof(client_addr);

	//establish bind
	if(bind(server_sockfd,(struct sockaddr *)&sock_server,sizeof(sock_server))==-1)
	{
		perror("bind error");
		exit(1);
	}

	//start listen
	if(listen(server_sockfd, queue_size)==-1)
	{
		std::cout<<"[listen get -1]"<<std::endl;
		perror("lister error");
		exit(1);
	}

	int conn;
	pthread_t manager;

	while(1)
	{
		//establish connection
		conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &lenth);
		if(conn<0)
		{
			std::cout<<"[accept get -1]"<<std::endl;
			perror("connect error");
			exit(1);
		}

		//create a thread to manage the socket
		if(pthread_create(&manager, NULL, socket_manager, &conn) != 0)
		{
			perror("pthread_create error");
			exit(1);
		}

		//detach the thread
		pthread_detach(manager);
	}
	close(server_sockfd);
	return 0;
}

