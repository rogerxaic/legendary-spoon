CC=gcc
CFLAGS=-Wall
DEPS = user_agence.h mqueue.h semaphore.h shmem.h
OBJ = user.o agence.o 

all: 
	@echo "make user ou make agence ou make [...]"

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

agence: agence.o $(DEPS)
	$(CC) -o $@ $< $(CFLAGS)

users: users.o $(DEPS)
	gcc -o $@ $< $(CFLAGS)


update:
	git pull origin master
	
push:
	git add .
	git commit -m "$$$$"
	git push origin master

clean:
	rm -rf *.o
