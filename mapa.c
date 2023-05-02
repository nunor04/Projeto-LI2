#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>

#define playerX 20
#define playerY 20
/* 
codigo de matriz para armazenar as informações do mapa
    0 -> espaço em branco '.';
    1 -> parede '#';
    2 -> espaço de dano ambiental (lava, veneno,...) '~';

    3 -> espaço de dano do jogador '+';
    4 -> espaço de dano do inimigo (sem display).
*/

void gerar(int nrows, int ncols, int **mapData)
{
        int row, col,
            xwall_size, ywall_size, start_row, start_col,
            i, j,
            count = 0;
    //srand(time(NULL));

    // Obtém o número máximo de linhas e colunas
    //getmaxyx(stdscr, nrows, ncols);

    for (row = 0; row < nrows; row++)
    {
        for (col = 0; col < ncols; col++)
            mapData[row][col] = 1;
    }
        //preenche a tela toda com '1'

    for (row = 2; row < nrows-2; row++)
    {
        for (col = 2; col < ncols-2; col++)
            mapData[row][col] = 0;
    }
        //preenche a tela toda, exceto as duas linhas de fora da tela com '0' para ter o efeito desejado das bordas

    // Coloca o nr 1 aleatoriamente em grupos
     while (count < nrows * ncols * 0.4)                          // 40% de ocupação (calcula a area do ecra e depois pega em 40% disso)
    {
        xwall_size = rand() % 4 + 4;
        ywall_size = rand() % 4 + 4; 
            // tamanho aleatório do grupo, com o segundo elemento funcionando como tipo "zoom"
        start_row = rand() % nrows;
        start_col = rand() % ncols;
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