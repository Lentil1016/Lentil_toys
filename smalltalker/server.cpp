#include"message_manager.h"
#include"thread_manager.h"
#include"subthread_s.h"
#include<log4cpp/Category.hh>
#include<log4cpp/PropertyConfigurator.hh>

int main(int argc, char* argv[])
{
	try{
		log4cpp::PropertyConfigurator::configure("./log.conf");
	}catch(log4cpp::ConfigureFailure& f){
		std::cout<<"Configure Problem "<<f.what()<<std::endl;
	}

	log4cpp::Category& log = log4cpp::Category::getInstance(std::string("server"));

	log.notice("log4cpp init done");

	thread_manager* tmp=thread_manager::get_instance();
	message_manager* mmp=message_manager::get_instance();
	sub_thread* stp=sub_thread::get_instance();

	log.debug("server is blocked now");
	mmp->return_till_exit();
	log.debug("server is going to quit now");
}
