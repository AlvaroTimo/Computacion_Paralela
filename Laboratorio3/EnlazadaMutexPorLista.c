#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Estructura para el nodo de la lista
struct list_node {
    int data;
    struct list_node *next;
};

// Variable global para head
struct list_node *head = NULL;

// Mutex para proteger el acceso a la lista
pthread_mutex_t lista_mutex = PTHREAD_MUTEX_INITIALIZER;

// Prototipos de funciones
int Member(int value);
int Insert(int value);
int Delete(int value);
void *insert_thread(void *arg);
void *query_thread(void *arg);
void *delete_thread(void *arg);

int main() {
    pthread_t threads[3]; // Un hilo para inserciones, uno para consultas y otro para eliminaciones

    // Crear hilos
    pthread_create(&threads[0], NULL, insert_thread, NULL);
    pthread_create(&threads[1], NULL, query_thread, NULL);
    pthread_create(&threads[2], NULL, delete_thread, NULL);

    // Esperar a que los hilos terminen
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

// Función para realizar inserciones en un hilo
void *insert_thread(void *arg) {
    for (int i = 1; i <= 10; i++) {
        pthread_mutex_lock(&lista_mutex);
        printf("Insertando %d \n", i);
        Insert(i);
        pthread_mutex_unlock(&lista_mutex);
    }

    pthread_exit(NULL);
}

// Función para realizar consultas en un hilo
void *query_thread(void *arg) {
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&lista_mutex);
        printf("Consulta %d: %s\n", i, Member(i) ? "Encontrado" : "No encontrado");
        pthread_mutex_unlock(&lista_mutex);
    }

    pthread_exit(NULL);
}

// Función para realizar eliminaciones en un hilo
void *delete_thread(void *arg) {
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&lista_mutex);
        int value = i * 2; // Eliminar valores pares
        printf("Eliminación %d: %s\n", i, Delete(value) ? "Eliminado" : "No encontrado");
        pthread_mutex_unlock(&lista_mutex);
    }

    pthread_exit(NULL);
}

// Definición de la función Member
int Member(int value) {
    struct list_node *curr = head;
    while (curr != NULL && curr->data < value)
        curr = curr->next;

    int result = (curr != NULL && curr->data == value);
    return result;
} /* Member */

// Definición de la función Insert
int Insert(int value) {
    struct list_node *curr = head;
    struct list_node *pred = NULL;
    struct list_node *temp;

    while (curr != NULL && curr->data < value) {
        pred = curr;
        curr = curr->next;
    }

    int result;

    if (curr == NULL || curr->data > value) {
        temp = malloc(sizeof(struct list_node));
        temp->data = value;
        temp->next = curr;

        if (pred == NULL) /* New first node */
            head = temp;
        else
            pred->next = temp;

        result = 1;
    } else { /* Value already in list */
        result = 0;
    }
    return result;
} /* Insert */

// Definición de la función Delete
int Delete(int value) {
    struct list_node *curr = head;
    struct list_node *pred = NULL;

    int result;

    while (curr != NULL && curr->data < value) {
        pred = curr;
        curr = curr->next;
    }

    if (curr != NULL && curr->data == value) {
        if (pred == NULL) { /* Deleting first node in list */
            head = curr->next;
            free(curr);
        } else {
            pred->next = curr->next;
            free(curr);
        }
        result = 1;
    } else { /* Value isn’t in list */
        result = 0;
    }
    return result;
} /* Delete */