MUDUO_DIRECTORY ?= ../build/release-install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
ALGLIB = ./alglib
TARGET = VehileProg

CXXFLAGS = -g -O0 -I$(MUDUO_INCLUDE) -I$(ALGLIB)
LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_net -lmuduo_base -L$(ALGLIB) -lalglib -lpthread -lrt -lconfig++
SRCS = ${wildcard *.cc}

all: $(TARGET)
clean:
	rm -f $(TARGET) core

$(TARGET): $(SRCS)
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
