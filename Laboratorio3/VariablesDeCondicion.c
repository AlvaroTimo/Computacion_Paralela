#include <pthread.h>
#include <stdio.h>

int counter = 0;
int thread_count;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

void* ThreadWork(void* arg) {
    pthread_mutex_lock(&mutex);
    counter++;

    if (counter == thread_count) {
        counter = 0;
        pthread_cond_broadcast(&cond_var);
    } else {
        while (pthread_cond_wait(&cond_var, &mutex) != 0);
    }

    pthread_mutex_unlock(&mutex);

    printf("All threads reached this point variables de condicion\n");
    pthread_exit(NULL);
}

int main() {
    // Inicializar variables y configurar hilos

    // Número de hilos
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

    // Liberar recursos de mutex y variable de condición
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);

    return 0;
}
