CXXFLAGS = -O0 -g -Wall -pthread
SRC = ${wildcard *.cpp}
OBJS = $(patsubst %.cpp,%.o,$(SRC))

libalglib.a: $(SRC)
	g++ $(CXXFLAGS) -c $^
	ar rcs $@ $(OBJS)
	
clean:
	rm -f *.o *.a core
