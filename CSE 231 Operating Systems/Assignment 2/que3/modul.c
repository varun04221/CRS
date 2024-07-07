#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/signal.h>

MODULE_DESCRIPTION("MY MODULE");
MODULE_AUTHOR("ME");
MODULE_LICENSE("GPL");

static void count_task(void){
    struct task_struct *task=current;
    int count=0;

    list_for_each_entry(task,&init_task.tasks,tasks){
        count++;
    }
    printk("Currently there are %d processes in the task struct\n",count);
    return;
}


static int dummy_init(void){
    printk("Module Loaded Successfully!\n");
    count_task();
    return 0;
}

static void dummy_exit(void){
    printk("Bye!!!\n");
    return;
}

module_init(dummy_init);
module_exit(dummy_exit);
