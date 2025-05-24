#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Compare function for qsort
int comp(const void* a, const void* b){
    Job* job_A = (Job *) a;
    Job* job_B = (Job *) b;
    return job_A->arrival_time - job_B->arrival_time;
}

int comp_sjf(const void* a, const void* b){
    Job* job_A = (Job *) a;
    Job* job_B = (Job *) b;
    return job_A->time_remaining - job_B->time_remaining;
}

void print_queue(Queue* q){
    for(int i = q->front; i < q->rear + 1; i++){
        Job* job_temp = dequeue(q);
        printf("%s\n", job_temp->timeline);
    }
}
void init_queue(Queue* q){
    q->rear = -1;
    q->front = 0;
    q->size = 0;
}

void enqueue(Queue* q, Job* job){
    if(is_queue_full(q)){
        printf("Queue Full\n");
        return;
    }
    q->jobs[++q->rear] = job;
    q->size++;
}

Job* dequeue(Queue* q){
    if(is_queue_empty(q)){
        printf("Queue Empty\n");
        return;
    }

    Job* job_temp = q->jobs[q->front++];
    q->size--;

    return job_temp;
}

int is_queue_empty(Queue* q){
    if(q->size < 0){
        return 1;
    }else{
        return 0;
    }
}

int is_queue_full(Queue* q){
    if(q->size == MAX_JOBS){
        return 1;
    }else{
        return 0;
    }
}

// First in first out scheduling
// It receives the jobs, then sort them by arrival time
void run_fifo(Queue* q, Job* jobs, int quantity, pthread_t threads){
    // Quick Sort
    qsort(jobs, quantity, sizeof(Job), comp);

    int current_time = 0;
    int job_index = 0;
    
    // Run for every job
    while(job_index < quantity){
       Job* current_job = &jobs[job_index];

       if(current_job->arrival_time <= current_time){
           for(int i = 0; i < current_job->burst_time; i++){
               // Current job running
               current_job->timeline[current_time] = '#';
               current_job->time_remaining--;
                
               // Add "_" or " " to other processes that are not currently running
               for(int j = 0; j < quantity; j++){
                   if(&jobs[j] != current_job){
                       if(jobs[j].arrival_time > current_time || jobs[j].time_remaining <= 0)
                       {
                           // Job idle or done
                           jobs[j].timeline[current_time] = ' ';
                       }else{
                           // Job waiting
                           jobs[j].timeline[current_time] = '_';
                       }
                   }
               }
               current_time++;
           }
       }
       job_index++;
    }
}

void run_sjf(Queue* q, Job* jobs, int quantity, pthread_t  threads){
    int current_time = 0;
    int jobs_done = 0;
    //qsort(jobs, quantity, sizeof(Job), comp_sjf);
    
    while(jobs_done != quantity){
        Job* current_job = NULL;
        
        // Select the shortest job
        for(int i = 0; i < quantity; i++){
            if(jobs[i].arrival_time <= current_time && jobs[i].state != DONE){
                if(current_job == NULL || jobs[i].time_remaining < current_job->time_remaining){
                    current_job = &jobs[i];
                }
            }
        }

        if(current_job){

            // run_job(current_job, jobs, quantity, current_time);

            // Mark it as done
            if(current_job->time_remaining <= 0 && current_job->state != DONE){
                current_job->state = DONE;
                jobs_done++;   
            }
        }

        current_time++;
    }
}

void run_priority(Queue* q, Job* jobs, int quantity){
    int current_time = 0;
    int jobs_done = 0;
    int iret[quantity];
    ThreadArgs* args;
    pthread_t threads[quantity];
    
    // Create threads
    for(int i = 0; i < quantity; i++){
        args->job = &jobs[i];
        args->current_time = &current_time;
        iret[i] = pthread_create(&threads[i], NULL, run_job, (void*) args);
    }
    
    while(jobs_done != quantity){
        Job* current_job = NULL;
        jobs_done = 0;
        
        // Select the highest priority job
        for(int i = 0; i < quantity; i++){
            if(jobs[i].arrival_time <= current_time && jobs[i].state != DONE){
                if(current_job == NULL || jobs[i].priority > current_job->priority){
                    current_job = &jobs[i];
                }
            }
        }

        if(current_job){
            current_job->state = RUNNING;
        }

        // Check if all jobs are done
        for(int i = 0; i < quantity; i++){
            if(jobs[i].state == DONE){
                jobs_done++;
            }
        }

        current_time++;
    }
}

void run_rr(Queue* q, Job* jobs, int quantity, pthread_t threads){
    int current_time = 0;
    int jobs_done = 0;
    int job_index = -1;
    
    while(jobs_done != quantity){
        Job* current_job = NULL;
        
        // Select next job
        job_index = (job_index + 1) % quantity;
        
        // See if next job is ready
        if(jobs[job_index].arrival_time <= current_time && jobs[job_index].state != DONE){
            current_job = &jobs[job_index];
        }else{
            continue;
        }
        
        if(current_job){

           // run_job(current_job, jobs, quantity, current_time);
            
            // Mark it as done
            if(current_job->time_remaining <= 0 && current_job->state != DONE){
                current_job->state = DONE;
                jobs_done++;   
            }
        }
        current_time++;
    }
}

void* run_job(void *arg){

    ThreadArgs* args = (ThreadArgs*) arg;
    int current_time = *(args->current_time);

    while(args->job->state != DONE){
        // Running job
        if(args->job->state == RUNNING){
            args->job->timeline[current_time] = '#';
            args->job->state = WAITING;
            args->job->time_remaining--;
        }else if(args->job->state == WAITING){
            args->job->timeline[current_time] = '_';
        }else if(args->job->state == IDLE){
            args->job->timeline[current_time] = ' ';
        }

        if(args->job->time_remaining <= 0){
            args->job->state = DONE;

        }
        printf("Job id %d has %d left\n", args->job->id, args->job->time_remaining);

    }
}
