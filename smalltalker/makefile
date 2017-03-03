CC=g++


all: client server

server: server.cpp message_manager.o thread_manager.o subthread_s.o
	g++ -g -o $@ $^ -lpthread

client: client.cpp subthread_c.h socket
	g++ -g -o client client.cpp -lpthread

thread_manager.o: thread_manager.cpp
	g++ -c -g -o $@ $^ -lpthread

message_manager.o: message_manager.cpp
	g++ -c -g -o $@ $^ -lpthread

subthread_s.o: subthread_s.cpp
	g++ -c -g -o $@ $^ -lpthread
