#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//kahn vs the stack - how topological sorting can be obtained using kahn's algorithm
#define N 6

int visited[N] = {0};
int time_counter;
double leave_times[N] = {0};


//returns status code of operation since C does not have exceptions
int kahn(int graph[N][N], int topological_order[N]){
    //data integrity check
    if (!graph || N <= 0 ) //raise error
        return 1;//https://stackoverflow.com/questions/2891766/how-can-i-throw-an-exception-in-c
    int sources[N];
    int source_count = 0;
    int order_index = 0;
    int in_degree[N] = {0};
    //calculate in-degrees
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (graph[i][j] >= 1) in_degree[j]++; //if there is an edge from i to j, increment in_degree of j
        }
    }
    //initialize sources stack
    for (int i = 0; i < N; i++){
        if (in_degree[i] == 0){
            sources[source_count++] = i;//if the in_degree = 0 this node is a source
        }
    }
    //process all sources
    while (source_count > 0){
        int current = sources[--source_count];//pop from stack
        topological_order[order_index++] = current;//add to topological order
        for (int neighbor = 0; neighbor < N; neighbor++){//each neighbor now has one less incoming edge
            if (graph[current][neighbor] >= 1){
                in_degree[neighbor]--;
                if (in_degree[neighbor] == 0){//if this neighbor has no more incoming edges
                    sources[source_count++] = neighbor;//it is now a source, push to stack
                }
            }
        }
    }
    return 0;
}

void DFS(int v, int graph[N][N]){
    visited[v] = 1;
    
    for (int neighbor = 0; neighbor < N; neighbor++){
        if (graph[v][neighbor] >= 1 && visited[neighbor] == 0){
            DFS(neighbor, graph);
        }
    }
    leave_times[time_counter++] = v;//sorts by leave time
}


int dfs_topological_sort(int graph[N][N], int topological_order[N]){
    time_counter = 0;
    for (int i = 0; i < N; i++){
        if (visited[i] == 0){
            DFS(i, graph);
        }
    }

    for (int i = 0; i < N; i++) {//reverse leave time = topological order
        topological_order[i] = leave_times[N - 1 - i];//as I gets higher, leave times get lower 
    }
    return 0;
}

void process_topological_order(int status_code, int topological_order[N]){
    if (status_code != 0){
        printf("Error in topological sort\n");
        return;
    }
    printf("Topological Order: ");
    for (int i = 0; i < N; i++){
        printf("%d ", topological_order[i]);
    }
    printf("\n");
}


int main(){
    int G1[N][N] = {
        {0, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1},
        {1, 1, 1, 0, 1, 1},
        {0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0},
    };

    int G2[N][N] = {
        {0, 0, 1, 0, 1, 1},
        {0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1},
        {0, 1, 2, 0, 0, 1},
        {0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0},
    };

    int topological_order[N];//to store the result (C cant return arrays)
    //kahn vs dfs G1
    process_topological_order(kahn(G1, topological_order), topological_order);
    for (int i = 0; i < N; i++) visited[i] = 0;
    process_topological_order(dfs_topological_sort(G1, topological_order), topological_order);
    for (int i = 0; i < N; i++) visited[i] = 0;
    //kahn vs dfs G2
    process_topological_order(kahn(G2, topological_order), topological_order);
    for (int i = 0; i < N; i++) visited[i] = 0;
    process_topological_order(dfs_topological_sort(G2, topological_order), topological_order);
    return 0;

}
