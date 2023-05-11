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
int uon = 0;
int atime2 = 0;
int grito = 0;
int ultcost = 18;
int healcost = 6;
int boss = 0;	//verifica se ele esta na bossroom ou não
int sightrange = 400; 	//distancia que o player consegue ver


MOBS mobs[41]; 

void mob_spawn(int mapData[LINES][COLS], MOBS mobs[41])
{
    for (int i = 0; i < 40; i++) {
	   int rnd = rand() % 6;
	   if(rnd == 0 || rnd == 1 || rnd == 2 || rnd == 3 || rnd == 4)
	   { 
		mobs[i].mobtype = 'e';
		mobs[i].mobDMG = 1;
        mobs[i].mobHP = 3;                                       //Esta funcao spawna os mobs random
	   }
	   else if(rnd  == 5) 
	   {
		mobs[i].mobtype = 'c';
		mobs[i].mobDMG = 1;   // 25% de chance de spawnar uma cobarde
        mobs[i].mobHP = 1;
	   }
		do {
			mobs[i].mobX = rand() % LINES;                       //ja meti para ficar longe do spawn, ta mesmo aqui abaixo (20,20) sendo o spawn do player (joao)         
            mobs[i].mobY = rand() % COLS;
        } while (mapData[mobs[i].mobX][mobs[i].mobY] != 0 ||sqrt(pow(mobs[i].mobX - 20, 2) + pow(mobs[i].mobY - 20, 2)) < 10);
       
	   if(mobs[i].mobtype == 'e')mapData[mobs[i].mobX][mobs[i].mobY] = 8;
	   else if(mobs[i].mobtype == 'c')mapData[mobs[i].mobX][mobs[i].mobY] = 12;
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

void boss_spawn(int mapData[LINES][COLS], MOBS mobs[41])		//guardar a boss estelita como inimigo nr 41 que normalmente n leva spawn nem respawn
{
	mobs[40].mobtype = 'E';
	mobs[40].mobDMG = 5;
	mobs[40].mobHP = 100;		//epah talvez um pouco dificil, n sei
	do
	{
		mobs[40].mobX = rand() % LINES;
        mobs[40].mobY = rand() % COLS;
    } while (mapData[mobs[40].mobX][mobs[40].mobY] != 0 ||sqrt(pow(mobs[40].mobX - 30, 2) + pow(mobs[40].mobY - 100, 2)) < 10);

	mapData[mobs[40].mobX][mobs[40].mobY] = 20;
}

void mob_respawn(int mapData[LINES][COLS], MOBS mobs[41], STATE* st)
{
		int i;
	for(i = 0; i < 40; i++)
	{
		if(mobs[i].mobHP <= 0)
		{
			mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			if (boss == 0)
			{
				if(rand() % 3 == 0)
				{	mobs[i].mobtype = 'e';
					mobs[i].mobDMG = 1;
        			mobs[i].mobHP = 3;  	//cria os status para novo mob
				}
				else if(rand() % 3 == 1)
				{
					mobs[i].mobtype = 'c';
					mobs[i].mobDMG = 1;
        			mobs[i].mobHP = 1; 
				}
				do 
				{
					mobs[i].mobX = rand() % LINES;        
            		mobs[i].mobY = rand() % COLS;
        		}
				while (mapData[mobs[i].mobX][mobs[i].mobY] != 0 ||sqrt(pow(mobs[i].mobX - st->playerX, 2) + pow(mobs[i].mobY - st->playerY, 2)) < 10);
	
				if(mobs[i].mobtype == 'e')mapData[mobs[i].mobX][mobs[i].mobY] = 8;
	        	else if(mobs[i].mobtype == 'c')mapData[mobs[i].mobX][mobs[i].mobY] = 12;
			}
		}
	}

	//aqui isto ve continuamente se ha algum mob morto e se ha entao usa a mesma cena de spawnar mas para 10 casas ou mais de distancia da posiçao atual do jogador
}


void mob_movement(int mapData[LINES][COLS], MOBS mobs[41], int playerX, int playerY)
{
  
  for(int i = 0; i < 41; i++)
  {
    int dx = mobs[i].mobX - playerX;
    int dy = mobs[i].mobY - playerY;
    double dist_squared = sqrt(dx*dx + dy*dy);
    bool xminus = (mapData[mobs[i].mobX - 1][mobs[i].mobY] == 0 || mapData[mobs[i].mobX - 1][mobs[i].mobY] == 10);
	bool yminus = (mapData[mobs[i].mobX][mobs[i].mobY - 1] == 0 || mapData[mobs[i].mobX][mobs[i].mobY - 1] == 10);
    bool xplus = (mapData[mobs[i].mobX + 1][mobs[i].mobY] == 0 || mapData[mobs[i].mobX + 1][mobs[i].mobY] == 10);
	bool yplus = (mapData[mobs[i].mobX][mobs[i].mobY + 1] == 0 || mapData[mobs[i].mobX][mobs[i].mobY + 1] == 10);
	if(dist_squared < 12 || grito == 1)
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

      if(wallFound == 1 && (grito == 0 || dist_squared > 40))
      {
        
        int s = rand() % 4;

        if(s == 0 && (yminus))
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobY--;
        }
        else if(s == 1 && (xplus))
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobX++;
        }
        else if(s == 2 && (yplus))
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobY++;
        }
        else if(s == 3 && (xminus))
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobX--;
        }

        if(mobs[i].mobtype == 'e')mapData[mobs[i].mobX][mobs[i].mobY] = 8;
	    else if(mobs[i].mobtype == 'c')mapData[mobs[i].mobX][mobs[i].mobY] = 12;	
		
      }
      else
	  {
	   
	   
	   if(mobs[i].mobtype == 'e' || mobs[i].mobtype == 'E')
	   {
		if(mobs[i].mobX > playerX)
		{
			if(mobs[i].mobY > playerY)
			{
			  if(mobs[i].mobX > playerX + 1 || mobs[i].mobY > playerY + 2) mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			  if(mobs[i].mobX > playerX + 1 && xminus) mobs[i].mobX--;
			  if(mobs[i].mobY > playerY + 2 && (yminus)) mobs[i].mobY--;                                                             // Os mobs agora ja nao vao para a mesma casa do player.
			}                                                                                                          
		    else if(mobs[i].mobY < playerY)                         
		    {                                                                                                            
			  if((mobs[i].mobY < playerY - 2 || mobs[i].mobX > playerX + 1) && (xminus || yplus)) mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			  if(mobs[i].mobX > playerX + 1 && xminus) mobs[i].mobX--;
			  if(mobs[i].mobY < playerY - 2 && yplus)mobs[i].mobY++;
		    }
		   else if(xminus)
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
			if(mobs[i].mobY > playerY)
			{
			 if((mobs[i].mobX < playerX - 1 || mobs[i].mobY > playerY + 2) && (xplus || yminus)) mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			 if(mobs[i].mobX < playerX - 1 && xplus) mobs[i].mobX++;
			 if( mobs[i].mobY > playerY + 2 && yminus) mobs[i].mobY--;
			}
		    else if(mobs[i].mobY < playerY)
		    {
			  if((mobs[i].mobY < playerY - 2 || mobs[i].mobX < playerX + 1) && (xplus || yplus)) mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			  if(mobs[i].mobX < playerX + 1 && xplus) mobs[i].mobX++;
			  if(mobs[i].mobY < playerY - 2 && yplus) mobs[i].mobY++;
		    }
		   else if((xplus))
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
			if(mobs[i].mobY > playerY + 2 && (yminus))
			{
				mapData[mobs[i].mobX][mobs[i].mobY] = 0;
				mobs[i].mobY--;
			}
		    else if(mobs[i].mobY < playerY - 2 && (yplus))
		    {
			    mapData[mobs[i].mobX][mobs[i].mobY] = 0;
				mobs[i].mobY++;
		    }
		   else if(mapData[mobs[i].mobX][mobs[i].mobY] == 0)
		   {
               continue;
		   }
		}
	      if(mobs[i].mobtype == 'e')
		  	mapData[mobs[i].mobX][mobs[i].mobY] = 8;
		  else
		  	mapData[mobs[i].mobX][mobs[i].mobY] = 20;
	   }
	   else if(mobs[i].mobtype == 'c')
	   {
		   grito = 1;
		   int nmobs = 0;
		   for(int j = -8; j <=8; j++)
           {
              for(int k = -8; k <= 8; k++)
              {
                int distanciacentro= sqrt(j*j + k*k);
                if (distanciacentro <= 8)
                {
                 int x = mobs[i].mobX + j;
                 int y = mobs[i].mobY + k;
                 
				 if(mapData[x][y] == 8 || mapData[x][y] == 12) nmobs++;
                }
              }
           }    
		 if(nmobs < 3 || (grito == 1 && dist_squared > 40)) 
	   	 {	
		    if(mobs[i].mobX > playerX)
			{
				if(mobs[i].mobY > playerY)
				{
				if((mobs[i].mobX > playerX || mobs[i].mobY > playerY) && (xplus || yplus)) mapData[mobs[i].mobX][mobs[i].mobY] = 0;
				if(mobs[i].mobX > playerX && xplus) mobs[i].mobX++;
				if(mobs[i].mobY > playerY && yplus) mobs[i].mobY++;                                                             // Os mobs agora ja nao vao para a mesma casa do player.
				}                                                                                                             // Mas agora que penso bastava so dar um numero para o player no mapData
				else if(mobs[i].mobY < playerY)                          // e a funcao ja funcionava nao tinha the acrescentar aqueles ifs todos
				{                                                                                                            // Se alguem quiser fazer esse pequeno upgrade eu agradecia(LOPES)
				if((mobs[i].mobY < playerY  || mobs[i].mobX > playerX) && (xplus || yminus)) mapData[mobs[i].mobX][mobs[i].mobY] = 0;
				if(mobs[i].mobX > playerX && xplus) mobs[i].mobX++;
				if(mobs[i].mobY < playerY && yminus)mobs[i].mobY--;
				}
			    else if(xplus)
			    {
				if(mobs[i].mobX > playerX )
				{
				mapData[mobs[i].mobX][mobs[i].mobY] = 0;
				mobs[i].mobX++;
				int move = rand()%2;
				if((move == 0 || mobs[i].mobY - 1 == 1) && yplus) mobs[i].mobY++;
				else if((move == 1 || mobs[i].mobY + 1 == 1) && yminus) mobs[i].mobY--;
				} 
			    }
			}
			else if(mobs[i].mobX < playerX)
			{ 
				if(mobs[i].mobY > playerY)
				{
				if((mobs[i].mobX < playerX || mobs[i].mobY > playerY) && (xminus || yplus)) mapData[mobs[i].mobX][mobs[i].mobY] = 0;
				if(mobs[i].mobX < playerX && xminus) mobs[i].mobX--;
				if( mobs[i].mobY > playerY && yplus) mobs[i].mobY++;
				}
				else if(mobs[i].mobY < playerY)
				{
				if((mobs[i].mobY < playerY || mobs[i].mobX < playerX) && (xminus || yminus))  mapData[mobs[i].mobX][mobs[i].mobY] = 0;
				if(mobs[i].mobX < playerX && xminus) mobs[i].mobX--;
				if(mobs[i].mobY < playerY && yminus ) mobs[i].mobY--;
				}
			    else if(xminus)
			    {
				if(mobs[i].mobX < playerX) 
				{
				mapData[mobs[i].mobX][mobs[i].mobY] = 0;
				mobs[i].mobX--;
				int move = rand()%2;
				if((move == 0 || mobs[i].mobY - 1 == 1) && yplus) mobs[i].mobY++;
				else if((move == 1 || mobs[i].mobY + 1 == 1) && yminus) mobs[i].mobY--;
				}  
			}
			}
			else if(mobs[i].mobX == playerX)
			{
				if((mobs[i].mobY > playerY || mobs[i].mobY - 1 == 1) && (yplus))
				{
					mapData[mobs[i].mobX][mobs[i].mobY] = 0;
					mobs[i].mobY++;
					int move = rand()%2;
					if((move == 0 || mobs[i].mobX - 1 == 1) && yplus) mobs[i].mobX++;
					else if((move == 1 || mobs[i].mobX + 1 == 1) && yplus) mobs[i].mobX--;
				}
				else if((mobs[i].mobY < playerY || mobs[i].mobY+1 == 1) && (yminus))
				{
					mapData[mobs[i].mobX][mobs[i].mobY] = 0;
					mobs[i].mobY--;
					int move = rand()%2;
					if((move == 0 || mobs[i].mobX - 1 == 1) && xplus) mobs[i].mobX++;
					else if((move == 1 || mobs[i].mobX + 1 == 1) && xminus) mobs[i].mobX--;
				}
			else if(mapData[mobs[i].mobX][mobs[i].mobY] == 0)
			{
				continue;
			}
			}
			mapData[mobs[i].mobX][mobs[i].mobY] = 12;
		 }
		 else
		 {
		   if(mobs[i].mobX > playerX)
		   {
			if(mobs[i].mobY > playerY)
			{
			  if((mobs[i].mobX > playerX + 1 || mobs[i].mobY > playerY + 2) && (xminus || yminus)) mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			  if(mobs[i].mobX > playerX + 1 && xminus) mobs[i].mobX--;
			  if(mobs[i].mobY > playerY + 2 && yminus) mobs[i].mobY--;                                                             
			}                                                                                                            
		    else if(mobs[i].mobY < playerY && (xminus || yplus))                      
		    {                                                                                                          
			  if(mobs[i].mobY < playerY - 2 || mobs[i].mobX > playerX + 1) mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			  if(mobs[i].mobX > playerX + 1 && xminus) mobs[i].mobX--;
			  if(mobs[i].mobY < playerY - 2 && yplus) mobs[i].mobY++;
		    }
		   else if(xminus)
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
			if(mobs[i].mobY > playerY)
			{
			 if((mobs[i].mobX < playerX - 1 || mobs[i].mobY > playerY + 2) && (xplus || yminus)) mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			 if(mobs[i].mobX < playerX - 1 && xplus) mobs[i].mobX++;
			 if( mobs[i].mobY > playerY + 2 && yminus) mobs[i].mobY--;
			}
		    else if(mobs[i].mobY < playerY)
		    {
			  if((mobs[i].mobY < playerY - 2|| mobs[i].mobX < playerX + 1) && (xplus || yplus))  mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			  if(mobs[i].mobX < playerX + 1 && xplus) mobs[i].mobX++;
			  if(mobs[i].mobY < playerY - 2 && yplus) mobs[i].mobY++;
		    }
		   else if((xplus))
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
			if(mobs[i].mobY > playerY + 2 && (yminus))
			{
				mapData[mobs[i].mobX][mobs[i].mobY] = 0;
				mobs[i].mobY--;
			}
		    else if(mobs[i].mobY < playerY - 2 && (yplus))
		    {
			    mapData[mobs[i].mobX][mobs[i].mobY] = 0;
				mobs[i].mobY++;
		    }
		   else if(mapData[mobs[i].mobX][mobs[i].mobY] == 0)
		   {
               continue;
		   }
		  }
	      mapData[mobs[i].mobX][mobs[i].mobY] = 12;	
		 }
		}
	  }
	}
    else 
	{
	  
	  int s2 = rand() % 4;

        if(s2 == 0 && yminus)
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobY--;
        }
        else if(s2 == 1 && xplus)
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobX++;
        }
        else if(s2 == 2 && yplus)
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobY++;
        }
        else if(s2 == 3 && xminus)
        {
          mapData[mobs[i].mobX][mobs[i].mobY] = 0;
          mobs[i].mobX--;
        }

         if(mobs[i].mobtype == 'e')mapData[mobs[i].mobX][mobs[i].mobY] = 8;
	     else if(mobs[i].mobtype == 'c')mapData[mobs[i].mobX][mobs[i].mobY] = 12;	
			
	}
  }
}

void mob_attack(STATE *st, MOBS mobs[41]) 
{
    for (int i = 0; i < 41; i++)                      // Esta funcao e respondavel por retirar hp do jogador													 
	{                                                // quando um mob se encontra perto. Mas com esta 
     int dx = mobs[i].mobX - st->playerX;            // abordagem todos os mobs e um bloco do jogador o conseguem atacar ao mesmo tempo.
     int dy = mobs[i].mobY - st->playerY;            // Da maneira atual no maximo o jogador leva 9 de dano por segundo se estiver rodeado por mobs.(LOPES)
     double distancia = sqrt(dx * dx + dy * dy);     
     if (distancia <= sqrt(8)) 		
	 {
       st->playerHP -= mobs[i].mobDMG;
     }
    }
}

void player_attack(STATE *st, MOBS mobs[41],int mapData[LINES][COLS])		//->SPACEBAR atk
{
		int i, j, k;
	if (r == 1)
	{
		for (i = st->playerX-1; i <= st->playerX+1; i++)		
		{
			for (j = st->playerY-2; j <= st->playerY+2; j++)		//a coluna do jogador e a coluna a sua direita
			{
				for (k = 0; k < 41; k++)		//percorre a lista dos mobs que possam estar nestas coordenadas
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
				for (k = 0; k < 41; k++)		//percorre a lista dos mobs que possam estar nestas coordenadas
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

void player_ulti(STATE *st, MOBS mobs[41], int mapData[LINES][COLS])
{ 
  for(int i = 0; i < 41; i++)
  {
    int dx = mobs[i].mobX - st->playerX;
    int dy = mobs[i].mobY - st->playerY;
    double distancia = sqrt(dx * dx + dy * dy);

    if(distancia <= 6)
    {
        mobs[i].mobHP -= st->playerDMG+5;		//dantes estava 5 fixo mas agora escala com os boosts que apanhas
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
				r = 0;
				do_movement_action(st, -1, -1, mapData);
				break;
		case KEY_UP:
			case '8':
				aon = 0;
				do_movement_action(st, -1, +0, mapData);
				break;
		case KEY_A3:
			case '9':
				aon = 0;
				r = 1;
				do_movement_action(st, -1, +1, mapData);
				break;
		case KEY_LEFT:
			case '4': 
				aon = 0;
			    r = 0;
				do_movement_action(st, +0, -1, mapData);
				break;
		case KEY_B2:
			case '5':
				break;
		case KEY_RIGHT:
			case '6': 
				aon = 0;
				r = 1;
				do_movement_action(st, +0, +1, mapData);
			    break;
		case KEY_C1:
			case '1': 
				aon = 0;
				r = 0;
				do_movement_action(st, +1, -1, mapData);
				break;
		case KEY_DOWN:
			case '2': 
				aon = 0;
				do_movement_action(st, +1, +0, mapData);
				break;
		case KEY_C3:
			case '3': 
				aon = 0;
				r = 1;
				do_movement_action(st, +1, +1, mapData);
				break;
		case 'z':
			aon = 0;
			aon = 1;
			player_attack(st, mobs, mapData);
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

void newroom(STATE *st,int mapData[LINES][COLS], MOBS mobs[40])
{			//se sair do mapa entao gera nova sala e volta tudo ao "inicio" dessa sala, mas com os buffs
		int i, j;
	if(st->playerTM < 3)
	{
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
		boss = 1;
		sightrange = 6400;
		bossroom(mapData);
		boss_spawn(mapData, mobs);
		st->playerX = 30;
		st->playerY = 100;
	}
}

void reset(int mapData[LINES][COLS], STATE* st, MOBS* mobs)
{
 gerar(mapData);
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
			//meti a coordenada y a 200 e os tm a 3 para testar a boss room, depois coloca-se a 20 e a 0 respetivamente
	STATE st = {20,200,20,20,1,0,3};			//coordenada 20,20, começa com 20HP atual, 20HP max e 1DMG inicial, 0 de blood, 0 TM iniciais                                    
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
       mvprintw(start_row++, start_col,"                              Y888bo.        .od888P                             ");
       mvprintw(start_row++, start_col,"                               Y888888888888888888P                              ");
       mvprintw(start_row++, start_col,"                                 Y88888888888888P                                ");
       mvprintw(start_row++, start_col,"                                   Y8888888888P                                  ");
       mvprintw(start_row++, start_col,"                                     Y888888P                                    ");
       mvprintw(start_row++, start_col,"                                       *****                                     ");
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
	 mob_respawn(mapData, mobs, &st);
	 ulti_clear(&st, mapData);
	 if (st.playerY >= COLS)
	 	newroom(&st,mapData, mobs);
	 refresh();
	 move(LINES - 1, 0);
	 i++;
	 s++;
	 atime++;
	 atime2++;
     

     if(atime2 == 6000) atime2 = 0 ,uon = 0;

	 if(atime == 9000)           
	 {
		aon = 0;
		atime = 0;
	 }
	 if(s == 10000) // 13500 voltas = +/- 1 segundo --> Ou seja o jogador leva 1 de dano a cada segundo. (LOPES)
	 {

	  mob_attack(&st, mobs);
	  s = 0;
	 }
	 if(i == 2800)  
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

	return 0;
}

/* Coisas a fazer:

 Se houver tempo:

 Acrescentar novos mobs
e tc

q



*/