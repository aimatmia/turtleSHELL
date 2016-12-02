GCC = gcc -g

all: shell.o t_rex.o main.o
	$(GCC) shell.o t_rex.o main.o -o shell.out

shell.o: shell.c shell.h
	$(GCC) -c shell.c

t_rex.o: t_rex.c t_rex.h
	$(GCC) -c t_rex.c

main.o: main.c shell.c t_rex.c
	$(GCC) -c main.c
run: 
	./shell.out

clean:
	rm -rf *~
	rm -rf *.o
