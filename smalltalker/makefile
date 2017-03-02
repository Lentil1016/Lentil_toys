all: client server

server: server.cpp thread.h  socket
	g++ -o server server.cpp -lpthread

client: client.cpp thread.h socket
	g++ -o client client.cpp -lpthread
