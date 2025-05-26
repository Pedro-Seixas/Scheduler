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
        {1, 2, 3, 3, 3, "", 0, 0},
        {2, 4, 6, 6, 1, "", 0, 0},
        {3, 0, 4, 4, 0, "", 0, 0}
    };

    for(int i = 0; i < NUMBER_OF_JOBS; i++){
        printf("Job #%d - Arrival Time: %d Burst Time: %d Priority: %d\n", 
                jobs[i].id, jobs[i].arrival_time, jobs[i].burst_time, jobs[i].priority);    
    } 

    printf("\nRunning Priority Scheduling...\n\n");
    
    // Run scheduler
    // run_fifo(&queue, jobs, NUMBER_OF_JOBS); 
    // run_sjf(&queue, jobs, NUMBER_OF_JOBS);
    int end_time = run_priority(&queue, jobs, NUMBER_OF_JOBS);
    
    // run_rr(&queue, jobs, NUMBER_OF_JOBS);
    
    printf(CYAN "Job ended after %d time units\n", end_time);
    printf("Throughput = %.2f\n" RESET, (float)NUMBER_OF_JOBS / end_time);
    
    // Print Line
    for(int i = 0; i < 110; i++){
        printf("_");
    }
    printf("\n\n");

    // Print timeline
    for(int i = 0; i < NUMBER_OF_JOBS; i++){
        printf(CYAN "Job %d:" RESET, jobs[i].id);

        for(int j = 0; j < end_time; j++){
            if(jobs[i].timeline[j] == '#'){
                printf(GREEN "%c" RESET, jobs[i].timeline[j]); 
            }else{
                printf(RED "%c" RESET, jobs[i].timeline[j]);
            }
        }

        printf(CYAN "| Response Time: %2d | Time Completed: %2d | Time Waited: %2d | Turnaround Time: %2d |\n" RESET, 
            jobs[i].response_time, jobs[i].time_completed,jobs[i].time_waited, 
            jobs[i].time_completed - jobs[i].response_time);
    }
    
    // Print Line
    for(int i = 0; i < 110; i++){
        printf("_");
    }
    printf("\n");
    return 0;
}
