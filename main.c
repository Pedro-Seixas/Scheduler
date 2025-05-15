#include "scheduler.h"
#include <stdio.h>

int main()
{    
    // {id, arrival_time, burst_time, time_remaining, priority, timeline}
    Job jobs[3] = {
        {0, 0, 5, 0, 0, "A"},
        {0, 0, 5, 0, 0, "B"},
        {0, 0, 5, 0, 0, "C"}
    };
    Queue queue;
    init_queue(&queue);

    enqueue(&queue, &jobs[0]);
    enqueue(&queue, &jobs[1]);
    enqueue(&queue, &jobs[2]);
    dequeue(&queue);
    
    print_queue(&queue);
    return 0;
}
