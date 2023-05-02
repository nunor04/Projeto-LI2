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
void do_movement_action(STATE *st, int dx, int dy, int **mapData)
{

    int novo_x = st->playerX + dx;
    int novo_y = st->playerY + dy;

    if (novo_x < 0 || novo_y < 0 )
	{
        return;
    }

    if(mapData[novo_x][novo_y]==1)
	{
        return;
    }

    else
	{
        st->playerX = novo_x;
        st->playerY = novo_y;
    }
}

void draw (STATE st, int nrows, int ncols, int **mapData)
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



void drawLight (int nrows, int ncols, int **mapData, STATE *st)
{
		int i, j;
    for (i = 0; i < nrows; i++)
	{
        for (j = 0; j < ncols; j++)
		{
            // calculate the distance to the current cell from the player's position
            int dy = j - st->playerY;
            int dx = i - st->playerX;
            int dist_squared = dx*dx + dy*dy;

            // check if the current cell is within the player's field of view
            if (dist_squared <= 400)
			{  // use a radius of 20 cells
                // perform a line-of-sight check from the player's position to the current cell
                int visible = 1;
                int y1 = st->playerY, x1 = st->playerX, y2 = j, x2 = i;
                dy = abs(y2 - y1);
				int sy = y1 < y2 ? 1 : -1;
                dx = abs(x2 - x1);
				int sx = x1 < x2 ? 1 : -1; 
                int err = (dy > dx ? dy : -dx) / 2;
                while (visible && (x1 != x2 || y1 != y2))
				{
                    if (mapData[x1][y1] == 1)
					{
                        visible = 0;
                    }
                    int e2 = err;
                    if (e2 > -dy)
					{
						err -= dx;
						y1 += sy;
					}
                    if (e2 < dx)
					{
						err += dy;
						x1 += sx;
					}
                }

                // draw the appropriate character for the current cell
                if (visible)
				{
                    if (mapData[i][j] == 0)
					{
                        mvaddch(i, j,'.');
                    }
					else
					{
                        mvaddch(i, j, '#');
                    }
                } 
				else
				{
                    mvaddch(i, j,' ');
                }
            } 
			else
			{
                mvaddch(i, j,' ');
            }
        }
    }
}



void update(STATE *st, int **mapData)
{
	int key = getch();

	//mvaddch(st->playerX, st->playerY, '.');
	switch(key)
	{
		case KEY_A1:
			case '7': do_movement_action(st, -1, -1, mapData);
				break;
		case KEY_UP:
			case '8': do_movement_action(st, -1, +0, mapData);
				break;
		case KEY_A3:
			case '9': do_movement_action(st, -1, +1, mapData);
				break;
		case KEY_LEFT:
			case '4': do_movement_action(st, +0, -1, mapData);
				break;
		case KEY_B2:
			case '5':
				break;
		case KEY_RIGHT:
			case '6': do_movement_action(st, +0, +1, mapData);
				break;
		case KEY_C1:
			case '1': do_movement_action(st, +1, -1, mapData);
				break;
		case KEY_DOWN:
			case '2': do_movement_action(st, +1, +0, mapData);
				break;
		case KEY_C3:
			case '3': do_movement_action(st, +1, +1, mapData);
				break;
		case 'q': endwin(); 
			exit(0);
			break;
	}
}
	
void drawplayer(STATE st)
{
	attron(COLOR_PAIR(COLOR_YELLOW));
	mvaddch(st.playerX, st.playerY, '@' | A_BOLD);
	attroff(COLOR_PAIR(COLOR_YELLOW));
}



int main()
{
	STATE st = {20,20,3};	//coordenada 20,20, começa com 3HP
	WINDOW *wnd = initscr();
	int ncols, nrows;
	getmaxyx(wnd,nrows,ncols);

	int **mapData;

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

	mapData = malloc(nrows * sizeof(int *));
    for (int i = 0; i < nrows; i++)
    {
        (mapData)[i] = malloc(ncols * sizeof(int));
    }

	gerar(nrows, ncols, mapData);

	/**
	 * Este código está muito mal escrito!
	 * Deveria existir uma função chamada draw_player! <<<---- TEMOS DE FAZER
	 *
	 * Se estamos a desenhar uma luz à volta do jogador
	 * deveria existir uma função chamada draw_light!	<<<---- mas esta aqui ja estou a fazer
	 *
	 */


	while(1) {
		move(nrows - 1, 0);
		attron(COLOR_PAIR(COLOR_BLUE));
		printw("(%d, %d) %d %d", st.playerX, st.playerY, ncols, nrows); //ao tirar isto da warning de ncols e nrows nao utilizadas
		attroff(COLOR_PAIR(COLOR_BLUE));
		//draw(st, nrows, ncols, mapData);
		drawLight(nrows, ncols, mapData, &st);
		drawplayer(st);
		move(st.playerX, st.playerY);
		update(&st,mapData);
	}

	return 0;
}