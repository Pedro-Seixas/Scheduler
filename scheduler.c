#include "scheduler.h"
#include <stdio.h>
#include <string.h>

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

void append_string(char *s, const char *to_append){
    if (strlen(s) + strlen(to_append) < sizeof(s)){
        strcat(s, to_append);
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

void run_fifo(Queue* q, Job* jobs, int quantity){
    int current_time = 0;
    int current_job = 0;

    while(1){
       if(current_job == quantity){
           current_job = 0;
       }
       for(int i = 0; i < jobs[current_job].burst_time; i++){
           run_job(&jobs[current_job]);
       }
       current_job++;
    } 
}

void run_sjf(Queue* q, Job* jobs, int quantity){

}

void run_priority(Queue* q, Job* jobs, int quantity){

}

void run_job(Job* job){
    char* c = "#";
    append_string(job->timeline, c);
    job->time_remaining--;
}
