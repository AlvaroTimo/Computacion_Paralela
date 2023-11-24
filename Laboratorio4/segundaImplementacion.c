#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void oddEvenSort(int a[], int n);

int main() {
    int sizes[] = {100,1000, 10000, 100000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int phase, i, tmp;
    double start_time, end_time;

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        int* a = (int*)malloc(n * sizeof(int));

        // printf("\n\nArreglo original de tamaño %d: ", n);
        for (i = 0; i < n; i++) {
            a[i] = rand() % 10000; // Llenar el arreglo con valores aleatorios
            // printf("%d ", a[i]);
        }
        // printf("\n");

        // Medición del tiempo de inicio
        start_time = omp_get_wtime();

        // Odd-Even Sort
        #pragma omp parallel num_threads(4) default(none) shared(a, n) private(i, tmp, phase)
        {
            for (phase = 0; phase < n; phase++) {
                if (phase % 2 == 0) {
                    #pragma omp for
                    for (i = 1; i < n; i += 2) {
                        if (a[i - 1] > a[i]) {
                            tmp = a[i - 1];
                            a[i - 1] = a[i];
                            a[i] = tmp;
                        }
                    }
                } else {
                    #pragma omp for
                    for (i = 1; i < n - 1; i += 2) {
                        if (a[i] > a[i + 1]) {
                            tmp = a[i + 1];
                            a[i + 1] = a[i];
                            a[i] = tmp;
                        }
                    }
                }
            }
        }

        // Medición del tiempo de finalización
        end_time = omp_get_wtime();

        // printf("Arreglo ordenado de tamaño %d: ", n);
        // for (i = 0; i < n; i++) {
        //     printf("%d ", a[i]);
        // }
        // printf("\n");

        // Imprimir tiempo de ejecución
        printf("Tiempo de ejecución para tamaño %d: %f segundos\n", n, end_time - start_time);

        free(a); // Liberar la memoria del arreglo
    }

    return 0;
}
