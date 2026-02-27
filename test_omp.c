#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        printf("Hola desde thread %d\n", omp_get_thread_num());
    }
    return 0;
}