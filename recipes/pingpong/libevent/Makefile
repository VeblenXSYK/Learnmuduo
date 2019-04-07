CFLAGS+=-std=c99 -g -Wall -O2
LDLIBS=-levent_core

all: client server

$(all):
	gcc $(CFLAGS) -o $@ $(LDLIBS)

clean:
	rm -f client server

.PHONY: all clean

