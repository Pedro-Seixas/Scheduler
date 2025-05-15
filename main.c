#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>

int comp(const void* a, const void* b){
    Job* job_A = (Job *) a;
    Job* job_B = (Job *) b;
    return job_A->arrival_time - job_B->arrival_time;
}
int main()
{    
    // {id, arrival_time, burst_time, time_remaining, priority, timeline}
    Job jobs[3] = {
        {0, 0, 5, 5, 0, ""},
        {1, 1, 3, 3, 0, ""},
        {2, 2, 6, 6, 0, ""}
    };
    int num_jobs = sizeof(jobs)  / sizeof(jobs[0]);

    qsort(jobs, num_jobs, sizeof(Job), comp);

    return 0;
}
