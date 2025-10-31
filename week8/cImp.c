#include "defs.h"



//direction helpers - optimize later
uint8_t turn_right(uint8_t dir) {
    return (dir + 1) % 4;
}

uint8_t opposite_dir(uint8_t dir) {
    return (dir + 2) % 4;
}

uint8_t turn_left(uint8_t dir) {
    return (dir + 3) % 4;
}

Node_t grid[N][N];
Link_t links[N*N*4];
TransitBuffer_t in_transitBuffer;
//simulation stats
int car_id = 0;
int completed = 0;
int sum_tt = 0;
int queue_samples = 0;
int sum_queue = 0;

Link_t *get_link(Node_t from, uint8_t dir) {
    return &links[link_index(from.x, from.y, dir)];
}

int enqueue_departure(Car car, Link_t* link) {
    //if the link has space move the car to stopped buffer
    int old_count = link->stopped.count;
    if (link->stopped.count < QUEUE_CAP) 
        link->stopped.cars[link->stopped.count++] = car;
    return link->stopped.count - old_count;//1 if enqueued else 0
}

int turn(int start_dir) {
    float r = (float)rand() / RAND_MAX;//0.0 - 1.0
    if (r < LTURN_PROB) start_dir = turn_left(start_dir);
    else if (r < LTURN_PROB + RTURN_PROB) start_dir = turn_right(start_dir);
    return start_dir;
}

//checks if neighbor exists in given direction
int get_neighbor(Node_t node, uint8_t dir, Node_t* neighbor) {
    neighbor->x = node.x;
    neighbor->y = node.y;
    switch (dir) {
        case NORTH: neighbor->x--; break;
        case EAST: neighbor->y++; break;
        case SOUTH: neighbor->x++; break;
        case WEST: neighbor->y--; break;
    }
    return neighbor->x >= 0 && neighbor->x < N && neighbor->y >= 0 && neighbor->y < N;
}

//future ext?
uint8_t signal_phase(uint16_t tick, Node_t node) {
    int phase = tick % CYCLE_TOTAL;
    uint8_t ns = 0;//is north-south green?
    if (phase < CYCLE_NS_GREEN) ns = 1;
    return ns;
}

uint8_t is_green(uint16_t tick, Node_t node, uint8_t dir) {
    int ns = signal_phase(tick, node);
    if (dir % 2 == 0) return ns;
    else return !ns; 
}


int serve_intersection(uint16_t tick, Node_t node) {
    //clears queues for approaches that have a green light
    uint8_t greenDir = signal_phase(tick, node);
    int served = 0; // num cars served
    for (uint8_t approachDir = 0; approachDir < 4; approachDir++){

    }
    return 0;
}

void init_grid(){
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j].x = i;
            grid[i][j].y = j;
        }
    }
    //create links
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
    while (sum_tt)
    return 0;
}