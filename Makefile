CC = g++
CCFLAGS = -ansi -Wall -Wno-deprecated -O2 -DNDEBUG
LDFLAGS =

HEADERS = Syllables.hh
SOURCES = Syllables.cc

all: testme

testme:	$(HEADERS) $(SOURCES) testme.cc
	$(CC) $(CCFLAGS) $(LDFLAGS) -o testme $(SOURCES) testme.cc

clean:
	rm -f testme
