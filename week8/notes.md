# Notes for Discrete event simulator (C implementation)



## Car rules:
- Acceleration: if the `v` < `v_max` and there is enough space ahead, increase speed by 1 unit.
- Deceleration: if the distance to the car ahead is less than current speed, reduce speed by j-1 units to avoid collision.
- Randomization: with probability `p`, reduce speed by 1 unit if speed > 0.
- Car Motion: each vechicle moves forward by `v` units each time step.




## Leo's assignment notes below
- Monte-Carlo modeling of car traffic in a city
- City is simulated as a grid of N x N rectilinear intersections
- One lane per direction 
- Cardinal directions only (N, S, E, W)
- traffic lights at each intersection with fixed cycle time
- road segments between intersections, discretized

### Outline
- Discrete-event simulation (DES)

    - General idea: model a system where “interesting” changes happen at discrete times (car finishes a block, light turns green, etc.), not continuously.
    - Things to look up: event scheduling, state variables, performance measures (throughput, delay, queue length).
- Traffic flow as a queuing network
    - Each road segment is basically a server with limited capacity and a stochastic service time (your sample_link_travel_time).
    - The stop line at the signal is a FIFO queue with a saturation flow (how many cars per green tick can discharge).
- Signalized intersection control
    - The `signal_phase` function gives a fixed 2-phase controller: N/S is green for some number of ticks, then E/W.
    - This is the simplest form of fixed-time control (a.k.a. pre-timed signals). Search terms: cycle length, split, phase, saturation flow rate.
- Cellular automata traffic models (Nagel–Schreckenberg style)
    - A [1992 cellular automaton model](https://en.wikipedia.org/wiki/Nagel%E2%80%93Schreckenberg_model) of single-lane highways that inspired a ton of simple-but-realistic traffic simulations.
- Boundary conditions and injection rate
    - `is_boundary_incoming_link` and `ARRIVAL_RATE` together define demand loading how cars enter the network. (If you want to go technical, this is the arrival rate in an M/M/1 queue, spatially distributed on the edges of the grid).

