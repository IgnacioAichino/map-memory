CFLAGS= -Wall -Werror -pedantic

fdmapped: main.o 
		gcc $(CFLAGS) -o fdmapped main.o 

main.o: main.c
		gcc $(CFLAGS) -c main.c

clean:
		rm -f fdmapped *.o 