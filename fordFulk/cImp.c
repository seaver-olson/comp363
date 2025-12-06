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
            path[(*path_length)++] = current;
            current = parent[current];
        }
        //reverse path
        for (int i = 0; i < *path_length / 2; i++){
            int temp = path[i];
            path[i] = path[*path_length - 1 - i];
            path[*path_length - 1 - i] = temp;
        }
    } else *path_length = 0;//no path found
}

int get_source(int graph[NODE_COUNT][NODE_COUNT]){
    int source = -1;
    for (int i = 0; i < NODE_COUNT; i++){
        int incoming = 0;
        for (int j = 0; j < NODE_COUNT; j++){
            if (graph[j][i] > 0){
                incoming++;
                j = NODE_COUNT;
            }
        }
        if (incoming == 0){
            source = i;
            i = NODE_COUNT;
        }
    }
    return source;
}

int get_sink(int graph[NODE_COUNT][NODE_COUNT]){
    int sink = -1;
    for (int i = 0; i < NODE_COUNT; i++){
        int outgoing = 0;
        for (int j = 0; j < NODE_COUNT; j++){
            if (graph[i][j] > 0){
                outgoing++;
                j = NODE_COUNT;
            }
        }
        if (outgoing == 0){
            sink = i;
            i = NODE_COUNT;
        }
    }
    return sink;
}

int ford_fulkerson(int graph[NODE_COUNT][NODE_COUNT], int source, int sink){
    int residual_graph[NODE_COUNT][NODE_COUNT];
    for (int i = 0; i < NODE_COUNT; i++){
        for (int j = 0; j < NODE_COUNT; j++){
            residual_graph[i][j] = graph[i][j];
        }
    }


    int max_flow = 0;
    //vars for path finding
    int path[NODE_COUNT];
    int path_length = 0;

    find_path(residual_graph, source, sink, path, &path_length);
    while (path_length > 0){
        printf("Found path: ");
        for (int i = 0; i < path_length; i++){
            printf("%d ", path[i]);
        } printf("\n");
        int min_cap = residual_graph[path[0]][path[1]];//min cap starts as first edge
        for (int i = 1; i < path_length - 1; i++){
            int cap_test = residual_graph[path[i]][path[i + 1]];
            if (cap_test < min_cap)
                min_cap = cap_test;
        }
        printf("Minimum capacity along path: %d\n", min_cap);

        //update residual graph
        for (int i = 0; i < path_length - 1; i++){
            residual_graph[path[i]][path[i + 1]] -= min_cap;//forward edge
            residual_graph[path[i + 1]][path[i]] += min_cap;//backward edge
        }

        max_flow += min_cap;//add to max flow
        find_path(residual_graph, source, sink, path, &path_length);//get new path
    }
    return max_flow;
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
    int max_flow = ford_fulkerson(residual_graph, source, sink);
    printf("Max Flow: %d\n", max_flow);
    return 0;
}