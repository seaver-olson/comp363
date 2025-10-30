#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define N 100//rows and cols (max 65535)

#define TOTAL_TICKS = 1000//sim time in ticks

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define LTURN_PROB 0.25f
#define RTURN_PROB 0.25f
#define STRAIGHT_PROB 0.5f

#define CYCLE_NS_GREEN 20
#define CYCLE_EW_GREEN 20
#define CYCLE_TOTAL (CYCLE_NS_GREEN + CYCLE_EW_GREEN)

#define FLOW_PER_TICK 1

#define QUEUE_CAP 10

#define BASE_TRAVEL_T 60//base travel time 

#define ARRIVAL_RATE .33f

#define IN_TRANSIT_CAP 50

#define link_index(x,y,dir) ((x*N+y)*4+dir)

typedef struct {
    int32_t x, y;//implys a max N of 65535
} Node_t;

typedef struct {
    uint32_t id;
    uint16_t enter_time;
} Car;

typedef struct {
    Car cars[IN_TRANSIT_CAP];
    uint16_t count; 
} TransitBuffer_t;


typedef struct {
    Node_t from, to;
    TransitBuffer_t in_transit;
    TransitBuffer_t stopped;
} Link_t;

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
