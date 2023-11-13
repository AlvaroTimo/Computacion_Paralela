#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

int counter = 0;
int thread_count;
sem_t count_sem, barrier_sem;

void* ThreadWork(void* arg) {
    sem_wait(&count_sem);

    if (counter == thread_count - 1) {
        counter = 0;
        sem_post(&count_sem);

        for (int j = 0; j < thread_count - 1; j++) {
            sem_post(&barrier_sem);
        }
    } else {
        counter++;
        sem_post(&count_sem);
        sem_wait(&barrier_sem);
    }
    printf("All threads reached this point semaforos\n");

    pthread_exit(NULL);
}

int main() {
    // Número de hilos
    thread_count = 5; 

    sem_init(&count_sem, 0, 1);
    sem_init(&barrier_sem, 0, 0);

    pthread_t threads[thread_count];

    // Crear hilos
    for (int i = 0; i < thread_count; ++i) {
        pthread_create(&threads[i], NULL, ThreadWork, NULL);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Liberar recursos de los semáforos
    sem_destroy(&count_sem);
    sem_destroy(&barrier_sem);

    return 0;
}
