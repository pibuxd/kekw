CC = gcc
EXE = kekw
SRC = $(wildcard src/*.c)
CFLAGS = -g
#OBJ = $(SRC:.c=.o)

kekw:
	$(CC) $(SRC) $(CFLAGS) -o $(EXE)

clean:
	rm *.out