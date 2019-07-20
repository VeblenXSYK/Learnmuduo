CXXFLAGS=-g -I/usr/local/include/hiredis/

BINARIES=example

all: $(BINARIES)

example: example.c

$(BINARIES):
	gcc $(CXXFLAGS) $(filter %.c,$^) -o $@ -lhiredis

clean:
	rm -f $(BINARIES)
