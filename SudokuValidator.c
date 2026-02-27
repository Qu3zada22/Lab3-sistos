#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <omp.h>

#define SIZE 9

int grid[SIZE][SIZE];

int check_subgrid(int row_start, int col_start) {

    int seen[10] = {0};

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {

            int num = grid[row_start + i][col_start + j];

            if(num < 1 || num > 9 || seen[num]) {
                return 0;
            }

            seen[num] = 1;
        }
    }

    return 1;
}

int check_column(int col) {

    int seen[10] = {0};

    for(int i = 0; i < SIZE; i++) {
        int num = grid[i][col];

        if(num < 1 || num > 9 || seen[num]) {
            return 0;
        }

        seen[num] = 1;
    }

    return 1;
}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("Uso: %s archivo_sudoku\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd < 0) {
        perror("Error al abrir archivo");
        return 1;
    }

    struct stat st;
    fstat(fd, &st);

    char *map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(map == MAP_FAILED) {
        perror("Error en mmap");
        close(fd);
        return 1;
    }

    int index = 0;
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            grid[i][j] = map[index++] - '0';
        }
    }

    munmap(map, st.st_size);
    close(fd);

    printf("Archivo cargado correctamente.\n");

    for(int i = 0; i < 9; i += 3) {
        for(int j = 0; j < 9; j += 3) {
            if(!check_subgrid(i, j)) {
                printf("Sudoku inválido (subcuadrantes)\n");
                return 0;
            }
        }
    }

    printf("Subcuadrantes válidos.\n");

    /* -------- OpenMP Nested -------- */

    omp_set_nested(1);
    omp_set_max_active_levels(2);
    omp_set_num_threads(2);

    #pragma omp parallel
    {
        #pragma omp parallel for
        for(int i = 0; i < SIZE; i++) {
            if(!check_column(i)) {
                printf("Columna %d inválida\n", i);
            }
        }
    }

    printf("Columnas revisadas con OpenMP nested.\n");

    sleep(30);  // Para observar LWP

    return 0;
}