#include "defs.h"



//direction helpers - optimize later
uint8_t turn_right(uint8_t dir) {
    return (dir + 1) % 4;
}

uint8_t opposite_dir(uint8_t dir) {
    return (dir + 2) % 4;
}

uint8_t turn_left(uint8_t dir) {
    return (dir + 3) & 3;//same as dir + 3 mod 4
}

static inline uint8_t in_bounds(int x, int y) {
    return (unsigned)x < N && (unsigned)y < N;
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
    int old_count = link->in_transit.count;
    if (old_count < IN_TRANSIT_CAP)
        link->in_transit.cars[link->in_transit.count++] = (CarInTransit){car, BASE_TRAVEL_T};
    return link->in_transit.count - old_count;
}

int is_boundary_incoming_link(Node_t src, Node_t dst){
    int si = src.x;
    int sj = src.y;
    int di = dst.x;
    int dj = dst.y;
    return (
        (si == 0 && di == si + 1 && sj == dj) || //from outside southbound
        (si == N - 1 && di == si - 1 && sj == dj) || //from outside northbound
        (sj == 0 && dj == sj + 1 && si == di) || //from outside eastbound
        (sj == N - 1 && dj == sj - 1 && si == di) //from outside westbound
    );
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
    return in_bounds(neighbor->x, neighbor->y);
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

void move_arrivals_to_queue() {
    for (int i = 0; i < N * N * 4; i++) {
        Link_t *link = &links[i];
    
        for (int j = 0; j < link->in_transit.count; j++) link->in_transit.cars[j].remaining_time--;

        int write_idx = 0;
        for (int j = 0; j < link->in_transit.count; j++) {
            if (link->in_transit.cars[j].remaining_time <= 0 && 
                link->stopped.count < QUEUE_CAP) {
                link->stopped.cars[link->stopped.count++] = link->in_transit.cars[j].car;
            } else {
                if (link->in_transit.cars[j].remaining_time < 0) link->in_transit.cars[j].remaining_time = 0;
                link->in_transit.cars[write_idx++] = link->in_transit.cars[j];
            }
        }
        link->in_transit.count = write_idx;
    }
}

void spawn_boundary_arrivals(uint16_t tick) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Node_t node = grid[i][j];
            for (int dir = 0; dir < 4; dir++) {
                Node_t neighbor;
                if (!get_neighbor(node, dir, &neighbor)) {
                    //spawn 
                    if ((float)rand() / RAND_MAX < ARRIVAL_RATE) {
                        Car new_car = {car_id++, tick};
                        Link_t *link = get_link(node, dir);
                        enqueue_departure(new_car, link);
                    }
                }
            }
        }
    }
}

int serve_intersection(uint16_t tick, Node_t node) {
    int served = 0;
    for (int dir = 0; dir < 4; dir++) {
        if (dir % 2 == is_green(tick, node, dir)) {
            Link_t* link = get_link(node, dir);
            int flow = FLOW_PER_TICK;
            while (flow > 0 && link->stopped.count > 0) {
                //dequeue from stopped buffer
                Car car = link->stopped.cars[0];
                for (int i = 1; i < link->stopped.count; i++) link->stopped.cars[i - 1] = link->stopped.cars[i];//restructure array
                link->stopped.count--;

                uint8_t new_dir = turn(dir);

                Node_t neighbor;
                if (get_neighbor(node, new_dir, &neighbor)) {
                    //car continues in grid
                    Link_t* next_link = get_link(node, new_dir);
                    enqueue_departure(car, next_link);
                } else {
                    //car exits the grid
                    completed++;
                    sum_tt += (tick - car.enter_time);
                }
                flow--;
                served++;
            }
        }
    }
    return served;
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
    }

}


int main(){
    srand(time(0));
    init_grid();
    int tick = 0;
    while (tick<TOTAL_TICKS) {
        move_arrivals_to_queue();
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                serve_intersection(tick, grid[i][j]);
            }
        }
        spawn_boundary_arrivals(tick);
        tick++;
    }
    printf("SIMULATION RESULTS:\n");
    printf("Total cars completed: %d\n", completed);
    if (completed > 0)
        printf("Average travel time: %.2f ticks\n", (float)sum_tt / completed);
    //i havent implemented queue sampling yet
    return 0;
}