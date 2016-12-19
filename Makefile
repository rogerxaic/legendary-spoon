CC=gcc
CFLAGS=-Wall
DEPS = user_agence.h mqueue.h semaphore.h shmem.h database.h
OBJ = user.o agence.o 

all: 
	@echo "make user ou make agence ou make [...]"

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

agence: agence.o $(DEPS)
	$(CC) -o $@ $< $(CFLAGS)

users: users.o $(DEPS)
	$(CC) -o $@ $< $(CFLAGS)

display: display.o $(DEPS)
	$(CC) -o $@ $< $(CFLAGS)

update:
	git pull origin master
	
push:
	git add .
	@read -p "Ajoute un commentaire sur ce que tu as ajouté/modifié:" comment; \
	git commit -m "$$comment"
	git push origin master

clean:
	rm -rf *.o
