CXXFLAGS=-g

BINARIES=udpclient udpserver

all: $(BINARIES)

udpclient: udpclient.cpp
udpserver: udpserver.cpp

$(BINARIES):
	g++ $(CXXFLAGS) $(filter %.cpp,$^) -o $@ -lpthread

clean:
	rm -f $(BINARIES)
