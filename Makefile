CC=gcc
EXE = kekw.out
SRC = $(wildcard src/*.c)
#OBJ = $(SRC:.c=.o)
CFLAGS = -g -Wall

kekw:
	$(CC) $(SRC) $(CFLAGSEXE) -o $(EXE)

clean:
	rm *.out