# Scheduler
This project is a program that simulates different job scheduling algorithms. 

Current job scheduling algorithms implemented are First Come First Serve (FCFS or FIFO), Short Job First Preemptive (SJF), Priority Scheduling, and Round Robin.

I have implemented a queue data structure but it has not been used yet, future work is to implement a ready queue to better simulate the different states of tasks, because currently I am using only arrays.

## To Run this Program
Clone the repository and write the following in your terminal:
```
make scheduler
```

## Future work: Implement all the states using queues
![image](https://github.com/user-attachments/assets/147399d2-f6c6-4f5d-bf1d-4541ccbe5113)

## TODOs (in order)
- Use pthreads to better simulate tasks
- Display throughput, wait time, response time, and turn-around time.
- Add colors
- Add a Demo Screenshot to the README
- Implement states and queues (maybe, I have other projects and certifications to work on)
