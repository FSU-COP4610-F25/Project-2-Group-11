## Project 2: Elevator Kernel Module
Elevator kernel module that requires modifying the Linux kernel to add custom system calls and managing concurrency, synchronization, and scheduling within the kernel environment.

## Group Members
- **Ethan Sauve**: ebs23d@fsu.edu
- **Amelia Castro**: ac22bz@fsu.edu
- **Jake Power**: jrp22d@fsu.edu
## Division of Labor

### Part 1: System Call Tracing
- **Responsibilities**: Create and trace empty.c and part1.c with five added system calls using strace.
- **Assigned to**: Ethan Sauve

### Part 2: Timer Kernel Module
- **Responsibilities**: Develop the my_timer kernel module, use ktime_get_real_ts64(), and implement the /proc/timer entry with elapsed time logic.
- **Assigned to**: Amelia Castro

### Part 3a: Adding System Calls
- **Responsibilities**: Define and implement start_elevator(), issue_request(), and stop_elevator() system calls (numbers 548-550) in kernel source files.
- **Assigned to**: Jake Power

### Part 3b: Kernel Compilation
- **Responsibilities**: Configure, compile, and install the modified Linux kernel, ensuring the new system calls are functional.
- **Assigned to**:Jake Power

### Part 3c: Threads
- **Responsibilities**: Implement the main elevator logic within a kthread to control elevator movement and operation.
- **Assigned to**: Jake Power

### Part 3d: Linked List
- **Responsibilities**: Implement or adapt linked lists (linux/list.h) to manage pets waiting on floors and pets currently on the elevator.
- **Assigned to**: Amelia Castro

### Part 3e: Mutexes
- **Responsibilities**: Apply mutexes to ensure thread-safe access and synchronization for all shared elevator and pet data.
- **Assigned to**: Amelia Castro

### Part 3f: Scheduling Algorithm
- **Responsibilities**: Implement the pet elevator scheduling logic, including pet loading/unloading, respecting capacity/weight limits, and displaying status via /proc/elevator.
- **Assigned to**: Amelia Castro, Ethan Sauve

## File Listing
```
.
├── part_1
│   ├── empty.c
│   ├── empty.trace
│   ├── part1.c
│   ├── part1.trace
│   └── Makefile
├── part_2
│   ├── src
│   │   └── my_timer.c
│   └── Makefile
├── part_3
│   ├── src
│   │   ├── elevator.c
│   │   └── elevator.h
|   ├── Makefile   
│   └── syscalls.c
└── README.md
```

## Development Log
### [Ethan Sauve]

| Date       | Work Completed / Notes |
|------------|------------------------|
| 10-28-25   | Part 1                 |
| 11-04-25   | README                 |
| 11-05-25   | Scheduling algorithm,  |
|            | README, and part 2/3   |
|            | header files           |

### [Amelia Castro]

| Date       | Work Completed / Notes |
|------------|------------------------|



### [Jake Power]

| Date       | Work Completed / Notes |
|------------|------------------------|


## Bugs
- **Bug 1**: 


## Considerations

