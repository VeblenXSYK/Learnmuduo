all: local_lat remote_lat
%: %.cpp
	g++ $^ -o $@ -lzmq
clean:
	@rm -rf local_lat remote_lat