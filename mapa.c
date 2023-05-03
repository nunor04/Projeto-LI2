#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include <errno.h>

#define playerX 20
#define playerY 20

/* 
codigo de matriz para armazenar as informações do mapa
    0 -> espaço em branco '.';
    1 -> parede '#';
    2 -> espaço de dano ambiental (lava, veneno,...) '~';

    3 -> espaço de dano do jogador (sem display);
    4 -> espaço de dano do inimigo (sem display);   >>> estes dois nao sei se se manterão, acho que apenas utilizarei raio da coordenada de cada para determinar dano

    5 -> item de cura, restaura 1HP ao jogador '+';
    6 -> item que aumenta playerMAXHP por 1 'h';
    7 -> item que aumenta playerDMG por 1 'd';
*/

void gerar(int mapData[LINES][COLS])
{
        int row, col,
            xwall_size, ywall_size, start_row, start_col,
            i, j,
            count = 0;
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
}