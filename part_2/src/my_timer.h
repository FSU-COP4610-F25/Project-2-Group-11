#ifndef MY_TIMER_H
#define MY_TIMER_H

#include <linux/init.h>
#include <linux/types.h>

extern int __init timer_init(void);
extern void __exit timer_exit(void);

struct seq_file;
struct inode;
struct file;

int timer_show(struct seq_file *m, void *v);

int timer_open(struct inode *inode, struct file *file);

#endif 
