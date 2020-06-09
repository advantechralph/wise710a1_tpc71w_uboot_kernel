/*****************************************************************************
  Copyright (c) 2019, Advantech Automation Corp.
  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY
  INFORMATION WHICH IS THE PROPERTY OF ADVANTECH AUTOMATION CORP.

  ANY DISCLOSURE, USE, OR REPRODUCTION, WITHOUT WRITTEN AUTHORIZATION FROM
  ADVANTECH AUTOMATION CORP., IS STRICTLY PROHIBITED.
 *****************************************************************************
 *
 * File:        adv-ecc508a.c
 * Version:     Initial  <11/19/2018>
 * Author:      Ji.Xu
 * E-mail:      ji.xu@advantech.com.cn
 *
 * Description: Driver for the Atmel ECC508A security chip. 
 *              Work in Advantech WISE-710/TPC-71W.
 *
 * Status:      working
 *
 * Change Log:
 *              Version 0.01 <11/19/2018> Ji.Xu
 *              - Alpha version v0.01
 *              Version 0.10 <12/21/2018> Ji.Xu
 *              - Beta version v0.10
 *              Version 1.00 <01/02/2019> Ji.Xu
 *              - Release version v1.00
 *
 -----------------------------------------------------------------------------*/
#include <linux/version.h>
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a, b, c) KERNEL_VERSION((a)*65536+(b)*256+(c))
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 18)
#include <linux/config.h>
#endif
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/i2c.h>
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/err.h>

#define ADVANTECH_SECURITY_VER           "1.00"
#define ADVANTECH_SECURITY_DATE          "01/02/2019" 

// magic number refer to kernel-4.1.15/Documentation/ioctl/ioctl-number.txt
#define ECC508A_MAGIC	'x'
#define IOCTL_ECC508A_I2C_READ		_IOWR(ECC508A_MAGIC, 0x00, i2c_data)
#define IOCTL_ECC508A_I2C_WRITE		_IOWR(ECC508A_MAGIC, 0x01, i2c_data)

typedef struct _ecc508a_data {
	struct i2c_client *client;
} ecc508a_data;

#define I2C_SMBUS_USE_MAX     257     /* As specified in SMBus standard */
typedef struct _ecc508a_ioctl_data {
	unsigned short	Address;
	unsigned char	ReadLength;
	unsigned char	ReadBuffer[I2C_SMBUS_USE_MAX];
	unsigned char	WriteLength;
	unsigned char	WriteBuffer[I2C_SMBUS_USE_MAX];
	unsigned int	length;
	unsigned char	isRead;
} ecc508a_ioctl_data;

ssize_t major = 0;
static struct class *ecc508a_class;
ecc508a_data *ecc508a;

/************************************************************************************/
/*                            i2c device handle                                     */
/************************************************************************************/
static const struct of_device_id ecc508a_of_match[] = {
	{ .compatible = "atmel,ecc508a" },
	{ }
};
MODULE_DEVICE_TABLE(of, ecc508a_of_match);

static int ecc508a_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
	int err = 0;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA
				| I2C_FUNC_SMBUS_I2C_BLOCK)) {
		dev_err(&adapter->dev, "doesn't support required functionality\n");
		return -EIO;
	}

	ecc508a = devm_kzalloc(&client->dev, sizeof(struct ecc508a_data), GFP_KERNEL);
	if (!ecc508a) {
		return -ENOMEM;
	}

	ecc508a->client = client;
	i2c_set_clientdata(client, ecc508a);

	err = ecc508a_init_client(client);
	if (err) {
		return err;
	}

	return err;
}

static int at24_remove(struct i2c_client *client)
{
	i2c_unregister_device(ecc508a->client);

	return 0;
}

static const struct i2c_device_id ecc508a_ids[] = {
	{ "ecc508a", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ecc508a_ids);

static struct i2c_driver ecc508a_driver = {
	.driver = {
		.name = "ecc508a",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(ecc508a_of_match),
	},
	.probe      = ecc508a_probe,
	.remove     = ecc508a_remove,
	.id_table   = ecc508a_ids,
};
module_i2c_driver(ecc508a_driver);

/************************************************************************************/
/*                            char device fops                                      */
/************************************************************************************/
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
static int ecc508a_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#else
static long ecc508a_ioctl(struct file* filp, unsigned int cmd, unsigned long arg )
#endif
{
	eeprom_config EepromConfig = {0};
	if (copy_from_user(&EepromConfig, (eeprom_config __user *)arg, sizeof(eeprom_config))) {
		printk(KERN_ERR "copy from user failed! \n");
		return -EFAULT;
	} 

	switch (cmd) {

	case IOCTL_ECC508A_I2C_READ:

		if (copy_to_user((eeprom_config __user *)arg, &EepromConfig, sizeof(eeprom_config))) {
			printk(KERN_ERR "copy to user failed! \n");
			return -EFAULT;
		} 
		break;

	case IOCTL_ECC508A_I2C_WRITE:

		break;

	default:
		return -EINVAL;
	}

	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
static int ecc508a_read(struct file *file, char *buf, size_t count, loff_t *ptr)
#else
static ssize_t ecc508a_read(struct file *file, char *buf, size_t count, loff_t *ptr)
#endif
{
	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
static int ecc508a_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
#else
static ssize_t ecc508a_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
#endif
{
	return 0;
}

static int ec_eeprom_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int ec_eeprom_release(struct inode *inode, struct file *file)
{
	return 0;
}

static struct file_operations ecc508a_fops = {
	owner:		THIS_MODULE,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
	ioctl:		ecc508a_ioctl,
#else
	unlocked_ioctl: ecc508a_ioctl,
#endif
	read:		ecc508a_read,
	write:		ecc508a_write,
	open:		ecc508a_open,
	release:	ecc508a_release,
};

/************************************************************************************/
/*                            ecc508a init                                          */
/************************************************************************************/
static int __init ecc508a_init(void)
{
	if ((major = register_chrdev(0, "advsecurity", &ecc508a_fops)) < 0) {
		printk(KERN_ERR "register chrdev failed! \n");
		return -ENODEV;
	}

	ecc508a_class = class_create(THIS_MODULE, "advsecurity");
	if (IS_ERR(ecc508a_class)) {
		printk(KERN_ERR "create chrclass failed! \n");
		return -ENODEV;
	}
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 27)
	device_create(ecc508a_class, NULL, MKDEV(major, 0), NULL, "advsecurity");
#else
	class_device_create(ecc508a_class, NULL, MKDEV(major, 0), NULL, "advsecurity");
#endif

	printk("=====================================================\n");
	printk("     Advantech security driver V%s [%s]\n", 
			ADVANTECH_SECURITY_VER, ADVANTECH_SECURITY_DATE);
	printk("=====================================================\n");

	return i2c_add_driver(&ecc508a_driver);
}
module_init(ecc508a_init);

static void __exit ecc508a_exit(void)
{
	device_destroy(ecc508a_class, MKDEV(major, 0));
	class_destroy(ecc508a_class);
	unregister_chrdev(major, "advsecurity");

	i2c_del_driver(&ecc508a_driver);

	printk("Advantech security driver exit!\n");
}
module_exit(ecc508a_exit);

MODULE_AUTHOR("ji.xu <ji.xu@advantech.com.cn>");
MODULE_DESCRIPTION("Advantech ECC508A security chip driver");
MODULE_LICENSE("GPL");
