#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/ktime.h>
#include <linux/time64.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>

#include "my_timer.h"

static int timer_show(struct seq_file *m, void *v);
static int timer_open(struct inode *inode, struct file *file);

static struct timespec64 last_time;
static bool is_first_read = true;

static struct proc_dir_entry *timer_entry;

static const struct proc_ops timer_fops = {
    .proc_open = timer_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};


static int timer_show(struct seq_file *m, void *v) {
    struct timespec64 current_time;
    ktime_get_real_ts64(&current_time);

    if (is_first_read == true) {
        seq_printf(m, "current time: %lld.%09ld\n", current_time.tv_sec, current_time.tv_nsec);

        is_first_read = false;
    } else {
        struct timespec64 elapsed_time;
        elapsed_time = timespec64_sub(current_time, last_time);
        seq_printf(m, "current time: %lld.%09ld\n", current_time.tv_sec, current_time.tv_nsec);
        seq_printf(m, "elapsed time: %lld.%09ld\n", elapsed_time.tv_sec, elapsed_time.tv_nsec);
    }

    last_time = current_time;
    return 0;
}

static int timer_open(struct inode *inode, struct file *file) {
    return single_open(file, timer_show, NULL);
}

static int __init timer_init(void) {
    printk(KERN_INFO "timer: Loading module...\n");
    

    timer_entry = proc_create("timer", 0, NULL, &timer_fops);
    if (!timer_entry) {
        printk(KERN_ALERT "timer: Failed to create /proc/timer\n");
        return -ENOMEM;
    }

    return 0;
}

static void __exit timer_exit(void) {
    printk(KERN_INFO "timer: Exiting module...\n");

    proc_remove(timer_entry);
}

module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple proc timer module for measuring elapsed time");
