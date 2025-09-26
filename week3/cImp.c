#include <stdio.h>
#include <stdlib.h>

#define ALPHABET_COUNT 26

char *input_string = "Hello";
int size = (sizeof(input_string)/sizeof(input_string[0]));
int freqs[ALPHABET_COUNT];

struct Node{
    int freq;
    char sym;
    struct Node *left;
    struct Node *right;
};

void set_left(struct Node *parent,struct Node *leftChild){
    parent->left = leftChild;
}

void set_right(struct Node *parent,struct Node *rightChild){
    parent->right = rightChild;
}

int lt(struct Node *node, struct Node *other){
    return node->freq < other->freq;
}

void count_frequencies(int freqs[ALPHABET_COUNT+1]){
    for (int i = 0; i < size; i++){
        if (((int)input_string[i])-65 != 32) freqs[(int)input_string[i]-65]++;
    }
}

int main(){
    count_frequencies(freqs);
    for (int i = 0; i < 26; i++){
        printf("%d \n", freqs[i]);
    }
    return 0;
}
