CC=gcc
CFLAGS=-Wall
DEPS = user.h agence.h
OBJ = user.o agence.o 

all: 
	@echo "make user ou make agence ou make [...]"

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

agence: agence.o user_agence.h
	$(CC) -o $@ $< $(CFLAGS)

user: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)


update:
	git pull origin master
	
push:
	git add .
	git commit -m "$$$$"
	git push origin master

clean:
	rm -rf *.o
