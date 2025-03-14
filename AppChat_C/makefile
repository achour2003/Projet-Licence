CC = gcc
CFLAGS = -Wall -pthread
TARGET = serveur client

all: $(TARGET)

serveur: serveur.o
	$(CC) $(CFLAGS) -o serveur serveur.o

client: client.o
	$(CC) $(CFLAGS) -o client client.o

serveur.o: serveur.c
	$(CC) $(CFLAGS) -c serveur.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f *.o $(TARGET)
