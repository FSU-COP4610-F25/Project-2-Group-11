#ifndef MY_TIMER_H
#define MY_TIMER_H

#include <linux/init.h>
#include <linux/types.h>

extern int __init timer_init(void);
extern void __exit timer_exit(void);

#endif 
