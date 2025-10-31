#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define N 100//rows and cols (max 65535)

#define TOTAL_TICKS 1000//sim time in ticks

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
    Car car;
    uint16_t remaining_time;
} CarInTransit;

typedef struct {
    CarInTransit cars[IN_TRANSIT_CAP];
    uint16_t count; 
} TransitBuffer_t;

typedef struct {
    Car cars[QUEUE_CAP];
    uint16_t count;
} stopBuffer_t;//removed the extra uint16_t for simplicity

typedef struct {
    Node_t from, to;
    TransitBuffer_t in_transit;
    stopBuffer_t stopped;
} Link_t;