#include"subthread_s.h"
#include"thread_manager.h"
#include"message_manager.h"

std::auto_ptr<thread_manager> thread_manager::m_instance(NULL);

thread_manager* thread_manager::get_instance()
{
	if(m_instance.get()==NULL)
		m_instance.reset(new thread_manager);
	return m_instance.get();
}

thread_manager::thread_manager():myport(11011),queue_size(20),\
						log(log4cpp::Category::getInstance(std::string("threadmngr")))
{
	//initialize local variables
    listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in server_addr;

	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(myport);

	//establish bind
	if(bind(listen_sock,(struct sockaddr *)&server_addr,sizeof(server_addr))==-1)
	{
		log.error("bind error");
		exit(1);
	}

	//start listen
	if(listen(listen_sock, queue_size)==-1)
	{
		log.error("lister error");
		exit(1);
	}
#ifdef DEBUG_LENTIL
	{
		std::ostringstream oss;
		oss<<"start listening on "<<myport;
		log.debug(oss.str().c_str());
	}
#endif

	pthread_create(&listener_thread, NULL, listener, NULL);
	pthread_detach(listener_thread);
	log.debug("listener thread created");
	log.debug("thread_manager init done");
}

//accept connections and initialize a pipe for message_manager
//then create new thread to manage the send&receive stuff
void* thread_manager::listener(void*)
{
	//local variables
	struct sockaddr_in client_addr;
	socklen_t lenth=sizeof(client_addr);
	int conn,listen=thread_manager::get_instance()->listen_sock;
	pthread_t manager;
	int pipe_fd[2];
	//start and keep listening
	while(1)
	{
		//establish connection
		conn = accept(listen, (struct sockaddr*)&client_addr, &lenth);
		if(conn<0)
		{
			m_instance->log.error("connect error");
			exit(1);
		}

		//create a pipe for message_manager to transfer message
		int pipe=message_manager::get_instance()->create_pipe();
		if(pipe==-1)
		{
			m_instance->log.error("pipe created failed");
			exit(1);
		}
		//create a thread to manage the socket
		int conn_pipe[]={conn, pipe};
		if(pthread_create(&manager, NULL, sub_thread::rs_manager, conn_pipe) != 0)
		{
			m_instance->log.error("pthread_create error");
			exit(1);
		}
		//detach the thread
		pthread_detach(manager);
		m_instance->log.notice("a new connection set up");
	}
	return 0;
}

thread_manager::~thread_manager()
{
	close(listen_sock);
	pthread_cancel(listener_thread);
}
