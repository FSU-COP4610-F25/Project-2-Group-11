#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <linux/list.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/types.h> 

#define MAX_PETS 5
#define MAX_WEIGHT 50
#define NUM_FLOORS 5

enum pet_type { CHIHUAHUA, PUG, PUGHUAHUA, DACHSHUND };
enum elevator_state { OFFLINE, IDLE, LOADING, UP, DOWN, STOPPING };

struct Pet {
    int type;
    int weight;
    int start_floor;
    int destination_floor;
    struct list_head list;
};

struct Elevator {
    enum elevator_state state;
    int current_floor;
    int current_weight;
    int pets_on_board;
    struct list_head floor_pets[NUM_FLOORS]; 
    struct list_head elevator_pets;
    int pets_waiting;
    int pets_serviced;
    struct mutex lock;
    struct task_struct *thread;
    int direction; 
};

extern struct Elevator elevator;

int start_elevator(void);
int issue_request(int start_floor, int destination_floor, int type);
int stop_elevator(void);

#endif 
