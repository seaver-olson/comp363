#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COUNT 4
#define INFINTY -1//funny right?
#define MAXEDGE 10
#define EPROB .67// 6 7

double G[COUNT][COUNT];
double T[COUNT][COUNT];

/*I found out that C passes arrays as pointers since they are essentially the same thing under the hood
This is a masterclass of a function, please take a second to appreciate all that it does, it is really beautiful
*/
void generateGraph(double G[COUNT][COUNT]){
    for (int i=0; i<COUNT;i++){
        G[i][i] = INFINTY;
        for (int j=i+1; j<COUNT;j++){
            if ((double)rand() / RAND_MAX < EPROB) G[i][j] = 1+rand()%MAXEDGE; 
            else G[i][j] = INFINTY;
            G[j][i] = G[i][j];
        }
    }
}

void printGraph(){
    for (int i=0; i<COUNT;i++){
        printf("[ ");
        for (int j=0; j<COUNT;j++){
            printf("%f ", G[i][j]);
        }
        printf("]\n");
    }
}


int main(){
    srand(time(0));
    generateGraph(G);
    printGraph();
}
