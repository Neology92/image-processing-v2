CC=gcc
CFLAGS=
LDLIBS=

all: program

program: main.o operacje.o 
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
	
main.o: main.c main.h operacje.h
	$(CC) $(CFLAGS) -c $< -o $@

operacje.o: operacje.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	-rm *.o $(objects)
