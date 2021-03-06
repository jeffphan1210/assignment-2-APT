CC = gcc
DEBUG = -DDEBUG -g
CFLAGS = -ansi -pedantic -Wall
PROG = test
OBJS = drive.o gene.o invector.o pop.o utility.o

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $^

all: $(OBJS)
	$(CC) $(CFLAGS) *.c -o $(PROG)

debug:
	$(CC) $(DEBUG) -o $(PROG) *.c

run:
	./$(PROG)	

clean:
	rm -f *.o *.h.gch $(PROG)