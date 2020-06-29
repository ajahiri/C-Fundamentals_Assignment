CC = gcc
CFLAGS = -Wall -Werror -ansi

main.out: main.o userpass_database.o encryption.o
	$(CC) $(CFLAGS) -o main.out main.o 
	
main.o: main.c userpass_database.h encryption.h
	$(CC) $(CFLAGS) -c -o main.o main.c

userpass_database.o: userpass_database.c userpass_database.h 
	$(CC) $(CFLAGS) -c -o userpass_database.o userpass_database.c 

encryption.o: encryption.c encryption.h
	$(CC) $(CFLAGS) -c -o encryption.o encryption.c 

clean:
	rm main.out main.o userpass_database.o encryption.o