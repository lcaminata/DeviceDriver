/*
	Device driver set up to take the driverTester.c file input of which user's age the user wishes
	to see, the age is stored on the driver.c code & is sent back to user. 
	
	On first linux terminal type:
	> tail -f /var/log/kern.log
	to see processes on kernel space	
	
	On second linux terminal type:
	>sudo insmod driver.ko
	to initialize kernel space code

	On third linux terminal type:
	>sudo ./test
	to initialize the executable tester

	On second terminal type:
	>sudo rmmod driver
	to end kernel space code
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luciano");

static int device;
static char inputReceived[256] = {0};
static short size;
static struct class* driverClass = NULL;
static struct device* driverDevice = NULL;

static int drive_open(struct inode *inode, struct file *file);
static ssize_t drive_read(struct file *file, char *buffer, size_t length, loff_t *off);
static ssize_t drive_write(struct file *file, const char *buffer, size_t length, loff_t *off);
static int drive_release(struct inode *inode, struct file *file);

static struct file_operations foperations = 
{
	.open = drive_open,
	.read = drive_read,
	.write = drive_write,
	.release = drive_release,
};

static int __init drive_init(void) {
	printk(KERN_INFO "Welcome to the kernel space\n");
	device = register_chrdev(0, "driver", &foperations);
	printk(KERN_INFO "Driver number registered: %d\n", device);
	driverClass = class_create(THIS_MODULE, "driverTest");	
	printk(KERN_INFO "Class created\n");
	driverDevice = device_create(driverClass, NULL, MKDEV(device, 0), NULL, "driver");
	printk(KERN_INFO "Device created\n");
	return 0;
}

static void __exit drive_exit(void) {
	device_destroy(driverClass, MKDEV(device,0));
	class_unregister(driverClass);
	class_destroy(driverClass);
	unregister_chrdev(device, "driver");
	printk(KERN_INFO "End of program\n\n");
}

static int drive_open(struct inode *inode, struct file *file) {
	printk(KERN_INFO "Device successfully opened\n");
	return 0;
}

static ssize_t drive_read(struct file *file, char *buffer, size_t length, loff_t *off) {
	int count = copy_to_user(buffer, inputReceived, size);
	printk(KERN_INFO "Sending output to the user space\n");
	size = 0;
	return size;	
} 

static ssize_t drive_write(struct file *file, const char *buffer, size_t length, loff_t *off) {
	int luciano = 22;
	int daniel = 50;
	int ivo = 19;
	int maria = 45;
	
	if(strcmp(buffer, "luciano") == 0) {
		sprintf(inputReceived, "Hello %s, you are %d years old", buffer, luciano);
	} else if(strcmp(buffer, "daniel") == 0) {
		sprintf(inputReceived, "Hello %s, you are %d years old", buffer, daniel);
	} else if(strcmp(buffer, "ivo") == 0) {
		sprintf(inputReceived, "Hello %s, you are %d years old", buffer, ivo);
	} else if(strcmp(buffer, "maria") == 0) {
		sprintf(inputReceived, "Hello %s, you are %d years old", buffer, maria);
	} else {
		sprintf(inputReceived, "Im sorry you are not a recognized user");
	}
	size = strlen(inputReceived);
	printk(KERN_INFO "User input received\n");
	return 0;
}

static int drive_release(struct inode *inode, struct file *file) {
	printk(KERN_INFO "Device successfully closed\n");
	return 0;
}

module_init(drive_init);
module_exit(drive_exit);
