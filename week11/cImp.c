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

void can_segment_dp(const char* string, char* outSegs[NUM_OF_WORDS]){
    int string_length = strlen(string);
    int parent[string_length + 1];
    int dp[string_length+1];
    memset(dp, 0, sizeof(dp));

    dp[0]=1;//empty case
    for (int i = 1; i <= string_length; i++){
        for (int j = 0; j < i; j++){
            if (dp[j]){
                int len = i - j;
                char* substr = (char*)malloc(sizeof(char) * (len + 1));
                strncpy(substr, &string[j], len);
                substr[len] = '\0';
                if (is_word(substr)){
                    dp[i] = 1;
                    parent[i] = j;
                    free(substr);
                    break;
                }
                free(substr);
            }
        }
    }

}


int main(){
    srand(time(0));
    while (current_trial < NUM_OF_TRIALS){
        current_trial++;
        char* outSegs[NUM_OF_WORDS];
        char* trial_string = (char*)malloc(sizeof(char) * NUM_OF_WORDS * 20);
        generate_random_string(trial_string, NUM_OF_WORDS, DISTORTION_PROB);
        printf("Trial %d: Testing string \"%s\"\n", current_trial, trial_string);
        can_segment_dp(trial_string, outSegs);
        printf("Segmented as: ");
        for (int i = 0; i < NUM_OF_WORDS; i++){
            if (outSegs[i]){
                printf("%s ", outSegs[i]);
                free(outSegs[i]);
            }
        }
        printf("\n");
        free(trial_string);
    }
    return 0;
}