CXXFLAGS=-g

BINARIES=shmRead shmRead_Posix shmWrite shmWrite_Posix

all: $(BINARIES)

shmRead: shmRead.cpp
shmRead_Posix: shmRead_Posix.cpp
shmWrite: shmWrite.cpp
shmWrite_Posix: shmWrite_Posix.cpp

$(BINARIES):
	g++ $(CXXFLAGS) $(filter %.cpp,$^) -o $@ -lpthread -lrt

clean:
	rm -f $(BINARIES)
