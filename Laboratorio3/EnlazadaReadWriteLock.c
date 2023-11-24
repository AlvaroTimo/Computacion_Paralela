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

// Variable global para el candado
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

// Prototipos de funciones
int Member(int value);
int Insert(int value);
int Delete(int value);

// Función principal (main)
int main() {

    // Inicializar el candado
    pthread_rwlock_init(&rwlock, NULL);

    int value_to_find = 7;
    int value_to_insert = 7;
    int value_to_delete = 7;

    
    // Validación para la función Insert
    if (Insert(value_to_insert)) {
        printf("Se insertó el valor %d en la lista.\n", value_to_insert);
    } else {
        printf("El valor %d ya se encuentra en la lista, no se insertó.\n", value_to_insert);
    }

    // Validación para la función Member
    if (Member(value_to_find)) {
        printf("El valor %d se encuentra en la lista.\n", value_to_find);
    } else {
        printf("El valor %d no se encuentra en la lista.\n", value_to_find);
    }


    // Validación para la función Delete
    if (Delete(value_to_delete)) {
        printf("Se eliminó el valor %d de la lista.\n", value_to_delete);
    } else {
        printf("El valor %d no se encuentra en la lista, no se eliminó.\n", value_to_delete);
    }

    // Validación para la función Member
    if (Member(value_to_find)) {
        printf("El valor %d se encuentra en la lista.\n", value_to_find);
    } else {
        printf("El valor %d no se encuentra en la lista.\n", value_to_find);
    }
    
    // Destruir el candado
    pthread_rwlock_destroy(&rwlock);

    return 0;
}

// Definición de la función Member
int Member(int value) {
    pthread_rwlock_rdlock(&rwlock);
    struct list_node *curr = head;

    while (curr != NULL && curr->data < value)
        curr = curr->next;

    if (curr == NULL || curr->data > value) {
        pthread_rwlock_unlock(&rwlock);
        return 0;
    } else {
        pthread_rwlock_unlock(&rwlock);
        return 1;
    }
} /* Member */


// Definición de la función Insert
int Insert(int value) {
    pthread_rwlock_wrlock(&rwlock);
    struct list_node *curr = head;
    struct list_node *pred = NULL;
    struct list_node *temp;

    while (curr != NULL && curr->data < value) {
        pred = curr;
        curr = curr->next;
    }

    if (curr == NULL || curr->data > value) {
        temp = malloc(sizeof(struct list_node));
        temp->data = value;
        temp->next = curr;

        if (pred == NULL) /* New first node */
            head = temp;
        else
            pred->next = temp;

        pthread_rwlock_unlock(&rwlock);
        return 1;
    } else { /* Value already in list */
        pthread_rwlock_unlock(&rwlock);
        return 0;
    }
} /* Insert */

// Definición de la función Delete
int Delete(int value) {
    pthread_rwlock_wrlock(&rwlock);
    struct list_node *curr = head;
    struct list_node *pred = NULL;

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
        pthread_rwlock_unlock(&rwlock);
        return 1;
    } else { /* Value isn’t in list */
        pthread_rwlock_unlock(&rwlock);
        return 0;
    }
} /* Delete */
