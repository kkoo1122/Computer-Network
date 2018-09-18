all: total
	@echo "Build Complete!!"

total: server.o client.o 
	g++ server.o -o server
	g++ client.o -o client
server.o: server.cpp 
	g++ -c server.cpp
client.o: client.cpp 
	g++ -c client.cpp
clean: 
	rm *.o output
