#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

#define SIZE 9

int sudoku[SIZE][SIZE] = {
    {5,3,4,6,7,8,9,1,2},
    {6,7,2,1,9,5,3,4,8},
    {1,9,8,3,4,2,5,6,7},
    {8,5,9,7,6,1,4,2,3},
    {4,2,6,8,5,3,7,9,1},
    {7,1,3,9,2,4,8,5,6},
    {9,6,1,5,3,7,2,8,4},
    {2,8,7,4,1,9,6,3,5},
    {3,4,5,2,8,6,1,7,9}
};

int check_row(int row) {
    int seen[10] = {0};
    for(int i = 0; i < SIZE; i++) {
        int num = sudoku[row][i];
        if(seen[num]) return 0;
        seen[num] = 1;
    }
    return 1;
}

int check_column(int col) {
    int seen[10] = {0};
    for(int i = 0; i < SIZE; i++) {
        int num = sudoku[i][col];
        if(seen[num]) return 0;
        seen[num] = 1;
    }
    return 1;
}

int main() {
    for(int i = 0; i < SIZE; i++) {
        if(!check_row(i) || !check_column(i)) {
            printf("Sudoku inválido\n");
            return 0;
        }
    }

    printf("Sudoku válido\n");
    return 0;
}