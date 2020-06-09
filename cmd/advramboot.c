/*
 * (C) Copyright 2018
 * ji.xu, ji.xu@advantech.com.cn
 *
 * License:	GPL-2.0+
 */

/*
 * Boot support
 */
#include <common.h>
#include <command.h>
#include <config.h>
#include <console.h>
#include <s_record.h>
#include <malloc.h>
#include <mapmem.h>
#include <net.h>
#include <ata.h>
#include <asm/io.h>
#include <asm/byteorder.h>
#include <linux/ctype.h>
#include <linux/stat.h>
#include <part.h>
#include <fat.h>
#include <fs.h>
#include <ext4fs.h>
#include <exports.h>
#include <image.h>

#if defined(CONFIG_CMD_USB) && defined(CONFIG_USB_STORAGE)
#include <usb.h>
#endif

int adv_do_ramdisk_boot(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	int expr = 0;
	char cmd[96] = {0};

	if (argc < 4) {
		printf("usage:  <interface> <dev[:part]> <addr> <filename>\n");
		return 0;
	}

	if (file_exists(argv[1], argv[2], "advupdate.txt", FS_TYPE_ANY)) {
		if (argv[2][2] == '1') {
			printf("** Recovery from SD card **\n");
			if (ret = do_load(cmdtp, flag, argc, argv, FS_TYPE_FAT)) {
				printf("** Unable to fatload %s %s %s %s **\n", 
						argv[1], argv[2], argv[3], argv[4]);
				return ret;
			}
		} else if (argv[2][2] == '3') {
			printf("** Recovery from eMMC **\n");
			if (ret = do_load(cmdtp, flag, argc, argv, FS_TYPE_EXT)) {
				printf("** Unable to ext4load %s %s %s %s **\n", 
						argv[1], argv[2], argv[3], argv[4]);
				return ret;
			}
		}
	} else {
		printf("The advupdate.txt was not exist! \n");
		return -1;
	}

	strncat(cmd, "/dev/ram0 rw init=/init initrd=", 31);
	strcat(cmd, argv[3]);
	strncat(cmd, ",16M rootfstype=ext2 rootwait", 29);

	return setenv("mmcroot", cmd);
}


U_BOOT_CMD(
	advramboot,	7,	0,	adv_do_ramdisk_boot,
	"load ramdisk.gz and boot in recovery mode.",
	"<interface> <dev[:part]> <addr> <filename>\n"
	"      for example:\n"
	"      advramboot mmc 0:1 0x524288000 ramdisk.gz\n"
	"      advramboot mmc 1:3 0x524288000 ramdisk.gz\n"
);



