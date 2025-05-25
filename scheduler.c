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
    pthread_cond_t cond[quantity];
    pthread_mutex_t mutex;
    
    for(int i = 0; i < quantity; i++){
        pthread_cond_init(&cond[i], NULL);
    }

    pthread_mutex_init(&mutex, NULL);

    ThreadArgs args[quantity];
    pthread_t threads[quantity];

    // Create threads
    for(int i = 0; i < quantity; i++){
        args[i].job = &jobs[i];
        args[i].current_time = &current_time;
        args[i].mutex = &mutex;
        args[i].cond = &cond[i];
        pthread_create(&threads[i], NULL, run_job, (void*)& args[i]);
    }

    while(jobs_done != quantity){
        pthread_mutex_lock(&mutex);

        Job* current_job = NULL;
        jobs_done = 0;

        // Check if all jobs are done
        for(int i = 0; i < quantity; i++){
            if(jobs[i].state == DONE){
                jobs_done++;
            }
        }

        // Select the highest priority job
        for(int i = 0; i < quantity; i++){
            if(jobs[i].arrival_time <= current_time && jobs[i].state != DONE){
                jobs[i].state = WAITING;
                if(current_job == NULL || jobs[i].priority > current_job->priority){
                    current_job = &jobs[i];
                }
            }else if(jobs[i].state != DONE){
                jobs[i].state = IDLE;
            }
        }

        // Run selected job, others wait and add "_" to the timeline
        for(int i = 0; i < quantity; i++){
            if(current_job == &jobs[i] && jobs[i].state != DONE){
                jobs[i].state = RUNNING;
            }else if(jobs[i].state == WAITING){
                jobs[i].timeline[current_time] = '_';
            }else{
                jobs[i].timeline[current_time] = ' ';
            }
        }
         
        if(current_job != NULL){
            // Signal Job to run
            pthread_cond_signal(&cond[current_job->id]);
            pthread_mutex_unlock(&mutex);

            usleep(2000);

            // Wait for thread to run
            while(!current_job->ran_this_cycle);

            pthread_mutex_lock(&mutex);
         
            // Advance time
            current_time++;
            current_job->ran_this_cycle = 0;

            pthread_mutex_unlock(&mutex);
        }
    }

    // End jobs
    for(int i = 0; i < quantity; i++){
        pthread_join(threads[i], NULL);
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

    while(1){
        pthread_mutex_lock(args->mutex);
        
        // Wait until job is selected
        while(args->job->state != RUNNING && args->job->state != DONE){
            pthread_cond_wait(args->cond, args->mutex);
        }
        
        // Run job
        if(args->job->ran_this_cycle == 0){
            args->job->timeline[*args->current_time] = '#';
            args->job->time_remaining--;
        }

        args->job->ran_this_cycle = 1;
        
        // Mark it as done
        if(args->job->time_remaining <= 0){
            args->job->state = DONE;
            pthread_mutex_unlock(args->mutex);
            break;
        }

        pthread_mutex_unlock(args->mutex);
        usleep(2000);
    }
}
