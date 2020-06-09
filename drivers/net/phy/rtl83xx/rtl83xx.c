/*
 * drivers/net/phy/realtek/rtl8363.c
 *
 * Driver for Realtek RTL8363 PHYs
 *
 * Author: Johnson Leung <r58129@freescale.com>
 *
 * Copyright (c) 2004 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#include <linux/phy.h>
#include <linux/module.h>
#include <linux/version.h>

#include "rtk_switch.h"
#include "port.h"
#include "rtl8367c_asicdrv_port.h"
#include "rtl8367c_asicdrv_led.h"
#include "rtl83xx_ioctl.h"
#include "eee.h"
#include "vlan.h"
#include "8364NB-VB_Advantech_phy_testmode.h"

#define RTL821x_PHYSR		0x11
#define RTL821x_PHYSR_DUPLEX	0x2000
#define RTL821x_PHYSR_SPEED	0xc000
#define RTL821x_INER		0x12
#define RTL821x_INER_INIT	0x6400
#define RTL821x_INSR		0x13
#define RTL8211E_INER_LINK_STATUS 0x400

#define RTL8211F_INER_LINK_STATUS 0x0010
#define RTL8211F_INSR		0x1d
#define RTL8211F_PAGE_SELECT	0x1f
#define RTL8211F_TX_DELAY	0x100

MODULE_DESCRIPTION("Realtek PHY driver");
MODULE_AUTHOR("Johnson Leung");
MODULE_LICENSE("GPL");

// Support SI test
//#define EMI_TEST
#define USE_PROC

#ifdef EMI_TEST
#include "rtl8364nbi_advantech_ssc.h"
#endif // EMI_TEST

#ifdef USE_PROC
#include <linux/seq_file.h>
#include <linux/proc_fs.h>

#define PROCFS_MAX_SIZE     128
#define PROC_DIR	"lantest"

static struct phy_device *gphydev;
static char S_procfs_buffer[PROCFS_MAX_SIZE];  

typedef struct _mode_info {
	unsigned int test_port;
	unsigned int test_mode;
} mode_info;     

mode_info mode_data[] = {
	{
		.test_port = 1,
		.test_mode = 0,
	},
};

static int mode_proc_read(struct seq_file *m, void *p);  

static void *c_start(struct seq_file *m, loff_t *pos)  
{  
	return *pos < 1 ? (void *)1 : NULL;  
}  

static void *c_next(struct seq_file *m, void *v, loff_t *pos)  
{  
	++*pos;  
	return NULL;  
}  

static void c_stop(struct seq_file *m, void *v)  
{  
	/*nothing to do*/  
}  

static int c_show(struct seq_file *m, void *p)  
{  
	mode_proc_read(m, p);    
	return 0;      
}  

static struct seq_operations proc_seq_ops = {  
	.show  = c_show,  
	.start = c_start,  
	.next  = c_next,  
	.stop  = c_stop  
};  

static int mode_proc_open(struct inode *inode, struct file *file)  
{  
	int ret = 0;      
	struct seq_file *m;      

	ret = seq_open(file, &proc_seq_ops);      
	m = file->private_data;  
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 19, 0)
	m->private = file->f_dentry->d_iname;
#else
	m->private = file->f_path.dentry->d_iname;
#endif

	return ret;   
}  

static int mode_proc_read(struct seq_file *m, void *p)  
{  
	unsigned char *name;
	unsigned int test_port = 1;
	unsigned int test_mode = 0;

	name = m->private;

	test_port = mode_data[0].test_port;
	test_mode = mode_data[0].test_mode;

	switch (test_mode) {
	default:
		printk("Unknow test mode! \n");
	case 0:
		printk("Current port%u mode: work \n", test_port);
		break;
	case 1:
		printk("Current port%u mode: test mode 1 \n", test_port);
		break;
	case 4:
		printk("Current port%u mode: test mode 4 \n", test_port);
		break;
	}
	
	printk("=====================================================\n");  
	printk(" Usage: \n");  
	printk("   PORT1 TEST NORMAL: echo 1 0 > /proc/lantest/mode \n");  
	printk("   PORT1 TEST MODE1:  echo 1 1 > /proc/lantest/mode \n");  
	printk("   PORT1 TEST MODE4:  echo 1 4 > /proc/lantest/mode \n");  
	printk("   PORT3 TEST NORMAL: echo 3 0 > /proc/lantest/mode \n");  
	printk("   PORT3 TEST MODE1:  echo 3 1 > /proc/lantest/mode \n");  
	printk("   PORT3 TEST MODE4:  echo 3 4 > /proc/lantest/mode \n");  
	printk("=====================================================\n");  

	return 0;  
}  

static int mode_proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *offp)  
{  
	unsigned int value1, value2;
	int procfs_buffer_size;  

	if(count > PROCFS_MAX_SIZE) {  
		procfs_buffer_size = PROCFS_MAX_SIZE;  
	} else {  
		procfs_buffer_size = count;  
	}  

	if (copy_from_user(S_procfs_buffer, buffer, procfs_buffer_size)) {  
		return -EFAULT;  
	}  
	*(S_procfs_buffer+procfs_buffer_size) = 0;  

	if(sscanf(S_procfs_buffer , "%u %u" , &value1, &value2) == 2) {
		mode_data[0].test_port = value1;
		mode_data[0].test_mode = value2;
	} else {
		printk("=====================================================\n");  
		printk(" Usage: \n");  
		printk("   PORT1 TEST NORMAL: echo 1 0 > /proc/lantest/mode \n");  
		printk("   PORT1 TEST MODE1:  echo 1 1 > /proc/lantest/mode \n");  
		printk("   PORT1 TEST MODE4:  echo 1 4 > /proc/lantest/mode \n");  
		printk("   PORT3 TEST NORMAL: echo 3 0 > /proc/lantest/mode \n");  
		printk("   PORT3 TEST MODE1:  echo 3 1 > /proc/lantest/mode \n");  
		printk("   PORT3 TEST MODE4:  echo 3 4 > /proc/lantest/mode \n");  
		printk("=====================================================\n");  
	}  

	switch (mode_data[0].test_mode) {
	default:
		printk("Unknow test mode! \n");
	case 0:
		printk("Change port%u mode: work \n", mode_data[0].test_port);
		if (mode_data[0].test_port == 1) {
			rtk_port_phyTestMode_set(gphydev, UTP_PORT1, 0);
		} else if (mode_data[0].test_port == 3) {
			rtk_port_phyTestMode_set(gphydev, UTP_PORT3, 0);
		} else {
			printk("Unknow lan port! \n");
		}
		break;
	case 1:
		printk("Change port%u mode: test mode 1 \n", mode_data[0].test_port);
		if (mode_data[0].test_port == 1) {
			rtk_port_phyTestMode_set(gphydev, UTP_PORT1, 1);
		} else if (mode_data[0].test_port == 3) {
			rtk_port_phyTestMode_set(gphydev, UTP_PORT3, 1);
		} else {
			printk("Unknow lan port! \n");
		}
		break;
	case 4:
		printk("Change port%u mode: test mode 4 \n", mode_data[0].test_port);
		if (mode_data[0].test_port == 1) {
			rtk_port_phyTestMode_set(gphydev, UTP_PORT1, 4);
		} else if (mode_data[0].test_port == 3) {
			rtk_port_phyTestMode_set(gphydev, UTP_PORT3, 4);
		} else {
			printk("Unknow lan port! \n");
		}
		break;
	}

	return count;  
}  

static struct file_operations proc_fops = {  
	.open  = mode_proc_open,  
	.read  = mode_proc_read,  
	.write = mode_proc_write,        
};  

int mode_create_proc(char *name)  
{  
	struct proc_dir_entry *mode_proc_entries;  
	unsigned char proc_name[64]={0};  

#ifdef PROC_DIR
	sprintf(proc_name, "%s/%s", (const char *)PROC_DIR, name);  
#else
	sprintf(proc_name, "%s", name);  
#endif //PROC_DIR

	mode_proc_entries = proc_create(proc_name, 0644, NULL, &proc_fops);  
	if (NULL == mode_proc_entries) {  
		remove_proc_entry(proc_name, NULL);  
		printk("Error: Could not initialize /proc/%s\n", proc_name);  
		return -ENOMEM;  
	}  

	return 0;  
}  

int mode_create_proc_parentdir(void)  
{  
#ifdef PROC_DIR
	struct proc_dir_entry *mydir = NULL;  

	mydir = proc_mkdir(PROC_DIR, NULL);  
	if (!mydir) {  
		printk(KERN_ERR "Can't create /proc/%s\n", (const char *)PROC_DIR);  
		return -1;  
	}  
#endif //PROC_DIR
	return 0;  
}  

void mode_remove_proc(char *name)  
{  
	unsigned char proc_name[64]={0};  

#ifdef PROC_DIR
	sprintf(proc_name, "%s/%s", (const char *)PROC_DIR, name);  
#else
	sprintf(proc_name, "%s", name);  
#endif //PROC_DIR
	remove_proc_entry(proc_name, NULL);  
	return;  
}  

void mode_remove_proc_parentdir(void)
{
#ifdef PROC_DIR
	remove_proc_entry(PROC_DIR, NULL);  
#endif //PROC_DIR
	return;  
}
#endif	//USE_PROC

static int rtl8363_config_init(struct phy_device *phydev)
{
	rtk_uint32 port;
	rtk_port_phy_ability_t pAbility;
	rtk_uint32 ledno, phyidx, status;
	//rtk_uint8 ledconf[2][3] = {{2, 0, 4}, {2, 4, 0}};
	//rtk_uint8 ledconf[2][3] = {{2, 2, 2}, {2, 0, 4}};
	rtk_uint8 ledconf[2][3] = {{2, 3, 4}, {2, 3, 4}}; 
	rtk_port_mac_ability_t mac_cfg;
	rtk_mode_ext_t mode;
	rtk_portmask_t portmask;

#ifdef USE_PROC
 	gphydev = phydev;
#endif // USE_PROC

	/* 
	 * Realtek API initial 
	 * */
	rtk_switch_init(phydev);

	mode = MODE_EXT_RGMII;
	mac_cfg.forcemode = MAC_FORCE;
	mac_cfg.speed = PORT_SPEED_1000M;
	mac_cfg.duplex = FULL_DUPLEX;
	mac_cfg.link = PORT_LINKUP;
#if 0
	mac_cfg.nway = DISABLED;
	mac_cfg.txpause = ENABLED;
	mac_cfg.rxpause = ENABLED;

	rtk_port_macForceLinkExt_set(phydev, EXT_PORT0, mode, &mac_cfg);
	rtk_port_rgmiiDelayExt_set(phydev, EXT_PORT0, 1, 2);
#else
	/*if(phydev->addr == 29)*/
	{
		mac_cfg.nway = DISABLED;
		mac_cfg.txpause = DISABLED;
		mac_cfg.rxpause = DISABLED;

		/* 
		 * The API is used to configure switch.
		 * */
		rtk_port_macForceLinkExt_set(phydev, EXT_PORT0, mode, &mac_cfg);

		/* 
		 * The API is used to set RGMII interface TX and RX delay. 
		 * In TX delay, the value 1 means delay 2ns and 0 means no delay. 
		 * In RX delay, there are 8 steps to tune the delay status.
		 * */
		rtk_port_rgmiiDelayExt_set(phydev, EXT_PORT0, 1, 2);
	}
	/*else
		{
		mac_cfg.nway = DISABLED;
		mac_cfg.txpause = ENABLED;
		mac_cfg.rxpause = ENABLED;

		rtk_port_macForceLinkExt_set(phydev, EXT_PORT0, mode, &mac_cfg);
		rtk_port_rgmiiDelayExt_set(phydev, EXT_PORT0, 1, 2);
	}*/
#endif

	/*
	 * This API is used to enable all PHYs if the default PHY status is disabled
	 * by pin strap. All PHYs are disabled by hardware pin, use API to enable all 
	 * PHYs.
	 * */
	rtk_port_phyEnableAll_set(phydev, 1);

	memset(&pAbility, 0x0, sizeof(rtk_port_phy_ability_t));

	pAbility.AsyFC = 1;
	pAbility.AutoNegotiation = 1;
	pAbility.FC = 1;
	pAbility.Full_10 = 1;
	pAbility.Full_100 = 1;
	pAbility.Half_10 = 1;
	pAbility.Half_100 = 1;
	pAbility.Full_1000 = 1;

	rtk_switch_greenEthernet_set(phydev, DISABLED);
	rtk_eee_init(phydev);

	for(port = 0; port < 5;port++) {
		/* 
		 * If 1000F capability bit is set to 1, API will also configure
		 * Auto-Negotiation register field bit enabled. 
		 * FC and AsyFC fields are defined as PAUSE bit and ASM_DIR bit 
		 * as Std 802.3 annex 28B. The encoding of Bit FC and AsyFC is 
		 * specified as following table.
		 * */
		rtk_port_phyAutoNegoAbility_set(phydev, port, &pAbility);

		/* 
		 * This API can be used to enable/disable EEE function per port.
		 * */
		rtk_eee_portEnable_set(phydev, port, DISABLED);
#ifdef EMI_TEST
		rtk_port_macForceLinkExtSSC_set(phydev, port, ENABLED);
#endif // EMI_TEST

	}

	if (phydev->addr == 0) {
		phyidx = 0;
	} else {
		phyidx = 1;
	}

	for (ledno = 0; ledno < 3; ledno++) {
		/*
		 * Set Leds indicated information mode.
		 * */
		status = rtl8367c_setAsicLedIndicateInfoConfig(phydev, ledno, ledconf[phyidx][ledno]);
	}


	phydev->speed = SPEED_1000;
	phydev->duplex = DUPLEX_FULL;
	phydev->supported = SUPPORTED_100baseT_Full;
	phydev->autoneg = AUTONEG_DISABLE;

#if 0
	/* Enable LED Group 0&1 from P1&P3 */
	RTK_PORTMASK_PORT_SET(portmask, UTP_PORT1);
	RTK_PORTMASK_PORT_SET(portmask, UTP_PORT3);

	rtk_led_enable_set(LED_GROUP_0, &portmask);
	rtk_led_enable_set(LED_GROUP_1, &portmask);
#endif

#if 0
	/* initialize VLAN */
	rtk_vlan_init(phydev);

	/*
	 * all the ports are in the default VLAN 1 after VLAN initialized, modify it
	 * as follows:
	 * VLAN1 member : UTP_PORT1, EXT_PORT0 with fid 1 ;
	 * VLAN2 member : UTP_PORT1&UTP_PORT3, EXT_PORT0 with fid 2
	 * */
	rtk_vlan_cfg_t vlan1, vlan2;

	memset(&vlan1, 0x00, sizeof(rtk_vlan_cfg_t));
	RTK_PORTMASK_PORT_SET(vlan1.mbr, UTP_PORT1);
	RTK_PORTMASK_PORT_SET(vlan1.mbr, EXT_PORT0);
	RTK_PORTMASK_PORT_SET(vlan1.untag, UTP_PORT1);
//	RTK_PORTMASK_PORT_SET(vlan1.untag, EXT_PORT0);
	rtk_vlan_set(phydev, 1, &vlan1);

	memset(&vlan2, 0x00, sizeof(rtk_vlan_cfg_t));
//	RTK_PORTMASK_PORT_SET(vlan2.mbr, UTP_PORT1);
	RTK_PORTMASK_PORT_SET(vlan2.mbr, UTP_PORT3);
	RTK_PORTMASK_PORT_SET(vlan2.mbr, EXT_PORT0);
	RTK_PORTMASK_PORT_SET(vlan2.untag, UTP_PORT1);
	RTK_PORTMASK_PORT_SET(vlan2.untag, UTP_PORT3);
//	RTK_PORTMASK_PORT_SET(vlan2.untag, EXT_PORT0);
	rtk_vlan_set(phydev, 2, &vlan2);

	/* set PVID for each port */
	rtk_vlan_portPvid_set(phydev, UTP_PORT1, 1, 0);
	rtk_vlan_portPvid_set(phydev, UTP_PORT3, 2, 0);
	rtk_vlan_portPvid_set(phydev, EXT_PORT0, 2, 0);
#endif

	return 0;
}

static int rtl8363_update_link(struct phy_device *phydev)
{
	phy_status_s status_portA;
	phy_status_s status_portB;

#if 0	// for debug test
	phy_status_s status;
	int i;
	for (i = 0; i < 5; i++) {
		status.Port = i;
		rtk_port_phyStatus_get(phydev, status.Port, &status.LinkStatus, &status.Speed, &status.Duplex);
    	printk("%s,%d: port[%d] - link:%d speed:%d duplex:%d \n", __func__, __LINE__, 
				status.Port, status.LinkStatus, status.Speed, status.Duplex);
	}
#endif 

	status_portA.Port = UTP_PORT1;
	status_portB.Port = UTP_PORT3;
	if (RT_ERR_OK == rtk_port_phyStatus_get(phydev, status_portA.Port, &status_portA.LinkStatus, 
				&status_portA.Speed, &status_portA.Duplex) && status_portA.LinkStatus == 1) {
		phydev->link = status_portA.LinkStatus;
		//phydev->speed = status_portA.Speed;
		//phydev->duplex = status_portA.Duplex;
		phydev->speed = SPEED_1000;
		phydev->duplex = DUPLEX_FULL;
	} else if (RT_ERR_OK == rtk_port_phyStatus_get(phydev, status_portB.Port, &status_portB.LinkStatus, 
				&status_portB.Speed, &status_portB.Duplex) && status_portB.LinkStatus == 1) { 
		phydev->link = status_portB.LinkStatus;
		//phydev->speed = status_portB.Speed;
		//phydev->duplex = status_portB.Duplex;
		phydev->speed = SPEED_1000;
		phydev->duplex = DUPLEX_FULL;
	} else {
		phydev->link = 0;
	}

	return 0;
}

static int rtl8363_read_status(struct phy_device *phydev)
{
	rtl8363_update_link(phydev);

	phydev->pause = 0;
	phydev->asym_pause = 0;

	return 0;
}

static int rtl8363_soft_reset(struct phy_device *phydev)
{
	int ret;

#if 0
	ret = phy_write(phydev, MII_BMCR, BMCR_RESET);
	if (ret < 0)
		return ret;

	return phy_poll_reset(phydev);
#else
	return 0;
#endif
}

static int rtl8363_config_aneg(struct phy_device *phydev)
{
	return 0;
}

static struct phy_driver realtek_drvs[] = {
	{
		.phy_id		= 0x001cc942,
		.name		= "Realtek RTL8363",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_GBIT_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.config_aneg	= &rtl8363_config_aneg,
		.config_init	= &rtl8363_config_init,
		.read_status	= &rtl8363_read_status,
		.update_link	= &rtl8363_update_link,
		.soft_reset = &rtl8363_soft_reset,
		.suspend	= genphy_suspend,
		.resume		= genphy_resume,
		.driver		= { .owner = THIS_MODULE },
	},
	{
		.phy_id		= 0x001cc943,
		.name		= "Realtek RTL8364B",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_GBIT_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.config_aneg	= &rtl8363_config_aneg,
		.config_init	= &rtl8363_config_init,
		.read_status	= &rtl8363_read_status,
		.update_link	= &rtl8363_update_link,
		.soft_reset = &rtl8363_soft_reset,
		.suspend	= genphy_suspend,
		.resume		= genphy_resume,
		.driver		= { .owner = THIS_MODULE },
	},

};

module_phy_driver(realtek_drvs);

static struct mdio_device_id __maybe_unused realtek_tbl[] = {
	{ 0x001cc942, 0x001fffff },
	{ 0x001cc943, 0x001fffff },
	{ }
};

MODULE_DEVICE_TABLE(mdio, realtek_tbl);

rtk_api_ret_t rtl8363_qos_set(struct phy_device *phydev)
{
    rtk_qos_pri2queue_t     q_map;
    rtk_api_ret_t           ret;
    rtk_priority_select_t   pri_sel;
    rtk_port_t              port;

    ret = rtk_qos_init(phydev, 5);
    if(ret != RT_ERR_OK)
        return ret;

    memset(&pri_sel, 0x00, sizeof(rtk_priority_select_t));
    pri_sel.dot1q_pri  = 5;
    pri_sel.port_pri   = 6;
    pri_sel.acl_pri    = 7;
    ret = rtk_qos_priSel_set(phydev, 0, &pri_sel);
    if(ret != RT_ERR_OK)
        return ret;

    memset(&q_map, 0, sizeof(rtk_qos_pri2queue_t));
    q_map.pri2queue[0] = 0;
    q_map.pri2queue[1] = 0;
    q_map.pri2queue[2] = 1;
    q_map.pri2queue[3] = 1;
    q_map.pri2queue[4] = 2;
    q_map.pri2queue[5] = 3;
    q_map.pri2queue[6] = 7;
    q_map.pri2queue[7] = 7;
    ret = rtk_qos_priMap_set(phydev, 5, &q_map);
    if(ret != RT_ERR_OK)
        return ret;

    for(port = UTP_PORT0; port < UTP_PORT5; port++) {
        rtk_qos_portPri_set(phydev, port, 0);
    }
    rtk_qos_portPri_set(phydev, EXT_PORT0, 7);

    return RT_ERR_OK;
}

static rtk_api_ret_t rtl8363_l2_floodPortMask_unknownda_set(struct phy_device *phydev, rtk_l2_flood_type_t flood_type)
{
    rtk_portmask_t flood_mask;
    rtk_uint32 retVal;
        
    flood_mask.bits[0] = 0;
    RTK_PORTMASK_PORT_SET(flood_mask, UTP_PORT1); 
    RTK_PORTMASK_PORT_SET(flood_mask, UTP_PORT3); 

    printk("%s,%d: Setting floodingPortMask (%d)\n", __FUNCTION__, __LINE__, flood_type);  //jjjj
    if((retVal=rtk_l2_floodPortMask_set(phydev, flood_type, &flood_mask))!= RT_ERR_OK)
    {
        printk("%s,%d: Setting error (%d)\n", __FUNCTION__, __LINE__, flood_type);
    }

    return retVal;
}

static int rtl83xx_aclrule_dmac_ethtype(struct phy_device *phydev, rtlsw_acl_rule_cmd_s *pAcl_cmd)
{
    rtk_filter_field_t      field1;
    rtk_filter_field_t      field2;
    rtk_filter_cfg_t        cfg;
    rtk_filter_action_t     act;
    rtk_filter_number_t     ruleNum;
    rtk_api_ret_t           ret_val;

    memset(&field1, 0, sizeof(rtk_filter_field_t));
    memset(&field2, 0, sizeof(rtk_filter_field_t));
    memset(&cfg,    0 , sizeof(cfg));
    memset(&act,    0 , sizeof(act));

    field1.fieldType = FILTER_FIELD_DMAC;
    field1.filter_pattern_union.dmac.dataType       = FILTER_FIELD_DATA_MASK;
    field1.filter_pattern_union.dmac.value.octet[0] = pAcl_cmd->dmac[0];
    field1.filter_pattern_union.dmac.value.octet[1] = pAcl_cmd->dmac[1];
    field1.filter_pattern_union.dmac.value.octet[2] = pAcl_cmd->dmac[2];
    field1.filter_pattern_union.dmac.value.octet[3] = pAcl_cmd->dmac[3];
    field1.filter_pattern_union.dmac.value.octet[4] = pAcl_cmd->dmac[4];
    field1.filter_pattern_union.dmac.value.octet[5] = pAcl_cmd->dmac[5];
    field1.filter_pattern_union.dmac.mask.octet[0]  = pAcl_cmd->dmask[0];
    field1.filter_pattern_union.dmac.mask.octet[1]  = pAcl_cmd->dmask[1];
    field1.filter_pattern_union.dmac.mask.octet[2]  = pAcl_cmd->dmask[2];
    field1.filter_pattern_union.dmac.mask.octet[3]  = pAcl_cmd->dmask[3];
    field1.filter_pattern_union.dmac.mask.octet[4]  = pAcl_cmd->dmask[4];
    field1.filter_pattern_union.dmac.mask.octet[5]  = pAcl_cmd->dmask[5];
    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field1);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error (id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    field2.fieldType                               = FILTER_FIELD_ETHERTYPE;
    field2.filter_pattern_union.etherType.dataType = FILTER_FIELD_DATA_MASK;
    field2.filter_pattern_union.etherType.value    = pAcl_cmd->pattern_union.ethtype;
    field2.filter_pattern_union.etherType.mask     = 0xffff;
    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field2);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error (id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT1);
    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT3);
    RTK_PORTMASK_ALLPORT_SET(cfg.activeport.mask);
    cfg.invert = FALSE;

    /* Action :  */
    memcpy(&act, &pAcl_cmd->filter_action, sizeof(act));

    ret_val = rtk_filter_igrAcl_cfg_add(phydev, pAcl_cmd->filter_id, &cfg, &act, &ruleNum);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error (id=%d, ruleNum=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ruleNum, ret_val);
        return ret_val;
    }
    return ret_val;
}

static int rtl83xx_aclrule_dmac_smac_ethtype(struct phy_device *phydev, rtlsw_acl_rule_cmd_s *pAcl_cmd)
{
    rtk_filter_field_t      field1;
    rtk_filter_field_t      field2;
    rtk_filter_field_t      field3;
    rtk_filter_cfg_t        cfg;
    rtk_filter_action_t     act;
    rtk_filter_number_t     ruleNum;
    rtk_api_ret_t           ret_val;

    memset(&field1, 0, sizeof(rtk_filter_field_t));
    memset(&field2, 0, sizeof(rtk_filter_field_t));
    memset(&field3, 0, sizeof(rtk_filter_field_t));
    memset(&cfg,    0 , sizeof(cfg));
    memset(&act,    0 , sizeof(act));

    field1.fieldType = FILTER_FIELD_DMAC;
    field1.filter_pattern_union.dmac.dataType       = FILTER_FIELD_DATA_MASK;
    field1.filter_pattern_union.dmac.value.octet[0] = pAcl_cmd->dmac[0];
    field1.filter_pattern_union.dmac.value.octet[1] = pAcl_cmd->dmac[1];
    field1.filter_pattern_union.dmac.value.octet[2] = pAcl_cmd->dmac[2];
    field1.filter_pattern_union.dmac.value.octet[3] = pAcl_cmd->dmac[3];
    field1.filter_pattern_union.dmac.value.octet[4] = pAcl_cmd->dmac[4];
    field1.filter_pattern_union.dmac.value.octet[5] = pAcl_cmd->dmac[5];
    field1.filter_pattern_union.dmac.mask.octet[0]  = pAcl_cmd->dmask[0];
    field1.filter_pattern_union.dmac.mask.octet[1]  = pAcl_cmd->dmask[1];
    field1.filter_pattern_union.dmac.mask.octet[2]  = pAcl_cmd->dmask[2];
    field1.filter_pattern_union.dmac.mask.octet[3]  = pAcl_cmd->dmask[3];
    field1.filter_pattern_union.dmac.mask.octet[4]  = pAcl_cmd->dmask[4];
    field1.filter_pattern_union.dmac.mask.octet[5]  = pAcl_cmd->dmask[5];
    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field1);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error 1(id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    field2.fieldType = FILTER_FIELD_SMAC;
    field2.filter_pattern_union.smac.dataType       = FILTER_FIELD_DATA_MASK;
    field2.filter_pattern_union.smac.value.octet[0] = pAcl_cmd->smac[0];
    field2.filter_pattern_union.smac.value.octet[1] = pAcl_cmd->smac[1];
    field2.filter_pattern_union.smac.value.octet[2] = pAcl_cmd->smac[2];
    field2.filter_pattern_union.smac.value.octet[3] = pAcl_cmd->smac[3];
    field2.filter_pattern_union.smac.value.octet[4] = pAcl_cmd->smac[4];
    field2.filter_pattern_union.smac.value.octet[5] = pAcl_cmd->smac[5];
    field2.filter_pattern_union.smac.mask.octet[0]  = pAcl_cmd->smask[0];
    field2.filter_pattern_union.smac.mask.octet[1]  = pAcl_cmd->smask[1];
    field2.filter_pattern_union.smac.mask.octet[2]  = pAcl_cmd->smask[2];
    field2.filter_pattern_union.smac.mask.octet[3]  = pAcl_cmd->smask[3];
    field2.filter_pattern_union.smac.mask.octet[4]  = pAcl_cmd->smask[4];
    field2.filter_pattern_union.smac.mask.octet[5]  = pAcl_cmd->smask[5];
    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field2);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error 2(id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    field3.fieldType                               = FILTER_FIELD_ETHERTYPE;
    field3.filter_pattern_union.etherType.dataType = FILTER_FIELD_DATA_MASK;
    field3.filter_pattern_union.etherType.value    = pAcl_cmd->pattern_union.ethtype;
    field3.filter_pattern_union.etherType.mask     = 0xffff;
    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field3);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error 3(id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT1);
    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT3);
    RTK_PORTMASK_ALLPORT_SET(cfg.activeport.mask);
    cfg.invert = FALSE;

    /* Action :  */
    memcpy(&act, &pAcl_cmd->filter_action, sizeof(act));

    ret_val = rtk_filter_igrAcl_cfg_add(phydev, pAcl_cmd->filter_id, &cfg, &act, &ruleNum);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error (id=%d, ruleNum=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ruleNum, ret_val);
        return ret_val;
    }
    return ret_val;
}

static int rtl83xx_aclrule_dmac_smac(struct phy_device *phydev, rtlsw_acl_rule_cmd_s *pAcl_cmd)
{
    rtk_filter_field_t      field1;
    rtk_filter_field_t      field2;
    rtk_filter_cfg_t        cfg;
    rtk_filter_action_t     act;
    rtk_filter_number_t     ruleNum;
    rtk_api_ret_t           ret_val;

    memset(&field1, 0, sizeof(rtk_filter_field_t));
    memset(&field2, 0, sizeof(rtk_filter_field_t));
    memset(&cfg,    0 , sizeof(cfg));
    memset(&act,    0 , sizeof(act));

    field1.fieldType = FILTER_FIELD_DMAC;
    field1.filter_pattern_union.dmac.dataType       = FILTER_FIELD_DATA_MASK;
    field1.filter_pattern_union.dmac.value.octet[0] = pAcl_cmd->dmac[0];
    field1.filter_pattern_union.dmac.value.octet[1] = pAcl_cmd->dmac[1];
    field1.filter_pattern_union.dmac.value.octet[2] = pAcl_cmd->dmac[2];
    field1.filter_pattern_union.dmac.value.octet[3] = pAcl_cmd->dmac[3];
    field1.filter_pattern_union.dmac.value.octet[4] = pAcl_cmd->dmac[4];
    field1.filter_pattern_union.dmac.value.octet[5] = pAcl_cmd->dmac[5];
    field1.filter_pattern_union.dmac.mask.octet[0]  = pAcl_cmd->dmask[0];
    field1.filter_pattern_union.dmac.mask.octet[1]  = pAcl_cmd->dmask[1];
    field1.filter_pattern_union.dmac.mask.octet[2]  = pAcl_cmd->dmask[2];
    field1.filter_pattern_union.dmac.mask.octet[3]  = pAcl_cmd->dmask[3];
    field1.filter_pattern_union.dmac.mask.octet[4]  = pAcl_cmd->dmask[4];
    field1.filter_pattern_union.dmac.mask.octet[5]  = pAcl_cmd->dmask[5];
    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field1);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error 1(id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    field2.fieldType = FILTER_FIELD_SMAC;
    field2.filter_pattern_union.smac.dataType       = FILTER_FIELD_DATA_MASK;
    field2.filter_pattern_union.smac.value.octet[0] = pAcl_cmd->smac[0];
    field2.filter_pattern_union.smac.value.octet[1] = pAcl_cmd->smac[1];
    field2.filter_pattern_union.smac.value.octet[2] = pAcl_cmd->smac[2];
    field2.filter_pattern_union.smac.value.octet[3] = pAcl_cmd->smac[3];
    field2.filter_pattern_union.smac.value.octet[4] = pAcl_cmd->smac[4];
    field2.filter_pattern_union.smac.value.octet[5] = pAcl_cmd->smac[5];
    field2.filter_pattern_union.smac.mask.octet[0]  = pAcl_cmd->smask[0];
    field2.filter_pattern_union.smac.mask.octet[1]  = pAcl_cmd->smask[1];
    field2.filter_pattern_union.smac.mask.octet[2]  = pAcl_cmd->smask[2];
    field2.filter_pattern_union.smac.mask.octet[3]  = pAcl_cmd->smask[3];
    field2.filter_pattern_union.smac.mask.octet[4]  = pAcl_cmd->smask[4];
    field2.filter_pattern_union.smac.mask.octet[5]  = pAcl_cmd->smask[5];
    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field2);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error 2(id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT1);
    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT3);
    RTK_PORTMASK_ALLPORT_SET(cfg.activeport.mask);
    cfg.invert = FALSE;

    /* Action :  */
    memcpy(&act, &pAcl_cmd->filter_action, sizeof(act));

    ret_val = rtk_filter_igrAcl_cfg_add(phydev, pAcl_cmd->filter_id, &cfg, &act, &ruleNum);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error (id=%d, ruleNum=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ruleNum, ret_val);
        return ret_val;
    }
    return ret_val;
}

static int rtl83xx_aclrule_dmac(struct phy_device *phydev, rtlsw_acl_rule_cmd_s *pAcl_cmd)
{
    rtk_filter_field_t      field1;
    rtk_filter_cfg_t        cfg;
    rtk_filter_action_t     act;
    rtk_filter_number_t     ruleNum;
    rtk_api_ret_t           ret_val;

    memset(&field1, 0, sizeof(rtk_filter_field_t));
    memset(&cfg,    0 , sizeof(cfg));
    memset(&act,    0 , sizeof(act));

    field1.fieldType = FILTER_FIELD_DMAC;
    field1.filter_pattern_union.dmac.dataType       = FILTER_FIELD_DATA_MASK;
    field1.filter_pattern_union.dmac.value.octet[0] = pAcl_cmd->dmac[0];
    field1.filter_pattern_union.dmac.value.octet[1] = pAcl_cmd->dmac[1];
    field1.filter_pattern_union.dmac.value.octet[2] = pAcl_cmd->dmac[2];
    field1.filter_pattern_union.dmac.value.octet[3] = pAcl_cmd->dmac[3];
    field1.filter_pattern_union.dmac.value.octet[4] = pAcl_cmd->dmac[4];
    field1.filter_pattern_union.dmac.value.octet[5] = pAcl_cmd->dmac[5];
    field1.filter_pattern_union.dmac.mask.octet[0]  = pAcl_cmd->dmask[0];
    field1.filter_pattern_union.dmac.mask.octet[1]  = pAcl_cmd->dmask[1];
    field1.filter_pattern_union.dmac.mask.octet[2]  = pAcl_cmd->dmask[2];
    field1.filter_pattern_union.dmac.mask.octet[3]  = pAcl_cmd->dmask[3];
    field1.filter_pattern_union.dmac.mask.octet[4]  = pAcl_cmd->dmask[4];
    field1.filter_pattern_union.dmac.mask.octet[5]  = pAcl_cmd->dmask[5];
    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field1);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error (id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT1);
    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT3);
    RTK_PORTMASK_ALLPORT_SET(cfg.activeport.mask);
    cfg.invert = FALSE;

    /* Action :  */
    memcpy(&act, &pAcl_cmd->filter_action, sizeof(act));

    ret_val = rtk_filter_igrAcl_cfg_add(phydev, pAcl_cmd->filter_id, &cfg, &act, &ruleNum);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error (id=%d, ruleNum=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ruleNum, ret_val);
        return ret_val;
    }
    return ret_val;
}

static int rtl83xx_aclrule_dmac_tcpudp_all(struct phy_device *phydev, rtlsw_acl_rule_cmd_s *pAcl_cmd)
{
    rtk_filter_field_t      field1;
    rtk_filter_field_t      field2;
    rtk_filter_field_t      field3;
    rtk_filter_cfg_t        cfg;
    rtk_filter_action_t     act;
    rtk_filter_number_t     ruleNum;
    rtk_api_ret_t           ret_val;

    memset(&field1, 0, sizeof(rtk_filter_field_t));
    memset(&field2, 0, sizeof(rtk_filter_field_t));
    memset(&field3, 0, sizeof(rtk_filter_field_t));
    memset(&cfg,    0 , sizeof(cfg));
    memset(&act,    0 , sizeof(act));

    field1.fieldType = FILTER_FIELD_DMAC;
    field1.filter_pattern_union.dmac.dataType       = FILTER_FIELD_DATA_MASK;
    field1.filter_pattern_union.dmac.value.octet[0] = pAcl_cmd->dmac[0];
    field1.filter_pattern_union.dmac.value.octet[1] = pAcl_cmd->dmac[1];
    field1.filter_pattern_union.dmac.value.octet[2] = pAcl_cmd->dmac[2];
    field1.filter_pattern_union.dmac.value.octet[3] = pAcl_cmd->dmac[3];
    field1.filter_pattern_union.dmac.value.octet[4] = pAcl_cmd->dmac[4];
    field1.filter_pattern_union.dmac.value.octet[5] = pAcl_cmd->dmac[5];
    field1.filter_pattern_union.dmac.mask.octet[0]  = pAcl_cmd->dmask[0];
    field1.filter_pattern_union.dmac.mask.octet[1]  = pAcl_cmd->dmask[1];
    field1.filter_pattern_union.dmac.mask.octet[2]  = pAcl_cmd->dmask[2];
    field1.filter_pattern_union.dmac.mask.octet[3]  = pAcl_cmd->dmask[3];
    field1.filter_pattern_union.dmac.mask.octet[4]  = pAcl_cmd->dmask[4];
    field1.filter_pattern_union.dmac.mask.octet[5]  = pAcl_cmd->dmask[5];
    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field1);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error 1(id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    field2.fieldType                                  = FILTER_FIELD_TCP_DPORT;
    field2.filter_pattern_union.tcpDstPort.dataType   = FILTER_FIELD_DATA_RANGE;
    field2.filter_pattern_union.tcpDstPort.rangeStart = 0;
    field2.filter_pattern_union.tcpDstPort.rangeEnd   = 65535;

    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field2);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error 2(id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    field3.fieldType                                  = FILTER_FIELD_UDP_DPORT;
    field3.filter_pattern_union.udpDstPort.dataType   = FILTER_FIELD_DATA_RANGE;
    field3.filter_pattern_union.udpDstPort.rangeStart = 0;
    field3.filter_pattern_union.udpDstPort.rangeEnd   = 65535;

    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field3);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error 3(id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT1);
    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT3);
    RTK_PORTMASK_ALLPORT_SET(cfg.activeport.mask);
    cfg.invert = FALSE;

    /* Action :  */
    memcpy(&act, &pAcl_cmd->filter_action, sizeof(act));

    ret_val = rtk_filter_igrAcl_cfg_add(phydev, pAcl_cmd->filter_id, &cfg, &act, &ruleNum);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error (id=%d, ruleNum=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ruleNum, ret_val);
        return ret_val;
    }
    return ret_val;
}

static int rtl83xx_aclrule_dmac_udp_dport(struct phy_device *phydev, rtlsw_acl_rule_cmd_s *pAcl_cmd)
{
    rtk_filter_field_t      field1;
    rtk_filter_field_t      field2;
    rtk_filter_cfg_t        cfg;
    rtk_filter_action_t     act;
    rtk_filter_number_t     ruleNum;
    rtk_api_ret_t           ret_val;

    memset(&field1, 0, sizeof(rtk_filter_field_t));
    memset(&field2, 0, sizeof(rtk_filter_field_t));
    memset(&cfg,    0 , sizeof(cfg));
    memset(&act,    0 , sizeof(act));

    field1.fieldType = FILTER_FIELD_DMAC;
    field1.filter_pattern_union.dmac.dataType       = FILTER_FIELD_DATA_MASK;
    field1.filter_pattern_union.dmac.value.octet[0] = pAcl_cmd->dmac[0];
    field1.filter_pattern_union.dmac.value.octet[1] = pAcl_cmd->dmac[1];
    field1.filter_pattern_union.dmac.value.octet[2] = pAcl_cmd->dmac[2];
    field1.filter_pattern_union.dmac.value.octet[3] = pAcl_cmd->dmac[3];
    field1.filter_pattern_union.dmac.value.octet[4] = pAcl_cmd->dmac[4];
    field1.filter_pattern_union.dmac.value.octet[5] = pAcl_cmd->dmac[5];
    field1.filter_pattern_union.dmac.mask.octet[0]  = pAcl_cmd->dmask[0];
    field1.filter_pattern_union.dmac.mask.octet[1]  = pAcl_cmd->dmask[1];
    field1.filter_pattern_union.dmac.mask.octet[2]  = pAcl_cmd->dmask[2];
    field1.filter_pattern_union.dmac.mask.octet[3]  = pAcl_cmd->dmask[3];
    field1.filter_pattern_union.dmac.mask.octet[4]  = pAcl_cmd->dmask[4];
    field1.filter_pattern_union.dmac.mask.octet[5]  = pAcl_cmd->dmask[5];
    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field1);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error 1(id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    field2.fieldType                                  = FILTER_FIELD_UDP_DPORT;
    field2.filter_pattern_union.tcpDstPort.dataType   = FILTER_FIELD_DATA_MASK;
    field2.filter_pattern_union.tcpDstPort.value      = pAcl_cmd->pattern_union.dport;
    field2.filter_pattern_union.tcpDstPort.mask       = 0xFFFF;

    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field2);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error 2(id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT1);
    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT3);
    RTK_PORTMASK_ALLPORT_SET(cfg.activeport.mask);
    cfg.invert = FALSE;

    /* Action :  */
    memcpy(&act, &pAcl_cmd->filter_action, sizeof(act));

    ret_val = rtk_filter_igrAcl_cfg_add(phydev, pAcl_cmd->filter_id, &cfg, &act, &ruleNum);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error (id=%d, ruleNum=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ruleNum, ret_val);
        return ret_val;
    }
    return ret_val;
}

static int rtl83xx_aclrule_ctag_priority(struct phy_device *phydev, rtlsw_acl_rule_cmd_s *pAcl_cmd)
{
    rtk_filter_field_t      field1;
    rtk_filter_cfg_t        cfg;
    rtk_filter_action_t     act;
    rtk_filter_number_t     ruleNum;
    rtk_api_ret_t           ret_val;

    memset(&field1, 0, sizeof(rtk_filter_field_t));
    memset(&cfg,    0 , sizeof(cfg));
    memset(&act,    0 , sizeof(act));

    field1.fieldType = FILTER_FIELD_CTAG;
    field1.filter_pattern_union.ctag.pri.dataType   = FILTER_FIELD_DATA_MASK;
    field1.filter_pattern_union.ctag.pri.value      = pAcl_cmd->pattern_union.priority;
    field1.filter_pattern_union.ctag.pri.mask       = 0xffffffff;
    ret_val = rtk_filter_igrAcl_field_add(phydev, &cfg, &field1);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error 1(id=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ret_val);
        return ret_val;
    }

    /* apply on port UTP_PORT1 & UTP_PORT3 , care C-tag */
    cfg.careTag.tagType[CARE_TAG_CTAG].value = TRUE;
    cfg.careTag.tagType[CARE_TAG_CTAG].mask  = TRUE;
    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT1);
    RTK_PORTMASK_PORT_SET(cfg.activeport.value, UTP_PORT3);
    RTK_PORTMASK_ALLPORT_SET(cfg.activeport.mask);
    cfg.invert = FALSE;

    /* Action :  */
    memcpy(&act, &pAcl_cmd->filter_action, sizeof(act));

    ret_val = rtk_filter_igrAcl_cfg_add(phydev, pAcl_cmd->filter_id, &cfg, &act, &ruleNum);
    if( ret_val != RT_ERR_OK)
    {
        printk("%s,%d : ACL error (id=%d, ruleNum=%d) error id=%d\n", __FUNCTION__, __LINE__, pAcl_cmd->filter_id, ruleNum, ret_val);
        return ret_val;
    }
    return ret_val;
}


int rtl83xx_ioctl(struct phy_device *phydev, struct ifreq *ifr, int cmd)
{
    void            *useraddr = (void *)ifr->ifr_data;
    rtl83xx_cmd_s   rtl_cmd;
    rtk_port_t      port;

    if( cmd != SIOCRTLCOMMAND )
        return 0;

    if (copy_from_user(&rtl_cmd, useraddr, sizeof(rtl_cmd)))
        return -EFAULT;

    switch(rtl_cmd)
    {
        case RTLCMD_PHYID_GET:
            {
                rtlsw_phyid_s phyid;
                if (copy_from_user(&phyid, useraddr, sizeof(phyid)))
                    return -EFAULT;

                phyid.phyid = phydev->phy_id;
                if (copy_to_user(useraddr, &phyid, sizeof(phyid)))
                    return -EFAULT;
            }
            break;
        case RTLCMD_PHYSTATUS_GET:
            {
                phy_status_s status;
                if (copy_from_user(&status, useraddr, sizeof(status)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_port_phyStatus_get(phydev, status.Port,
                                                                &status.LinkStatus,
                                                                &status.Speed,
                                                                &status.Duplex))
                {
                    if (copy_to_user(useraddr, &status, sizeof(status)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_PHYAUTOABILITY_GET:
            {
                phy_ability_s ability;
                if (copy_from_user(&ability, useraddr, sizeof(ability)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_port_phyAutoNegoAbility_get(phydev, ability.Port, &ability.Ability))
                {
                    if (copy_to_user(useraddr, &ability, sizeof(ability)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_PHYAUTOABILITY_SET:
            {
                phy_ability_s ability;
                if (copy_from_user(&ability, useraddr, sizeof(ability)))
                    return -EFAULT;
                if( RT_ERR_OK != rtk_port_phyAutoNegoAbility_set(phydev, ability.Port, &ability.Ability))
                    return -EFAULT;
            }
            break;
        case RTLCMD_PHYFORCEABILITY_GET:
            {
                phy_ability_s ability;
                if (copy_from_user(&ability, useraddr, sizeof(ability)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_port_phyForceModeAbility_get(phydev, ability.Port, &ability.Ability))
                {
                    if (copy_to_user(useraddr, &ability, sizeof(ability)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_PHYFORCEABILITY_SET:
            {
                phy_ability_s ability;
                if (copy_from_user(&ability, useraddr, sizeof(ability)))
                    return -EFAULT;
                if( RT_ERR_OK != rtk_port_phyForceModeAbility_set(phydev, ability.Port, &ability.Ability))
                    return -EFAULT;
            }
            break;
        case RTLCMD_PHYADMIN_GET:
            {
                phy_admin_s admin;
                if (copy_from_user(&admin, useraddr, sizeof(admin)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_port_adminEnable_get(phydev, admin.Port, &admin.enabled))
                {
                    if (copy_to_user(useraddr, &admin, sizeof(admin)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_PHYADMIN_SET:
            {
                phy_admin_s admin;
                if (copy_from_user(&admin, useraddr, sizeof(admin)))
                    return -EFAULT;
                if( RT_ERR_OK != rtk_port_adminEnable_set(phydev, admin.Port, admin.enabled))
                    return -EFAULT;
            }
            break;
        case RTLCMD_TRAPRMAACTION_GET:
            {
                rtlsw_trap_ramAction_s trap;
                if (copy_from_user(&trap, useraddr, sizeof(trap)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_trap_rmaAction_get(phydev, trap.type, &trap.rma_action))
                {
                    if (copy_to_user(useraddr, &trap, sizeof(trap)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_TRAPRMAACTION_SET:
            {
                rtlsw_trap_ramAction_s trap;
                if (copy_from_user(&trap, useraddr, sizeof(trap)))
                    return -EFAULT;
                if( RT_ERR_OK != rtk_trap_rmaAction_set(phydev, trap.type, trap.rma_action))
                    return -EFAULT;
            }
            break;
        case RTLCMD_LLDPENABLED_GET:
            {
                rtlsw_enabled_s en;
                if (copy_from_user(&en, useraddr, sizeof(en)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_trap_lldpEnable_get(phydev, &en.enabled))
                {
                    if (copy_to_user(useraddr, &en, sizeof(en)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_LLDPENABLED_SET:
            {
                rtlsw_enabled_s en;
                if (copy_from_user(&en, useraddr, sizeof(en)))
                    return -EFAULT;

                if( RT_ERR_OK != rtk_trap_lldpEnable_set(phydev, en.enabled))
                    return -EFAULT;

                if( RT_ERR_OK != rtk_cpu_enable_set(phydev, en.enabled))
                    return -EFAULT;
            }
            break;
        case RTLCMD_CPUENABLED_GET:
            {
                rtlsw_enabled_s en;
                if (copy_from_user(&en, useraddr, sizeof(en)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_cpu_enable_get(phydev, &en.enabled))
                {
                    if (copy_to_user(useraddr, &en, sizeof(en)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_CPUENABLED_SET:
            {
                rtlsw_enabled_s en;
                if (copy_from_user(&en, useraddr, sizeof(en)))
                    return -EFAULT;

                if( RT_ERR_OK != rtk_cpu_enable_set(phydev, en.enabled))
                    return -EFAULT;
            }
            break;
        case RTLCMD_CPUTAGPORT_GET:
            {
                rtlsw_cpu_tagPort_s tagPort;
                if (copy_from_user(&tagPort, useraddr, sizeof(tagPort)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_cpu_tagPort_get(phydev, &tagPort.port, &tagPort.mode))
                {
                    if (copy_to_user(useraddr, &tagPort, sizeof(tagPort)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_CPUTAGPORT_SET:
            {
                rtlsw_cpu_tagPort_s tagPort;
                if (copy_from_user(&tagPort, useraddr, sizeof(tagPort)))
                    return -EFAULT;

                if( RT_ERR_OK != rtk_cpu_tagPort_set(phydev, tagPort.port, tagPort.mode))
                    return -EFAULT;
            }
            break;
        case RTLCMD_CPUAWAREPORT_SET:
            {
                rtlsw_cpu_tagPort_s tagPort;
                rtk_portmask_t      portmask;
                if (copy_from_user(&tagPort, useraddr, sizeof(tagPort)))
                    return -EFAULT;

                RTK_PORTMASK_PORT_SET(portmask, tagPort.port);
                if( RT_ERR_OK != rtk_cpu_awarePort_set(phydev, &portmask))
                    return -EFAULT;
            }
            break;
        case RTLCMD_PORTSTATALL_GET:
            {
                rtlsw_port_stat_s port_stat;
                if (copy_from_user(&port_stat, useraddr, sizeof(port_stat)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_stat_port_getAll(phydev, port_stat.port, &port_stat.port_cntr))
                {
                    if (copy_to_user(useraddr, &port_stat, sizeof(port_stat)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_PORTSTAT_GET:
            {
                rtlsw_port_stat_field_s port_field;
                if (copy_from_user(&port_field, useraddr, sizeof(port_field)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_stat_port_get(phydev, port_field.port, port_field.cntr_idx, &port_field.cntr))
                {
                    if (copy_to_user(useraddr, &port_field, sizeof(port_field)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_PORTSTAT_RESET:
            {
                rtlsw_port_stat_s stat_port;
                if (copy_from_user(&stat_port, useraddr, sizeof(stat_port)))
                    return -EFAULT;
                if ( RT_ERR_OK != rtk_stat_port_reset(phydev, stat_port.port))
                    return EFAULT;
            }
            break;
        case RTLCMD_STATALL_RESET:
            {
                rtlsw_enabled_s stat_en;
                if (copy_from_user(&stat_en, useraddr, sizeof(stat_en)))
                    return -EFAULT;
                if( RT_ERR_OK != rtk_stat_global_reset(phydev))
                    return -EFAULT;
            }
            break;
        case RTLCMD_QOSINIT_SET:
            {
                rtlsw_qnum_s q;
                if (copy_from_user(&q, useraddr, sizeof(q)))
                    return -EFAULT;

                if( RT_ERR_OK != rtk_qos_init(phydev, q.qnum))
                    return -EFAULT;
            }
            break;
        case RTLCMD_QOS1Q_SET:
            {
                if( RT_ERR_OK !=rtl8363_qos_set(phydev))
                    return -EFAULT;
            }
            break;
        case RTLCMD_IGRACL_SET:
            {
                rtlsw_enabled_acl_s en;
                if (copy_from_user(&en, useraddr, sizeof(en)))
                    return -EFAULT;

                if(en.enabled == ENABLED)
                {
                    if( RT_ERR_OK != rtk_filter_igrAcl_init(phydev))
                        return -EFAULT;
                }
                else
                {
                    if( RT_ERR_OK != rtk_filter_igrAcl_cfg_delAll(phydev))
                        return -EFAULT;

                    RTK_SCAN_ALL_PHY_PORTMASK(phydev, port)
                    {
                        (void) rtk_filter_igrAcl_state_set(phydev, port, DISABLED);
                    }
                }
            }
            break;
        case RTLCMD_ACLRULE_SET:
            {
                rtlsw_acl_rule_cmd_s acl_cmd;
                if (copy_from_user(&acl_cmd, useraddr, sizeof(acl_cmd)))
                    return -EFAULT;
                switch(acl_cmd.rule_cmd)
                {
                    case RTLSW_CMD_ACLRULE_DEL_ID:
                        if( RT_ERR_OK != rtk_filter_igrAcl_cfg_del(phydev, acl_cmd.filter_id))
                            return -EFAULT;
                        break;
                    case RTLSW_CMD_ACLRULE_DMAC_ETHTYPE:
                        if( RT_ERR_OK != rtl83xx_aclrule_dmac_ethtype(phydev, &acl_cmd))
                            return -EFAULT;
                        break;
                    case RTLSW_CMD_ACLRULE_DMAC_SMAC_ETHTYPE:
                        if( RT_ERR_OK != rtl83xx_aclrule_dmac_smac_ethtype(phydev, &acl_cmd))
                            return -EFAULT;
                        break;
                    case RTLSW_CMD_ACLRULE_DMAC:
                        if( RT_ERR_OK != rtl83xx_aclrule_dmac(phydev, &acl_cmd))
                            return -EFAULT;
                        break;
                    case RTLSW_CMD_ACLRULE_DMAC_TCPUDP_ALL:
                        if( RT_ERR_OK != rtl83xx_aclrule_dmac_tcpudp_all(phydev, &acl_cmd))
                            return -EFAULT;
                        break;
                    case RTLSW_CMD_ACLRULE_DMAC_SMAC:
                        if( RT_ERR_OK != rtl83xx_aclrule_dmac_smac(phydev, &acl_cmd))
                            return -EFAULT;
                        break;
                    case RTLSW_CMD_ACLRULE_CTAG_PRIORITY:
                        if( RT_ERR_OK != rtl83xx_aclrule_ctag_priority(phydev, &acl_cmd))
                            return -EFAULT;
                        break;
                    case RTLSW_CMD_ACLRULE_DMAC_UDP_DPORT:
                        if( RT_ERR_OK != rtl83xx_aclrule_dmac_udp_dport(phydev, &acl_cmd))
                            return -EFAULT;
                        break;
                    default:
                        return -EFAULT;
                }
            }
            break;
        case RTLCMD_L2FLOODINGPORTMASK_SET:
            {
                rtlsw_flooding_cmd_s flood_cmd;
                if (copy_from_user(&flood_cmd, useraddr, sizeof(flood_cmd)))
                    return -EFAULT;
                if( RT_ERR_OK != rtl8363_l2_floodPortMask_unknownda_set(phydev, flood_cmd.flood_type) )
                    return -EFAULT;
            }
            break;
        case RTLCMD_RATE_SHAREDMETER_GET:
            {
                rtlsw_sharedMeter_s shared;
                if (copy_from_user(&shared, useraddr, sizeof(shared)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_rate_shareMeter_get(phydev, shared.index, &shared.type, &shared.rate, &shared.ifg_include))
                {
                    if (copy_to_user(useraddr, &shared, sizeof(shared)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_RATE_SHAREDMETER_SET:
            {
                rtlsw_sharedMeter_s shared;
                if (copy_from_user(&shared, useraddr, sizeof(shared)))
                    return -EFAULT;
                if( RT_ERR_OK != rtk_rate_shareMeter_set(phydev, shared.index, shared.type, shared.rate, shared.ifg_include))
                    return -EFAULT;
            }
            break;
        case RTLCMD_RATE_SHAREDMETERBUCKET_GET:
            {
                rtlsw_sharedMeterBucket_s sharedBucket;
                if (copy_from_user(&sharedBucket, useraddr, sizeof(sharedBucket)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_rate_shareMeterBucket_get(phydev, sharedBucket.index, &sharedBucket.bucket_size))
                {
                    if (copy_to_user(useraddr, &sharedBucket, sizeof(sharedBucket)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_RATE_SHAREDMETERBUCKET_SET:
            {
                rtlsw_sharedMeterBucket_s sharedBucket;
                if (copy_from_user(&sharedBucket, useraddr, sizeof(sharedBucket)))
                    return -EFAULT;
                if( RT_ERR_OK != rtk_rate_shareMeterBucket_set(phydev, sharedBucket.index, sharedBucket.bucket_size))
                    return -EFAULT;
            }
            break;
        case RTLCMD_RATE_IGR_BANDWIDTH_CTRLRATE_GET:
            {
                rtlsw_rate_igr_bw_ctrlrate_s igrbwcr;
                if (copy_from_user(&igrbwcr, useraddr, sizeof(igrbwcr)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_rate_igrBandwidthCtrlRate_get(phydev, igrbwcr.port, &igrbwcr.rate, &igrbwcr.ifg_include, &igrbwcr.fc_enable))
                {
                    if (copy_to_user(useraddr, &igrbwcr, sizeof(igrbwcr)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_RATE_IGR_BANDWIDTH_CTRLRATE_SET:
            {
                rtlsw_rate_igr_bw_ctrlrate_s igrbwcr;
                if (copy_from_user(&igrbwcr, useraddr, sizeof(igrbwcr)))
                    return -EFAULT;
                if( RT_ERR_OK != rtk_rate_igrBandwidthCtrlRate_set(phydev, igrbwcr.port, igrbwcr.rate, igrbwcr.ifg_include, igrbwcr.fc_enable))
                    return -EFAULT;
            }
            break;
        case RTLCMD_RATE_EGR_BANDWIDTH_CTRLRATE_GET:
            {
                rtlsw_rate_egr_bw_ctrlrate_s egrbwcr;
                if (copy_from_user(&egrbwcr, useraddr, sizeof(egrbwcr)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_rate_egrBandwidthCtrlRate_get(phydev, egrbwcr.port, &egrbwcr.rate, &egrbwcr.ifg_include))
                {
                    if (copy_to_user(useraddr, &egrbwcr, sizeof(egrbwcr)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_RATE_EGR_BANDWIDTH_CTRLRATE_SET:
            {
                rtlsw_rate_egr_bw_ctrlrate_s egrbwcr;
                if (copy_from_user(&egrbwcr, useraddr, sizeof(egrbwcr)))
                    return -EFAULT;
                if( RT_ERR_OK != rtk_rate_egrBandwidthCtrlRate_set(phydev, egrbwcr.port, egrbwcr.rate, egrbwcr.ifg_include))
                    return -EFAULT;
            }
            break;
        case RTLCMD_RATE_EGR_QUEUEBW_CTRLENABLE_GET:
            {
                rtlsw_rate_egr_queuebw_ctrlenable_s egrbwcren;
                if (copy_from_user(&egrbwcren, useraddr, sizeof(egrbwcren)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_rate_egrQueueBwCtrlEnable_get(phydev, egrbwcren.port, egrbwcren.queue, &egrbwcren.enable))
                {
                    if (copy_to_user(useraddr, &egrbwcren, sizeof(egrbwcren)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_RATE_EGR_QUEUEBW_CTRLENABLE_SET:
            {
                rtlsw_rate_egr_queuebw_ctrlenable_s egrbwcren;
                if (copy_from_user(&egrbwcren, useraddr, sizeof(egrbwcren)))
                    return -EFAULT;
                if( RT_ERR_OK != rtk_rate_egrQueueBwCtrlEnable_set(phydev, egrbwcren.port, egrbwcren.queue, egrbwcren.enable))
                    return -EFAULT;
            }
            break;
        case RTLCMD_RATE_EGR_QUEUEBW_CTRLRATE_GET:
            {
                rtlsw_rate_egr_queuebw_ctrlrate_s egrbwcrrt;
                if (copy_from_user(&egrbwcrrt, useraddr, sizeof(egrbwcrrt)))
                    return -EFAULT;
                if( RT_ERR_OK == rtk_rate_egrQueueBwCtrlRate_get(phydev, egrbwcrrt.port, egrbwcrrt.queue, &egrbwcrrt.index))
                {
                    if (copy_to_user(useraddr, &egrbwcrrt, sizeof(egrbwcrrt)))
                        return -EFAULT;
                }
                else
                    return -EFAULT;
            }
            break;
        case RTLCMD_RATE_EGR_QUEUEBW_CTRLRATE_SET:
            {
                rtlsw_rate_egr_queuebw_ctrlrate_s egrbwcrrt;
                if (copy_from_user(&egrbwcrrt, useraddr, sizeof(egrbwcrrt)))
                    return -EFAULT;
                if( RT_ERR_OK != rtk_rate_egrQueueBwCtrlRate_set(phydev, egrbwcrrt.port, egrbwcrrt.queue, egrbwcrrt.index))
                    return -EFAULT;
            }
            break;
        default:
            return -EOPNOTSUPP;
    }
    return 0;
}

#ifdef USE_PROC
static int __init rtl83xx_ioctl_init(void)
{
#ifdef PROC_DIR
	mode_create_proc_parentdir();  
#endif //PROC_DIR
	mode_create_proc("mode");
	mode_data[0].test_port = 1;
	mode_data[0].test_mode = 0;
}

static void __exit rtl83xx_ioctl_exit(void)
{
	mode_remove_proc("mode");
#ifdef PROC_DIR
	mode_remove_proc_parentdir();
#endif //PROC_DIR
}

module_init(rtl83xx_ioctl_init);
module_exit(rtl83xx_ioctl_exit);
#endif //USE_PROC

EXPORT_SYMBOL(rtl83xx_ioctl);
