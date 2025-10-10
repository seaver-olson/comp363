#include <stdio.h>
#include <stdlib.h>

#define TESTCASES 7

//penalties
#define GAP_PEN 1
#define MISMATCH_PEN 5
#define MATCH_PEN 0


#define GET_PENALTY(a,b) ((a==b)?MATCH_PEN:MISMATCH_PEN)

typedef struct {
    int matches;
    int mismatches;
    int gaps;
} result;

//Leo said to not include and nonstandard library so no strlen() :(
int len(char* string){
    int len = 0;
    while (string[len] != '\0') len++;
    return len;
}

result align(const char* M, const char* N){
    result res = {0};
    int m = len(M);
    int n = len(N);

    int P[m+1][n+1];//penalty matrix
}

int main(){
    char* tests[TESTCASES][2] = {
        {"CRANE", "RAIN"},
        {"CYCLE", "BICYCLE"},
        {"ASTRONOMY", "GASTRONOMY"},
        {"INTENTION", "EXECUTION"},
        {"AGGTAB", "GXTXAYB"},
        {"GATTACA", "GCATGCU"},
        {"DELICIOUS", "RELIGIOUS"}
    };

    for (int i = 0; i < TESTCASES; i++){

    }
    return 0;
}