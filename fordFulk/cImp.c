#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NODE_COUNT 5
#define MAX_CAPACITY 10


void find_path(int graph[NODE_COUNT][NODE_COUNT], int source, int sink, int path[NODE_COUNT], int* path_length){
    int parent[NODE_COUNT];
    int marked[NODE_COUNT] = {0};
    int stack[NODE_COUNT];
    int stack_top = 0;
    for (int i = 0; i < NODE_COUNT; i++) parent[i] = -1;//no parents yet

    stack[stack_top++] = source;//source on stack
    marked[source] = 1;//source visited

    while (stack_top > 0){//while stack not empty
        int current = stack[--stack_top];//pop
        if (current == sink)
            stack_top = 0;//clear stack to break loop
        else {
            for (int neighbor = 0; neighbor < NODE_COUNT; neighbor++){
                if (graph[current][neighbor] > 0 && marked[neighbor] == 0){//if there is capacity and not visited
                    stack[stack_top++] = neighbor;//push to stack
                    marked[neighbor] = 1;//mark visited
                    parent[neighbor] = current;//set parent
                    if (neighbor == sink){
                        stack_top = 0;//clear stack to break loop
                        neighbor = NODE_COUNT;//break for loop
                    }
                }
            }
        }
    }
    if (parent[sink] != -1){
        //reconstruct path
        *path_length = 0;
        int current = sink;
        while (current != -1){
            path[*path_length++] = current;//add current node to path
            current = parent[current];//go to parent
        }

        //reverse path
        for (int i = 0; i < *path_length / 2; i++){
            int temp = path[i];
            path[i] = path[*path_length - 1 - i];
            path[*path_length - 1 - i] = temp;
        }
    } else *path_length = 0;//no path found
}

void generate_residual_graph(int graph[NODE_COUNT][NODE_COUNT]){
    for (int i = 0; i < NODE_COUNT; i++){
        for (int j = 0; j < NODE_COUNT; j++){
            graph[i][j] = 0;
        }
    }
}

int find_min_capacity(){
    int min = __INT_MAX__;
    for (int i = 0; i < )

}

int get_source(int graph[NODE_COUNT][NODE_COUNT]){
    int source = -1;//inefficient but simple and no breaks or early returns
    for (int i = 0; i < NODE_COUNT; i++)
        for (int j = 0; j < NODE_COUNT; j++)
            if (graph[j][i] > 0)
                source = i; 
    return source;
}

int get_sink(int graph[NODE_COUNT][NODE_COUNT]){
    int sink = -1;
    for (int i = 0; i < NODE_COUNT; i++)
        for (int j = 0; j < NODE_COUNT; j++)
            if (graph[i][j] > 0)
                sink = i; 
    return sink;
}

int main(){
    int residual_graph[NODE_COUNT][NODE_COUNT] = {
        {0, 20,  0,  0,  0},
        {0,  0,  5,  6,  0},
        {0,  0,  0,  3,  7},
        {0,  0,  0,  0,  8},
        {0,  0,  0,  0,  0}
    };
    int source = get_source(residual_graph);
    int sink = get_sink(residual_graph);
    printf("Source: %d, Sink: %d\n", source, sink);
    return 0;
}