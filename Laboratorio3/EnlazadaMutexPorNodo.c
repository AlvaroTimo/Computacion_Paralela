#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct list_node {
    int data;
    struct list_node* next;
    pthread_mutex_t mutex;
};

struct list_node* head_p = NULL;
pthread_mutex_t head_p_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function declarations
int Member(int value);
int Insert(int value, struct list_node** head);
int Delete(int value, struct list_node** head);
void* ThreadWork(void* rank);

int thread_count = 4;

int main() {
    // Initialize variables and setup
    pthread_t* thread_handles;
    long i;

    // Allocate memory for thread handles
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    // Create threads
    for (i = 0; i < thread_count; i++) {
        pthread_create(&thread_handles[i], NULL, ThreadWork, (void*)i);
    }

    // Join threads
    for (i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);
    }

    // Free allocated memory
    free(thread_handles);

    return 0;
}

void* ThreadWork(void* rank) {
    long my_rank = (long)rank;

    // Perform operations in parallel
    int value_to_insert = my_rank + 1;
    int value_to_delete = my_rank % 2 + 1;
    int value_to_search = thread_count - my_rank;

    // Insert
    pthread_mutex_lock(&head_p_mutex);
    Insert(value_to_insert, &head_p);
    pthread_mutex_unlock(&head_p_mutex);

    // Delete
    pthread_mutex_lock(&head_p_mutex);
    Delete(value_to_delete, &head_p);
    pthread_mutex_unlock(&head_p_mutex);

    // Member
    int is_member = Member(value_to_search);

    // Print results
    printf("Thread %ld: Inserted %d, Deleted %d, Searched %d (Result: %s)\n",
            my_rank, value_to_insert, value_to_delete, value_to_search,
            is_member ? "Found" : "Not Found");

    return NULL;
}

int Member(int value) {
    struct list_node *temp = head_p;
    pthread_mutex_lock(&head_p_mutex);

    while (temp != NULL && temp->data < value) {
        if (temp->next != NULL)
            pthread_mutex_lock(&(temp->next->mutex));
        if (temp == head_p)
            pthread_mutex_unlock(&head_p_mutex);
        pthread_mutex_unlock(&(temp->mutex));
        temp = temp->next;
    }

    if (temp == NULL || temp->data > value) {
        if (temp == head_p)
            pthread_mutex_unlock(&head_p_mutex);
        if (temp != NULL)
            pthread_mutex_unlock(&(temp->mutex));
        return 0;
    } else {
        if (temp == head_p)
            pthread_mutex_unlock(&head_p_mutex);
        pthread_mutex_unlock(&(temp->mutex));
        return 1;
    }
} /* Member */

int Insert(int value, struct list_node** head) {
    struct list_node *curr = *head;
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
            *head = temp;
        else
            pred->next = temp;

        return 1;
    } else { /* Value already in list */
        return 0;
    }
} /* Insert */

int Delete(int value, struct list_node** head) {
    struct list_node *curr = *head;
    struct list_node *pred = NULL;

    while (curr != NULL && curr->data < value) {
        pred = curr;
        curr = curr->next;
    }

    if (curr != NULL && curr->data == value) {
        if (pred == NULL) { /* Deleting first node in list */
            *head = curr->next;
            free(curr);
        } else {
            pred->next = curr->next;
            free(curr);
        }
        return 1;
    } else { /* Value isn’t in list */
        return 0;
    }
} /* Delete */
