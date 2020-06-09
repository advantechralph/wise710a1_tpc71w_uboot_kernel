/*
 * fm25.c -- support SPI FRAMs, such as Cypress FM25 models
 *
 * Copyright (C) 2014 Jiri Prchal
 * Copyright (C) 2019 Ji Xu, Advantech
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include <linux/version.h>
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a, b, c) KERNEL_VERSION((a)*65536+(b)*256+(c))
#endif
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/ioport.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/param.h>
#include <linux/device.h>
#include <linux/err.h>

#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/sysfs.h>
#include <linux/mod_devicetable.h>
#include <linux/log2.h>
#include <linux/bitops.h>
#include <linux/jiffies.h>
#include <linux/of.h>

#include <linux/spi/spi.h>
#include <linux/spi/eeprom.h>
#include <linux/of.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 10, 0)
#include <linux/uaccess.h>
#endif

#define ADVANTECH_FM25_EEPROM_VER           "1.00"
#define ADVANTECH_FM25_EEPROM_DATE          "01/21/2019" 

struct fm25_data {
	struct spi_device *spi;
	struct memory_accessor mem;
	struct mutex lock;
	struct spi_eeprom chip;
	struct bin_attribute bin;
	unsigned addrlen;
	int	has_sernum;
};
struct fm25_data *global_fm25;

ssize_t major = 0;
static struct class *eeprom_class;

#define	FM25_WREN	0x06		/* latch the write enable */
#define	FM25_WRDI	0x04		/* reset the write enable */
#define	FM25_RDSR	0x05		/* read status register */
#define	FM25_WRSR	0x01		/* write status register */
#define	FM25_READ	0x03		/* read byte(s) */
#define	FM25_WRITE	0x02		/* write byte(s)/sector */
#define	FM25_SLEEP	0xb9		/* enter sleep mode */
#define	FM25_RDID	0x9f		/* read device ID */
#define	FM25_RDSN	0xc3		/* read S/N */

#define	FM25_SR_WEN	0x02		/* write enable (latched) */
#define	FM25_SR_BP0	0x04		/* BP for software writeprotect */
#define	FM25_SR_BP1	0x08
#define	FM25_SR_WPEN	0x80		/* writeprotect enable */

#define	FM25_ID_LEN	9		/* ID lenght */
#define	FM25_SN_LEN	8		/* serial number lenght */

#define	FM25_MAXADDRLEN	3		/* 24 bit addresses */

#define	io_limit	PAGE_SIZE	/* bytes */

static ssize_t
fm25_data_read(
	struct fm25_data *fm25,
	char *buf,
	unsigned offset,
	size_t count
)
{
	u8 command[FM25_MAXADDRLEN + 1];
	u8 *cp;
	ssize_t status;
	struct spi_transfer	t[2];
	struct spi_message	m;
	u8 instr;

	if (unlikely(offset >= fm25->bin.size))
		return 0;
	if ((offset + count) > fm25->bin.size)
		count = fm25->bin.size - offset;
	if (unlikely(!count))
		return count;

	cp = command;

	instr = FM25_READ;
	*cp++ = instr;

	/* 8/16/24-bit address is written MSB first */
	switch (fm25->addrlen) {
	default:	/* case 3 */
		*cp++ = offset >> 16;
	case 2:
		*cp++ = offset >> 8;
	case 1:
	case 0:	/* can't happen: for better codegen */
		*cp++ = offset >> 0;
	}

	spi_message_init(&m);
	memset(t, 0, sizeof t);

	t[0].tx_buf = command;
	t[0].len = fm25->addrlen + 1;
	spi_message_add_tail(&t[0], &m);

	t[1].rx_buf = buf;
	t[1].len = count;
	spi_message_add_tail(&t[1], &m);

	mutex_lock(&fm25->lock);

	/* Read it all at once.
	 *
	 * REVISIT that's potentially a problem with large chips, if
	 * other devices on the bus need to be accessed regularly or
	 * this chip is clocked very slowly
	 */
	status = spi_sync(fm25->spi, &m);
	dev_dbg(&fm25->spi->dev,
		"read %Zd bytes at %d --> %d\n",
		count, offset, (int) status);

	mutex_unlock(&fm25->lock);
	return status ? status : count;
}

static ssize_t
fm25_id_read(struct fm25_data *fm25, char *buf)
{
	u8 command = FM25_RDID;
	ssize_t	status;
	struct spi_transfer	t[2];
	struct spi_message	m;

	spi_message_init(&m);
	memset(t, 0, sizeof t);

	t[0].tx_buf = &command;
	t[0].len = 1;
	spi_message_add_tail(&t[0], &m);

	t[1].rx_buf = buf;
	t[1].len = FM25_ID_LEN;
	spi_message_add_tail(&t[1], &m);

	mutex_lock(&fm25->lock);

	status = spi_sync(fm25->spi, &m);
	dev_dbg(&fm25->spi->dev,
		"read %Zd bytes of ID --> %d\n",
	 FM25_ID_LEN, (int) status);

	mutex_unlock(&fm25->lock);
	return status ? status : FM25_ID_LEN;
}

static ssize_t
fm25_sernum_read(struct fm25_data *fm25, char *buf)
{
	u8 command = FM25_RDSN;
	ssize_t status;
	struct spi_transfer t[2];
	struct spi_message m;

	spi_message_init(&m);
	memset(t, 0, sizeof t);

	t[0].tx_buf = &command;
	t[0].len = 1;
	spi_message_add_tail(&t[0], &m);

	t[1].rx_buf = buf;
	t[1].len = FM25_SN_LEN;
	spi_message_add_tail(&t[1], &m);

	mutex_lock(&fm25->lock);

	status = spi_sync(fm25->spi, &m);
	dev_dbg(&fm25->spi->dev,
		"read %Zd bytes of serial number --> %d\n",
		FM25_SN_LEN, (int) status);

	mutex_unlock(&fm25->lock);
	return status ? status : FM25_SN_LEN;
}

static ssize_t
sernum_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	char binbuf[FM25_SN_LEN];
	struct fm25_data *fm25;
	int i;
	char *pbuf = buf;

	fm25 = dev_get_drvdata(dev);
	fm25_sernum_read(fm25, binbuf);
	for (i = 0; i < FM25_SN_LEN; i++)
		pbuf += sprintf(pbuf, "%02x ", binbuf[i]);
	sprintf(--pbuf, "\n");
	return (3 * i);
}
static const DEVICE_ATTR_RO(sernum);

static ssize_t
fm25_bin_read(struct file *filp, struct kobject *kobj,
	      struct bin_attribute *bin_attr,
	      char *buf, loff_t off, size_t count)
{
	struct device *dev;
	struct fm25_data *fm25;

	dev = container_of(kobj, struct device, kobj);
	fm25 = dev_get_drvdata(dev);

	return fm25_data_read(fm25, buf, off, count);
}


static ssize_t
fm25_data_write(struct fm25_data *fm25, const char *buf, loff_t off,
	      size_t count)
{
	ssize_t	status = 0;
	unsigned written = 0;
	unsigned buf_size;
	u8 *bounce;

	if (unlikely(off >= fm25->bin.size))
		return -EFBIG;
	if ((off + count) > fm25->bin.size)
		count = fm25->bin.size - off;
	if (unlikely(!count))
		return count;

	/* Temp buffer starts with command and address */
	buf_size = io_limit;
	bounce = kmalloc(buf_size + fm25->addrlen + 1, GFP_KERNEL);
	if (!bounce)
		return -ENOMEM;

	/* For write, rollover is within the page ... so we write at
	 * most one page, then manually roll over to the next page.
	 */
	mutex_lock(&fm25->lock);
	do {
		unsigned segment;
		unsigned offset = (unsigned) off;
		u8 *cp = bounce;
		u8 instr;

		*cp = FM25_WREN;
		status = spi_write(fm25->spi, cp, 1);
		if (status < 0) {
			dev_dbg(&fm25->spi->dev, "WREN --> %d\n",
					(int) status);
			break;
		}

		instr = FM25_WRITE;
		*cp++ = instr;

		/* 8/16/24-bit address is written MSB first */
		switch (fm25->addrlen) {
		default:	/* case 3 */
			*cp++ = offset >> 16;
		case 2:
			*cp++ = offset >> 8;
		case 1:
		case 0:	/* can't happen: for better codegen */
			*cp++ = offset >> 0;
		}

		/* Write as much of a page as we can */
		segment = buf_size - (offset % buf_size);
		if (segment > count)
			segment = count;
		memcpy(cp, buf, segment);
		status = spi_write(fm25->spi, bounce,
				segment + fm25->addrlen + 1);
		dev_dbg(&fm25->spi->dev,
				"write %u bytes at %u --> %d\n",
				segment, offset, (int) status);
		if (status < 0)
			break;

		/* REVISIT this should detect (or prevent) failed writes
		 * to readonly sections of the EEPROM...
		 */

		off += segment;
		buf += segment;
		count -= segment;
		written += segment;

	} while (count > 0);

	mutex_unlock(&fm25->lock);

	kfree(bounce);
	return written ? written : status;
}

static ssize_t
fm25_bin_write(struct file *filp, struct kobject *kobj,
	       struct bin_attribute *bin_attr,
	       char *buf, loff_t off, size_t count)
{
	struct device *dev;
	struct fm25_data *fm25;

	dev = container_of(kobj, struct device, kobj);
	fm25 = dev_get_drvdata(dev);

	return fm25_data_write(fm25, buf, off, count);
}

/*-------------------------------------------------------------------------*/

/* Let in-kernel code access the eeprom data. */

static ssize_t fm25_mem_read(struct memory_accessor *mem, char *buf,
			     off_t offset, size_t count)
{
  struct fm25_data *fm25 = container_of(mem, struct fm25_data, mem);

  return fm25_data_read(fm25, buf, offset, count);
}

static ssize_t fm25_mem_write(struct memory_accessor *mem, const char *buf,
			  off_t offset, size_t count)
{
	struct fm25_data *fm25 = container_of(mem, struct fm25_data, mem);

	return fm25_data_write(fm25, buf, offset, count);
}

/*-------------------------------------------------------------------------*/

static int fm25_np_to_chip(struct device *dev,
			   struct device_node *np,
			   struct spi_eeprom *chip)
{
	memset(chip, 0, sizeof(*chip));
	strncpy(chip->name, np->name, sizeof(chip->name));

	if (of_find_property(np, "read-only", NULL))
		chip->flags |= EE_READONLY;
	return 0;
}

static int fm25_probe(struct spi_device *spi)
{
	struct fm25_data *fm25 = NULL;
	struct spi_eeprom chip;
	struct device_node *np = spi->dev.of_node;
	int	err;
	char id[FM25_ID_LEN];

	/* Chip description */
	if (!spi->dev.platform_data) {
		if (np) {
			err = fm25_np_to_chip(&spi->dev, np, &chip);
			if (err)
				return err;
		} else {
			dev_err(&spi->dev, "Error: no chip description\n");
			return -ENODEV;
		}
	} else {
		chip = *(struct spi_eeprom *)spi->dev.platform_data;
	}

	fm25 = devm_kzalloc(&spi->dev, sizeof(*fm25), GFP_KERNEL);
	if (!fm25) {
		return -ENOMEM;
	}

	mutex_init(&fm25->lock);
	fm25->chip = chip;
	fm25->spi = spi_dev_get(spi);
	spi_set_drvdata(spi, fm25);

	/* Get ID of chip */
	fm25_id_read(fm25, id);
	if (id[6] != 0xc2) {
		dev_err(&spi->dev, "Error: no Cypress FRAM (id %02x)\n", id[6]);
		return -ENODEV;
	}
	/* set size found in ID */
	switch (id[7]) {
	case 0x21:
		fm25->chip.byte_len = 16 * 1024;
		break;
	case 0x22:
		fm25->chip.byte_len = 32 * 1024;
		break;
	case 0x23:
		fm25->chip.byte_len = 64 * 1024;
		break;
	case 0x24:
		fm25->chip.byte_len = 128 * 1024;
		break;
	case 0x25:
		fm25->chip.byte_len = 256 * 1024;
		break;
	default:
		dev_err(&spi->dev, "Error: unsupported size (id %02x)\n", id[7]);
		return -ENODEV;
		break;
	}

	if (fm25->chip.byte_len > 64 * 1024) {
		fm25->addrlen = 3;
		fm25->chip.flags |= EE_ADDR3;
	}
	else {
		fm25->addrlen = 2;
		fm25->chip.flags |= EE_ADDR2;
	}

	if (id[8])
		fm25->has_sernum = 1;
	else
		fm25->has_sernum = 0;

	fm25->chip.page_size = PAGE_SIZE;

	/* Export the EEPROM bytes through sysfs, since that's convenient.
	 * And maybe to other kernel code; it might hold a board's Ethernet
	 * address, or board-specific calibration data generated on the
	 * manufacturing floor.
	 *
	 * Default to root-only access to the data; EEPROMs often hold data
	 * that's sensitive for read and/or write, like ethernet addresses,
	 * security codes, board-specific manufacturing calibrations, etc.
	 */
	sysfs_bin_attr_init(&fm25->bin);
	fm25->bin.attr.name = "fram";
	fm25->bin.attr.mode = S_IRUGO;
	fm25->bin.read = fm25_bin_read;
	fm25->mem.read = fm25_mem_read;

	fm25->bin.size = fm25->chip.byte_len;
	if (!(chip.flags & EE_READONLY)) {
		fm25->bin.write = fm25_bin_write;
		fm25->bin.attr.mode |= S_IWUSR | S_IWGRP;
		fm25->mem.write = fm25_mem_write;
	}

	err = sysfs_create_bin_file(&spi->dev.kobj, &fm25->bin);
	if (err)
		return err;

	/* Export the FM25 serial number */
	if (fm25->has_sernum) {
		err = device_create_file(&spi->dev, &dev_attr_sernum);
		if (err)
			return err;
	}

	if (chip.setup)
		chip.setup(&fm25->mem, chip.context);

	dev_info(&spi->dev, "%Zd %s %s fram%s\n",
		(fm25->bin.size < 1024)
			? fm25->bin.size
			: (fm25->bin.size / 1024),
		(fm25->bin.size < 1024) ? "Byte" : "KByte",
		fm25->chip.name,
		(chip.flags & EE_READONLY) ? " (readonly)" : "");

	global_fm25 = fm25;

	return 0;
}

static int fm25_remove(struct spi_device *spi)
{
	struct fm25_data *fm25 = spi_get_drvdata(spi);

	sysfs_remove_bin_file(&spi->dev.kobj, &fm25->bin);
	if (fm25->has_sernum)
		device_remove_file(&spi->dev, &dev_attr_sernum);
	return 0;
}

/*-------------------------------------------------------------------------*/

static const struct of_device_id fm25_of_match[] = {
	{ .compatible = "cypress,fm25", },
	{ }
};
MODULE_DEVICE_TABLE(of, fm25_of_match);

static struct spi_driver fm25_driver = {
	.driver = {
		.name = "fm25",
		.owner = THIS_MODULE,
		.of_match_table = fm25_of_match,
	},
	.probe = fm25_probe,
	.remove = fm25_remove,
};
module_spi_driver(fm25_driver);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
static int advfm25_eeprom_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
#else
static long advfm25_eeprom_ioctl(struct file* filp, unsigned int cmd, unsigned long arg )
#endif
{
	switch ( cmd ) 
	{
	default:
		return -EINVAL;
	}

	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
static int advfm25_eeprom_read(struct file *file, char *buf, size_t count, loff_t *ptr)
#else
static ssize_t advfm25_eeprom_read(struct file *file, char *buf, size_t count, loff_t *ptr)
#endif
{
	char tmpbuf[2048] = {0};
	ssize_t readlen = 0;
	
	if (global_fm25 == NULL) {
		printk(KERN_ERR "Error: device not found \n");
		return -EFAULT;
	}

	readlen = fm25_data_read(global_fm25, tmpbuf, *ptr, count);
	printk(KERN_ERR "[%s:%d] readlen:%d \n", __func__, __LINE__, readlen);
	if (readlen != count) {
		printk(KERN_ERR "Error: fm25_read() \n");
		return -EFAULT;
	}

	if (copy_to_user(buf, tmpbuf, count)) {
		printk(KERN_ERR "Error: copy_from_user() \n");
		return -EFAULT;
	}

	return readlen;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
static int advfm25_eeprom_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
#else
static ssize_t advfm25_eeprom_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
#endif
{
	char tmpbuf[2048] = {0};
	ssize_t writelen = 0;
	
	if (global_fm25 == NULL) {
		printk(KERN_ERR "Error: device not found \n");
		return -EFAULT;
	}

	if (copy_from_user(tmpbuf, buf, count)) {
		printk(KERN_ERR "Error: copy_from_user() \n");
		return -EFAULT;
	}

	writelen = fm25_data_write(global_fm25, tmpbuf, *ppos, count);
	if (writelen != count) {
		printk(KERN_ERR "Error: fm25_write() \n");
		return -EFAULT;
	}

	return writelen;
}

static loff_t advfm25_eeprom_llseek(struct file *file, loff_t offset, int origin)
{
	loff_t newpos = 0;

	switch (origin)
	{
	case 0:		/* SEEK_SET */
		newpos = offset;
		break;
	case 1:		/* SEEK_CUR */
		newpos += file->f_pos + offset;
		break;
	case 2:		/* SEEK_END */
		newpos += 0x20000;
		break;
	default:	/* can't happen */
		return -EINVAL;
	}

	if ( newpos < 0 ) {
		return -EINVAL;
	}
	file->f_pos = newpos;

	return newpos;
}

static int advfm25_eeprom_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int advfm25_eeprom_release(struct inode *inode, struct file *file)
{
	return 0;
}

static struct file_operations advfm25_eeprom_fops = {
	owner:		THIS_MODULE,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
	ioctl:		advfm25_eeprom_ioctl,
#else
	unlocked_ioctl: advfm25_eeprom_ioctl,
#endif
	read:		advfm25_eeprom_read,
	write:		advfm25_eeprom_write,
	llseek:		advfm25_eeprom_llseek,
	open:		advfm25_eeprom_open,
	release:	advfm25_eeprom_release,
};

static int __init advfm25_init(void)
{
	if ((major = register_chrdev(0, "adveeprom", &advfm25_eeprom_fops)) < 0) {
		printk("register chrdev failed!\n");
		return -ENODEV;
	}

	eeprom_class = class_create(THIS_MODULE, "adveeprom");
	if (IS_ERR(eeprom_class)) {
		printk(KERN_ERR "Error creating eeprom class.\n");
		return -EINVAL;
	}
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 27)
	device_create(eeprom_class, NULL, MKDEV(major, 0), NULL, "adveeprom");
#else
	class_device_create(eeprom_class, NULL, MKDEV(major, 0), NULL, "adveeprom");
#endif

	printk("=====================================================\n");
	printk("     Advantech fm25v10 eeprom driver V%s [%s]\n", 
			ADVANTECH_FM25_EEPROM_VER, ADVANTECH_FM25_EEPROM_DATE);
	printk("=====================================================\n");

	return 0;
}
module_init(advfm25_init);

static void __exit advfm25_exit(void)
{
	device_destroy(eeprom_class, MKDEV(major, 0));
	class_destroy(eeprom_class);
	unregister_chrdev( major, "adveeprom" );
	printk("Advantech eeprom exit!\n");
}
module_exit(advfm25_exit);

MODULE_DESCRIPTION("Driver for Cypress SPI FRAMs (Advantech TPC-71W fm25v10 eeprom)");
MODULE_AUTHOR("Ji.Xu");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:fram");
