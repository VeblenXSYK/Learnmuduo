all: client server server_multithread
%: %.cpp
	g++ $^ -o $@ -lzmq -lpthread
clean:
	@rm -rf client server server_multithread