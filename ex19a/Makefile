EXECUTABLE=ex19a
CFLAGS=-Wall -g

all: ex19a

ex19a: ../bin/object.o
	cc $(CFLAGS) ../bin/object.o $(EXECUTABLE).c -o ../bin/$(EXECUTABLE)

../bin/object.o: object.c
	cc $(CFLAGS) -c object.c -o ../bin/object.o

clean:
	rm -f ../bin/$(EXECUTABLE)
	rm -rf ../bin/$(EXECUTABLE).dSYM
	rm -f ../bin/object.o
