#include "subthread_c.h"

const int myport		=11011;
const int queue_size	=20;
const int max_retry		=5;

int main()
{
	int retry=0, sock_client_fd = socket(AF_INET, SOCK_STREAM, 0);

	//define server socket
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(0));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(myport);
	server_addr.sin_addr.s_addr = inet_addr("10.130.38.37");    // 服务器ip

	//set up connection
	while (retry<max_retry && connect(sock_client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cout<<"[connect fail, retry in 3 seconds]"<<std::endl;
		sleep(3);
		retry++;
	}
	if (retry==max_retry) //on condition of timeout
		std::cout<<"[timeout, cannot connect with server]"<<std::endl;
	else
	{
		//on condition of connection established
		retry=0;

		while(start_cs(sock_client_fd,1)==-1)//launch client and server
		{
			//on condition of requiring a connecting retry
			close(sock_client_fd);//fully close the old connection.
			sock_client_fd = socket(AF_INET, SOCK_STREAM, 0);//and create a new one

			//restarting connection
			while (retry<max_retry&&connect(sock_client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
			{
				//start reconnection
				retry++;
				std::cout<<"[connection is down, retry in 3 seconds]"<<std::endl;
				sleep(3);
			}
			if(retry=max_retry)//on condition of retry timeout
			{
				std::cout<<"[timeout, cannot reconnect with server]"<<std::endl;
				break;
			}
		}
	}
	close(sock_client_fd);
	return 0;
}


