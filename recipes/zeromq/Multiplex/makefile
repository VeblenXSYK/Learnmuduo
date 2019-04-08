all: block multiplex
%: %.cpp
	g++ $^ -o $@ -lzmq
clean:
	@rm -rf block multiplex