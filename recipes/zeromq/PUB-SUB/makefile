all: publisher subscriber
%: %.cpp
	g++ $^ -o $@ -lzmq
clean:
	@rm -rf publisher subscriber