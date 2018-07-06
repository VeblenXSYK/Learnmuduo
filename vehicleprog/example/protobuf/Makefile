MUDUO_DIRECTORY ?= ../../../build/release-install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
TARGET = echoserver

CXXFLAGS = -g -O0 -I$(MUDUO_INCLUDE)
LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_net -lmuduo_base -lpthread -lprotobuf
SRCS = ${wildcard *.cc}

all: $(TARGET)

clean:
	rm -f $(TARGET) commweb.pb* commweb_pb2* core

$(TARGET): $(SRCS)
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
