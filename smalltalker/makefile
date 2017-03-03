all: client server

server: server.cpp thread.h  socket
	g++ -g -o server server.cpp -lpthread

client: client.cpp thread.h socket
	g++ -g -o client client.cpp -lpthread
