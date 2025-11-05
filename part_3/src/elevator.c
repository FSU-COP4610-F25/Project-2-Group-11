#include <linux/list.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/seq_file.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group 11");
MODULE_DESCRIPTION("Elevator Kernel Module");

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
};

static struct Elevator elevator;

static int get_pet_weight(int type) {
    switch (type) {
        case CHIHUAHUA: 
            return 3;
        case PUG: 
            return 14;
        case PUGHUAHUA: 
            return 10;
        case DACHSHUND: 
            return 16;
        default: 
            printk(KERN_WARNING "Elevator: Invalid pet type requested.\n");
            return 0;
    }
}

static char get_pet_char(int type) {
    switch (type) {
        case CHIHUAHUA: 
            return 'C';
        case PUG: 
            return 'P';
        case PUGHUAHUA: 
            return 'H';
        case DACHSHUND: 
            return 'D';
        default: 
            printk(KERN_WARNING "Elevator: Invalid pet type requested.\n");
            return '?';
    }
}

int start_elevator(void) {
    int already_active = 0;

    printk(KERN_INFO "ELEVATOR_MODULE: start_elevator() called.\n");
    
    mutex_lock(&elevator.lock);

    if (elevator.state != OFFLINE) {
        already_active = 1;
    } else {
        elevator.state = IDLE;
    }

    mutex_unlock(&elevator.lock);

    return already_active;
}

int issue_request(int start_floor, int destination_floor, int type) {
    printk(KERN_INFO "ELEVATOR_MODULE: issue_request() called.\n");
    
    struct Pet *new_pet;
    int weight;

    printk(KERN_INFO "Elevator: New pet request: Type %d, %d -> %d\n", type, start_floor, destination_floor);

    if (start_floor < 1 || start_floor > NUM_FLOORS ||
        destination_floor < 1 || destination_floor > NUM_FLOORS ||
        start_floor == destination_floor) {
        return 1;
    }

    weight = get_pet_weight(type);
    if (weight == 0) {
        return 1;
    }

    new_pet = kmalloc(sizeof(struct Pet), GFP_KERNEL);
    if (!new_pet) {
        printk(KERN_ERR "Elevator: Failed to allocate memory for new pet.\n");
        return -ENOMEM;
    }

    new_pet->type = type;
    new_pet->weight = weight;
    new_pet->start_floor = start_floor;
    new_pet->destination_floor = destination_floor;
    INIT_LIST_HEAD(&new_pet->list);

    mutex_lock(&elevator.lock);

    list_add_tail(&new_pet->list, &elevator.floor_pets[start_floor - 1]);
    elevator.pets_waiting++;

    mutex_unlock(&elevator.lock);

    return 0;
}

int stop_elevator(void) {
    int already_stopping = 0;

    printk(KERN_INFO "ELEVATOR_MODULE: stop_elevator() called.\n");

    mutex_lock(&elevator.lock);

    if (elevator.state == OFFLINE || elevator.state == STOPPING) {
        already_stopping = 1;
    } else {
        elevator.state = STOPPING; 
    }

    mutex_unlock(&elevator.lock);

    return already_stopping;
}

EXPORT_SYMBOL(start_elevator);
EXPORT_SYMBOL(issue_request);
EXPORT_SYMBOL(stop_elevator);


static int elevator_thread_func(void *data) {
    printk(KERN_INFO "Elevator: Thread started.\n");

    while (!kthread_should_stop()) {  
        
        mutex_lock(&elevator.lock);

        switch (elevator.state) {
            case OFFLINE: {
                struct Pet *pet, *tmp;
                int i;
                for (i = 0; i < NUM_FLOORS; i++) {
                    list_for_each_entry_safe(pet, tmp, &elevator.floor_pets[i], list) {
                        list_del(&pet->list);
                        kfree(pet);
                        elevator.pets_waiting--;
                    }
                }
                mutex_unlock(&elevator.lock);
                break;
            }

            case STOPPING: {                
                struct Pet *pet, *tmp;
                int current_floor_index = elevator.current_floor - 1;

                mutex_unlock(&elevator.lock);
                ssleep(1);
                mutex_lock(&elevator.lock);

                list_for_each_entry_safe(pet, tmp, &elevator.elevator_pets, list) {
                    if (pet->destination_floor == elevator.current_floor) {
                        elevator.pets_on_board--;
                        elevator.current_weight -= pet->weight;
                        elevator.pets_serviced++;
                        elevator.pets_waiting--; 
                        list_del(&pet->list);
                        kfree(pet);
                    }
                }

                if (list_empty(&elevator.elevator_pets)) {
                    elevator.state = OFFLINE;
                } else {
                    struct Pet *next_pet;
                    next_pet = list_first_entry(&elevator.elevator_pets, struct Pet, list);
                    
                    if (next_pet->destination_floor > elevator.current_floor) {
                        elevator.state = UP;
                    } else {
                        elevator.state = DOWN;
                    }
                }

                mutex_unlock(&elevator.lock);
                break;
            }

            case IDLE: {
                int i;
                int current_floor_index = elevator.current_floor - 1;

                if (!list_empty(&elevator.floor_pets[current_floor_index])) {
                    elevator.state = LOADING;
                
                } else {
                    for (i = 0; i < NUM_FLOORS; i++) {
                        if (!list_empty(&elevator.floor_pets[i])) {
                            if (i > current_floor_index) {
                                elevator.state = UP;
                            } else {
                                elevator.state = DOWN;
                            }
                            break;
                        }
                    }
                }
                mutex_unlock(&elevator.lock);
                break;
            }
            case LOADING: {
                struct Pet *pet, *tmp;
                int current_floor_index = elevator.current_floor - 1;

                mutex_unlock(&elevator.lock);
                ssleep(1);
                mutex_lock(&elevator.lock);
                
                list_for_each_entry_safe(pet, tmp, &elevator.elevator_pets, list) {
                    if (pet->destination_floor == elevator.current_floor) {
                        elevator.pets_on_board--;
                        elevator.current_weight -= pet->weight;
                        elevator.pets_serviced++;
                        elevator.pets_waiting--; 
                        list_del(&pet->list);
                        kfree(pet);
                    }
                }
                
                list_for_each_entry_safe(pet, tmp, &elevator.floor_pets[current_floor_index], list) {
                    if (elevator.pets_on_board < MAX_PETS &&
                        (elevator.current_weight + pet->weight) <= MAX_WEIGHT) {
                        
                        list_del(&pet->list);
                        list_add_tail(&pet->list, &elevator.elevator_pets);
                        elevator.pets_on_board++;
                        elevator.current_weight += pet->weight;
                    } else {
                        break; 
                    }
                }
                
                if (list_empty(&elevator.elevator_pets)) {
                    elevator.state = IDLE;
                } else {
                    struct Pet *next_pet;
                    next_pet = list_first_entry(&elevator.elevator_pets, struct Pet, list);
                    
                    if (next_pet->destination_floor > elevator.current_floor) {
                        elevator.state = UP;
                    } else {
                        elevator.state = DOWN;
                    }
                }

                mutex_unlock(&elevator.lock);
                break;
            }

            case UP:
                mutex_unlock(&elevator.lock);
                ssleep(2);
                mutex_lock(&elevator.lock);

                elevator.current_floor++;
                elevator.state = LOADING;
                
                mutex_unlock(&elevator.lock);
                break;
            case DOWN:
                mutex_unlock(&elevator.lock);
                ssleep(2);
                mutex_lock(&elevator.lock);

                elevator.current_floor--;
                elevator.state = LOADING;

                mutex_unlock(&elevator.lock);
                break;
            default:
                mutex_unlock(&elevator.lock);
                break;
        }
        msleep(10);

    }

    printk(KERN_INFO "Elevator: Thread stopping.\n");
    return 0;
}


static int elevator_show(struct seq_file *m, void *v) {
    int i;
    struct Pet *pet;
    const char *state_str;

    mutex_lock(&elevator.lock);

    switch (elevator.state) {
        case OFFLINE: state_str = "OFFLINE"; break;
        case IDLE:    state_str = "IDLE"; break;
        case LOADING: state_str = "LOADING"; break;
        case UP:      state_str = "UP"; break;
        case DOWN:    state_str = "DOWN"; break;
        default:      state_str = "???"; break;
    }
    
    seq_printf(m, "Elevator state: %s\n", state_str);
    seq_printf(m, "Current floor: %d\n", elevator.current_floor);
    seq_printf(m, "Current load: %d lbs\n", elevator.current_weight);

    seq_printf(m, "Elevator status: ");
    if (list_empty(&elevator.elevator_pets)) {
        seq_printf(m, "[empty]\n");
    } else {
        list_for_each_entry(pet, &elevator.elevator_pets, list) {
            seq_printf(m, "%c%d ", get_pet_char(pet->type), pet->destination_floor);
        }
        seq_printf(m, "\n");
    }

    for (i = NUM_FLOORS - 1; i >= 0; i--) {
        int floor_num = i + 1;
        
        if (elevator.current_floor == floor_num) {
            seq_printf(m, "[*] Floor %d: ", floor_num);
        } else {
            seq_printf(m, "[ ] Floor %d: ", floor_num);
        }

        if (list_empty(&elevator.floor_pets[i])) {
            seq_printf(m, "0\n");
        } else {
            int count = 0;
            list_for_each_entry(pet, &elevator.floor_pets[i], list) {
                count++;
            }
            seq_printf(m, "%d ", count);
            
            list_for_each_entry(pet, &elevator.floor_pets[i], list) {
                seq_printf(m, "%c%d ", get_pet_char(pet->type), pet->destination_floor);
            }
            seq_printf(m, "\n");
        }
    }

    seq_printf(m, "Number of pets: %d\n", elevator.pets_on_board);
    seq_printf(m, "Number of pets waiting: %d\n", elevator.pets_waiting - elevator.pets_on_board);
    seq_printf(m, "Number of pets serviced: %d\n", elevator.pets_serviced);

    mutex_unlock(&elevator.lock);
    return 0;
}

static int elevator_open(struct inode *inode, struct file *file) {
    return single_open(file, elevator_show, NULL);
}

static const struct proc_ops elevator_proc_ops = {
    .proc_open = elevator_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init elevator_init(void) {
    int i;

    printk(KERN_INFO "Elevator: Loading the module.\n");
    elevator.state = OFFLINE;
    elevator.current_floor = 1;
    elevator.current_weight = 0;
    elevator.pets_on_board = 0;
    elevator.pets_serviced = 0;
    elevator.pets_waiting = 0;

    mutex_init(&elevator.lock);

    INIT_LIST_HEAD(&elevator.elevator_pets);

    for (i = 0; i < NUM_FLOORS; i++) {
        INIT_LIST_HEAD(&elevator.floor_pets[i]);
    }

    if (proc_create("elevator", 0, NULL, &elevator_proc_ops) == NULL) {
        printk(KERN_ERR "Elevator: Failed to create /proc/elevator.\n");
        return -ENOMEM;
    }
    elevator.thread = kthread_run(elevator_thread_func, NULL, "elevator_thread");
    
    if (IS_ERR(elevator.thread)) {
        printk(KERN_ERR "Elevator: Failed to create kthread.\n");
        
        proc_remove("elevator", NULL); 
        return PTR_ERR(elevator.thread);
    }

    printk(KERN_INFO "Elevator: Module loaded successfully.\n");

    return 0;
}

static void __exit elevator_exit(void) {
    printk(KERN_INFO "Elevator: Removing Module.\n");
    
    if (elevator.thread) {
        kthread_stop(elevator.thread);
    }
    
    proc_remove("elevator", NULL);
}

module_init(elevator_init);
module_exit(elevator_exit);
