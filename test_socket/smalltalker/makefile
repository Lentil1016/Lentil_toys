CC=g++
LD_FLAG= -lpthread -llog4cpp -DDEBUG_LENTIL

all: client server

server: server.cpp message_manager.o thread_manager.o subthread_s.o
	g++ -g -o $@ $^ ${LD_FLAG}

client: client.cpp subthread_c.h socket
	g++ -g -o client client.cpp -lpthread

thread_manager.o: thread_manager.cpp
	g++ -c -g -o $@ $^ ${LD_FLAG}

message_manager.o: message_manager.cpp
	g++ -c -g -o $@ $^ ${LD_FLAG}

subthread_s.o: subthread_s.cpp
	g++ -c -g -o $@ $^ ${LD_FLAG}
