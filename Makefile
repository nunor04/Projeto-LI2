CFLAGS=-Wall -Wextra -pedantic -O2
LIBS=-lm -lcurses

jogo: main.o mapa.o mobs.o player.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm jogo *.o
