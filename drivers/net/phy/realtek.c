/*
 * drivers/net/phy/realtek.c
 *
 * Driver for Realtek PHYs
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
#include <linux/bitops.h>
#include <linux/phy.h>
#include <linux/module.h>

#define DEBUG_8211F

#define RTL821x_PHYSR				0x11
#define RTL821x_PHYSR_DUPLEX		BIT(13)
#define RTL821x_PHYSR_SPEED			GENMASK(15, 14)
#define RTL821x_INER				0x12
#define RTL821x_INSR				0x13
#define RTL821x_PAGE_SELECT			0x1f

#define RTL8211B_INER_INIT			0x6400

#define RTL8211E_INER_LINK_STATUS	BIT(10)

#define RTL8211F_INER				0x12
#ifndef DEBUG_8211F
#define RTL8211F_INSR				0x1d
#else
#define RTL8211F_INSR				0x13
#define RTL8211F_PHYSR				0x1A
#define RTL8211F_PHYSR_DUPLEX		BIT(3)
#define RTL8211F_PHYSR_SPEED		GENMASK(5, 4)
#endif
#define RTL8211F_INER_LINK_STATUS	BIT(4)
#define RTL8211F_TX_DELAY			BIT(8)	// Page 0xd08. Reg 17, Bit[8] = 1;

#define RTL8201F_ISR				0x1e
#define RTL8201F_IER				0x13

#define RTL8366RB_POWER_SAVE			0x15
#define RTL8366RB_POWER_SAVE_ON			BIT(12)

MODULE_DESCRIPTION("Realtek PHY driver");
MODULE_AUTHOR("Johnson Leung");
MODULE_LICENSE("GPL");

static int rtl821x_read_page(struct phy_device *phydev)
{
	return phy_read(phydev, RTL821x_PAGE_SELECT);
}

static int rtl821x_write_page(struct phy_device *phydev, int page)
{
	return phy_write(phydev, RTL821x_PAGE_SELECT, page);
}

static int rtl8201_ack_interrupt(struct phy_device *phydev)
{
	int err;

	err = phy_read(phydev, RTL8201F_ISR);

	return (err < 0) ? err : 0;
}

static int rtl821x_ack_interrupt(struct phy_device *phydev)
{
	int err;

	err = phy_read(phydev, RTL821x_INSR);

	return (err < 0) ? err : 0;
}

static int rtl8211f_ack_interrupt(struct phy_device *phydev)
{
	int err;

#ifndef DEBUG_8211F
	phy_write(phydev, RTL821x_PAGE_SELECT, 0xa43);
	err = phy_read(phydev, RTL8211F_INSR);
	phy_write(phydev, RTL821x_PAGE_SELECT, 0);
#else
	phy_write(phydev, RTL821x_PAGE_SELECT, 0);
	err = phy_read(phydev, RTL8211F_INSR);
#endif

	return (err < 0) ? err : 0;
}

static int rtl8201_config_intr(struct phy_device *phydev)
{
	int err;
	u16 val;

	if (phydev->interrupts == PHY_INTERRUPT_ENABLED)
		val = BIT(13) | BIT(12) | BIT(11);
	else
		val = 0;

	phy_write(phydev, RTL821x_PAGE_SELECT, 0x7);
	err = phy_write(phydev, RTL8201F_IER, val);
	phy_write(phydev, RTL821x_PAGE_SELECT, 0);

	return err;
}

static int rtl8211b_config_intr(struct phy_device *phydev)
{
	int err;

	if (phydev->interrupts == PHY_INTERRUPT_ENABLED)
		err = phy_write(phydev, RTL821x_INER,
				RTL8211B_INER_INIT);
	else
		err = phy_write(phydev, RTL821x_INER, 0);

	return err;
}

static int rtl8211e_config_intr(struct phy_device *phydev)
{
	int err;

	if (phydev->interrupts == PHY_INTERRUPT_ENABLED)
		err = phy_write(phydev, RTL821x_INER,
				RTL8211E_INER_LINK_STATUS);
	else
		err = phy_write(phydev, RTL821x_INER, 0);

	return err;
}

static int rtl8211f_config_intr(struct phy_device *phydev)
{
	int err;
	u16 val;

	if (phydev->interrupts == PHY_INTERRUPT_ENABLED)
		val = RTL8211F_INER_LINK_STATUS;
	else
		val = 0;

#ifndef DEBUG_8211F
	phy_write(phydev, RTL821x_PAGE_SELECT, 0xa42);
	err = phy_write(phydev, RTL821x_INSR, val);
	phy_write(phydev, RTL821x_PAGE_SELECT, 0);
#else
	phy_write(phydev, RTL821x_PAGE_SELECT, 0);
	err = phy_write(phydev, RTL821x_INSR, val);
#endif

	return err;
}

static int rtl8211_config_aneg(struct phy_device *phydev)
{
	int ret;

	ret = genphy_config_aneg(phydev);
	if (ret < 0)
		return ret;

	/* Quirk was copied from vendor driver. Unfortunately it includes no
	 * description of the magic numbers.
	 */
	if (phydev->speed == SPEED_100 && phydev->autoneg == AUTONEG_DISABLE) {
		phy_write(phydev, 0x17, 0x2138);
		phy_write(phydev, 0x0e, 0x0260);
	} else {
		phy_write(phydev, 0x17, 0x2108);
		phy_write(phydev, 0x0e, 0x0000);
	}

	return 0;
}
#if 0
static int rtl8211c_config_init(struct phy_device *phydev)
{
	/* RTL8211C has an issue when operating in Gigabit slave mode */
	phy_set_bits(phydev, MII_CTRL1000,
		     CTL1000_ENABLE_MASTER | CTL1000_AS_MASTER);

	return genphy_config_init(phydev);
}
#endif
static int rtl8211e_config_init(struct phy_device *phydev)
{
	int ret;
	int txdelay;
	u16 val = 0;

	ret = genphy_config_init(phydev);
	if (ret < 0)
		return ret;

	phy_write(phydev, RTL821x_PAGE_SELECT, 0x7);
	phy_write(phydev, 0x1e, 0x2c);
	phy_write(phydev, 0x1c, 0x742);
	phy_write(phydev, 0x1a, 0x40);

	/* restore to default page 0 */
	phy_write(phydev, RTL821x_PAGE_SELECT, 0);

	return ret;
}

static int rtl8211f_config_init(struct phy_device *phydev)
{
	int ret;
	int txdelay;
	u16 val = 0;

	ret = genphy_config_init(phydev);
	if (ret < 0)
		return ret;

	val = RTL8211F_TX_DELAY;
	phy_write(phydev, RTL821x_PAGE_SELECT, 0xd08);
	txdelay = phy_read(phydev, 0x11);
	phy_write(phydev, 0x11, (txdelay & ~RTL8211F_TX_DELAY) | val);

	/* restore to default page 0 */
	phy_write(phydev, RTL821x_PAGE_SELECT, 0);

	/* disable EEE green ethernet */
	phy_write(phydev, 0x1B, 0x8011);
	phy_write(phydev, 0x1C, 0x573f);

	return ret;
}

static int rtl8211b_suspend(struct phy_device *phydev)
{
	phy_write(phydev, MII_MMD_DATA, BIT(9));

	return genphy_suspend(phydev);
}

static int rtl8211b_resume(struct phy_device *phydev)
{
	phy_write(phydev, MII_MMD_DATA, 0);

	return genphy_resume(phydev);
}

static int rtl8211f_suspend(struct phy_device *phydev)
{
	printk("8211f can not support suspend! \n");
	return 0;
}

static int rtl8211f_resume(struct phy_device *phydev)
{
	printk("8211f can not support resume! \n");
	return 0;
}

#if 0
static int rtl8366rb_config_init(struct phy_device *phydev)
{
	int ret;

	ret = genphy_config_init(phydev);
	if (ret < 0)
		return ret;

	ret = phy_set_bits(phydev, RTL8366RB_POWER_SAVE,
			   RTL8366RB_POWER_SAVE_ON);
	if (ret) {
		dev_err(&phydev->mdio.dev,
			"error enabling power management\n");
	}

	return ret;
}
#endif
static struct phy_driver realtek_drvs[] = {
	{
		.phy_id         = 0x00008201,
		.name           = "RTL8201CP Ethernet",
		.phy_id_mask    = 0x0000ffff,
		.features       = PHY_BASIC_FEATURES,
		.flags          = PHY_HAS_INTERRUPT,
		.config_aneg    = &genphy_config_aneg,
		.read_status    = &genphy_read_status,
		.driver         = { .owner = THIS_MODULE,},
	}, {
		.phy_id		= 0x001cc816,
		.name		= "RTL8201F Fast Ethernet",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_BASIC_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.ack_interrupt	= &rtl8201_ack_interrupt,
		.config_intr	= &rtl8201_config_intr,
		.suspend	= genphy_suspend,
		.resume		= genphy_resume,
		//.read_page	= rtl821x_read_page,
		//.write_page	= rtl821x_write_page,
		.config_aneg    = &genphy_config_aneg,
		.read_status    = &genphy_read_status,
		.driver         = { .owner = THIS_MODULE,},
	}, {
		.phy_id		= 0x001cc910,
		.name		= "RTL8211 Gigabit Ethernet",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_GBIT_FEATURES,
		.config_aneg	= rtl8211_config_aneg,
		//.read_mmd	= &genphy_read_mmd_unsupported,
		//.write_mmd	= &genphy_write_mmd_unsupported,
		.read_status    = &genphy_read_status,
		.driver         = { .owner = THIS_MODULE,},
	}, {
		.phy_id		= 0x001cc912,
		.name		= "RTL8211B Gigabit Ethernet",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_GBIT_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.config_aneg	= &genphy_config_aneg,
		.read_status	= &genphy_read_status,
		.ack_interrupt	= &rtl821x_ack_interrupt,
		.config_intr	= &rtl8211b_config_intr,
		.driver		= { .owner = THIS_MODULE,},
		//.read_mmd	= &genphy_read_mmd_unsupported,
		//.write_mmd	= &genphy_write_mmd_unsupported,
		.suspend	= rtl8211b_suspend,
		.resume		= rtl8211b_resume,
	}, {
		.phy_id		= 0x001cc913,
		.name		= "RTL8211C Gigabit Ethernet",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_GBIT_FEATURES,
		//.config_init	= rtl8211c_config_init,
		//.read_mmd	= &genphy_read_mmd_unsupported,
		//.write_mmd	= &genphy_write_mmd_unsupported,
		.config_aneg    = &genphy_config_aneg,
		.read_status    = &genphy_read_status,
		.driver         = { .owner = THIS_MODULE,},
	}, {
		.phy_id		= 0x001cc914,
		.name		= "RTL8211DN Gigabit Ethernet",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_GBIT_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.ack_interrupt	= rtl821x_ack_interrupt,
		.config_intr	= rtl8211e_config_intr,
		.suspend	= genphy_suspend,
		.resume		= genphy_resume,
		.config_aneg    = &genphy_config_aneg,
		.read_status    = &genphy_read_status,
		.driver         = { .owner = THIS_MODULE,},
	}, {
		.phy_id		= 0x001cc915,
		.name		= "RTL8211E Gigabit Ethernet",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_GBIT_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.config_aneg	= &genphy_config_aneg,
		.config_init	= &rtl8211e_config_init,
		.read_status	= &genphy_read_status,
		.ack_interrupt	= &rtl821x_ack_interrupt,
		.config_intr	= &rtl8211e_config_intr,
		.suspend	= genphy_suspend,
		.resume		= genphy_resume,
		.driver		= { .owner = THIS_MODULE,},
	}, {
		.phy_id		= 0x001cc916,
		.name		= "RTL8211F Gigabit Ethernet",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_GBIT_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.config_aneg	= &genphy_config_aneg,
		//.config_aneg	= rtl8211_config_aneg,
		.config_init	= &rtl8211f_config_init,
		.read_status	= &genphy_read_status,
		.ack_interrupt	= &rtl8211f_ack_interrupt,
		.config_intr	= &rtl8211f_config_intr,
		.suspend	= genphy_suspend,
		.resume		= genphy_resume,
		//.suspend	= rtl8211f_suspend,
		//.resume		= rtl8211f_resume,
		.driver		= { .owner = THIS_MODULE },
		//.read_page	= rtl821x_read_page,
		//.write_page	= rtl821x_write_page,
	}, {
		.phy_id		= 0x001cc961,
		.name		= "RTL8366RB Gigabit Ethernet",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_GBIT_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		//.config_init	= &rtl8366rb_config_init,
		.suspend	= genphy_suspend,
		.resume		= genphy_resume,
		.config_aneg    = &genphy_config_aneg,
		.read_status    = &genphy_read_status,
		.driver         = { .owner = THIS_MODULE,},
	},
};

module_phy_driver(realtek_drvs);

static struct mdio_device_id __maybe_unused realtek_tbl[] = {
	{ 0x001cc816, 0x001fffff },
	{ 0x001cc910, 0x001fffff },
	{ 0x001cc912, 0x001fffff },
	{ 0x001cc913, 0x001fffff },
	{ 0x001cc914, 0x001fffff },
	{ 0x001cc915, 0x001fffff },
	{ 0x001cc916, 0x001fffff },
	{ 0x001cc961, 0x001fffff },
	{ }
};

MODULE_DEVICE_TABLE(mdio, realtek_tbl);