#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>

#include "mapa.h"
#include "types.h"

/**
 *
 * Falta muita coisa, incluindo por exemplo:
 * - o mapa
 * - os monstros
 * - a fila de prioridade para saber quem se move
 * - o que está em cada casa
 *
 */

/**
 *
 * Um pequeno exemplo que mostra o que se pode fazer
 */
void do_movement_action(STATE *st, int dx, int dy, int mapData[LINES][COLS])
{
    if(mapData[st->playerX + dx][st->playerY + dy] == 1)
		;

    else
	{
        st->playerX += dx;
        st->playerY += dy;
    }
}

MOBS mobs[25];
void mob_spawn(int mapData[LINES][COLS], MOBS mobs[15])
{
    for (int i = 0; i < 25; i++) {
        mobs[i].mobtype = 'e';
		mobs[i].mobDMG = 1;
        mobs[i].mobHP = 2;
        do {
            mobs[i].mobX = rand() % LINES;
            mobs[i].mobY = rand() % COLS;
        } while (mapData[mobs[i].mobX][mobs[i].mobY] != 0);
       mapData[mobs[i].mobX][mobs[i].mobY] = 8;
    }
}

void mob_movement(int mapData[LINES][COLS], MOBS mobs[5])
{
    for (int i = 0; i < 25; i++)
    {
        int x = rand() % 4;

        if(x == 0 && mapData[mobs[i].mobX][mobs[i].mobY - 1] == 0)
        {
            mapData[mobs[i].mobX][mobs[i].mobY] = 0;
            mobs[i].mobY--;
        }
        else if(x == 1 && mapData[mobs[i].mobX + 1][mobs[i].mobY] == 0)
        {
            mapData[mobs[i].mobX][mobs[i].mobY] = 0;
            mobs[i].mobX++;
        }
        else if(x == 2 && mapData[mobs[i].mobX][mobs[i].mobY + 1] == 0)
        {
            mapData[mobs[i].mobX][mobs[i].mobY] = 0;
            mobs[i].mobY++;
        }
        else if(x == 3 && mapData[mobs[i].mobX -1][mobs[i].mobY] == 0)
        {
            mapData[mobs[i].mobX][mobs[i].mobY] = 0;
            mobs[i].mobX--;
        }
        mapData[mobs[i].mobX][mobs[i].mobY] = 8;
    }
}

void draw (int mapData[LINES][COLS])
{
		int i, j;
	for (i = 0; i < LINES; i++)
	{
		for (j = 0; j < COLS; j++)
		{
				if(mapData[i][j] == 0)
					mvaddch(i, j, '.');
				if(mapData[i][j] == 1)
					mvaddch(i, j, '#');
				if(mapData[i][j] == 2)
					mvaddch(i, j, '~');
				if(mapData[i][j] == 6)
				{
					attron(COLOR_PAIR(COLOR_YELLOW));
					mvaddch(i, j, 'h');
					attroff(COLOR_PAIR(COLOR_YELLOW));
				}
				if(mapData[i][j] == 7)
				{
					attron(COLOR_PAIR(COLOR_YELLOW));
                    mvaddch(i, j, 'd');
					attroff(COLOR_PAIR(COLOR_YELLOW));
                }
		}
	}
}



void drawLight (int mapData[LINES][COLS], STATE *st)
{
		int i, j;
    for (i = 0; i < LINES; i++)
	{
        for (j = 0; j < COLS; j++)
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
					if (mapData[i][j] == 1)
					{
                        mvaddch(i, j,'#');
                    }
					if(mapData[i][j] == 6)
					{
						attron(COLOR_PAIR(COLOR_YELLOW));
                        mvaddch(i, j, 'h');
						attroff(COLOR_PAIR(COLOR_YELLOW));
                    }
					if(mapData[i][j] == 7)
					{
						attron(COLOR_PAIR(COLOR_YELLOW));
                        mvaddch(i, j, 'd');
						attroff(COLOR_PAIR(COLOR_YELLOW));
                    }
                    if(mapData[i][j] == 8)
					{
						attron(COLOR_PAIR(COLOR_RED));
						mvaddch(i, j, 'e');
					    attroff(COLOR_PAIR(COLOR_RED));
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



void update(STATE *st, int mapData[LINES][COLS])
{
	int key = getch();

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
		case 'r':
			st->playerX = 20;
			st->playerY = 20;
			break;		//reset para testes, TIRAR NA VERSÃO FINAL
		case 'q': 
			endwin(); 
			exit(0);
			break;
	}
}
	
void drawplayer(STATE *st)
{
	attron(COLOR_PAIR(COLOR_YELLOW));
	mvaddch(st->playerX, st->playerY, '@' | A_BOLD);
	attroff(COLOR_PAIR(COLOR_YELLOW));
}

void drawHP(STATE *st)
{
		int i;
	mvaddstr(1, 0, "HP:");	//usa os primeiros 3 indices
	for (i = 0; i < st->playerMAXHP; i++)
		mvaddch(1, 3+i, '-');
	for (i = 0; i < st->playerHP; i++)
		mvaddch(1, 3+i, 'o');
}

void drawDMG(STATE *st)
{
		int i;
	mvaddstr(0, 0, "DMG BOOST:");	//usa os primeiros 3 indices
	for (i = 0; i < st->playerDMG-1; i++)
		mvaddstr(0, 11+i, ">");
}

void itemcollect(STATE *st, int mapData[LINES][COLS])
{
	if(mapData[st->playerX][st->playerY] == 6)
	{
		st->playerMAXHP++;
		st->playerHP = st->playerMAXHP;
		mapData[st->playerX][st->playerY] = 0;
	}
	if(mapData[st->playerX][st->playerY] == 7)
	{
		st->playerDMG++;
		mapData[st->playerX][st->playerY] = 0;
	}
}


int main()
{
	initscr();

	int mapData[LINES][COLS];

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
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);

	gerar(mapData);
    mob_spawn(mapData, mobs);
	
	STATE st = {20,20,3,3,1};			//coordenada 20,20, começa com 3HP atual, 3HP max e 1DMG inicial
 
	while(1)
	{
		move(LINES - 1, 0);
		itemcollect(&st, mapData);
		//draw(mapData);
		drawLight(mapData, &st);
		drawplayer(&st);
		drawHP(&st);
		drawDMG(&st);
		mob_movement(mapData,mobs);
		move(st.playerX, st.playerY);
		update(&st,mapData);
	}

	return 0;
}