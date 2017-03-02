#include "thread.h"

const int myport		=11010;
const int queue_size	=20;

int main(int argc, char* argv[])
{
	//define sockfd
	//protofamily	:AF_INET represent IPV4
	//type			:SOCK_STREAM represent Stream_socket
	//Protocal		:IPPROTO_TCP represent TCP
	//int socket(int protofamily, int type, int protocol);

	int server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int conn;
	//define sockaddr_in
	//
	//struct sockaddr_in{
	//sa_family_t		sin_family;	//address family: AF_INET
	//in_port_t			sin_port;	//port in network byte order
	//struct in_addr	sin_addr;	//internet address
	//}
	//
	//struct sockaddr_in6{
	//uint32_t			s_addr;		//address in network byte order
	//}

	struct sockaddr_in sock_server;
	sock_server.sin_addr.s_addr=htonl(INADDR_ANY);
	sock_server.sin_family=AF_INET;
	sock_server.sin_port=htons(myport);

	//create socket struct
	struct sockaddr_in client_addr;
	socklen_t lenth=sizeof(client_addr);

	//establish bind
	//
	if(bind(server_sockfd,(struct sockaddr *)&sock_server,sizeof(sock_server))==-1)
	{
		perror("bind error");
		exit(1);
	}

	do{
		//start listen
		//int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		if(listen(server_sockfd, queue_size)==-1)
		{
			perror("lister error");
			exit(1);
		}

		conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &lenth);
		if(conn<0)
		{
			perror("connect error");
			exit(1);
		}
	}while(start_cs(conn,0)==-1);//launch client and server

	close(conn);
	close(server_sockfd);
	return 0;
}


