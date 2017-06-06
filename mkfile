all:socket_server socket_client

socket_server:socket_server.o
	g++ socket_server.o -o socket_server

socket_server.o: socket_server.cpp
	g++ -c socket_server.cpp

socket_client:socket_client.o
	g++ socket_client.o -o socket_client

socket_client.o: socket_client.cpp
	g++ -c socket_client.cpp
	
clean:
	rm -rf *.o socket_server socket_client
