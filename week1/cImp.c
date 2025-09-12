#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COUNT 4
#define MAXEDGE 10
#define EPROB .67// 6 7

double INFINITY = 1.0 / 0.0;

double G[COUNT][COUNT];
double T[COUNT][COUNT];

typedef struct {
    int source;
    int dest;
} Edge;

//finds the root so we can compare if two vertecies are on the same component
int find(int components[], int u){ 
    if (components[u] == u){
        return u;//returns parent if it is root
    } else {
        return find(components, components[u]);//recursive call on parent
    }
}

/*I found out that C passes arrays as pointers since they are essentially the same thing under the hood
This is a masterclass of a function, please take a second to appreciate all that it does, it is really beautiful
*/
void generateGraph(double G[COUNT][COUNT]){
    for (int i=0; i<COUNT;i++){
        G[i][i] = INFINITY;
        for (int j=i+1; j<COUNT;j++){
            if ((double)rand() / RAND_MAX < EPROB) G[i][j] = 1+rand()%MAXEDGE; 
            else G[i][j] = INFINITY;
            G[j][i] = G[i][j];
        }
    }
}

void printGraph(){
    for (int i=0; i<COUNT;i++){
        printf("[ ");
        for (int j=0; j<COUNT;j++){
            //extra logic but makes the output so much cleaner
            if (G[i][j] != INFINITY) printf("%d ", (int) G[i][j]);
            else printf("\u221E ");//unicode for infinity
        }
        printf("]\n");
    }
}

int safeEdge(int u, int v){
    for (int i=0;i<COUNT;i++){
        if (G[u][i] < G[u][v]) return 0;
    }
    return 1;
}


void BMST(double G[COUNT][COUNT], double T[COUNT][COUNT]){
    
    //init T as edgeless version of G
    for (int i=0;i<COUNT;i++){
        for (int j=0;j<COUNT;j++){
            T[i][j] = INFINITY;
        }
    }
    int components[COUNT];
    for (int i=0; i<COUNT; i++) components[i] = i;//label components
    int componentCount = COUNT;
    int idx = 0;//to traverse

    
    while (componentCount > 1){
        for (int u = 0; u < COUNT; u++){
            for (int v = 0; v < COUNT; v++){
                if (G[u][v] != INFINITY && components[u] != components[v] && safeEdge(u,v)){
                    int uComp = find(components, components[u]);
                    int vComp = find(components, components[v]);
                    if (uComp != vComp) {

                    }
                }
            }  
        }
    }
}

int main(){
    srand(time(0));
    generateGraph(G);
    printf("Input Graph = \n");
    printGraph();
    printf("Minimum Spanning Tree: \n");
    BMST(G,T);
    printGraph();
    return 0;
}

//While exploring some different ways to conserve memory in the program I calculated the speedup between using the diagonal dead space of G to stash the temp data i needed to allocate in BMST vs keeping a separate helper array 

//I found that the speed up formula was approx (n^2 + 2n) / (n^2 + n) which gave me
//1 + 1/(n+1)

//this is the reason I left this fix out, as n approaching infinity the speed up literally erases itself and becomes more of a burden if you try to take into account cleaning the input array after calculations
