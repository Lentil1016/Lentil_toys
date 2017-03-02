#include "thread.h"

const int myport		=11010;
const int queue_size	=20;

int main()
{
	int sock_client_fd = socket(AF_INET, SOCK_STREAM, 0);

	//define server socket
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(0));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(myport);
	server_addr.sin_addr.s_addr = inet_addr("10.130.38.37");    // 服务器ip

	//set up connection
	if (connect(sock_client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("connect");
		exit(1);
	}

	//launch client and server
	start_cs(sock_client_fd,1);

	close(sock_client_fd);
	return 0;
}
