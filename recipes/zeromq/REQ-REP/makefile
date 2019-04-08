all: client server
%: %.cpp
	g++ $^ -o $@ -lzmq
clean:
	@rm -rf client server