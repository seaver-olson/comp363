#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_COUNT 26
#define TABLE_COUNT 27

char input_string[] = "HELLO WORLD";
int size = (sizeof(input_string)/sizeof(input_string[0]));
int freqs[TABLE_COUNT];

typedef struct Node{
    int freq;
    char sym;
    struct Node *left;
    struct Node *right;
}Node;

Node* create_node(int frequency, char symbol) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->freq = frequency;
    node->sym = symbol;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void set_left(struct Node *parent,struct Node *leftChild){
    parent->left = leftChild;
}

void set_right(struct Node *parent,struct Node *rightChild){
    parent->right = rightChild;
}

int lt(struct Node *node, struct Node *other){
    return node->freq < other->freq;
}

int find_min(Node* forest[], int size){
    int min = 0;
    for (int i=1;i<size;i++)if (lt(forest[i], forest[min])) min = i;
    return min;
}

void filter(char* input, char* output){
    int counter = 0;
    for (int i = 0; i < size; i++){
        if (isalpha(input[i])) output[counter++] = toupper(input[i]); 
        else if (input[i] == ' ') output[counter++] = ' ';
    }
    output[counter] = '\0';//terminate bit for end of string
}

void count_frequencies(const char* input, int freqs[ALPHABET_COUNT+1]){
    for (int i = 0; i < ALPHABET_COUNT+1; i++) {
        freqs[i] = 0;
    }

    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        if (c >= 'A' && c <= 'Z') {
            freqs[c - 'A']++;
        } else if (c == ' ') {
            freqs[ALPHABET_COUNT]++; 
        }
    }
}

int main(){
    char filtered[size];
    filter(input_string, filtered);
    count_frequencies(filtered,freqs);
    for (int i = 0; i < ALPHABET_COUNT; i++){
        if (freqs[i] > 0) printf("%c: %d\n", 'A' + i, freqs[i]);
    }
    if (freqs[ALPHABET_COUNT] > 0) printf("SPACE: %d\n", freqs[ALPHABET_COUNT]);
    return 0;
}
