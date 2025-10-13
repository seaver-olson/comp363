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
    int P[m+1][n+1];

    for (int i = 0; i <= m; i++) P[i][0] = i * GAP_PEN;
    for (int j = 0; j <= n; j++) P[0][j] = j * GAP_PEN;

    for (int i = 1; i <= m; i++){
        for (int j = 1; j <= n; j++){
            int diag = P[i-1][j-1] + GET_PENALTY(M[i-1], N[j-1]);
            int up   = P[i-1][j] + GAP_PEN;
            int left = P[i][j-1] + GAP_PEN;

            int min = diag;
            if (up < min) min = up;
            if (left < min) min = left;
            P[i][j] = min;
        }
    }

    int r1 = m, r2 = n;
    char strM[m+n+1], strN[m+n+1];
    int pos = 0;

    while (r1 > 0 && r2 > 0){
        int curr = P[r1][r2];
        int diagCost = P[r1-1][r2-1] + GET_PENALTY(M[r1-1], N[r2-1]);
        int upCost   = P[r1-1][r2] + GAP_PEN;
        int leftCost = P[r1][r2-1] + GAP_PEN;

        if (curr == diagCost) {
            strM[pos] = M[r1-1];
            strN[pos] = N[r2-1];
            if (M[r1-1] == N[r2-1]) res.matches++;
            else res.mismatches++;
            r1--; r2--;
        } else if (curr == upCost) {
            strM[pos] = M[r1-1];
            strN[pos] = '-';
            res.gaps++;
            r1--;
        } else {
            strM[pos] = '-';
            strN[pos] = N[r2-1];
            res.gaps++;
            r2--;
        }
        pos++;
    }

    while (r1 > 0){
        strM[pos] = M[r1-1];
        strN[pos] = '-';
        res.gaps++; r1--; pos++;
    }
    while (r2 > 0){
        strM[pos] = '-';
        strN[pos] = N[r2-1];
        res.gaps++; r2--; pos++;
    }

    // reverse the strings for correct display
    for (int i = 0; i < pos / 2; i++){
        char t = strM[i]; strM[i] = strM[pos-1-i]; strM[pos-1-i] = t;
        t = strN[i]; strN[i] = strN[pos-1-i]; strN[pos-1-i] = t;
    }

    strM[pos] = '\0';
    strN[pos] = '\0';

    printf("%s : %s\n", strM, M);
    printf("%s : %s\n", strN, N);

    return res;
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