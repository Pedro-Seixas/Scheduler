#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct 
{
    int id;
    int arrival_time;
    int burst_time;
    int time_remaining;
    int priority;
    char timeline[100];
} Job;

void run_fifo(Job jobs, quantity);
void run_sjf(Job jobs, quantity);
void run_priority(Job jobs, quantity);
void run_job(Job job, quantity);
#endif
