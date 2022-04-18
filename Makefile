CC = gcc
EXE = kekw.out
SRC = $(wildcard src/*.c)
DEFINES = _POSIX_C_SOURCE=200809L -D intptr_t=long
CFLAGS = -g -O3
kekw:
	$(CC) $(SRC) $(CFLAGS) ${DEFINES} -o $(EXE)

clean:
	rm -f *.out
