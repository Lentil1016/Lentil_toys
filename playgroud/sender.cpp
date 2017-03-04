#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
#include<pthread.h>

void* sub_func(void* pipeid)
{
	char buffer[100];
	int a=*(int*)pipeid;
	while(1)
	{
		read(a,buffer,100);
		std::cout<<buffer<<std::endl;
	}
}

int main(int argc, char* argv[])
{
	int pipe_fd[2];
	pid_t pid;
	char buf_r[100];
	char*p_wbuf;
	int r_num;

	memset(buf_r, 0, sizeof(buf_r));

	if(pipe(pipe_fd)<0)
	{
		std::cout<<"[pipe create error]\n";
		return -1;
	}

	pthread_t sub_thread1,sub_thread2;
	pthread_create(&sub_thread1, NULL, sub_func, &pipe_fd[0]);
	pthread_create(&sub_thread2, NULL, sub_func, &pipe_fd[0]);
	pthread_detach(sub_thread1);
	pthread_detach(sub_thread2);

	char buffer[100];
	while(1)
	{
		std::cin>>buffer;
		write(pipe_fd[1],buffer,100);
	}

}



