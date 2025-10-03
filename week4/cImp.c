#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
Accesses an element in a flat array that represents a 2D table
input:
    table - pointer to the array 
    cols - num of cols in the table
    rowNum - row index of the element
    colNum - col index of the element

there are two ways I use this macro
int value = TABLE_AT(s, cols, rowNum, colNum); reads an element
TABLE_AT(S,cols,rowNum,colNum) = value; write element

I made this because I don't really like using int** in C to store pointers to pointers so I felt this was cleaner
*/
#define TABLE_AT(table, cols, rowNum, colNum)  ((table)[(rowNum) * (cols) + (colNum)])

typedef struct {
    int *value; // array of item values
    int *weight;// array of item weights
    int count;// number of items in museum
    int cMax;// max capacity of our truck
} Museum;


//C is so silly and doesn't let you create 2d arrays whose size isn't known at compile time
int* tableAlloc(int rows, int cols){
    //Calloc has minimal overhead and I don't need to init data myself
    return calloc(rows * cols, sizeof(int));
}


int max(int a, int b){
    return (a>b) ? a : b;
}

//helper function to reverse array since algorithm discovers from last to first
void reverseArray(int *array, int size){
    for (int i = 0; i<size/2; i++){
        int tmp=array[i];
        array[i] = array[(size-1)-i];
        array[(size-1)-i]=tmp;
    }
}

Museum* generateRandomMuseum(int count, int cMax, int maxValue, int maxWeight){
    Museum *museum = malloc(sizeof(Museum));
    
    museum->count = count;
    museum->cMax = cMax;

    //allocate value and weight pointers and set zero case to 0
    museum->value = calloc(count+1, sizeof(int));
    museum->weight = calloc(count+1, sizeof(int));
    
    //random value assignment
    for (int i = 1; i <= count; i++){
        museum->value[i] = rand() % maxValue+1;
        museum->weight[i] = rand() % maxWeight+1;
    }
    return museum;
}

void freeMuseum(Museum *museum){
    free(museum->value); free(museum->weight); free(museum);
}
/*
Input: pointer to museum struct
Output: flat table (int *) which holds the max val for every given S(i,cap)

Allocates a flat table then find the maximum value at each given item and capacity
I was able to learn how to use Macros today and created the TABLE_AT macro to make the flat table possible
without it I would have had to create a string of pointers to pointers for ints spaced apart at sizeof(int) * rowsize
*/
int* build_dp_table(Museum *museum){
    int rows = museum->count+1;//number of items + 1 (for zero case)
    int cols = museum->cMax+1;//capacity + 1 (for zero case)
    int *S = tableAlloc(rows,cols);//flat array the represents the 2D table
    //start at 1 because 0 is empty case //for each combination of i and capacity find the best value possible
    for (int item = 1; item <= museum->count;item++){
        for (int capacity = 1; capacity <= museum->cMax; capacity++){
            //can this item be added to our subset
            if (museum->weight[item] > capacity){
                //since current item weighs too much we exclude it by making the max the val without this item
                TABLE_AT(S, cols, item, capacity) = TABLE_AT(S, cols, item-1, capacity);
            } else {
                int without = TABLE_AT(S, cols, item-1, capacity);//max value if we skip this item
                int with = museum->value[item] + TABLE_AT(S, cols, item-1, capacity - museum->weight[item]);//max if we take this item
                TABLE_AT(S, cols, item, capacity) = max(without, with);
            }
        }
    }
    return S;
}

int* optimalSubsetValue(Museum *museum, int *S, int *OSsize){
    int cols = museum->cMax + 1;
    int row = museum->count;
    int col = museum->cMax;
    int *OS = malloc(museum->count * sizeof(int));
    int count = 0;//optimal subset size
    //loop until at zero case since zero case can't happen
    while (row > 0 && col > 0) {
        if (TABLE_AT(S, cols, row, col) != TABLE_AT(S, cols, row-1, col)){
            OS[count++] = row;
            col-= museum->weight[row];
        }
        row--;
    }
    *OSsize=count;
    reverseArray(OS, count);
    return OS;
}

int main(){
    int debugFlag = 0;//Leo flip this to 1 to see proof of success
    //if you want to check the last row of each column AKA the max at each capacity set debugFlag = 2

    srand(time(0));

    Museum *museum = generateRandomMuseum(100, 500, 100, 100); 
    int *S = build_dp_table(museum);

    int subset_size;
    int *subset = optimalSubsetValue(museum, S, &subset_size);

    printf("Optimal subset (%d items):", subset_size);
    for (int i = 0; i < subset_size; i++){
        printf(" %d", subset[i]);
    }
    printf("\n");

    printf("Maximum value achievable: %d\n", TABLE_AT(S, museum->cMax+1, museum->count, museum->cMax));

    if (debugFlag>=1){
        //weight proof
        int total_weight = 0;
        for (int i = 0; i < subset_size; i++) {
            total_weight += museum->weight[subset[i]];
        }
        printf("Total weight of subset: %d / %d\n", total_weight, museum->cMax);
        //value proof
        int total_value = 0;
        for (int i = 0; i < subset_size; i++) {
            total_value += museum->value[subset[i]];
        }
        printf("Total value of subset (manual sum): %d\n", total_value);
    } if (debugFlag == 2){
        printf("max values for each capacity:\n");
        for (int c = 0; c <= museum->cMax; c++) {
            printf("%d ", TABLE_AT(S, museum->cMax+1, museum->count, c));
        }
        printf("\n");
    }
    free(subset);
    free(S);
    freeMuseum(museum);
    return EXIT_SUCCESS;//just learned about this
}
