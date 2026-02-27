#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

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

    int index = 0;
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            grid[i][j] = map[index++] - '0';
        }
    }

    munmap(map, st.st_size);
    close(fd);

    for(int i = 0; i < 9; i += 3) {
    for(int j = 0; j < 9; j += 3) {
        if(!check_subgrid(i, j)) {
            printf("Sudoku inválido (subcuadrantes)\n");
            return 0;
        }
    }
}

printf("Subcuadrantes válidos.\n");
return 0;
}