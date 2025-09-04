/*
Note to Leo - I have a few too many return statements sorry
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int cap;
    int size;
    int *array;
} Heap;

Heap *HeapInit(int cap){
    Heap *heap = malloc(sizeof(Heap));
    heap->array = malloc(sizeof(int) * cap);
    heap->size = 0;
    heap->cap = cap;
    return heap;
}

void addElement(Heap *heap, int element){
    //realloc rule
    if (heap->size >= heap->cap) {
        heap->cap*=(heap->cap > 100 ? 1.25 : 2); //see note at bottom of page
        printf("Error: while adding %d to heap ran out of memory, reallocating to hold %d elements now\n", element, heap->cap);
        heap->array = realloc(heap->array, heap->cap * sizeof(int));
    }
    heap->array[heap->size] = element;
    heap->size++;
    //restore heap 
    int curr = heap->size -1;
    while (curr > 0){
        int parent = (curr-1)/2;
        if (heap->array[curr] < heap->array[parent]){
            int temp = heap->array[curr];
            heap->array[curr] = heap->array[parent];
            heap->array[parent] = temp;
            
            curr = parent;
        } else {
            curr = 0;//early exit;
        }
    }
}

void displayHeap(Heap *heap){
    int size = sizeof(int);
    printf("Heap : [ ");
    for (int i = 0; i < heap->size; i++){
        printf("%d ", heap->array[i]);
    }
    printf("]\n");
}

int main(){
    Heap *heap = HeapInit(1);
    int example[] = {999999,23,34,64,12,22,11,1,2,3,56,23,75};
    int exampleSize = sizeof(example)/sizeof(example[0]);
    printf("===ADD TEST===\n");
    for (int i = 0; i < exampleSize; i++){
        addElement(heap, example[i]);
        printf("Added %d to heap successfully\n", example[i]);
        displayHeap(heap);
    }
    return 0;
}

/*
for the reallocation if the cap is met I really wanted to do heap->cap++ to keep this program as memory tight as possible.
Sadly, I found that if I was adding a lot of elements the amortized cost of each add was (what i am assuming) O(n^2) or at the least > O(n).
This is easily beat when you take into account that if we just double the cap each time it is needed, we do use a lot more memory but we also do not have to preform the realloc for another heap->cap run throughs
after doing some digging I found the languages like Python and Java actually use a grow factor based on how large cap is so that they don't throw memory around too much.
As I am not a nerd I did not do that and instead opt'd to create a small threshold value at 100 elements so if the cap is larger it only reallocs the  list with 1.25 the memory
*/
