#include "scheduler.h"
#include <stdio.h>

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

void run_fifo(Queue* q, Job* jobs, int quantity){
    int current_time = 0;
    while(1){
        // do something
    } 
}

void run_sjf(Queue* q, Job* jobs, int quantity){

}

void run_priority(Queue* q, Job* jobs, int quantity){

}

void run_job(Queue* q, Job* job, int quantity){

}
