#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>

#include "types.h"

#define playerX 20
#define playerY 20


/* 
codigo de matriz para armazenar as informações do mapa
    0 -> espaço em branco '.';
    1 -> parede '#';
    2 -> espaço de dano ambiental (lava, veneno,...) '~';


    6 -> item que aumenta playerMAXHP por 1 'h';
    7 -> item que aumenta playerDMG por 1 'd';
    8 -> mobs

    10 -> sangue
    11 -> efeito do ulti do jogador
    15 -> infinity stones, sei la man;
    20 -> "ESTELITA"
*/

void gerar(int mapData[LINES][COLS])
{
        int row, col,
            xwall_size, ywall_size, start_row, start_col,
            i, j, k, l,
            count = 0,
            xh = 1, yh = 1,
            exit = 0;
    srand(time(NULL));  //inicializa o rand

    for (row = 0; row < LINES; row++)
    {
        for (col = 0; col < COLS; col++)
        {
            if (row >= 2 && row < LINES-2 && col >= 2 && col < COLS-2)
            {
                mapData[row][col] = 0;
            }
            else
            {
                mapData[row][col] = 1;
            }
        }
    }
        //preenche a tela toda, exceto as duas linhas de fora da tela com '0' para ter o efeito desejado das bordas

    // Coloca o nr 1 aleatoriamente em grupos
     while (count < LINES * COLS * 0.4)                          // 40% de ocupação (calcula a area do ecra e depois pega em 40% disso)
    {
        xwall_size = rand() % 4 + 4;
        ywall_size = rand() % 4 + 4; 
            // tamanho aleatório do grupo, com o segundo elemento funcionando como tipo "zoom"
        start_row = rand() % LINES;
        start_col = rand() % COLS;
            // os mods(%) garantem que startrow e col cabem no ecrã porque se for o mod entao será sempre menor que maxrow e col
        // Preenche o grupo com o nr '1'
        for (i = start_row; i < start_row + xwall_size; i++)
        {
            for (j = start_col; j < start_col + ywall_size; j++)
            {
                if(sqrt(pow(i - playerX, 2) + pow(j - playerY, 2)) > 5)
                    mapData[i][j] = 1;
            }
        }

        count += xwall_size * ywall_size;
            //tira a area preenchida pelo "bloco" dos espaços possiveis para preencher
    }

    //aqui vou fazer a saida do mapa para outras "salas" que depois precisará de ser detetado na main para gerar o mapa de novo
    while(exit == 0 || exit == 1 ||exit == LINES-1 ||exit == LINES-2 || exit == LINES-3)
    {
        exit = rand() % LINES;
    }
    
    for (k = 0; k <= 1; k++)    //exit+k
    {
        for (l = COLS; l >= COLS -15; l--)    //faz uma saida de 2 casas que limpa espaço 5 casas a frente para garantir que dá para chegar lá
        {
            mapData[exit+k][l] = 0;     //parede da direita
        }
    }
    
        //estelita item time
    while(mapData[xh][yh] == 1 || mapData[xh][yh] == 6 || mapData[xh][yh] == 7 || sqrt(pow(xh - playerX, 2) + pow(yh - playerY, 2)) < 30)     
            //so para o loop quando as coordenadas escolhidas não estão dentro de uma parede
    {
        xh = rand() % LINES;    //vai randomizando as coordenadas do item até estarem acessíveis ao jogador
        yh = rand() % COLS;
    }
    mapData[xh][yh] = 15;   //1 TM por mapa, yh vai ser dificil ter o 3
    xh = 0, yh = 0;

}


    //aqui e que se faz a arena do boss
void bossroom(int mapData[LINES][COLS])
{
        int row, col;
    for (row = 0; row < LINES; row++)
    {
        for (col = 0; col < COLS; col++)
        {
            if (row > LINES/3 && row < 2*(LINES/3) && col > COLS/3 && col < 2*(COLS/3))
            {
                mapData[row][col] = 0;
            }
            else
            {
                mapData[row][col] = 1;
            }
        }
    }
}




