#include <stdio.h>
#include <stdlib.h>


//min-heap
#define MAX 100

struct MinHeap {
    int arr[MAX];
    int size;
};

// Swap helper
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Create empty heap
struct MinHeap* createHeap() {
    struct MinHeap *heap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    heap->size = 0;
    return heap;
}

// Heapify up
void heapifyUp(struct MinHeap *heap, int index) {
    if (index && heap->arr[(index - 1) / 2] > heap->arr[index]) {
        swap(&heap->arr[index], &heap->arr[(index - 1) / 2]);
        heapifyUp(heap, (index - 1) / 2);
    }
}

// Heapify down
void heapifyDown(struct MinHeap *heap, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;

    if (left < heap->size && heap->arr[left] < heap->arr[smallest])
        smallest = left;

    if (right < heap->size && heap->arr[right] < heap->arr[smallest])
        smallest = right;

    if (smallest != index) {
        swap(&heap->arr[index], &heap->arr[smallest]);
        heapifyDown(heap, smallest);
    }
}

// Insert element
void insert(struct MinHeap *heap, int key) {
    if (heap->size == MAX) {
        printf("Heap overflow!\n");
        return;
    }
    heap->arr[heap->size] = key;
    heapifyUp(heap, heap->size);
    heap->size++;
}

// Delete root
void deleteRoot(struct MinHeap *heap) {
    if (heap->size <= 0) {
        printf("Heap is empty!\n");
        return;
    }
    printf("Deleted root: %d\n", heap->arr[0]);
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
}

// Print heap
void printHeap(struct MinHeap *heap) {
    printf("Heap array: ");
    for (int i = 0; i < heap->size; i++)
        printf("%d ", heap->arr[i]);
    printf("\n");
}

int main() {
    struct MinHeap *heap = createHeap();

    insert(heap, 10);
    insert(heap, 5);
    insert(heap, 20);
    insert(heap, 3);
    insert(heap, 15);

    printHeap(heap);

    deleteRoot(heap);
    printHeap(heap);

    deleteRoot(heap);
    printHeap(heap);

    return 0;
}
