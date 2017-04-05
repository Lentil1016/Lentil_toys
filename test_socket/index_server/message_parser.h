#ifndef _MESSAGE_PARSER_H_
#define _MESSAGE_PARSER_H_
#include<sstream>
#include<pthread.h>

struct parser_struct
{
	std::stringstream* ss;
	pthread_mutex_t* ss_mutex;
	int runflag;
};


class message_parser
{
public:
	message_parser();
	~message_parser();
	message_parser& operator<<(const char*);
private:
	message_parser(const message_parser&);
	std::stringstream ss;
	pthread_mutex_t ss_mutex;
	pthread_t parser;
	parser_struct _struct;

	static void* parsing(void*);
};

#endif
