#include "sample.h"
#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/module.h>

static char hello[128]="hello!!"; 
static struct dentry *my_debugfs_root=NULL; 
static struct dentry *sub_dir=NULL;
static struct dentry *c_entry=NULL;

static int c_open(struct inode *inode, struct file *filp) 
{ 
    filp->private_data = inode->i_private;
    return 0; 
} 
  
static ssize_t c_read(struct file *filp, char __user *buffer, 
        size_t count, loff_t *ppos) 
{ 
    if(*ppos >= 32) 
        return 0; 
    if(*ppos + count > 32) 
        count = 32 - *ppos;
  
    if(copy_to_user(buffer, hello + *ppos, count)) 
        return-EFAULT; 
  
    *ppos += count;
  
    return count; 
} 
  
static ssize_t c_write(struct file *filp, const char __user *buffer,
        size_t count, loff_t *ppos) 
{ 
    if(*ppos >= 32) 
        return 0; 
    if(*ppos + count > 32) 
        count = 32 - *ppos;
  
    if(copy_from_user(hello + *ppos, buffer, count)) 
        return -EFAULT; 
  
    *ppos += count;
  
    return count; 
} 
  
struct file_operations c_fops = { 
    .owner = THIS_MODULE,
    .open = c_open,
    .read = c_read,
    .write = c_write,
};   
  
#if 0
static int test_restart_handler_func(struct notifier_block *this, unsigned long mode, void *cmd){
    printk("%s, %d: \n", __FUNCTION__, __LINE__);
    return NOTIFY_DONE;
}

static struct notifier_block test_restart_handler = {
        .notifier_call = test_restart_handler_func,
        .priority = 128,
};
#endif

static int __init sample_init(void){
    printk("%s:%d\n", __FUNCTION__, __LINE__);
#if 0
    register_restart_handler(&test_restart_handler);
#endif
    my_debugfs_root = debugfs_create_dir("mydebug", NULL); 
    sub_dir = debugfs_create_dir("subdir", my_debugfs_root); 
    c_entry = debugfs_create_file("c", 0644, sub_dir, NULL, &c_fops); 
    return 0;
}

static void __exit sample_exit(void){
    printk("%s:%d\n", __FUNCTION__, __LINE__);
    debugfs_remove(c_entry);
    debugfs_remove(sub_dir);
    debugfs_remove(my_debugfs_root);
}

module_init(sample_init);
module_exit(sample_exit);
MODULE_DESCRIPTION("Sample Driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ralph Wang");

