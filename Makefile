CC = gcc
EXE = kekw.out
SRC = $(wildcard src/*.c)
CFLAGS = -g -O3

kekw:
	$(CC) $(SRC) $(CFLAGS) -o $(EXE)

clean:
	rm -f *.out