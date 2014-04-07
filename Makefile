CC  = gcc

CFLAGS   = -g -Wall
LDFLAGS = -g 

.PHONY: default
default: lsc

lsc:

.PHONY: clean
clean:
	rm -f *.o *~ *.swp *.swo a.out core lsc

.PHONY: all
all: clean lsc
