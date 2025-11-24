#include "dict.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NUM_OF_TRIALS 10
#define NUM_OF_WORDS 15
#define DISTORTION_PROB 0.1f

int current_trial = 0;

int binary_search(const char* target){
    int low = 0;
    int high = dict_len - 1;
    int finalResult = -1;
    while (low <= high && finalResult == -1){
        int mid = low + (high - low) / 2;
        int result = strcmp(little_dictionary[mid], target);
        if (result == 0)
            finalResult = mid;
        else if (result < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return finalResult;
}

int is_word(const char* target){
    return binary_search(target) != -1;
}

void generate_random_string(char* string, int length, float distortion_prob){
    int write_pointer = 0;//I DID NOT REALIZE HOW ANNOYING C IS
    for (int i = 0; i < length; i++){
        const char* word = little_dictionary[rand() % dict_len];
        int word_len = strlen(word);
        for (int j = 0; j < word_len; j++){
            if (((float)rand() / RAND_MAX) >= distortion_prob){
                string[write_pointer++] = word[j];//increment write pointer after use
            }
        }
    }
    string[write_pointer] = '\0';
}

int main(){
    srand(time(0));
    while (current_trial < NUM_OF_TRIALS){
        current_trial++;
        char* trial_string = (char*)malloc(sizeof(char) * NUM_OF_WORDS * 20);//okay i didnt really know what to do here so i just assumed max word length is 20 for malloc
        generate_random_string(trial_string, NUM_OF_WORDS, DISTORTION_PROB);
        printf("Trial %d: Testing string \"%s\"\n", current_trial, trial_string);
        
    }
    return 0;
}