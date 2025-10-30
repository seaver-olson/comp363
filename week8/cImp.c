#include "defs.h"

Node_t grid[N][N];
Link_t links[N*N*4];


int turn(int start_dir) {
    float r = (float)rand() / RAND_MAX;//0.0 - 1.0
    if (r < LTURN_PROB) {
        return turn_left(start_dir);
    } else if (r < LTURN_PROB + RTURN_PROB) {
        return turn_right(start_dir);
    } else {
        return opposite_dir(start_dir);
    }
}

//checks if neighbor exists in given direction
int get_neighbor(Node_t node, uint8_t dir, Node_t* neighbor) {
    neighbor->x = node.x;
    neighbor->y = node.y;
    switch (dir) {
        case NORTH: neighbor->y++; break;
        case EAST: neighbor->x++; break;
        case SOUTH: neighbor->y--; break;
        case WEST: neighbor->x--; break;
    }
    return neighbor->x >= 0 && neighbor->x < N && neighbor->y >= 0 && neighbor->y < N;
}

int signal_phase(uint16_t tick, uint8_t ){
    int phase = tick % CYCLE_TOTAL;
    int ns = 0;//is north-south green?
    if (phase < CYCLE_NS_GREEN) ns = 1;
    return ns;
}

void init_grid(){
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j].x = i;
            grid[i][j].y = j;
        }
    }
    //create linkks
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Node_t node = grid[i][j];
            printf("Node (%d, %d)\n", node.x, node.y);
            for (int dir = 0; dir < 4; dir++) {
                Node_t neighbor;
                if (get_neighbor(node, dir, &neighbor)) {
                    Link_t link;
                    link.from = node;
                    link.to = neighbor;
                    link.in_transit.count = 0;
                    link.stopped.count = 0;
                    links[link_index(node.x, node.y, dir)] = link;//I REFUSE TO DO 3D ARRAY
                }
            }
        }
        //boundary links for arrivals
    }
}


int main(){
    srand(time(0));
    init_grid();

    return 0;
}