#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COUNT 4
#define MAXEDGE 10
#define EPROB.67// 6 7
double INFINITY = 1.0 / 0.0;

double G[COUNT][COUNT];//invariant input
double T[COUNT][COUNT];//output

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

void printGraph(double X[COUNT][COUNT]){
    for (int i=0; i<COUNT;i++){
        printf("[ ");
        for (int j=0; j<COUNT;j++){
            //extra logic but makes the output so much cleaner
            if (X[i][j] != INFINITY) printf("%d ", (int) X[i][j]);
            else printf("inf ");//unicode for infinity
        }
        printf("]\n");
    }
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
        Edge cheapest[COUNT];
        for (int i=0; i<COUNT; i++){ 
            cheapest[i].source = -1; 
            cheapest[i].dest = -1; 
        }

        //get cheapest edge for each component
        for (int u = 0; u < COUNT; u++){
            int uComp = find(components, u);
            for (int v = 0; v < COUNT; v++){
                if (G[u][v] != INFINITY){
                    int vComp = find(components, v);
                    if (uComp != vComp ) {
                        //if no source yet or new source is faster than old source
                        if (cheapest[uComp].source == -1 || G[u][v] < G[cheapest[uComp].source][cheapest[uComp].dest]) {
                            cheapest[uComp].source = u;
                            cheapest[uComp].dest = v;
                        }
                    }
                }
            }  
        }

        //add edges
        for (int i=0;i<COUNT;i++){
            if (cheapest[i].source != -1){
                int u = cheapest[i].source;
                int v = cheapest[i].dest;
                int uComp = find(components, u);
                int vComp = find(components, v);
                if (uComp != vComp){
                    T[u][v] = G[u][v];
                    T[v][u] = G[u][v];
                    components[vComp] = uComp;  // merge components
                    componentCount--;
                }
            }
        }


    }
}

int main(){
    srand(time(0));
    generateGraph(G);
    printf("Input Graph = \n");
    printGraph(G);
    printf("Minimum Spanning Tree: \n");
    BMST(G,T);
    printf("MST edges:\n");
    for (int i = 0; i < COUNT; i++) {
        for (int j = i+1; j < COUNT; j++) { 
            if (T[i][j] != INFINITY) {
                printf("Edge %d - %d: weight %d\n", i, j, (int)T[i][j]);
            }
        }
    }
    //uncomment if u want proof of invariant holding up
    //printf("Proof Input Graph still invariant:\n");
    //printGraph(G);
    return 0;
}

//While exploring some different ways to conserve memory in the program I calculated the speedup between using the diagonal dead space of G to stash the temp data i needed to allocate in BMST vs keeping a separate helper array 

//I found that the speed up formula was approx (n^2 + 2n) / (n^2 + n) which gave me
//1 + 1/(n+1)

//this is the reason I left this fix out, as n approaching infinity the speed up literally erases itself and becomes more of a burden if you try to take into account cleaning the input array after calculations
//and then you told me G was invariant and I cant change it and I lost all hope either way and will now become a botany student
