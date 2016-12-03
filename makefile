GCC = gcc -g

all: t_rex.o main.o
	$(GCC) t_rex.o main.o -o shell.out

t_rex.o: t_rex.c t_rex.h
	$(GCC) -c t_rex.c

main.o: main.c t_rex.c
	$(GCC) -c main.c
	
run: 
	./shell.out

clean:
	rm -rf *~
	rm -rf *.o
