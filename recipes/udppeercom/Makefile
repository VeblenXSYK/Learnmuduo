CXXFLAGS=-g

BINARIES=peercomA peercomB

all: $(BINARIES)

peercomA: peercomA.cpp
peercomB: peercomB.cpp

$(BINARIES):
	gcc $(CXXFLAGS) $(filter %.cpp,$^) -o $@

clean:
	rm -f $(BINARIES)
