




#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>

#include "mapa.h"
#include "types.h"

int r = 0;
int aon = 0;

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
   
	 
	if(r == 0)
	{ 	if(mapData[st->playerX + dx][st->playerY + dy] == 1 || mapData[st->playerX + dx][st->playerY + dy - 1] == 1)
		;

     else if(mapData[st->playerX + dx][st->playerY + dy] != 8 && mapData[st->playerX + dx][st->playerY + dy - 1] != 8)
	 {
        mapData[st->playerX][st->playerY-1] = 0;
		mapData[st->playerX][st->playerY+1] = 0;
		st->playerX += dx;
        st->playerY += dy;
     }
	}
    else if (r == 1)
	{
	   if(mapData[st->playerX + dx][st->playerY + dy] == 1 || mapData[st->playerX + dx][st->playerY + dy + 1] == 1 ||mapData[st->playerX + dx][st->playerY + dy] == 8)
		;

     else if(mapData[st->playerX + dx][st->playerY + dy] != 8 && mapData[st->playerX + dx][st->playerY + dy + 1] != 8 )
	 {
        mapData[st->playerX][st->playerY-1] = 0;
		mapData[st->playerX][st->playerY+1] = 0;
		st->playerX += dx;
        st->playerY += dy;
     }
	}
}

MOBS mobs[5];
void mob_spawn(int mapData[LINES][COLS], MOBS mobs[5])
{
    for (int i = 0; i < 5; i++) {
	    mobs[i].mobtype = 'e';
		mobs[i].mobDMG = 1;
        mobs[i].mobHP = 2;                                       //Esta funcao spawna os mobs random
	    do {                                                     //mas sinto que 5 é demasiado pouco (joao)
			mobs[i].mobX = rand() % LINES;                       //ja meti para ficar longe do spawn, ta mesmo aqui abaixo (20,20) sendo o spawn do player (joao)         
            mobs[i].mobY = rand() % COLS;
        } while (mapData[mobs[i].mobX][mobs[i].mobY] != 0 ||sqrt(pow(mobs[i].mobX - 20, 2) + pow(mobs[i].mobY - 20, 2)) < 10);
       mapData[mobs[i].mobX][mobs[i].mobY] = 8;
	}
}

void mob_respawn(int mapData[LINES][COLS], MOBS mobs[5], STATE* st)
{
		int i;
	for(i = 0; i < 5; i++)
	{
		if(mobs[i].mobHP <= 0)
		{
				mobs[i].mobtype = 'e';
				mobs[i].mobDMG = 1;
        		mobs[i].mobHP = 2;  	//cria os status para novo mob
			if(rand() % 5 == 0)
				mapData[mobs[i].mobX][mobs[i].mobY] = 5;	//chance de drop de item de cura
			else
				mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			do 
			{
				mobs[i].mobX = rand() % LINES;        
            	mobs[i].mobY = rand() % COLS;
        	}
			while (mapData[mobs[i].mobX][mobs[i].mobY] != 0 ||sqrt(pow(mobs[i].mobX - st->playerX, 2) + pow(mobs[i].mobY - st->playerY, 2)) < 10);
       		mapData[mobs[i].mobX][mobs[i].mobY] = 8;
		}
	}

	//aqui isto ve continuamente se ha algum mob morto e se ha entao usa a mesma cena de spawnar mas para 10 casas ou mais de distancia da posiçao atual do jogador
}


void mob_movement(int mapData[LINES][COLS], MOBS mobs[5], int playerX, int playerY)
{
  for(int i = 0; i < 5; i++)
  {
    int dx = mobs[i].mobX - playerX;
    int dy = mobs[i].mobY - playerY;
    double dist_squared = sqrt(dx*dx + dy*dy);

    if(dist_squared < 12)
    {
	 
      int x = mobs[i].mobX;
      int y = mobs[i].mobY;
      int sx = dx > 0 ? -1 : 1;
      int sy = dy > 0 ? -1 : 1;
      int err = 0;
      int deltaError = abs(dy) * 2;
      int wallFound = 0;

      for(int j = 0; j <= abs(dx); j++)
      {
        if(mapData[x][y] == 1) 
        {
          wallFound = 1;
          break;
        }

        err += deltaError;

        if(err > abs(dx) * 2)
        {
          err -= abs(dx) * 2;
          y += sy;
        }

        x += sx;
      }

      if(wallFound == 1)
      {
        
        int s = rand() % 4;

        if(s == 0 && mapData[mobs[i].mobX][mobs[i].mobY - 1] == 0)
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobY--;
        }
        else if(s == 1 && mapData[mobs[i].mobX + 1][mobs[i].mobY] == 0)
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobX++;
        }
        else if(s == 2 && mapData[mobs[i].mobX][mobs[i].mobY + 1] == 0)
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobY++;
        }
        else if(s == 3 && mapData[mobs[i].mobX -1][mobs[i].mobY] == 0)
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobX--;
        }

        mapData[mobs[i].mobX][mobs[i].mobY] = 8;
      }
      else
	  {
		if(mobs[i].mobX > playerX)
		{
			if(mobs[i].mobY > playerY && mapData[mobs[i].mobX - 1][mobs[i].mobY - 1] == 0)
			{
			  if(mobs[i].mobX > playerX + 1 || mobs[i].mobY > playerY + 1) mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			  if(mobs[i].mobX > playerX + 1) mobs[i].mobX--;
			  if(mobs[i].mobY > playerY + 1) mobs[i].mobY--;                                                             // Os mobs agora ja nao vao para a mesma casa do player.
			}                                                                                                            // Mas agora que penso bastava so dar um numero para o player no mapData
		    else if(mobs[i].mobY < playerY && mapData[mobs[i].mobX - 1][mobs[i].mobY + 1] == 0)                          // e a funcao ja funcionava nao tinha the acrescentar aqueles ifs todos
		    {                                                                                                            // Se alguem quiser fazer esse pequeno upgrade eu agradecia(LOPES)
			  if(mobs[i].mobY < playerY - 1 || mobs[i].mobX > playerX + 1) mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			  if(mobs[i].mobX > playerX + 1) mobs[i].mobX--;
			  if(mobs[i].mobY < playerY - 1)mobs[i].mobY++;
		    }
		   else if(mapData[mobs[i].mobX - 1][mobs[i].mobY] == 0)
		   {
             if(mobs[i].mobX > playerX + 1)
			 {
               mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			   mobs[i].mobX--;
			 } 
		   }
		}
	    else if(mobs[i].mobX < playerX)
		{
			if(mobs[i].mobY > playerY && mapData[mobs[i].mobX + 1][mobs[i].mobY - 1] == 0)
			{
			 if(mobs[i].mobX < playerX - 1 || mobs[i].mobY > playerY + 1) mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			 if(mobs[i].mobX < playerX - 1) mobs[i].mobX++;
			 if( mobs[i].mobY > playerY + 1) mobs[i].mobY--;
			}
		    else if(mobs[i].mobY < playerY  && mapData[mobs[i].mobX + 1][mobs[i].mobY + 1] == 0)
		    {
			  if(mobs[i].mobY < playerY -1 || mobs[i].mobX < playerX + 1)  mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			  if(mobs[i].mobX < playerX + 1) mobs[i].mobX++;
			  if(mobs[i].mobY < playerY -1 ) mobs[i].mobY++;
		    }
		   else if(mapData[mobs[i].mobX + 1][mobs[i].mobY] == 0)
		   {
             if(mobs[i].mobX < playerX - 1) 
			 {
               mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			   mobs[i].mobX++;
			 }  
		   }
		}
	    else if(mobs[i].mobX == playerX)
		{
			if(mobs[i].mobY > playerY + 1 && mapData[mobs[i].mobX][mobs[i].mobY - 1] == 0)
			{
				mapData[mobs[i].mobX][mobs[i].mobY] = 0;
				mobs[i].mobY--;
			}
		    else if(mobs[i].mobY < playerY - 1 && mapData[mobs[i].mobX][mobs[i].mobY + 1] == 0)
		    {
			    mapData[mobs[i].mobX][mobs[i].mobY] = 0;
				mobs[i].mobY++;
		    }
		   else if(mapData[mobs[i].mobX][mobs[i].mobY] == 0)
		   {
               continue;
		   }
		}
	      mapData[mobs[i].mobX][mobs[i].mobY] = 8;
	  }
	}
    else 
	{
	  int s2 = rand() % 4;

        if(s2 == 0 && mapData[mobs[i].mobX][mobs[i].mobY - 1] == 0)
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobY--;
        }
        else if(s2 == 1 && mapData[mobs[i].mobX + 1][mobs[i].mobY] == 0)
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobX++;
        }
        else if(s2 == 2 && mapData[mobs[i].mobX][mobs[i].mobY + 1] == 0)
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobY++;
        }
        else if(s2 == 3 && mapData[mobs[i].mobX -1][mobs[i].mobY] == 0)
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobX--;
        }

        mapData[mobs[i].mobX][mobs[i].mobY] = 8;	
	}
  }
}

void mob_attack(STATE *st, MOBS mobs[5]) 
{
    for (int i = 0; i < 5; i++)                      // Esta funcao e respondavel por retirar hp do jogador													 
	{                                                // quando um mob se encontra perto. Mas com esta 
     int dx = mobs[i].mobX - st->playerX;            // abordagem todos os mobs e um bloco do jogador o conseguem atacar ao mesmo tempo.
     int dy = mobs[i].mobY - st->playerY;            // Da maneira atual no maximo o jogador leva 9 de dano por segundo se estiver rodeado por mobs.(LOPES)
     double distancia = sqrt(dx * dx + dy * dy);     
     if (distancia <= sqrt(4)) 
	 {
        st->playerHP -= mobs[i].mobDMG;
     }
    }
}

void player_attack(STATE *st, MOBS mobs[5],int mapData[LINES][COLS]) //-> Funcao que permite o player atacar mobs
	//so acionado na tecla	|v
	//da dano nas casas a volta dele apenas	|v
	//muda os icons no mapa a volta para indicar ataque		(JOAO)
	//passado algum delay reverte ao que era		<- estes os dois e que falta fazer
	//quando um mob recebe dano fazer flash da sua cor para preto e devolta para vermelho tambem com a cena dos intervalos	|v
{
		int i, j, k;
	for (i = st->playerX-2; i <= st->playerX+2; i++)		
	{
		for (j = st->playerY-2; j <= st->playerY+2; j++)		//vem o quadrado a volta do jogador
		{
			for (k = 0; k < 5; k++)		//percorre a lista dos mobs que possam estar nestas coordenadas
			{
				if (i == mobs[k].mobX && j == mobs[k].mobY)
				{
					//attron(COLOR_PAIR(COLOR_BLACK));
					//mvaddch(i, j, 'e');
					//attroff(COLOR_PAIR(COLOR_BLACK));		//flash inicial preto dos mobs
                    mobs[k].mobHITS = 1;
					mobs[k].mobHP-= st->playerDMG;	//retira a hp
                    mapData[mobs[k].mobX + 1][mobs[k].mobY] = 10;
				    mapData[mobs[k].mobX - 1][mobs[k].mobY] = 10;
				    mapData[mobs[k].mobX][mobs[k].mobY+1] = 10;
				    mapData[mobs[k].mobX][mobs[k].mobY-1] = 10;
					//for(h = 0; h < 100000; h++)
					//	;
					//attron(COLOR_PAIR(COLOR_RED));
					//mvaddch(i, j, 'e');
					//attroff(COLOR_PAIR(COLOR_RED));		//passado delay, reverte a cor para vermelho
				}
			}
		}
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

void reset_blood(MOBS mobs[5],int mapData[LINES][COLS])
{
	for(int i = 0; i < 5; i++)
	{
	  if(mobs[i].mobHITS == 1)
	  {
		mapData[mobs[i].mobX + 1][mobs[i].mobY] = 0;
	    mapData[mobs[i].mobX - 1][mobs[i].mobY] = 0;
	    mapData[mobs[i].mobX][mobs[i].mobY+1] = 0;
	    mapData[mobs[i].mobX][mobs[i].mobY-1] = 0;
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
					if(mapData[i][j] == 5)
					{
						attron(COLOR_PAIR(COLOR_BLUE));
                        mvaddch(i, j, '+');
						attroff(COLOR_PAIR(COLOR_BLUE));
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
						attron(COLOR_PAIR(COLOR_MAGENTA));
						mvaddch(i, j, 'e');
					    attroff(COLOR_PAIR(COLOR_MAGENTA));
					}
				   if(mapData[i][j] == 10)
					{
						attron(COLOR_PAIR(COLOR_RED));
						mvaddch(i, j, '*');
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
 
  	nodelay(stdscr, TRUE); // make getch non-blocking
    int key = getch();
    nodelay(stdscr, FALSE);

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
			case '4': 
			    r = 0;
				do_movement_action(st, +0, -1, mapData);
				break;
		case KEY_B2:
			case '5':
				break;
		case KEY_RIGHT:
			case '6': 
			    r = 1;
				do_movement_action(st, +0, +1, mapData);
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
		case 'c':
			aon = 1;
			player_attack(st, mobs, mapData);
			break;
		case 'q': 
			endwin(); 
			exit(0);
			break;
	}
  
}
	
void drawplayer(STATE *st, int mapData[LINES][COLS])
{
	attron(COLOR_PAIR(COLOR_YELLOW));
	mvaddch(st->playerX, st->playerY, '@' | A_BOLD);
	if(r == 1 && aon == 0 && mapData[st->playerX][st->playerY+1] != 8)  // acrescentei uma espada ao jogador ('|') (LOPES)
	{
      mapData[st->playerX][st->playerY+1] = 9;
	  mvaddch(st->playerX, st->playerY+1, '|' | A_BOLD); 
	}
	if(r == 0 && aon == 0 && mapData[st->playerX][st->playerY-1] != 8) 
	{
      mapData[st->playerX][st->playerY-1] = 9;
	  mvaddch(st->playerX, st->playerY-1, '|' | A_BOLD);
	}
	if(r == 1 && aon == 1 && mapData[st->playerX][st->playerY+1] != 8) 
	{
      mapData[st->playerX][st->playerY+1] = 9;
	  mvaddch(st->playerX, st->playerY+1, '/' | A_BOLD);
	  mvaddch(st->playerX, st->playerY+1, '_' | A_BOLD);                           // a espada vai para baixo quando o jogador ataca (LOPES)
	}   
	if(r == 0 && aon == 1 && mapData[st->playerX][st->playerY-1] != 8)
	{
      mapData[st->playerX][st->playerY-1] = 9;
	  mvaddch(st->playerX, st->playerY-1, '\\' | A_BOLD);
	  mvaddch(st->playerX, st->playerY-1, '_' | A_BOLD);
	} 
	attroff(COLOR_PAIR(COLOR_YELLOW));
}

void drawHP(STATE *st)
{
		int i;
	mvaddstr(1, 0, "HP:");	//usa os primeiros 3 indices
	for (i = 0; i < st->playerMAXHP; i++)
		mvaddch(1, 3+i, '-');
	for (i = 0; i < st->playerHP; i++)
		mvaddch(1, 3+i, '|');  // alterei o simbolo de 'o' para '|'. Acho que fica melhor fica a parecer uma barra de HP.(LOPES)
    if(st->playerMAXHP < st->playerHP)
	{
	 for (i = 0; i < st->playerHP - st->playerMAXHP; i++)
	 mvaddch(1, st->playerHP-i, ' ');
	}
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
	if(mapData[st->playerX][st->playerY] == 5 || mapData[st->playerX][st->playerY + 2] == 5 || mapData[st->playerX][st->playerY - 2] == 5)
	{
	   if(st->playerHP < 20)	st->playerHP++;
	   if(mapData[st->playerX][st->playerY] == 5) mapData[st->playerX][st->playerY] = 0;
	   if(mapData[st->playerX][st->playerY + 2] == 5) mapData[st->playerX][st->playerY + 2] = 0;
	   if(mapData[st->playerX][st->playerY - 2] == 5) mapData[st->playerX][st->playerY - 2] = 0;
	}	
	if(mapData[st->playerX][st->playerY] == 6 || mapData[st->playerX][st->playerY + 2] == 6 || mapData[st->playerX][st->playerY - 2] == 6)
	{
		
		if(st->playerHP < 20)st->playerHP++;
	    if(mapData[st->playerX][st->playerY] == 6) mapData[st->playerX][st->playerY] = 0;
	    if(mapData[st->playerX][st->playerY + 2] == 6) mapData[st->playerX][st->playerY + 2] = 0;
	    if(mapData[st->playerX][st->playerY - 2] == 6) mapData[st->playerX][st->playerY - 2] = 0;
	}
	if(mapData[st->playerX][st->playerY] == 7 || mapData[st->playerX][st->playerY + 2] == 7 || mapData[st->playerX][st->playerY - 2] == 7)
	{
		st->playerDMG++;
		mapData[st->playerX][st->playerY] = 0;
	    if(mapData[st->playerX][st->playerY] == 7) mapData[st->playerX][st->playerY] = 0;
	    if(mapData[st->playerX][st->playerY + 2] == 7) mapData[st->playerX][st->playerY + 2] = 0;
	    if(mapData[st->playerX][st->playerY - 2] == 7) mapData[st->playerX][st->playerY - 2] = 0;
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

	init_pair(COLOR_MAGENTA,COLOR_MAGENTA,COLOR_BLACK );
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);

	gerar(mapData);
    mob_spawn(mapData, mobs);
	
	STATE st = {20,20,20,20,1};			//coordenada 20,20, começa com 3HP atual, 3HP max e 1DMG inicial                                    
	int i = 0;                          // alterei o hp do player para 20 para ser mais facil definir o dano dos mobs. Se o player tivesse de hp 3, ia ter de por o dano dos mobs decimal                       // caso contrario o mobs matava o player em 3 ataques o que imensamente rapido oq ue significava alterar todas as funcoes de int para float e nao me apetecia.
    int s = 0;                          // (LOPES)                                  
    int atime = 0;

	while (true) 
	{
	 //if(st.playerHP == 0) break;         se ativarmos o if comentado quando o player morre o jogo fecha.
	 reset_blood(mobs, mapData);
	 drawLight(mapData, &st);             //mas essa maneira e um bocado merda, estou a trabalhar numa forma
	 drawplayer(&st,mapData);                     // de quando o jogador morrer aparece gameover  e 2s depois o jogo reinicia
	 drawHP(&st);                         // com um novo mapa e tudo novo. (LOPES)
	 drawDMG(&st);
	 itemcollect(&st, mapData);
	 update(&st,mapData);
	 mob_respawn(mapData, mobs, &st);                    
	 refresh();
	 move(LINES - 1, 0);
	 i++;
	 s++;
	 atime++;
	 
	 if(atime == 10000)           
	 {
		aon = 0;
		atime = 0;
	 }
	 if(s == 12000) // 13500 voltas = +/- 1 segundo --> Ou seja o jogador leva 1 de dano a cada segundo. (LOPES)
	 {
	  mob_attack(&st, mobs);
	  s = 0;
	 }
	 if(i == 6000)  
	 {
		mob_movement(mapData, mobs, st.playerX, st.playerY);
		i = 0;
	 }
	}

	return 0;
}

/* Coisas a fazer:
 
 
 Corrigir a velocidade dos mobs.(Diminuir a velocidade de perseguicao para algo possivel de se sobreviver. Definir uma velocidade de vaguemento menor que a de perseguicao.)
	-> epah para mim a velocidade deles estava ok, mas achei estranho terem de estar mesmo em cima do player para atacar -> Resolvido (lopes)(joao)
 Corrigir os buffs do player.		<- explica? -> Tipo se o jogador recebe um buff de dano , fazer com que de efetivamente mais dano.
 Criar um sistema de game over para qnd o player morre. 

 Se houver tempo:

 animações de ataque para o jogador e animação de receber dano dos mobs
 Acrescentar novos mobs
 Acrescentar novas salas -- a boss estelita ira acontecer (joao)
e tc

Cenas que acho melhor alterar:



*/