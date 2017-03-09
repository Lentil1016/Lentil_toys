CC=g++
LD_FLAG= -lpthread -llog4cpp -DDEBUG_LENTIL -DINDEX_SERVER

all: client server index_client

index_client: index_client.cpp
	g++ -g -o $@ $^ ${LD_FLAG}

server: server.cpp message_manager.o thread_manager.o subthread_s.o read_guard.o
	g++ -g -o $@ $^ ${LD_FLAG}

client: client.cpp subthread_c.h socket
	g++ -g -o client client.cpp -lpthread

thread_manager.o: thread_manager.cpp
	g++ -c -g -o $@ $^ ${LD_FLAG}

message_manager.o: message_manager.cpp
	g++ -c -g -o $@ $^ ${LD_FLAG}

subthread_s.o: subthread_s.cpp
	g++ -c -g -o $@ $^ ${LD_FLAG}

read_guard.o: read_guard.cpp
	g++ -c -g -o $@ $^ ${LD_FLAG} 
