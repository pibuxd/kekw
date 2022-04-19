CC = gcc
EXE = kekw.out
SRC = $(wildcard src/*.c)
DEFINES = -D _POSIX_C_SOURCE=200809L
CFLAGS = -g -O3
kekw:
	$(CC) $(SRC) $(CFLAGS) ${DEFINES} -o $(EXE)

clean:
	rm -f *.out
