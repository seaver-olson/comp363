#include <stdio.h>

typedef struct{
    int cap;
    int size;
    int *array;
} Heap;

void createHeap(int cap){
	return malloc(sizeof(Heap));
}

int main(){
    int example[] = {999999,23,34,64,12,22,11,1,2,3,56,23,75};
    int exampleSize = sizeof(example)/sizeof(example[0]);
    printf("===ADD TEST===\n");
    
    return 0;
}
