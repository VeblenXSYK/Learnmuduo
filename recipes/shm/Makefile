CXXFLAGS=-g

BINARIES=shmRead shmWrite

all: $(BINARIES)

shmRead: shmRead.cpp
shmWrite: shmWrite.cpp

$(BINARIES):
	g++ $(CXXFLAGS) $(filter %.cpp,$^) -o $@ -lpthread

clean:
	rm -f $(BINARIES)
