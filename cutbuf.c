#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/delay.h>

#define DEVICE_NAME "cutbuf"
#define BUFFER_SIZE 4096

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nizar BOUSSARSAR");
MODULE_DESCRIPTION("cutbuf device driver.");
MODULE_SUPPORTED_DEVICE(DEVICE_NAME);

int device_init(void);
void device_exit(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

module_init(device_init);
module_exit(device_exit);

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static int device_major = 100;

static char *device_buffer;
static char *buff_rptr;
static char *buff_wptr;

int device_init() {
    int ret;
    ret = register_chrdev(device_major, DEVICE_NAME, &fops);
    if(ret < 0) {
        printk(KERN_ALERT "cutbuf: cannot obtain major number %d.\n", device_major);
        return ret;
    }
    device_buffer = kmalloc(1024, GFP_KERNEL);
    memset(device_buffer, 0, BUFFER_SIZE);
    printk(KERN_INFO "cutbuf: cutbuf loaded.\n");
    return 0;
}

void device_exit() {
    unregister_chrdev(device_major, DEVICE_NAME);
    kfree(device_buffer);
    printk(KERN_INFO "cutbuf: cutbuf unloaded.\n");
}

static int device_open(struct inode *nd, struct file *fp) {
    buff_rptr = buff_wptr = device_buffer;
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode *nd, struct file *fp) {
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read(struct file *fp, char *buff, size_t length, loff_t *offset) {
    int bytes_read = strlen(buff_rptr);
    if(bytes_read > length) bytes_read = length;
    copy_to_user(buff, buff_rptr, bytes_read);
    buff_rptr += bytes_read;
    msleep(100);
    return bytes_read;
}

static ssize_t device_write(struct file *fp, const char *buff, size_t length, loff_t *offset) {
    int bytes_written = BUFFER_SIZE - (buff_wptr - device_buffer);
    if(bytes_written > length) bytes_written = length;
    copy_from_user(buff_wptr, buff, bytes_written);
    buff_wptr += bytes_written;
    return bytes_written;
}

/*
End of Source Code
*/
