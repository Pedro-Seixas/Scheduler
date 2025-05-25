#ifndef SCHEDULER_H
#define SCHEDULER_H
#define MAX_JOBS 100
#define IDLE     0
#define RUNNING  1
#define WAITING  2
#define DONE     3
#include <pthread.h>

typedef struct 
{
    int id;
    int arrival_time;
    int burst_time;
    int time_remaining;
    int priority;
    char timeline[100];
    int state;
    int ran_this_cycle;
} Job;

// Create a Queue data structure
typedef struct
{
    Job* jobs[MAX_JOBS];
    int rear;
    int front;
    int size;
} Queue;

typedef struct
{
    Job* job;
    int* current_time;
    pthread_mutex_t* mutex;
    pthread_cond_t* cond;
} ThreadArgs;

void init_queue(Queue* q);
void enqueue(Queue* q, Job* job);
Job* dequeue(Queue* q);
int is_queue_full(Queue* q);
int is_queue_empty(Queue* q);
void print_queue(Queue* q);
void append_string(char* s, const char* to_append);
void run_fifo(Queue* q, Job* jobs, int quantity, pthread_t threads);
void run_sjf(Queue* q, Job* jobs, int quantity, pthread_t threads);
void run_priority(Queue* q, Job* jobs, int quantity);
void run_rr(Queue* q, Job* jobs, int quantity, pthread_t threads);
void *run_job(void *arg);
#endif
