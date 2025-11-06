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
- **Assigned to**: Jake Power, Amelia Castro

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
│   │   ├── my_timer.c
│   │   └── my_timer.h
│   └── Makefile
├── part_3
│   ├── src
│   │   ├── elevator.c
│   │   └── elevator.h
|   ├── Makefile   
│   └── syscalls.c
|── Makefile
└── README.md
```
## How to Compile
```
make
```

## How to Run
```
make run
```

## Development Log
### [Ethan Sauve]

| Date       | Work Completed / Notes                                                                      |
|------------|---------------------------------------------------------------------------------------------|
| 10-28-25   | Part 1                                                                                      |
| 11-04-25   | README                                                                                      |
| 11-05-25   | Scheduling algorithm, README, part 1 makefile, and part 2/3 header files, overall makefile  |

### [Amelia Castro]

| Date       | Work Completed / Notes                                                                      |
|------------|---------------------------------------------------------------------------------------------|
| 10-26-25   | Part 2: Timer Kernel Module                                                                 |
| 11-02-25   | Step 4: Implement Elevator                                                                  |
| 11-04-25   | Organized directories, implemented makefiles for Timer Kernel and Elevator                  |



### [Jake Power]

| Date       | Work Completed / Notes                                                                      |
|------------|---------------------------------------------------------------------------------------------|
|            |                                                                                             |
|            |                                                                                             |
|            |                                                                                             |


## Meetings
Document online/in-person meetings, their purpose, and what was discussed.

| Date       | Attendees            | Topics Discussed | Outcomes / Decisions |
|------------|----------------------|------------------|-----------------------|
| 2025-10-12 | [Amelia Castro, Jake Power, Ethan Sauve]              | [Project 2]   | [Determine the division of labor and responsabilities for the project.] |
| 2025-10-22 | [Amelia Castro, Jake Power, Ethan Sauve]              | [Project 2]   | [Continue work on Project 2. Discuss about progress done. Helped each other out on some bugs]  |
| 2025-10-27 - 2025-11-02 | [Amelia Castro, Jake Power, Ethan Sauve] | [Project 2]   | [On-going online communication about the progress of the project. Testing. Solving Bugs. Clarifying conceptual ideas of the project. Kept working on Project 2.]  |
