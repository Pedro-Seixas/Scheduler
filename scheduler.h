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
    Job* jobs[MAX_JOBS];
    int rear;
    int front;
    int size;
} Queue;

void init_queue(Queue* q);
void enqueue(Queue* q, Job* job);
Job* dequeue(Queue* q);
int is_queue_full(Queue* q);
int is_queue_empty(Queue* q);
void print_queue(Queue* q);
void run_fifo(Queue* q, Job* jobs, int quantity);
void run_sjf(Queue* q, Job* jobs, int quantity);
void run_priority(Queue* q, Job* jobs, int quantity);
void run_job(Queue* q, Job* job, int quantity);
#endif
