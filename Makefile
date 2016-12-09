CC=gcc
CFLAGS=-Wall
DEPS = user.h agence.h
OBJ = user.o agence.o 

all: user

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

user: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
