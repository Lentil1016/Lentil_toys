#include"message_manager.h"
#include"thread_manager.h"

int main(int argc, char* argv[])
{
	pthread_mutex_t run_flag;
	pthread_mutex_init(&run_flag, NULL);
	//TODO:finish the exit of main

	message_manager* mmp=message_manager::get_instance();
	thread_manager* tmp=thread_manager::get_instance();
	mmp->return_till_exit();
}
