all: ventilator worker sink
%: %.cpp
	g++ $^ -o $@ -lzmq
clean:
	@rm -rf ventilator worker sink