#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>

#include "mapa.h"

/**
 *
 * Falta muita coisa, incluindo por exemplo:
 * - o mapa
 * - os monstros
 * - a fila de prioridade para saber quem se move
 * - o que está em cada casa
 *
 */
typedef struct state
{
	int playerX;
	int playerY;
	int playerHP;	//o que vai contar a hp do jogador, podera ser usado depois para o sistema de vida
} STATE;


/**
 *
 * Um pequeno exemplo que mostra o que se pode fazer
 */
void do_movement_action(STATE *st, int dx, int dy) {
	st->playerX += dx;
	st->playerY += dy;
}

void drawLight(STATE st, int nrows, int ncols, int mapData[][ncols])
{
		int i, j;
	for (i = 0; i < nrows; i++)
	{
		for (j = 0; j < ncols; j++)
		{
			if(sqrt(pow(i - st.playerX, 2) + pow(j - st.playerY, 2)) <= 20)
			{
				if(mapData[i][j] == 0)
					mvaddch(i, j, '.');
				if(mapData[i][j] == 1)
					mvaddch(i, j, '#');
				if(mapData[i][j] == 2)
					mvaddch(i, j, '~');
			}
		}
	}
}

void update(STATE *st)
{
	int key = getch();

	mvaddch(st->playerX, st->playerY, '.');
	switch(key) {
		case KEY_A1:
			case '7': do_movement_action(st, -1, -1);
				break;
		case KEY_UP:
			case '8': do_movement_action(st, -1, +0);
				break;
		case KEY_A3:
			case '9': do_movement_action(st, -1, +1);
				break;
		case KEY_LEFT:
			case '4': do_movement_action(st, +0, -1);
				break;
		case KEY_B2:
			case '5':
				break;
		case KEY_RIGHT:
			case '6': do_movement_action(st, +0, +1);
				break;
		case KEY_C1:
			case '1': do_movement_action(st, +1, -1);
				break;
		case KEY_DOWN:
			case '2': do_movement_action(st, +1, +0);
				break;
		case KEY_C3:
			case '3': do_movement_action(st, +1, +1);
				break;
		case 'q': endwin(); 
			exit(0);
			break;
	}
}

int main()
{
	STATE st = {20,20,3};	//coordenada 20,20, começa com 3HP
	WINDOW *wnd = initscr();
	int ncols, nrows;
	getmaxyx(wnd,nrows,ncols);

	int mapData[nrows][ncols];

	srandom(time(NULL));
	start_color();

	cbreak();
	noecho();
	nonl();	
	intrflush(stdscr, false);
	keypad(stdscr, true);

	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);

	gerar(nrows, ncols, mapData);

	/**
	 * Este código está muito mal escrito!
	 * Deveria existir uma função chamada draw_player! <<<---- TEMOS DE FAZER
	 *
	 * Se estamos a desenhar uma luz à volta do jogador
	 * deveria existir uma função chamada draw_light!	<<<---- mas esta aqui ja estou a fazer
	 *
	 */

	//drawplayer;

	while(1) {
		move(nrows - 1, 0);
		attron(COLOR_PAIR(COLOR_BLUE));
		printw("(%d, %d) %d %d", st.playerX, st.playerY, ncols, nrows); //ao tirar isto da warning de ncols e nrows nao utilizadas
		attroff(COLOR_PAIR(COLOR_BLUE));
		drawLight(st, nrows, ncols, mapData);
		attron(COLOR_PAIR(COLOR_YELLOW));
		mvaddch(st.playerX, st.playerY, '@' | A_BOLD);
		attroff(COLOR_PAIR(COLOR_YELLOW));
		attron(COLOR_PAIR(COLOR_YELLOW));
		// mvaddch(st.playerX - 1, st.playerY - 1, '.' | A_BOLD);
		// mvaddch(st.playerX - 1, st.playerY + 0, '.' | A_BOLD);
		// mvaddch(st.playerX - 1, st.playerY + 1, '.' | A_BOLD);
		// mvaddch(st.playerX + 0, st.playerY - 1, '.' | A_BOLD);
		// mvaddch(st.playerX + 0, st.playerY + 1, '.' | A_BOLD);
		// mvaddch(st.playerX + 1, st.playerY - 1, '.' | A_BOLD);
		// mvaddch(st.playerX + 1, st.playerY + 0, '.' | A_BOLD);
		// mvaddch(st.playerX + 1, st.playerY + 1, '.' | A_BOLD);
        attroff(COLOR_PAIR(COLOR_YELLOW));
		move(st.playerX, st.playerY);
		update(&st);
	}

	return 0;
}
