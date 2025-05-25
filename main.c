#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUMBER_OF_JOBS 4

// Main function
int main()
{    
    // Initialize the main queue
    Queue queue;
    init_queue(&queue);

    // {id, arrival_time, burst_time, time_remaining, priority, timeline, state}
    Job jobs[NUMBER_OF_JOBS] = {
        {0, 1, 7, 7, 2, "", 0, 0},
        {1, 0, 3, 3, 3, "", 0, 0},
        {2, 0, 6, 6, 1, "", 0, 0},
        {3, 0, 4, 4, 0, "", 0, 0}
    };
   
    // Run scheduler
    // run_fifo(&queue, jobs, NUMBER_OF_JOBS); 
    // run_sjf(&queue, jobs, NUMBER_OF_JOBS);
    run_priority(&queue, jobs, NUMBER_OF_JOBS);
    
    // run_rr(&queue, jobs, NUMBER_OF_JOBS);
    
    // Print timelines
    for(int i = 0; i < NUMBER_OF_JOBS; i++){
        printf("Job %i: %s\n", jobs[i].id, jobs[i].timeline);
    }

    return 0;
}
