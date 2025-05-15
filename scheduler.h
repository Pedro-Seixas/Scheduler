#ifndef SCHEDULER_H
#define SCHEDULER_H
#define MAX_JOBS 100

typedef struct 
{
    int id;
    int arrival_time;
    int burst_time;
    int time_remaining;
    int priority;
    char timeline[100];
} Job;

// Create a Queue data structure
typedef struct
{
    Job jobs[MAX_JOBS];
    int rear;
    int front;
} Queue;

void enqueue(Queue* q, Job* job);
void dequeue(Queue* q, Job* job);
void is_queue_empty(Queue* q);
void run_fifo(Job* jobs, quantity);
void run_sjf(Job* jobs, quantity);
void run_priority(Job* jobs, quantity);
void run_job(Job* job, quantity);
#endif
