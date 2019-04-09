all: client server delear
%: %.cpp
	g++ $^ -o $@ -lzmq
clean:
	@rm -rf client server delear