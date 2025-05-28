# Scheduler
This project is a program that simulates different job scheduling algorithms. 

Current job scheduling algorithms implemented are First Come First Serve (FCFS or FIFO), Shortes Job First Preemptive (SJF), Priority Scheduling Preemptive, and Round Robin.

I have implemented a queue data structure but it has not been used yet, future work is to implement a ready queue to better simulate the different states of tasks, because currently I am using only arrays.

To better simulate a process, I am using threads where I select which thread to run by using mutexes and conditional variables.

## To Run this Program
Clone the repository and write the following in your terminal:

Since it uses pthreads, this program is unix-only.
```
make scheduler
```

## Examples
"#" = Running

"_" = Waiting

" " = Idle (not ready/arrived)
### Priority Preemptive Scheduling
![image](https://github.com/user-attachments/assets/2d47027f-f79d-4bf0-ae2e-e8da1cb2c49f)

### Shortest Job First Preemptive Scheduling
![image](https://github.com/user-attachments/assets/5ae07b15-eb7f-4951-9fcc-f5c7d59b6ed2)

### Round Robin Scheduling
![image](https://github.com/user-attachments/assets/eb61c0da-98ab-4038-9c69-903a0f18ceb5)

## Future work: Implement all the states using queues
![image](https://github.com/user-attachments/assets/147399d2-f6c6-4f5d-bf1d-4541ccbe5113)
