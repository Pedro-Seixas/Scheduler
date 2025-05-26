#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

Job* select_highest_priority_job(Job* jobs, int quantity, int current_time);
Job* select_shortest_job(Job* jobs, int quantity, int current_time);
void update_timeline(Job* current_job, Job* jobs, int quantity, int current_time);
void run_job(Job* current_job, int* current_time, pthread_cond_t* cond, pthread_mutex_t* mutex);
int are_all_jobs_done(Job* jobs, int quantity);

// Compare function for qsort
int comp(const void* a, const void* b){
    Job* job_A = (Job *) a;
    Job* job_B = (Job *) b;
    return job_A->arrival_time - job_B->arrival_time;
}

// First in first out scheduling
// It receives the jobs, then sort them by arrival time
void run_fifo(Job* jobs, int quantity, pthread_t threads){
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

int run_sjf(Job* jobs, int quantity){
    int current_time = 0;
    int done_flag = 0;
    
    pthread_cond_t cond[quantity];
    pthread_mutex_t mutex;
    
    ThreadArgs args[quantity];
    pthread_t threads[quantity];

    // Init mutex and conditional variable
    for(int i = 0; i < quantity; i++){
        pthread_cond_init(&cond[i], NULL);
    }
    
    pthread_mutex_init(&mutex, NULL);
    
    // Create threads
    for(int i = 0; i < quantity; i++){
        args[i].job = &jobs[i];
        args[i].current_time = &current_time;
        args[i].mutex = &mutex;
        args[i].cond = &cond[i];
        pthread_create(&threads[i], NULL, process, (void*)& args[i]);
    }

    while(!done_flag){
        pthread_mutex_lock(&mutex);
        Job* current_job = NULL;
       
        // Check if jobs are done
        done_flag = are_all_jobs_done(jobs, quantity);
        
        // Select the shortest job
        current_job = select_shortest_job(jobs, quantity, current_time);
        
        // Run selected job, others wait and add "_" to the timeline
        update_timeline(current_job, jobs, quantity, current_time);
        
        if(current_job){
            // Run Job
            run_job(current_job, &current_time, cond, &mutex);
        }else{
            current_time++;
            pthread_mutex_unlock(&mutex);
        }
    }
    
    // End jobs
    for(int i = 0; i < quantity; i++){
        pthread_join(threads[i], NULL);
    }
    
    return current_time;
}

// Priority Preemptive
int run_priority(Job* jobs, int quantity){
    int current_time = 0;
    int done_flag  = 0;

    pthread_cond_t cond[quantity];
    pthread_mutex_t mutex;
    
    ThreadArgs args[quantity];
    pthread_t threads[quantity];

    // Init mutex and conditional variable
    for(int i = 0; i < quantity; i++){
        pthread_cond_init(&cond[i], NULL);
    }
    pthread_mutex_init(&mutex, NULL);
    
    // Create threads
    for(int i = 0; i < quantity; i++){
        args[i].job = &jobs[i];
        args[i].current_time = &current_time;
        args[i].mutex = &mutex;
        args[i].cond = &cond[i];
        pthread_create(&threads[i], NULL, process, (void*)& args[i]);
    }

    while(!done_flag){
        pthread_mutex_lock(&mutex);
        Job* current_job = NULL;

        // Check if all jobs are done
        done_flag = are_all_jobs_done(jobs, quantity);

        // Select the highest priority job
        current_job = select_highest_priority_job(jobs, quantity, current_time);

        // Update the timeline for jobs NOT running
        update_timeline(current_job, jobs, quantity, current_time);
            
        if(current_job){
            // Run Thread/Job
            run_job(current_job, &current_time, cond, &mutex); 
        }else{
            current_time++;
            pthread_mutex_unlock(&mutex);
        }    
    }
    
    // End jobs
    for(int i = 0; i < quantity; i++){
        pthread_join(threads[i], NULL);
    }
    
    return current_time;
}

// Round Robin
int run_rr(Job* jobs, int quantity){
    int current_time = 0;
    int done_flag  = 0;
    int job_index = -1;

    // This flag ensures the scheduler does not enter in a loop
    // Checking for jobs without progressing time
    int number_of_jobs_not_arrived = 0;

    pthread_cond_t cond[quantity];
    pthread_mutex_t mutex;
    
    ThreadArgs args[quantity];
    pthread_t threads[quantity];

    // Init mutex and conditional variable
    for(int i = 0; i < quantity; i++){
        pthread_cond_init(&cond[i], NULL);
    }
    pthread_mutex_init(&mutex, NULL);
    
    // Create threads
    for(int i = 0; i < quantity; i++){
        args[i].job = &jobs[i];
        args[i].current_time = &current_time;
        args[i].mutex = &mutex;
        args[i].cond = &cond[i];
        pthread_create(&threads[i], NULL, process, (void*)& args[i]);
    }

    while(!done_flag){
        pthread_mutex_lock(&mutex);
        Job* current_job = NULL;
               
        // Check if all jobs are done
        done_flag = are_all_jobs_done(jobs, quantity);

        // Select next job
        job_index = (job_index + 1) % quantity;

        // Select next job if ready
        if(jobs[job_index].arrival_time <= current_time && jobs[job_index].state != DONE){
            current_job = &jobs[job_index];
        }else{
            if(number_of_jobs_not_arrived == quantity){
                current_time++;
                number_of_jobs_not_arrived = 0;
                job_index = -1;
            }else{
                number_of_jobs_not_arrived++;
            }

            pthread_mutex_unlock(&mutex);
            continue;
        }
        
        for(int i = 0; i < quantity; i++){
            if(&jobs[i] != current_job && jobs[i].arrival_time <= current_time && jobs[i].state != DONE ){
                jobs[i].state = WAITING;
            }else if(&jobs[i] != current_job && jobs[i].state != DONE){
                jobs[i].state = IDLE;
            } 
        }
        
        // Update timeline
        update_timeline(current_job, jobs, quantity, current_time);
        
        printf("Selected %d\n", current_job->id);
        
        if(current_job){
            run_job(current_job, &current_time, cond, &mutex);
            printf("Entered here?\n");
        }else{
            current_time++;
            pthread_mutex_unlock(&mutex);
        }
    }
    
    // End jobs
    for(int i = 0; i < quantity; i++){
        pthread_join(threads[i], NULL);
    }
    
    return current_time;
}

// Thread Function
void* process(void *arg){

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
            printf("Job %d DONE\n", args->job->id);
            args->job->time_completed = *args->current_time;
            pthread_mutex_unlock(args->mutex);
            break;
        }

        pthread_mutex_unlock(args->mutex);
        usleep(2000);
    }
}

/*
 *Helper functions
 * 
*/

Job* select_shortest_job(Job* jobs, int quantity, int current_time){
    Job* current_job = NULL;

    for(int i = 0; i < quantity; i++){
        if(jobs[i].arrival_time <= current_time && jobs[i].state != DONE){
            jobs[i].state = WAITING;
            if(current_job == NULL || jobs[i].time_remaining <= current_job->time_remaining){
                current_job = &jobs[i];
            }
        }else if (jobs[i].state != DONE){
            jobs[i].state = IDLE;
        }
    }

    return current_job;
}

Job* select_highest_priority_job(Job* jobs, int quantity, int current_time){
    Job* current_job = NULL;
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
    return current_job;
}

void update_timeline(Job* current_job, Job* jobs, int quantity, int current_time){
    for(int i = 0; i < quantity; i++){
        if(current_job == &jobs[i] && jobs[i].state != DONE){
            jobs[i].state = RUNNING;    
            
            // Get Response time
            if(jobs[i].was_response_time_measured == 0){
                jobs[i].response_time = current_time;
                jobs[i].was_response_time_measured = 1;
            }
        }else if(jobs[i].state == WAITING){
            jobs[i].time_waited++;
            jobs[i].timeline[current_time] = '_';
        }else{
            jobs[i].timeline[current_time] = ' ';
        }
    }
}

void run_job(Job* current_job, int* current_time, pthread_cond_t* cond, pthread_mutex_t* mutex){
    if(current_job != NULL){
        // Signal Job to run
        pthread_cond_signal(&cond[current_job->id]);
        pthread_mutex_unlock(mutex);
        
        // Wait for thread to run
        while(!current_job->ran_this_cycle);

        pthread_mutex_lock(mutex);
         
        // Advance time
        (*current_time)++;

        current_job->ran_this_cycle = 0;

        pthread_mutex_unlock(mutex);
    }
}

int are_all_jobs_done(Job* jobs, int quantity){
    int jobs_done = 0;
    for(int i = 0; i < quantity; i++){
        if(jobs[i].state == DONE){
            jobs_done++;
        }
    }
    return jobs_done == quantity;
}

