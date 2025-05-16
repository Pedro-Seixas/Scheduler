#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>

// Compare function for qsort
int comp(const void* a, const void* b){
    Job* job_A = (Job *) a;
    Job* job_B = (Job *) b;
    return job_A->arrival_time - job_B->arrival_time;
}

// Main function
int main()
{    
    Queue queue;
    init_queue(&queue);

    // {id, arrival_time, burst_time, time_remaining, priority, timeline}
    Job jobs[3] = {
        {0, 2, 5, 5, 0, "", 0},
        {1, 0, 3, 3, 0, "", 0},
        {2, 1, 6, 6, 0, "", 0}
    };
    
    // Defining parameters for qsort function
    int num_jobs = sizeof(jobs)  / sizeof(jobs[0]);
    
    // Quick Sort
    qsort(jobs, num_jobs, sizeof(Job), comp);
    
    run_fifo(&queue, jobs, 3); 

    for(int i = 0; i < 3; i++){
        printf("%s\n", jobs[i].timeline);
    }
    return 0;
}
