#define _XOPEN_SOURCE 700   // enables POSIX.1-2008 functions
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <string.h>


// Node definition
typedef struct Node {
    int id;
    char name[32];
    _Atomic int refcount;   // reference counter
    struct Node *next;
} Node;

// Global head and lock
Node *head = NULL;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

// 🔹 Reference counting helpers
void acquire(Node *n) {
    atomic_fetch_add(&n->refcount, 1);
}

void release(Node *n) {
    if (atomic_fetch_sub(&n->refcount, 1) == 1) {
        printf("Freeing node id=%d\n", n->id);
        free(n);
    }
}

// 🔹 CREATE (adds node with refcount=1)
void create_record(int id, const char *name) {
    pthread_rwlock_wrlock(&rwlock);

    Node *n = malloc(sizeof(Node));
    n->id = id;
    strncpy(n->name, name, sizeof(n->name) - 1);
    n->name[sizeof(n->name) - 1] = '\0';
    atomic_init(&n->refcount, 1);

    n->next = head;
    head = n;

    pthread_rwlock_unlock(&rwlock);
    printf("[CREATE] id=%d, name=%s\n", id, name);
}

// 🔹 READ (acquires + returns node, caller must release)
Node* read_record(int id) {
    pthread_rwlock_rdlock(&rwlock);

    Node *cur = head;
    while (cur) {
        if (cur->id == id) {
            acquire(cur);   // safe reference
            pthread_rwlock_unlock(&rwlock);
            return cur;
        }
        cur = cur->next;
    }

    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

// 🔹 UPDATE (acquire before modify, release after)
int update_record(int id, const char *newName) {
    pthread_rwlock_wrlock(&rwlock);  // exclusive write lock

    Node *cur = head;
    while (cur) {
        if (cur->id == id) {
            strncpy(cur->name, newName, sizeof(cur->name) - 1);
            cur->name[sizeof(cur->name) - 1] = '\0';
            printf("[UPDATE] id=%d, newName=%s\n", id, newName);

            pthread_rwlock_unlock(&rwlock);
            return 1;
        }
        cur = cur->next;
    }

    pthread_rwlock_unlock(&rwlock);
    return 0;
}


// 🔹 DELETE (remove from list, release node)
int delete_record(int id) {
    pthread_rwlock_wrlock(&rwlock);

    Node **cur = &head;
    while (*cur) {
        if ((*cur)->id == id) {
            Node *tmp = *cur;
            *cur = tmp->next;

            // release: memory freed only when no readers/writers hold ref
            release(tmp);

            pthread_rwlock_unlock(&rwlock);
            printf("[DELETE] id=%d\n", id);
            return 1;
        }
        cur = &(*cur)->next;
    }

    pthread_rwlock_unlock(&rwlock);
    return 0; // not found
}

// 🔹 Print helper (acquire each node before printing)
void print_list() {
    pthread_rwlock_rdlock(&rwlock);
    Node *cur = head;
    printf("Current list: ");
    while (cur) {
        acquire(cur);
        printf("[%d:%s] ", cur->id, cur->name);
        Node *next = cur->next;
        release(cur);
        cur = next;
    }
    printf("\n");
    pthread_rwlock_unlock(&rwlock);
}

// 🔹 Worker thread (demonstration)
// 🔹 Worker threads for CRUD
//
void* worker_create(void *arg) {
    int id = *(int*)arg;
    char name[32];
    sprintf(name, "User%d", id);
    create_record(id, name);
    return NULL;
}

void* worker_read(void *arg) {
    int id = *(int*)arg;
    Node *n = read_record(id);
    if (n) {
        printf("[READ] id=%d, name=%s\n", n->id, n->name);
        release(n);
    } else {
        printf("[READ] id=%d not found\n", id);
    }
    return NULL;
}

void* worker_update(void *arg) {
    int id = *(int*)arg;
    char newName[32];
    sprintf(newName, "Updated%d", id);
    if (!update_record(id, newName)) {
        printf("[UPDATE] id=%d not found\n", id);
    }
    return NULL;
}

void* worker_delete(void *arg) {
    int id = *(int*)arg;
    if (!delete_record(id)) {
        printf("[DELETE] id=%d not found\n", id);
    }
    return NULL;
}

// 🔹 Main demo
int main() {
    pthread_t threads[20];
    int ids[5] = {1, 2, 3, 4, 5};
    int t = 0;

    // Create records
    for (int i = 0; i < 5; i++)
        pthread_create(&threads[t++], NULL, worker_create, &ids[i]);

    // Read records
    for (int i = 0; i < 5; i++)
        pthread_create(&threads[t++], NULL, worker_read, &ids[i]);

    // Update records
    for (int i = 0; i < 5; i++)
        pthread_create(&threads[t++], NULL, worker_update, &ids[i]);

    // Delete records
    for (int i = 0; i < 5; i++)
        pthread_create(&threads[t++], NULL, worker_delete, &ids[i]);

    // Join all
    for (int i = 0; i < t; i++)
        pthread_join(threads[i], NULL);

    // Final state
    print_list();
    return 0;
}