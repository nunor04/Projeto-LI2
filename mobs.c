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

void mob_spawn(int mapData[LINES][COLS], MOBS mobs[40], int pfloor)
{
	for (int i = 0; i < 40; i++) {
	   int rnd = rand() % 6;
	   if(rnd == 0 || rnd == 1 || rnd == 2 || rnd == 3 || rnd == 4)
	   { 
		mobs[i].mobtype = 'e';
		mobs[i].mobDMG = 1 + pfloor;
        mobs[i].mobHP = 3 + pfloor;                                       //Esta funcao spawna os mobs random
	   }
	   else if(rnd  == 5) 
	   {
		mobs[i].mobtype = 'c';
		mobs[i].mobDMG = 1 + pfloor;   // 25% de chance de spawnar uma cobarde
        mobs[i].mobHP = 1 + pfloor;
	   }
		do {
			mobs[i].mobX = rand() % LINES;                       //ja meti para ficar longe do spawn, ta mesmo aqui abaixo (20,20) sendo o spawn do player (joao)         
            mobs[i].mobY = rand() % COLS;
        } while (mapData[mobs[i].mobX][mobs[i].mobY] != 0 ||sqrt(pow(mobs[i].mobX - 20, 2) + pow(mobs[i].mobY - 20, 2)) < 10);

	   if(mobs[i].mobtype == 'e')mapData[mobs[i].mobX][mobs[i].mobY] = 8;
	   else if(mobs[i].mobtype == 'c')mapData[mobs[i].mobX][mobs[i].mobY] = 12;
	}
}

void mob_respawn(int mapData[LINES][COLS], MOBS mobs[40], STATE* st, int pfloor, int ngp)
{
		int i, k;
	for(i = 0; i < 40; i++)
	{
		if(mobs[i].mobHP <= 0)
		{
			k = rand();
			if ((k % 15 == 1 || k % 15 == 2) && st->playerMAXHP < 34 + 6*ngp)
				mapData[mobs[i].mobX][mobs[i].mobY] = 6;	//1/10 de spawnar heal
			else if (k % 15 == 0 && st->playerDMG < 5 + ngp)
				mapData[mobs[i].mobX][mobs[i].mobY] = 7;	//1/15 de spawnar dmg
			else
				mapData[mobs[i].mobX][mobs[i].mobY] = 0;
			
			if (bosson == 0)		//n queremos tar a spawnar bichanada na sala do boss
			{
				if(rand() % 3 == 0)
				{	mobs[i].mobtype = 'e';
					mobs[i].mobDMG = 1 + pfloor;
        			mobs[i].mobHP = 3 + pfloor;  	//cria os status para novo mob
				}
				else if(rand() % 3 == 1)
				{
					mobs[i].mobtype = 'c';
					mobs[i].mobDMG = 1 + pfloor;
        			mobs[i].mobHP = 1 + pfloor; 
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

void mob_clear(MOBS mobs[40])
{
		int i;		//necessita da mob_respawn ativa para funcionar direito
	for (i = 0; i < 40; i++)
		mobs[i].mobHP = 0;
}

void mob_movement(int mapData[LINES][COLS], MOBS mobs[40], int playerX, int playerY)
{ 
	 for(int i = 0; i < 40; i++)
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
	   
	   
	   if(mobs[i].mobtype == 'e')
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
		  	mapData[mobs[i].mobX][mobs[i].mobY] = 8;
		  
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

void mob_attack(STATE *st, MOBS mobs[40]) 
{
 
   for (int i = 0; i < 40; i++)                      // Esta funcao e respondavel por retirar hp do jogador													 
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

void boss_spawn(int mapData[LINES][COLS], MOBS boss[1], int ngp)		//guardar a boss estelita como inimigo nr 41 que normalmente n leva spawn nem respawn
{
	if (ngp == 0)
	{
		boss[0].mobtype = 'E';
		boss[0].mobDMG = 5;
		boss[0].mobHP = 150;		//epah talvez um pouco dificil, n sei
	}
	else
	{
		boss[0].mobtype = 'E';
		boss[0].mobDMG = 10;
		boss[0].mobHP = 250;
	}
	do
	{
		boss[0].mobX = rand() % LINES;
        boss[0].mobY = rand() % COLS;
    } while (mapData[boss[0].mobX][boss[0].mobY] != 0 ||sqrt(pow(boss[0].mobX - 30, 2) + pow(boss[0].mobY - 100, 2)) < 10);

	mapData[boss[0].mobX][boss[0].mobY] = 20;
}

void boss_attack(STATE *st, MOBS boss[1])
{
	                    												 
	{                                                
     int dx = boss[0].mobX - st->playerX;            
     int dy = boss[0].mobY - st->playerY;            
     double distancia = sqrt(dx * dx + dy * dy);     
     if (distancia <= sqrt(8)) 		
	 {
       st->playerHP -= boss[0].mobDMG;
     }
    }
  
}

void boss_movement(int mapData[LINES][COLS], MOBS boss[1], int playerX, int playerY)
{
	bool xminus = (mapData[boss[0].mobX - 1][boss[0].mobY] == 0 || mapData[boss[0].mobX - 1][boss[0].mobY] == 10 || mapData[boss[0].mobX - 1][boss[0].mobY] == 21);
	bool yminus = (mapData[boss[0].mobX][boss[0].mobY - 1] == 0 || mapData[boss[0].mobX][boss[0].mobY - 1] == 10 || mapData[boss[0].mobX - 1][boss[0].mobY] == 21);
    bool xplus = (mapData[boss[0].mobX + 1][boss[0].mobY] == 0 || mapData[boss[0].mobX + 1][boss[0].mobY] == 10 || mapData[boss[0].mobX - 1][boss[0].mobY] == 21);
	bool yplus = (mapData[boss[0].mobX][boss[0].mobY + 1] == 0 || mapData[boss[0].mobX][boss[0].mobY + 1] == 10 || mapData[boss[0].mobX - 1][boss[0].mobY] == 21);
	
	if(boss[0].mobX > playerX)
		{
			if(boss[0].mobY > playerY)
			{
			  if(boss[0].mobX > playerX + 1 || boss[0].mobY > playerY + 2) mapData[boss[0].mobX][boss[0].mobY] = 0;
			  if(boss[0].mobX > playerX + 1 && xminus) boss[0].mobX--;
			  if(boss[0].mobY > playerY + 2 && (yminus)) boss[0].mobY--;                                                             // Os mobs agora ja nao vao para a mesma casa do player.
			}                                                                                                          
		    else if(boss[0].mobY < playerY)                         
		    {                                                                                                            
			  if((boss[0].mobY < playerY - 2 || boss[0].mobX > playerX + 1) && (xminus || yplus)) mapData[boss[0].mobX][boss[0].mobY] = 0;
			  if(boss[0].mobX > playerX + 1 && xminus) boss[0].mobX--;
			  if(boss[0].mobY < playerY - 2 && yplus)boss[0].mobY++;
		    }
		   else if(xminus)
		   {
             if(boss[0].mobX > playerX + 1)
			 {
               mapData[boss[0].mobX][boss[0].mobY] = 0;
			   boss[0].mobX--;
			 } 
		   }
		}
	    else if(boss[0].mobX < playerX)
		{
			if(boss[0].mobY > playerY)
			{
			 if((boss[0].mobX < playerX - 1 || boss[0].mobY > playerY + 2) && (xplus || yminus)) mapData[boss[0].mobX][boss[0].mobY] = 0;
			 if(boss[0].mobX < playerX - 1 && xplus) boss[0].mobX++;
			 if( boss[0].mobY > playerY + 2 && yminus) boss[0].mobY--;
			}
		    else if(boss[0].mobY < playerY)
		    {
			  if((boss[0].mobY < playerY - 2 || boss[0].mobX < playerX + 1) && (xplus || yplus)) mapData[boss[0].mobX][boss[0].mobY] = 0;
			  if(boss[0].mobX < playerX + 1 && xplus) boss[0].mobX++;
			  if(boss[0].mobY < playerY - 2 && yplus) boss[0].mobY++;
		    }
		   else if((xplus))
		   {
             if(boss[0].mobX < playerX - 1) 
			 {
               mapData[boss[0].mobX][boss[0].mobY] = 0;
			   boss[0].mobX++;
			 }  
		   }
		}
	    else if(boss[0].mobX == playerX)
		{
			if(boss[0].mobY > playerY + 2 && (yminus))
			{
				mapData[boss[0].mobX][boss[0].mobY] = 0;
				boss[0].mobY--;
			}
		    else if(boss[0].mobY < playerY - 2 && (yplus))
		    {
			    mapData[boss[0].mobX][boss[0].mobY] = 0;
				boss[0].mobY++;
		    }
		}
		  	mapData[boss[0].mobX][boss[0].mobY] = 20;
}

void boss_ulti(STATE *st, MOBS boss[1], int mapData[LINES][COLS])
{
	int dx = boss[0].mobX - st->playerX;
    int dy = boss[0].mobY - st->playerY;
    double distancia = sqrt(dx * dx + dy * dy);
  if(distancia >= 4 && rand()%6 == 1)	
  {
	bulton = 1;
	udx = st->playerX;
	udy = st->playerY;
	for(int j = -1; j <= 1; j++)
    {
	  for(int k = -1; k <= 1; k++)
      {
        int distanciacentro= sqrt(j*j + k*k);
        if (distanciacentro <= 2)
        {
		  int x = udx + j;
          int y = udy + k;
          if (x >= 0 && x < LINES && y >= 0 && y < COLS && mapData[x][y] == 0)
          {
			  mapData[x][y] = 22;
          }
        }
      }
    }
  }
}

void boss_clear(STATE *st, int mapData[LINES][COLS])
{
 if(bulton == 1)
  {
	int dx = udx - st->playerX;
    int dy = udy - st->playerY;
    double distancia = sqrt(dx * dx + dy * dy);
	
	if(distancia <= 2)st->playerHP -= 5;
	
	for (int i = udx- 1; i <= udx + 1; i++)
    {
        for (int j = udy - 1; j <= udy + 1; j++)
        {
            if (mapData[i][j] == 22)
                mapData[i][j] = 0;
        }
    }
   bulton = 0;
  }
}