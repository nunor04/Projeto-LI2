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
#include "mobs.h"
#include "player.h"

int r = 0;
int aon = 0;
int uon = 0;
int atime2 = 0;
int grito = 0;
int ultcost = 18;
int healcost = 6;
int bosson = 0;	//verifica se ele esta na bossroom ou não
int sightrange = 400; 	//distancia que o player consegue ver
MOBS boss[1];
MOBS mobs[40];

void drawLight (int mapData[LINES][COLS], STATE *st)
{
		int i, j;
    for (i = 0; i < LINES; i++)
	{
        for (j = 0; j < COLS; j++)
		{
            //calculo a distancia (i,j) às coordenadas atuais do jogador
            int dy = j - st->playerY;
            int dx = i - st->playerX;
            int dist_squared = dx*dx + dy*dy;

            //limita a visão do jogador a um raio de 20 casas
            if (dist_squared <= sightrange)
			{
                //usa-se a verificação LOS de Bresenham do jogador para a casa (i,j)
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

                //agora imprime o que é suposto estar em cada casa de acordo com a tabela de interpretação do mapData
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
					   mvaddch(i, j, 'e' | A_BOLD);
					   attroff(COLOR_PAIR(COLOR_MAGENTA));				   
					}
				    if(mapData[i][j] == 10)
					{
						attron(COLOR_PAIR(COLOR_RED));
						mvaddch(i, j, '*');
					    attroff(COLOR_PAIR(COLOR_RED));
					}
				    if(mapData[i][j] == 12)
					{
					   attron(COLOR_PAIR(COLOR_MAGENTA));
					   mvaddch(i, j, 'c' | A_BOLD);
					   attroff(COLOR_PAIR(COLOR_MAGENTA));				   
					}
					if(mapData[i][j] == 15)
					{
					   attron(COLOR_PAIR(COLOR_CYAN));
					   mvaddch(i, j, 'o' | A_BOLD);
					   attroff(COLOR_PAIR(COLOR_CYAN));				   
					}
					if(mapData[i][j] == 20)
					{
					   attron(COLOR_PAIR(COLOR_MAGENTA));
					   mvaddch(i, j, 'E' | A_BOLD);
					   attroff(COLOR_PAIR(COLOR_MAGENTA));				   
					}
				     if(mapData[i][j] == 13)
					{
					   attron(COLOR_PAIR(COLOR_CYAN));
					   mvaddch(i, j, '.');
					   attroff(COLOR_PAIR(COLOR_CYAN));				   
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

void drawHP(STATE *st)
{
		int i;
	mvaddstr(1, 0, "HP:");	//usa os primeiros 3 indices
	for (i = 0; i < st->playerMAXHP; i++)
		mvaddch(1, 3+i, '-' | A_BOLD);
	for (i = 0; i < st->playerHP; i++)
	{
		if (i < 20)
			mvaddch(1, 3+i, '|' | A_BOLD);
		else
		{
			attron(COLOR_PAIR(COLOR_YELLOW));
			mvaddch(1, 3+i, '|' | A_BOLD);
			attroff(COLOR_PAIR(COLOR_YELLOW));
		}
	}  // alterei o simbolo de 'o' para '|'. Acho que fica melhor fica a parecer uma barra de HP.(LOPES)
    /*if(st->playerMAXHP < st->playerHP)
	 {
	 for (i = 0; i < st->playerHP - st->playerMAXHP; i++)
	 mvaddch(1, st->playerHP-i, ' ');
	} */		//n sei o que e que isto esta aqui a fazer (joao)
}

void drawDMG(STATE *st)
{
		int i;
	mvaddstr(0, 0, "DMG BOOST:");	//usa os primeiros 3 indices
	for (i = 0; i < st->playerDMG-1; i++)
	{
		attron(COLOR_PAIR(COLOR_YELLOW));
		mvaddch(0, 11+i, '>' | A_BOLD);
		attroff(COLOR_PAIR(COLOR_YELLOW));
	}
}

void drawBLOOD(STATE *st)
{
	int start = (COLS/2 - 8);
	
	mvaddstr(0, start, "BLOOD ACQUIRED");
	
	for(int i = 0; i < st->playerBLOOD; i++)
	{
		attron(COLOR_PAIR(COLOR_RED));
		mvaddch(1, COLS/2 - 10 + i, '|' | A_BOLD);
		attroff(COLOR_PAIR(COLOR_RED));
	}
   	if(st->playerBLOOD >= healcost && st->playerHP < st->playerMAXHP)
		mvaddstr(0, COLS/2 - 20, "C <- HEAL");
   	if(st->playerBLOOD == ultcost)
   	{
		mvaddstr(0, COLS/2 + 9, "ULTI -> X");
   	}
}

void drawTM(STATE *st)
{
		int i;
	mvaddstr(0, COLS-12, "HOLY TEXTS");	//usa os primeiros 3 indices
	for (i = 0; i < 3; i++)
	{
		attron(COLOR_PAIR(COLOR_BLACK));
		mvaddch(1, COLS-9 +i, 'o');
		attroff(COLOR_PAIR(COLOR_BLACK));
	}
	for (i = 0; i < st->playerTM; i++)
	{
		attron(COLOR_PAIR(COLOR_BLUE));
		mvaddch(1, COLS-9 +i, 'o' | A_BOLD);
		attroff(COLOR_PAIR(COLOR_BLUE));
	}
}


void itemcollect(STATE *st, int mapData[LINES][COLS])		//só o player e que apanha itens, mesmo a espada estando por cima, ela n apanha
{	
	if(mapData[st->playerX][st->playerY] == 6 && st->playerMAXHP < 35)		//meti cap para a vida maxima que pode ter é de 35
	{																		//apesar que se for para o boss o mais rapido possivel so ira ter 29 no max
		st->playerMAXHP++;
		mapData[st->playerX][st->playerY] = 0;
	}
	if(mapData[st->playerX][st->playerY] == 7 && st->playerDMG < 5)		//meti tambem cap para o dano maximo que o jogador pode dar de 5
	{																		
		st->playerDMG++;
		mapData[st->playerX][st->playerY] = 0;
	}
    if(mapData[st->playerX][st->playerY] == 10 && st->playerBLOOD < ultcost)
    {
	    st->playerBLOOD++;
		mapData[st->playerX][st->playerY] = 0;
	}
	if(mapData[st->playerX][st->playerY] == 15 && st->playerTM < 3)
    {
	    st->playerTM++;
		mapData[st->playerX][st->playerY] = 0;
	}

}

void newroom(STATE *st,int mapData[LINES][COLS], MOBS mobs[40], MOBS boss[1])
{			//se sair do mapa entao gera nova sala e volta tudo ao "inicio" dessa sala, mas com os buffs
		int i, j;
	if(st->playerTM < 3 && bosson == 0)
	{
		mob_clear(mobs);
		gerar(mapData);
		mob_spawn(mapData, mobs);
		st->playerX = 20;
		st->playerY = 0;

    	for (i = 0; i <= 1; i++)
    	{
    	    for (j = 0; j <= 7; j++)
    	    {
    	        mapData[20+i][j] = 0;     //porta por onde entra a esquerda
    	    }
    	}
	}
	else	//aqui irá acontecer a bossroom
	{
		bosson = 1;
		sightrange = 6400;
		mob_clear(mobs);
		bossroom(mapData);
		boss_spawn(mapData, boss);
		st->playerX = 30;
		st->playerY = 100;
	    st->playerHP = 60;
	    st->playerDMG = 50;
	}
}

void reset(int mapData[LINES][COLS], STATE* st, MOBS* mobs)
{
 sightrange = 400;
 bosson = 0;
 gerar(mapData);
 mob_clear(mobs);
 mob_spawn(mapData, mobs);
 *st = (STATE){20,20,20,20,1,0,0};                                                    
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

	init_pair(COLOR_CYAN,COLOR_CYAN,COLOR_BLUE);
	init_pair(COLOR_MAGENTA,COLOR_MAGENTA,COLOR_BLACK );
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);

	gerar(mapData);
    mob_spawn(mapData, mobs);
			//meti a coordenada y a 200 e os tm a 3 para testar a boss room, depois coloca-se a 20 e a 0 respetivamente, 50 dmg e 18 blood
	STATE st = {20,200,20,20,1,0,3};			//coordenada 20,20, começa com 20HP atual, 20HP max e 1DMG inicial, 0 de blood, 0 TM iniciais                                    
	int spdboss = 0;
	int i = 0;                          // alterei o hp do player para 20 para ser mais facil definir o dano dos mobs. Se o player tivesse de hp 3, ia ter de por o dano dos mobs decimal                       // caso contrario o mobs matava o player em 3 ataques o que imensamente rapido oq ue significava alterar todas as funcoes de int para float e nao me apetecia.
    int s = 0;                          // (LOPES)                                  
    int atime = 0;
	int chamamento = 0;
	
	
	while (true) 
	{
	 if (st.playerHP <= 0) 
     {
       clear();
	   int start_row = LINES/2-15;
       int start_col = COLS/2-40;
	   mvprintw(start_row++, start_col,"                               .,od88888888888bo,.                               ");
       mvprintw(start_row++, start_col,"                           .d88888888888888888888888b.                           ");
       mvprintw(start_row++, start_col,"                         .d88888888888888888888888888888b.                       ");  
       mvprintw(start_row++, start_col,"                       .d888888888888888888888888888888888b.                     ");
       mvprintw(start_row++, start_col,"                     .d8888888888888888888888888888888888888b.                   ");
       mvprintw(start_row++, start_col,"                   d88888888888888888888888888888888888888888b                   ");
       mvprintw(start_row++, start_col,"                   d8888888888888888888888888888888888888888888b                 ");
       mvprintw(start_row++, start_col,"                  d888888888888888888888888888888888888888888888                 ");
       mvprintw(start_row++, start_col,"                  8888888888888888888888888888888888888888888888                 ");
       mvprintw(start_row++, start_col,"                  8888888888888888888888888888888888888888888888                 ");
       mvprintw(start_row++, start_col,"                  8888888888888888888888888888888888888888888888                 ");
       mvprintw(start_row++, start_col,"                  Y88888888888888888888888888888888888888888888P                 ");
       mvprintw(start_row++, start_col,"                   8888888888P     Y8888888888P      Y888888888                  ");
       mvprintw(start_row++, start_col,"                   88888888P        Y88888888P        Y88888888                  ");
       mvprintw(start_row++, start_col,"                   Y8888888          ]888888P          8888888P                  ");
       mvprintw(start_row++, start_col,"                    Y888888          d888888b          888888P                   ");
       mvprintw(start_row++, start_col,"                     Y88888b        d88888888b        d88888P                    ");
       mvprintw(start_row++, start_col,"                      Y888888b.   .d88888888888b.   .d888888                     ");
       mvprintw(start_row++, start_col,"                       Y8888888888888888P Y8888888888888888                      ");
       mvprintw(start_row++, start_col,"                        888888888888888P   Y88888888888888                       ");
       mvprintw(start_row++, start_col,"                         8888888888888[     ]888888888888                        ");
       mvprintw(start_row++, start_col,"                            Y888888888888888888888888P                           ");
       mvprintw(start_row++, start_col,"                                 Y88888888888888P                                ");
       mvprintw(start_row++, start_col,"                             888b  Y8888888888P  d888                            ");
       mvprintw(start_row++, start_col,"                             888b              d888                              ");
       mvprintw(start_row++, start_col,"                              Y888bo.        .od888P       ____  _  ____ ____                   ");
       mvprintw(start_row++, start_col,"   \\  \\ /// _ \\/ \\ /\\          Y888888888888888888P       /  _ \\/ \\/  _//  _ \\                       ");
       mvprintw(start_row++, start_col,"    \\  / | / \\|| | ||            Y88888888888888P         | | \\|| || \\  | | \\|                     ");
       mvprintw(start_row++, start_col,"    / /  | \\_/|| \\_/|              Y8888888888P           | |_/|| || /_ | |_/|                      ");
       mvprintw(start_row++, start_col,"   /_/   \\____/\\____/                Y888888P             \\____/\\_/\\____\\____/       ");
       mvprintw(start_row++, start_col,"                                      ******                                     ");
       mvprintw(start_row++, start_col,"    (Q) TO QUIT                                                  (R) TO RESTART  ");
	   refresh();
       int key2 = getch();
	   switch(key2)
	   {
		 case 'q':
		  endwin(); 
		  exit(0);
			break;
	     
		 case 'r':
		 reset(mapData, &st, mobs);
	   }  
    
     }      
	 drawLight(mapData, &st);            //mas essa maneira e um bocado merda, estou a trabalhar numa forma
	 drawplayer(&st,mapData);                     // de quando o jogador morrer aparece gameover  e 2s depois o jogo reinicia
	 drawHP(&st);                         // com um novo mapa e tudo novo. (LOPES)
	 drawBLOOD(&st);
	 drawDMG(&st);
	 if (st.playerTM >= 1)
	 	drawTM(&st);
	 itemcollect(&st, mapData);
	 update(&st,mapData);
	 if(bosson == 0)mob_respawn(mapData, mobs, &st);
	 ulti_clear(&st, mapData);
	 if (st.playerY >= COLS)
	 	newroom(&st,mapData, mobs, boss);
	 refresh();
	 move(LINES - 1, 0);
	 if(bosson == 1) 
	 {
		int start_row = LINES/2-5;
        int start_col = COLS/2-70;
		if(boss[0].mobHP == 0) 
		 {
           clear();
         mvprintw(start_row++, start_col,"        @@@@@@@@@@@@@@@@@@                   			 ");
         mvprintw(start_row++, start_col,"      @@@@@@@@@@@@@@@@@@@@@@@               			 ");
         mvprintw(start_row++, start_col,"    @@@@@@@@@@@@@@@@@@@@@@@@@@@             			 ");
         mvprintw(start_row++, start_col,"   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@            			 ");
         mvprintw(start_row++, start_col,"  @@@@@@@@@@@@@@@/      \\@@@/   @     ########   #######   ######   ######     ########   ##   ########  ########       			 ");
         mvprintw(start_row++, start_col," @@@@@@@@@@@@@@@@\\      @@  @___@     ##     ## ##     ## ##    ## ##    ##    ##     ##  ##   ##        ##     ##      			 ");
         mvprintw(start_row++, start_col," @@@@@@@@@@@@@ @@@@@@@@@@  | \\@@@@@   ##     ## ##     ## ##       ##          ##     ##  ##   ##        ##     ##      			 ");
         mvprintw(start_row++, start_col," @@@@@@@@@@@@@ @@@@@@@@@\\__@_/@@@@@   ########  ##     ##  ######   ######     ##     ##  ##   ######    ##     ##                          ");
         mvprintw(start_row++, start_col,"   @@@@@@@@@@@@@@@/,/,/./'/_|.\'\\,\\    ##     ## ##     ##       ##       ##    ##     ##  ##   ##       ##     ##                          ");
         mvprintw(start_row++, start_col,"     @@@@@@@@@@@@@|  | | | | | | |    ##     ## ##     ## ##    ## ##    ##    ##     ##  ##   ##        ##     ##                          ");
         mvprintw(start_row++, start_col,"                   \\_|_|_|_|_|_|_|_|  ########   #######   ######   ######     ########   ##   ########  ########                           ");
		   refresh();
		   sleep(50);
		   reset(mapData, &st, mobs);
		 }
	 }
	 i++;
	 spdboss++;
	 s++;
	 atime++;
	 atime2++;
    
     if(atime2 == 6000) 
	 {
       atime2 = 0 ,uon = 0;
	   
	 }
    
	 if(atime == 9000)           
	 {
		aon = 0;
		atime = 0;
		
	 }
	 if(s == 10000 && bosson == 0) // 13500 voltas = +/- 1 segundo --> Ou seja o jogador leva 1 de dano a cada segundo. (LOPES)
	 {

	  mob_attack(&st, mobs);
	  s = 0;
	 }
	 
	 if(spdboss == 800) boss_movement(mapData, boss, st.playerX, st.playerY), spdboss = 0;
	
	 if(i == 2800)  
	 {
		if(bosson == 1) 
		{
          boss_attack(&st, boss), i = 0;	  
		}
		
		
		if(bosson == 0)
		{
		 mob_movement(mapData, mobs, st.playerX, st.playerY);
		 chamamento++;
		 if(chamamento >= 12) 
		 {
			chamamento = 0;
			grito = 0;
		 }
		 i = 0;
		}
	 }
	
	}

	return 0;
	
}

/* Coisas a fazer:

 Se houver tempo:

 Acrescentar novos mobs
e tc

q



*/