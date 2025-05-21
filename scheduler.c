#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void check_job_state(Queue* q, Job* jobs, int quantity, int current_time){
    for(int i = 0; i < quantity; i++){
        if(jobs[i].arrival_time <= current_time){
            jobs[i].state = READY;
            enqueue(q, &jobs[i]);
        }
        if(jobs[i].burst_time == 0){
            jobs[i].state = DONE;
        }
    }
}

// First in first out scheduling
// It receives the jobs, then sort them by arrival time
void run_fifo(Queue* q, Job* jobs, int quantity){
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

void run_sjf(Queue* q, Job* jobs, int quantity){
    int current_time = 0;
    // int job_index = 0;
    int jobs_done = 0;
    //qsort(jobs, quantity, sizeof(Job), comp_sjf);
    
    while(jobs_done != quantity){
        Job* current_job = NULL;
        
        // Select the shortest job
        for(int i = 0; i < quantity; i++){
            if(jobs[i].time_remaining <= current_time && jobs[i].state != DONE){
                if(current_job == NULL || jobs[i].time_remaining < current_job->time_remaining){
                    current_job = &jobs[i];
                }
            }
        }

        if(current_job){
            current_job->timeline[current_time] = '#';
            current_job->time_remaining--;

            for(int i = 0; i < quantity; i++){
                if(&jobs[i] != current_job){
                    if(jobs[i].time_remaining > current_time || jobs[i].state == DONE){
                        jobs[i].timeline[current_time] = ' ';
                    }else{
                        jobs[i].timeline[current_time] = '_';
                    }
                }
            }

            if(current_job->time_remaining <= 0 && current_job->state != DONE){
                current_job->state = DONE;
                jobs_done++;
            }
        }else{
            printf("No job ready \n");
        }
        current_time++;
        
    }
}

void run_priority(Queue* q, Job* jobs, int quantity){

}

void run_job(Job* job){
    char* c = "#";
    job->time_remaining--;
}
