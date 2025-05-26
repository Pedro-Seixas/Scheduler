# Scheduler
This project is a program that simulates different job scheduling algorithms. 

Current job scheduling algorithms implemented are First Come First Serve (FCFS or FIFO), Short Job First Preemptive (SJF), Priority Scheduling Preemptive, and Round Robin.

I have implemented a queue data structure but it has not been used yet, future work is to implement a ready queue to better simulate the different states of tasks, because currently I am using only arrays.

To better simulate a process, I am using threads where I select which thread to run by using mutexes and conditional variables.

## To Run this Program
Clone the repository and write the following in your terminal:
```
make scheduler
```

## Example with Priority Scheduler and Threads
"#" = Running

"_" = Waiting

" " = Idle (not ready/arrived)

![image](https://github.com/user-attachments/assets/13cb49ff-ee92-4e42-8732-9a812344285d)

## Future work: Implement all the states using queues
![image](https://github.com/user-attachments/assets/147399d2-f6c6-4f5d-bf1d-4541ccbe5113)

## TODOs (in order)
- Use pthreads to better simulate tasks [DONE]
- Add a Demo Screenshot to the README [DONE]
- Display throughput, wait time, response time, and turn-around time [Done]
- Add colors
- Implement states and queues (maybe, I have other projects and certifications to work on)
