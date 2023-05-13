#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include "mapa.h"
#include "types.h"
#include "variables.h"

void player_attack(STATE *st, MOBS mobs[40],int mapData[LINES][COLS], MOBS boss[0])		//->SPACEBAR atk
{
  if(bosson == 0)
  {	 int i, j, k;
	if (r == 1)
	{
		for (i = st->playerX-1; i <= st->playerX+1; i++)		
		{
			for (j = st->playerY-2; j <= st->playerY+2; j++)		//a coluna do jogador e a coluna a sua direita
			{
				for (k = 0; k < 40; k++)		//percorre a lista dos mobs que possam estar nestas coordenadas
				{
					if (i == mobs[k].mobX && j == mobs[k].mobY)
					{
						 mobs[k].mobHP-= st->playerDMG;	//retira a hp
    	               	if(mapData[mobs[k].mobX + 1][mobs[k].mobY] == 0) 
					   			mapData[mobs[k].mobX + 1][mobs[k].mobY] = 10;
					   	if(mapData[mobs[k].mobX - 1][mobs[k].mobY] == 0)
					   			mapData[mobs[k].mobX - 1][mobs[k].mobY] = 10;
					   	if(mapData[mobs[k].mobX][mobs[k].mobY+1] == 0)
					   			mapData[mobs[k].mobX][mobs[k].mobY+1] = 10;
					   	if(mapData[mobs[k].mobX][mobs[k].mobY-1] == 0 )
					   			mapData[mobs[k].mobX][mobs[k].mobY-1] = 10;		//spawn do sangue ao matar mob
					}
				}
			}
		}
	}
	else
	{
		for (i = st->playerX-1; i <= st->playerX+1; i++)		
		{
			for (j = st->playerY-2; j <= st->playerY; j++)		//a coluna do jogador e a coluna a sua esquerda
			{
				for (k = 0; k < 40; k++)		//percorre a lista dos mobs que possam estar nestas coordenadas
				{
					if (i == mobs[k].mobX && j == mobs[k].mobY)
					{
						 mobs[k].mobHP-= st->playerDMG;	//retira a hp
    	               	if(mapData[mobs[k].mobX + 1][mobs[k].mobY] == 0) 
					   			mapData[mobs[k].mobX + 1][mobs[k].mobY] = 10;
					   	if(mapData[mobs[k].mobX - 1][mobs[k].mobY] == 0) 
					   			mapData[mobs[k].mobX - 1][mobs[k].mobY] = 10;
					   	if(mapData[mobs[k].mobX][mobs[k].mobY+1] == 0) 
					   			mapData[mobs[k].mobX][mobs[k].mobY+1] = 10;
					   	if(mapData[mobs[k].mobX][mobs[k].mobY-1] == 0) 
					   			mapData[mobs[k].mobX][mobs[k].mobY-1] = 10;		//spawn do sangue ao matar mob
					}
				}
			}
		}
	}
  }
  else if(bosson == 1)
  {
     int i, j;
	if (r == 1)
	{
		for (i = st->playerX-1; i <= st->playerX+1; i++)		
		{
			for (j = st->playerY-2; j <= st->playerY+2; j++)		//a coluna do jogador e a coluna a sua direita
			{
						
				
			     	if (i == boss[0].mobX && j == boss[0].mobY)
					{
						 boss[0].mobHP-= st->playerDMG;	//retira a hp
    	               	if(mapData[boss[0].mobX + 1][boss[0].mobY] == 0) 
					   			mapData[boss[0].mobX + 1][boss[0].mobY] = 10;
					   	if(mapData[boss[0].mobX - 1][boss[0].mobY] == 0)
					   			mapData[boss[0].mobX - 1][boss[0].mobY] = 10;
					   	if(mapData[boss[0].mobX][boss[0].mobY+1] == 0)
					   			mapData[boss[0].mobX][boss[0].mobY+1] = 10;
					   	if(mapData[boss[0].mobX][boss[0].mobY-1] == 0 )
					   			mapData[boss[0].mobX][boss[0].mobY-1] = 10;		//spawn do sangue ao matar mob
					}
				
			}
		}
	}
	else
	{
		for (i = st->playerX-1; i <= st->playerX+1; i++)		
		{
			for (j = st->playerY-2; j <= st->playerY; j++)		//a coluna do jogador e a coluna a sua esquerda
			{
						
					if (i == boss[0].mobX && j == boss[0].mobY)
					{
						 boss[0].mobHP-= st->playerDMG;	//retira a hp
    	               	if(mapData[boss[0].mobX + 1][boss[0].mobY] == 0) 
					   			mapData[boss[0].mobX + 1][boss[0].mobY] = 10;
					   	if(mapData[boss[0].mobX - 1][boss[0].mobY] == 0) 
					   			mapData[boss[0].mobX - 1][boss[0].mobY] = 10;
					   	if(mapData[boss[0].mobX][boss[0].mobY+1] == 0) 
					   			mapData[boss[0].mobX][boss[0].mobY+1] = 10;
					   	if(mapData[boss[0].mobX][boss[0].mobY-1] == 0) 
					   			mapData[boss[0].mobX][boss[0].mobY-1] = 10;		//spawn do sangue ao matar mob
					}
				
			}
		}
    }
  
  }
}

void player_ulti(STATE *st, MOBS mobs[40], int mapData[LINES][COLS])
{ 
  for(int i = 0; i < 40; i++)
  {
    int dx = mobs[i].mobX - st->playerX;
    int dy = mobs[i].mobY - st->playerY;
    double distancia = sqrt(dx * dx + dy * dy);

    if(distancia <= 6)
    {
        mobs[i].mobHP -= st->playerDMG+6;		//dantes estava 5 fixo mas agora escala com os boosts que apanhas
    }

    for(int j = -5; j <= 5; j++)
    {
      for(int k = -5; k <= 5; k++)
      {
        int distanciacentro= sqrt(j*j + k*k);
        if (distanciacentro <= 5)
        {
          int x = st->playerX + j;
          int y = st->playerY + k;
          if (x >= 0 && x < LINES && y >= 0 && y < COLS && mapData[x][y] == 0)
          {
              mapData[x][y] = 11;
          }
        }
      }
    }
    for (int i = st->playerX - 5; i <= st->playerX + 5; i++)
    {
        for (int j = st->playerY - 5; j <= st->playerY + 5; j++)
        {
            attron(COLOR_PAIR(COLOR_CYAN));
			mvaddch(i, j, '*');
            attroff(COLOR_PAIR(COLOR_CYAN));
	    } 
    }
 
 }

 st->playerBLOOD = 0;
}

void drawplayer(STATE *st, int mapData[LINES][COLS])		//eu sei que o nome ta todo comido mas funcemina, e é mais simples
{
	attron(COLOR_PAIR(COLOR_YELLOW));
	mvaddch(st->playerX, st->playerY, '@' | A_BOLD);
	if (aon == 1)
	{
		if (r == 1)
		{
	    if (mapData[st->playerX][st->playerY+1] == 0 || mapData[st->playerX][st->playerY+1] == 10)
				mvaddch(st->playerX, st->playerY+1, '_' | A_BOLD);
		}
		else
		{	
			if (mapData[st->playerX][st->playerY-1] == 0 || mapData[st->playerX][st->playerY-1] == 10)
				mvaddch(st->playerX, st->playerY-1, '_' | A_BOLD);
		}
	}
	else
	{
		if (r == 1)
		{
			if (mapData[st->playerX][st->playerY+1] == 0 || mapData[st->playerX][st->playerY+1] == 10)
				mvaddch(st->playerX, st->playerY+1, '/' | A_BOLD);
		}
		else
		{	
			if (mapData[st->playerX][st->playerY-1] == 0 || mapData[st->playerX][st->playerY-1] == 10) 
				mvaddch(st->playerX, st->playerY-1, '\\' | A_BOLD);
		}
	}
	attroff(COLOR_PAIR(COLOR_YELLOW));
}

void ulti_clear(STATE *st, int mapData[LINES][COLS])
{
  if(uon == 0)
  {
	
	for (int i = st->playerX - 5; i <= st->playerX + 5; i++)
    {
        for (int j = st->playerY - 5; j <= st->playerY + 5; j++)
        {
            if (mapData[i][j] == 11)
                mapData[i][j] = 0;
        }
    }
  }
}

void update(STATE *st, int mapData[LINES][COLS])
{
 
  	nodelay(stdscr, TRUE); // make getch non-blocking
    int key = getch();
    nodelay(stdscr, FALSE);

	switch(key)
	{
		case KEY_A1:
			case '7':
				aon = 0;
				uon = 0;
				r = 0;
				do_movement_action(st, -1, -1, mapData);
				break;
		case KEY_UP:
			case '8':
				aon = 0;
				uon = 0;
				do_movement_action(st, -1, +0, mapData);
				break;
		case KEY_A3:
			case '9':
				aon = 0;
				uon = 0;
				r = 1;
				do_movement_action(st, -1, +1, mapData);
				break;
		case KEY_LEFT:
			case '4': 
				aon = 0;
				uon = 0;
			    r = 0;
				do_movement_action(st, +0, -1, mapData);
				break;
		case KEY_B2:
			case '5':
				break;
		case KEY_RIGHT:
			case '6': 
				aon = 0;
				uon = 0;
				r = 1;
				do_movement_action(st, +0, +1, mapData);
			    break;
		case KEY_C1:
			case '1': 
				aon = 0;
				uon = 0;
				r = 0;
				do_movement_action(st, +1, -1, mapData);
				break;
		case KEY_DOWN:
			case '2': 
				aon = 0;
				uon = 0;
				do_movement_action(st, +1, +0, mapData);
				break;
		case KEY_C3:
			case '3': 
				aon = 0;
				uon = 0;
				r = 1;
				do_movement_action(st, +1, +1, mapData);
				break;
		case 'z':
			aon = 0;
			aon = 1;
			player_attack(st, mobs, mapData, boss);
			break;
		case 'x':
            if(st->playerBLOOD == ultcost)
			{
            	player_ulti(st, mobs, mapData);
				uon = 1;
				atime2 = 0;
			}
			break;
		case 'c':			//c cura agora :)
            if (st->playerBLOOD >= healcost && st->playerHP < st->playerMAXHP-1)
			{
              	st->playerHP+=2;
				st->playerBLOOD-= healcost;
			}
			else if (st->playerBLOOD >= healcost && st->playerHP == st->playerMAXHP-1)
			{
              	st->playerHP++;
				st->playerBLOOD-= healcost;
			}
			break;
		case 'q': 
			endwin(); 
			exit(0);
			break;
	}
  
}

void do_movement_action(STATE *st, int dx, int dy, int mapData[LINES][COLS])
{
	if(mapData[st->playerX + dx][st->playerY + dy] == 1 || mapData[st->playerX + dx][st->playerY + dy] == 8 ||mapData[st->playerX + dx][st->playerY + dy] == 8)
		;

    else
	{
		st->playerX += dx;
        st->playerY += dy;
    }
}