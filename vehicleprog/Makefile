MUDUO_DIRECTORY ?= ../build/release-install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
TARGET = VehileProg

CXXFLAGS = -g -O0 -I$(MUDUO_INCLUDE)
LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_net -lmuduo_base -lpthread -lrt
SRCS = ${wildcard *.cc}

all: $(TARGET)
clean:
	rm -f $(TARGET) core

$(TARGET): $(SRCS)
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean