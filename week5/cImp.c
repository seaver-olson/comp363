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
int len(const char* string){
    int len = 0;
    while (string[len] != '\0') len++;
    return len;
}

result align(const char* M, const char* N){
    result res = {0};
    int m = len(M);
    int n = len(N);

    int P[m+1][n+1];//penalty matrix

    int max = (m > n) ? m : n;
    //fill in base case 
    for (int i = 0; i <= max; i++){
        //if i understand pipeline dynamic predictive branching like I think I do these branches should be predicted pretty early into the loop making the conditionals meaningless for performance eventually
        if (i <= m) P[i][0] = i * GAP_PEN;
        if (i <= n) P[0][i] = i * GAP_PEN;
    }

    for (int i=1; i <= m; i++){
        for (int j = 1; j<=n; j++){
            //three cases for optimal substructure
            int diag = P[i-1][j-1] + GET_PENALTY(M[i-1],N[j-1]);
            int up = P[i-1][j] + GAP_PEN;
            int left = P[i][j-1] + GAP_PEN;

            //get minimum penalty
            int min = diag;
            if (up < min) min = up;
            if (left < min) min = left;
            
            //assign min
            P[i][j] = min;
        }
    }

    char residual1[max*2+1];
    char residual2[max*2+1];

    int r1 = m;//holds position in first residual list
    int r2 = n;//holds position in second residual list

    while (r1 > 0 && r2 > 0){
        int curr = P[r1][r2];
    }
    
    
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
        result res = align(tests[i][0], tests[i][1]);
        printf("Testcase %d: gaps: %d, match: %d, mismatch: %d\n", i, res.gaps, res.matches, res.mismatches);
    }
    return 0;
}