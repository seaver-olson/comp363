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

Node* build_huffman(Node* forest[], int forest_size) {
    while (forest_size > 1){
        //find smallest freq node in forest
        int idx = find_min(forest, forest_size);
        Node* min1 = forest[idx];
        forest[idx] = forest[--forest_size];

        // reuse idx for second min
        idx = find_min(forest, forest_size);
        Node* min2 = forest[idx];
        forest[idx] = forest[--forest_size];

        Node* parent = create_node(min1->freq + min2->freq, '\0');
        set_left(parent, min1);
        set_right(parent, min2);

        forest[forest_size++] = parent;
    }
    return forest[0];
}

//recursive code generator starting at root
void generate_codes(Node* root, char* code, int depth){
    if (root){
        if (root->left == NULL && root->right == NULL){
            code[depth]='\0';//terminates the code so C doesn't kill me
            if (root->sym == ' ') printf("SPACE: %s\n", code);
            else printf("%c: %s\n", root->sym, code);
        }
        code[depth] = '0';
        generate_codes(root->left, code, depth+1);
        code[depth] = '1';
        generate_codes(root->right, code, depth+1);//right side scan
    }
}


int main(){
    char filtered[size];
    filter(input_string, filtered);
    printf("Filtered: %s\n", filtered);//filtered string check
    //frequency counting and printing
    count_frequencies(filtered,freqs);
    for (int i = 0; i < ALPHABET_COUNT; i++){
        if (freqs[i] > 0) printf("%c: %d\n", 'A' + i, freqs[i]);
    }
    if (freqs[ALPHABET_COUNT] > 0) printf("SPACE: %d\n", freqs[ALPHABET_COUNT]);

    Node* forest[TABLE_COUNT];
    int forest_size = 0;
    //basically Leo's init_forest() function that initializes a node for each non zero frequency letter + space at the end
    for (int i = 0; i < ALPHABET_COUNT; i++) if (freqs[i] > 0) forest[forest_size++] = create_node(freqs[i], 'A' + i);
    if (freqs[ALPHABET_COUNT] > 0) forest[forest_size++] = create_node(freqs[ALPHABET_COUNT], ' ');

    Node* root = build_huffman(forest, forest_size);
    char code[100];
    generate_codes(root, code, 0);

    return 0;
}
