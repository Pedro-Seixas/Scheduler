#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <pthread.h>

#define MAX_JOBS 100
#define IDLE     0
#define RUNNING  1
#define WAITING  2
#define DONE     3

#define RED     "\e[1;31m"
#define GREEN   "\e[1;32m"
#define CYAN    "\e[1;36m"
#define RESET   "\e[0m"

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
    int time_completed;
    int response_time;
    int was_response_time_measured;
    int time_waited;
} Job;

typedef struct
{
    Job* job;
    int* current_time;
    pthread_mutex_t* mutex;
    pthread_cond_t* cond;
} ThreadArgs;

void run_fifo(Job* jobs, int quantity, pthread_t threads);
void run_sjf(Job* jobs, int quantity, pthread_t threads);
int run_priority(Job* jobs, int quantity);
void run_rr(Job* jobs, int quantity, pthread_t threads);
void *process(void *arg);
#endif
