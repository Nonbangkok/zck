CC = gcc
CFLAGS = -Wall -g
OBJ = main.o executor.o builtin.o parser.o

myshell: $(OBJ)
	$(CC) $(CFLAGS) -o zck $(OBJ)

%.o: %.c shell.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o zck
