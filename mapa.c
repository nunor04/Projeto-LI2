#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>


void gerar()
{
    int row, col;
    int max_row, max_col;
    int count = 0;
    //srand(time(NULL));

    // Obtém o número máximo de linhas e colunas
    getmaxyx(stdscr, max_row, max_col);

    // Preenche o ecrã com o caractere '.'
    // for (row = 0; row < max_row; row++) {
    //     if(row==0 || row==1 || row==max_row-1 || row==max_row-2){
    //         for(col = 0; col < max_col; col++){
    //         mvaddch(row,col,'#');
    //         }
    //     }
    //     else{
    //         for (col = 0; col < max_col; col++) {
    //             if(col==0 || col==1 || col==max_col-1 || col==max_col-2){
    //                 for(row = 0; row < max_row; row++){
    //                 mvaddch(row,col,'#');
    //                 }
    //             }
    //             else{
    //                 for(row = 2; row < max_row; row++){
    //                     mvaddch(row, col, '.');
    //                 }
    //             }
    //         }
    //     }
    // }

    for(row = 0; row < max_row; row++)
    {
        for(col = 0; col < max_col; col++)
            mvaddch(row,col,'#');
    }

    for(row = 2; row < max_row-2; row++)
    {
        for(col = 2; col < max_col-2; col++)
            mvaddch(row,col,'.');
    }

    // Coloca o caractere '#' aleatoriamente em grupos
    while (count < max_row * max_col * 0.4) { // 40% de ocupação
        int group_size = rand() % 4 + 4; // tamanho aleatório do grupo
        int start_row = rand() % max_row;
        int start_col = rand() % max_col;
        int i, j;

        // Verifica se o grupo cabe no ecrã
        if (start_row + group_size >= max_row || start_col + group_size >= max_col) {
            continue;
        }

        // Preenche o grupo com o caractere '#'
        for (i = start_row; i < start_row + group_size; i++) {
            for (j = start_col; j < start_col + group_size; j++) {
                mvaddch(i, j, '#');
            }
        }

        count += group_size * group_size;
    }
}
