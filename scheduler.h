#ifndef SCHEDULER.H
#define SCHEDULER.H

typedef struct 
{
    int id;
    int arrival_time;
    int burst_time;
    int time_remaining;
    int priority;
    char timeline[100];
} Job;

void run_fifo();
void run_sjf();
void run_priority();
void run_job();
#endif SCHEDULER.H
