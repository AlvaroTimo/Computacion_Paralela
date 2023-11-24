#include <pthread.h>
#include <stdio.h>

int counter = 0;
int thread_count;
pthread_mutex_t barrier_mutex = PTHREAD_MUTEX_INITIALIZER;

void* ThreadWork(void* arg) {
    pthread_mutex_lock(&barrier_mutex);
    counter++;
    pthread_mutex_unlock(&barrier_mutex);

    while (counter < thread_count);

    printf("All threads reached this point espera continua y mutex\n");
    pthread_exit(NULL);
}

int main() {
    thread_count = 5;

    pthread_t threads[thread_count];

    // Crear hilos
    for (int i = 0; i < thread_count; ++i) {
        pthread_create(&threads[i], NULL, ThreadWork, NULL);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
