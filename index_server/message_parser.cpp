#include"message_parser.h"
#include<string.h>
#include"read_guard.h"

extern read_guard guard;

message_parser::message_parser()
{
	pthread_mutex_init(&ss_mutex, NULL);
	_struct.ss = &ss;
	_struct.ss_mutex = &ss_mutex;
	pthread_create(&parser, NULL, parsing,&_struct);
	pthread_detach(parser);
}

message_parser& message_parser::operator<< (const char* buffer)
{
	pthread_mutex_lock(&ss_mutex);
	if(ss.tellg()>10000)
		ss.seekg(0);
	if(ss.tellp()>10000)
		ss.seekp(0);
	ss<<buffer;
	pthread_mutex_unlock(&ss_mutex);
}

message_parser::message_parser(const message_parser&)
{

}

message_parser::~message_parser()
{
	pthread_cancel(parser);
	pthread_mutex_destroy(&ss_mutex);
}

void* message_parser::parsing(void* a)
{
	parser_struct& _struct = *(parser_struct*)a;
	char buffer[1024];
	int index=0;
	while(1)
	{
		while(_struct.ss->tellg()<_struct.ss->tellp())
		{
			index = 0;
			_struct.ss->getline(buffer, 1024);
			//std::cout<<buffer<<"\n"<<_struct.ss->gcount()<<" "<<_struct.ss->tellg()<<" "<<\
			//	_struct.ss->tellp()<<" "<<_struct.ss->str().capacity()<<std::endl;
			char* blank1 = strchr(buffer,' '), *blank2 = strrchr(buffer, ' ');
			if (blank1==blank2)
			{
				std::cout<<"blank1 == blank2"<<std::endl;
				break;
			}
			std::string order(buffer, blank1-buffer);
			std::string match(blank2+1 ,buffer + strlen(buffer) - blank2);
			for(blank1++; blank1!=blank2; blank1++)
			{
				index = index * 10 + *blank1-48;
			}
			std::cout<<"["<<order<<" "<<index<<" "<<match<<"]"<<std::endl;
			try
			{
				if(order == "verify")
				{
					guard.verify(1,match);
				}
				else if(order == "query")
				{
					guard.query(index);
				}
				else if(order == "find")
				{
					guard.find(match);
				}
				else if(order == "push_back")
				{
					guard.push_back(match);
				}
				else if(order == "assign")
				{
					guard.assign(index, match);
				}
				else if(order == "erase")
				{
					guard.erase(index);
				}
			}
			catch(char const* e)
			{
				std::cout<<e<<std::endl;
			}
		}
	}
}
