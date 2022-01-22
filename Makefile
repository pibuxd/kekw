CC=gcc
EXE = kekw.out
SRC = $(wildcard src/*.c)
CFLAGS = -g -O3
#OBJ = $(SRC:.c=.o)

kekw:
	$(CC) $(SRC) $(CFLAGS) -o $(EXE)

clean:
	rm *.out