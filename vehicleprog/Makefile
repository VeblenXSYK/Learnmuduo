MUDUO_DIRECTORY ?= ../build/release-install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
ALGLIB = ./alglib

#源文件
SRCS = ${wildcard *.cc codec/*.cc}

#目标文件
TARGET = VehileProg
OBJS = $(SRCS:.cc=.o)

#编译链接选项
CXXFLAGS = -g -O0 -I$(MUDUO_INCLUDE) -I$(ALGLIB) -I./codec
LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_net -lmuduo_base -L$(ALGLIB) -lalglib -lconfig++ -lprotobuf -lrt -lpthread

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ -o $@ $^ $(LDFLAGS)
	
%.o: %.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $< $(INCLUDES)
	
clean:
	rm -f $(TARGET) $(OBJS)

#避免目录中出现同名的all或者clean文件
.PHONY: all clean
