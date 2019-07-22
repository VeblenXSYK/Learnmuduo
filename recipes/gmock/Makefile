CXXFLAGS=-g

BINARIES=test

all: $(BINARIES)

test: test.cpp helloworld.cpp

$(BINARIES):
	g++ $(CXXFLAGS) $(filter %.cpp,$^) -o $@ -lgtest -lgmock -lpthread 

clean:
	rm -f $(BINARIES)
