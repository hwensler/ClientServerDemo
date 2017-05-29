all: server client

server: Server.cpp
	g++ Server.cpp -std=c++11 -lpthread -o Server

client: Client.cpp
	g++ Client.cpp -std=c++11 -lpthread -o Client
