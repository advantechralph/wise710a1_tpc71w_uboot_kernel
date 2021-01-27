/*
 * Copyright 2011-2015 Freescale Semiconductor, Inc.
 * Copyright 2011 Linaro Ltd.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/cpu.h>
#include <linux/delay.h>
#include <linux/export.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/irqchip.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/pm_opp.h>
#include <linux/pci.h>
#include <linux/phy.h>
#include <linux/reboot.h>
#include <linux/regmap.h>
#include <linux/micrel_phy.h>
#include <linux/mfd/syscon.h>
#include <linux/mfd/syscon/imx6q-iomuxc-gpr.h>
#include <linux/of_net.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/system_misc.h>
#ifdef CONFIG_MX6_CLK_FOR_BOOTUI_TRANS
#include <linux/memblock.h>
#endif

#include "common.h"
#include "cpuidle.h"
#include "hardware.h"

/* For imx6q sabrelite board: set KSZ9021RN RGMII pad skew */
static int ksz9021rn_phy_fixup(struct phy_device *phydev)
{
	if (IS_BUILTIN(CONFIG_PHYLIB)) {
		/* min rx data delay */
		phy_write(phydev, MICREL_KSZ9021_EXTREG_CTRL,
			0x8000 | MICREL_KSZ9021_RGMII_RX_DATA_PAD_SCEW);
		phy_write(phydev, MICREL_KSZ9021_EXTREG_DATA_WRITE, 0x0000);

		/* max rx/tx clock delay, min rx/tx control delay */
		phy_write(phydev, MICREL_KSZ9021_EXTREG_CTRL,
			0x8000 | MICREL_KSZ9021_RGMII_CLK_CTRL_PAD_SCEW);
		phy_write(phydev, MICREL_KSZ9021_EXTREG_DATA_WRITE, 0xf0f0);
		phy_write(phydev, MICREL_KSZ9021_EXTREG_CTRL,
			MICREL_KSZ9021_RGMII_CLK_CTRL_PAD_SCEW);
	}

	return 0;
}

static void mmd_write_reg(struct phy_device *dev, int device, int reg, int val)
{
	phy_write(dev, 0x0d, device);
	phy_write(dev, 0x0e, reg);
	phy_write(dev, 0x0d, (1 << 14) | device);
	phy_write(dev, 0x0e, val);
}

static int ksz9031rn_phy_fixup(struct phy_device *dev)
{
	/*
	 * min rx data delay, max rx/tx clock delay,
	 * min rx/tx control delay
	 */
	mmd_write_reg(dev, 2, 4, 0);
	mmd_write_reg(dev, 2, 5, 0);
	mmd_write_reg(dev, 2, 8, 0x003ff);

	return 0;
}

/*
 * fixup for PLX PEX8909 bridge to configure GPIO1-7 as output High
 * as they are used for slots1-7 PERST#
 */
static void ventana_pciesw_early_fixup(struct pci_dev *dev)
{
	u32 dw;

	if (!of_machine_is_compatible("gw,ventana"))
		return;

	if (dev->devfn != 0)
		return;

	pci_read_config_dword(dev, 0x62c, &dw);
	dw |= 0xaaa8; // GPIO1-7 outputs
	pci_write_config_dword(dev, 0x62c, dw);

	pci_read_config_dword(dev, 0x644, &dw);
	dw |= 0xfe;   // GPIO1-7 output high
	pci_write_config_dword(dev, 0x644, dw);

	msleep(100);
}
DECLARE_PCI_FIXUP_EARLY(PCI_VENDOR_ID_PLX, 0x8609, ventana_pciesw_early_fixup);
DECLARE_PCI_FIXUP_EARLY(PCI_VENDOR_ID_PLX, 0x8606, ventana_pciesw_early_fixup);
DECLARE_PCI_FIXUP_EARLY(PCI_VENDOR_ID_PLX, 0x8604, ventana_pciesw_early_fixup);

static int ar8031_phy_fixup(struct phy_device *dev)
{
	u16 val;

#ifndef CONFIG_ARCH_ADVANTECH
	/* Set RGMII IO voltage to 1.8V */
	phy_write(dev, 0x1d, 0x1f);
	phy_write(dev, 0x1e, 0x8);
#endif

	/* disable phy AR8031 SmartEEE function. */
	phy_write(dev, 0xd, 0x3);
	phy_write(dev, 0xe, 0x805d);
	phy_write(dev, 0xd, 0x4003);
	val = phy_read(dev, 0xe);
	val &= ~(0x1 << 8);
	phy_write(dev, 0xe, val);

	/* To enable AR8031 output a 125MHz clk from CLK_25M */
	phy_write(dev, 0xd, 0x7);
	phy_write(dev, 0xe, 0x8016);
	phy_write(dev, 0xd, 0x4007);

	val = phy_read(dev, 0xe);
	val &= 0xffe3;
	val |= 0x18;
	phy_write(dev, 0xe, val);

	/* introduce tx clock delay */
	phy_write(dev, 0x1d, 0x5);
	val = phy_read(dev, 0x1e);
	val |= 0x0100;
	phy_write(dev, 0x1e, val);

	return 0;
}

#define PHY_ID_AR8031	0x004dd074

#if defined(CONFIG_ARCH_ADVANTECH) && defined(CONFIG_REALTEK_PHY)
static int rtl8211e_phy_fixup(struct phy_device *dev)
{
	//pr_warn("%s:%d \n", __func__, __LINE__);
	/*PHY LED OK*/
	phy_write(dev, 0x1f, 0x0007);
	phy_write(dev, 0x1e, 0x002c);
	//phy_write(dev, 0x1c, 0x0742);
	phy_write(dev, 0x1c, 0x0427);
	phy_write(dev, 0x1a, 0x0010);
	phy_write(dev, 0x1f, 0x0000);

	phy_write(dev, 0x1f, 0x0005);
	phy_write(dev, 0x05, 0x8b82);
	phy_write(dev, 0x06, 0x052b);
	phy_write(dev, 0x1f, 0x0000);

	return 0;
}

static int rtl8211f_phy_fixup(struct phy_device *dev)
{
	//pr_warn("%s:%d \n", __func__, __LINE__);
	u16 val;

	// PHY WORK LED 
	// LED2: 1000M,100M,10M Active
	// LED1: 100M LINK
	// LED0: 1000M LINK
	// EEE LED0/1/2 off
	phy_write(dev, 0x1f, 0xd04);
	phy_write(dev, 0x10, 0x6c48);
	phy_write(dev, 0x11, 0x0);
	phy_write(dev, 0x1f, 0x0);

	// Disable Green Ethernet
	phy_write(dev, 0x1b, 0x8011);
	phy_write(dev, 0x1c, 0x573f);
	// Enable Green Ethernet
	//phy_write(dev, 0x1b, 0x8011);
	//phy_write(dev, 0x10, 0xd73f);

#if 0
	// Read all register
	phy_write(dev, 0x1f, 0x0);
	val = phy_read(dev, 0x0);pr_warn("[%s:%d] 0x0->0[BMCR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x1);pr_warn("[%s:%d] 0x0->1[BMSR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x2);pr_warn("[%s:%d] 0x0->2[PHYID1]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x3);pr_warn("[%s:%d] 0x0->3[PHYID2]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x4);pr_warn("[%s:%d] 0x0->4[ANAR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x5);pr_warn("[%s:%d] 0x0->5[ANLPAR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x6);pr_warn("[%s:%d] 0x0->6[ANER]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x7);pr_warn("[%s:%d] 0x0->7[ANNPTR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x8);pr_warn("[%s:%d] 0x0->8[ANNPRR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x9);pr_warn("[%s:%d] 0x0->9[GBCR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0xa);pr_warn("[%s:%d] 0x0->a[GBSR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0xd);pr_warn("[%s:%d] 0x0->d[MACR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0xe);pr_warn("[%s:%d] 0x0->e[MAADR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0xf);pr_warn("[%s:%d] 0x0->f[GBESR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x12);pr_warn("[%s:%d] 0x0->12[INER]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x18);pr_warn("[%s:%d] 0x0->18[PHYCR1]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x19);pr_warn("[%s:%d] 0x0->19[PHYCR2]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x1a);pr_warn("[%s:%d] 0x0->1a[PHYSR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x13);pr_warn("[%s:%d] 0x0->13[INSR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x1f);pr_warn("[%s:%d] 0x0->1f[EPAGSR]=0x%X \n", __func__, __LINE__, val);
	phy_write(dev, 0x1f, 0xd04);
	val = phy_read(dev, 0x10);pr_warn("[%s:%d] 0xd04->10[LCR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x11);pr_warn("[%s:%d] 0xd04->11[EEELCR]=0x%X \n", __func__, __LINE__, val);
	phy_write(dev, 0x1f, 0xe40);
	val = phy_read(dev, 0x10);pr_warn("[%s:%d] 0xe40->10[PTP_CTL]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x11);pr_warn("[%s:%d] 0xe40->11[PTP_INER]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x12);pr_warn("[%s:%d] 0xe40->12[PTP_INSR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x13);pr_warn("[%s:%d] 0xe40->13[SYNCE_CTL]=0x%X \n", __func__, __LINE__, val);
	phy_write(dev, 0x1f, 0xe41);
	val = phy_read(dev, 0x10);pr_warn("[%s:%d] 0xe41->10[PTP_CTL_CFG]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x11);pr_warn("[%s:%d] 0xe41->11[PTP_CFG_NS_LO]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x12);pr_warn("[%s:%d] 0xe41->12[PTP_CFG_NS_HI]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x13);pr_warn("[%s:%d] 0xe41->13[PTP_CFG_S_LO]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x14);pr_warn("[%s:%d] 0xe41->14[PTP_CFG_S_MI]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x15);pr_warn("[%s:%d] 0xe41->15[PTP_CFG_S_HI]=0x%X \n", __func__, __LINE__, val);
	phy_write(dev, 0x1f, 0xe42);
	val = phy_read(dev, 0x10);pr_warn("[%s:%d] 0xe42->10[PTP_TAI_CFG]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x11);pr_warn("[%s:%d] 0xe42->11[PTP_TRIG_CFG]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x12);pr_warn("[%s:%d] 0xe42->12[PTP_TAI_STA]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x13);pr_warn("[%s:%d] 0xe42->13[PTP_TAI_TS_NS_LO]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x14);pr_warn("[%s:%d] 0xe42->14[PTP_TAI_TS_NS_HI]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x15);pr_warn("[%s:%d] 0xe42->15[PTP_TAI_TS_S_LO]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x16);pr_warn("[%s:%d] 0xe42->16[PTP_TAI_TS_S_HI]=0x%X \n", __func__, __LINE__, val);
	phy_write(dev, 0x1f, 0xe43);
	val = phy_read(dev, 0x10);pr_warn("[%s:%d] 0xe43->10[PTP_TRX_TS_STA]=0x%X \n", __func__, __LINE__, val);
	phy_write(dev, 0x1f, 0xe44);
	val = phy_read(dev, 0x10);pr_warn("[%s:%d] 0xe44->10[PTP_TRX_TS_INFO]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x11);pr_warn("[%s:%d] 0xe44->11[PTP_TRX_TS_SH]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x12);pr_warn("[%s:%d] 0xe44->12[PTP_TRX_TS_SID]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x13);pr_warn("[%s:%d] 0xe44->13[PTP_TRX_TSNS_LO]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x14);pr_warn("[%s:%d] 0xe44->14[PTP_TRX_TSNS_HI]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x15);pr_warn("[%s:%d] 0xe44->15[PTP_TRX_TSS_LO]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x16);pr_warn("[%s:%d] 0xe44->16[PTP_TRX_TSS_MI]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x17);pr_warn("[%s:%d] 0xe44->17[PTP_TRX_TSS_HI]=0x%X \n", __func__, __LINE__, val);
	phy_write(dev, 0x1f, 0xde1);
	val = phy_read(dev, 0x11);pr_warn("[%s:%d] 0xde1->11[SERDES INER]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x12);pr_warn("[%s:%d] 0xde1->12[SERDES INSR]=0x%X \n", __func__, __LINE__, val);
	phy_write(dev, 0x1f, 0xd08);
	val = phy_read(dev, 0x14);pr_warn("[%s:%d] 0xd08->14[SGMII ANARSEL]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x10);pr_warn("[%s:%d] 0xd08->10[SGMII ANAR]=0x%X \n", __func__, __LINE__, val);
	val = phy_read(dev, 0x17);pr_warn("[%s:%d] 0xd08->17[???]=0x%X \n", __func__, __LINE__, val);
#endif
	
	phy_write(dev, 0x1f, 0x0);
	return 0;
}

#define PHY_ID_REALTEK_8211E	0x001cc915
#define PHY_ID_REALTEK_8211F	0x001cc916
#define REALTEK_PHY_ID_MASK 	0x001fffff
#endif

static int ar8035_phy_fixup(struct phy_device *dev)
{
	u16 val;

	/* Ar803x phy SmartEEE feature cause link status generates glitch,
	 * which cause ethernet link down/up issue, so disable SmartEEE
	 */
	phy_write(dev, 0xd, 0x3);
	phy_write(dev, 0xe, 0x805d);
	phy_write(dev, 0xd, 0x4003);

	val = phy_read(dev, 0xe);
	phy_write(dev, 0xe, val & ~(1 << 8));

	/*
	 * Enable 125MHz clock from CLK_25M on the AR8031.  This
	 * is fed in to the IMX6 on the ENET_REF_CLK (V22) pad.
	 * Also, introduce a tx clock delay.
	 *
	 * This is the same as is the AR8031 fixup.
	 */
	ar8031_phy_fixup(dev);

	/*check phy power*/
	val = phy_read(dev, 0x0);
	if (val & BMCR_PDOWN)
		phy_write(dev, 0x0, val & ~BMCR_PDOWN);

	return 0;
}

#define PHY_ID_AR8035 0x004dd072

static void __init imx6q_enet_phy_init(void)
{
	if (IS_BUILTIN(CONFIG_PHYLIB)) {
		phy_register_fixup_for_uid(PHY_ID_KSZ9021, MICREL_PHY_ID_MASK,
				ksz9021rn_phy_fixup);
		phy_register_fixup_for_uid(PHY_ID_KSZ9031, MICREL_PHY_ID_MASK,
				ksz9031rn_phy_fixup);
		phy_register_fixup_for_uid(PHY_ID_AR8031, 0xffffffff,
				ar8031_phy_fixup);
		phy_register_fixup_for_uid(PHY_ID_AR8035, 0xffffffef,
				ar8035_phy_fixup);
#if defined(CONFIG_ARCH_ADVANTECH) && defined(CONFIG_REALTEK_PHY)
		phy_register_fixup_for_uid(PHY_ID_REALTEK_8211E, 0xffffffff,
				rtl8211e_phy_fixup);
		phy_register_fixup_for_uid(PHY_ID_REALTEK_8211F, 0xffffffff,
				rtl8211f_phy_fixup);
#endif
	}
}

static void __init imx6q_1588_init(void)
{
	struct device_node *np;
	struct clk *ptp_clk;
	struct regmap *gpr;

	np = of_find_compatible_node(NULL, NULL, "fsl,imx6q-fec");
	if (!np) {
		pr_warn("%s: failed to find fec node\n", __func__);
		return;
	}

	ptp_clk = of_clk_get(np, 2);
	if (IS_ERR(ptp_clk)) {
		pr_warn("%s: failed to get ptp clock\n", __func__);
		goto put_node;
	}

	/*
	 * If enet_ref from ANATOP/CCM is the PTP clock source, we need to
	 * set bit IOMUXC_GPR1[21].  Or the PTP clock must be from pad
	 * (external OSC), and we need to clear the bit.
	 */
	gpr = syscon_regmap_lookup_by_compatible("fsl,imx6q-iomuxc-gpr");
	if (!IS_ERR(gpr))
		regmap_update_bits(gpr, IOMUXC_GPR1,
				IMX6Q_GPR1_ENET_CLK_SEL_MASK,
				IMX6Q_GPR1_ENET_CLK_SEL_ANATOP);
	else
		pr_err("failed to find fsl,imx6q-iomux-gpr regmap\n");

	clk_put(ptp_clk);
put_node:
	of_node_put(np);
}

static void __init imx6q_csi_mux_init(void)
{
	/*
	 * MX6Q SabreSD board:
	 * IPU1 CSI0 connects to parallel interface.
	 * Set GPR1 bit 19 to 0x1.
	 *
	 * MX6DL SabreSD board:
	 * IPU1 CSI0 connects to parallel interface.
	 * Set GPR13 bit 0-2 to 0x4.
	 * IPU1 CSI1 connects to MIPI CSI2 virtual channel 1.
	 * Set GPR13 bit 3-5 to 0x1.
	 */
	struct regmap *gpr;

	gpr = syscon_regmap_lookup_by_compatible("fsl,imx6q-iomuxc-gpr");
	if (!IS_ERR(gpr)) {
#ifdef CONFIG_MX6_CLK_FOR_BOOTUI_TRANS
		if (of_machine_is_compatible("fsl,imx6q-sabresd")) {
			regmap_update_bits(gpr, IOMUXC_GPR1, 3 << 19, 0 << 19);
		} else if (of_machine_is_compatible("fsl,imx6q-sabreauto")) {
			regmap_update_bits(gpr, IOMUXC_GPR1, 3 << 19, 1 << 19);
		} else if (of_machine_is_compatible("fsl,imx6dl-sabresd") ||
			 of_machine_is_compatible("fsl,imx6dl-sabreauto"))
			regmap_update_bits(gpr, IOMUXC_GPR13, 0x3F, 0x1C);
#else
		if (of_machine_is_compatible("fsl,imx6q-sabresd") ||
			of_machine_is_compatible("fsl,imx6q-sabreauto"))
			regmap_update_bits(gpr, IOMUXC_GPR1, 1 << 19, 1 << 19);
		else if (of_machine_is_compatible("fsl,imx6dl-sabresd") ||
			 of_machine_is_compatible("fsl,imx6dl-sabreauto"))
			regmap_update_bits(gpr, IOMUXC_GPR13, 0x3F, 0x0C);
#endif
	} else {
		pr_err("%s(): failed to find fsl,imx6q-iomux-gpr regmap\n",
		       __func__);
	}
}

static void __init imx6q_axi_init(void)
{
	struct regmap *gpr;
	unsigned int mask;

	gpr = syscon_regmap_lookup_by_compatible("fsl,imx6q-iomuxc-gpr");
	if (!IS_ERR(gpr)) {
		/*
		 * Enable the cacheable attribute of VPU and IPU
		 * AXI transactions.
		 */
		mask = IMX6Q_GPR4_VPU_WR_CACHE_SEL |
			IMX6Q_GPR4_VPU_RD_CACHE_SEL |
			IMX6Q_GPR4_VPU_P_WR_CACHE_VAL |
			IMX6Q_GPR4_VPU_P_RD_CACHE_VAL_MASK |
			IMX6Q_GPR4_IPU_WR_CACHE_CTL |
			IMX6Q_GPR4_IPU_RD_CACHE_CTL;
		regmap_update_bits(gpr, IOMUXC_GPR4, mask, mask);

		/* Increase IPU read QoS priority */
		regmap_update_bits(gpr, IOMUXC_GPR6,
				IMX6Q_GPR6_IPU1_ID00_RD_QOS_MASK |
				IMX6Q_GPR6_IPU1_ID01_RD_QOS_MASK,
				(0xf << 16) | (0x7 << 20));
		regmap_update_bits(gpr, IOMUXC_GPR7,
				IMX6Q_GPR7_IPU2_ID00_RD_QOS_MASK |
				IMX6Q_GPR7_IPU2_ID01_RD_QOS_MASK,
				(0xf << 16) | (0x7 << 20));
	} else {
		pr_warn("failed to find fsl,imx6q-iomuxc-gpr regmap\n");
	}
}

static void __init imx6q_enet_clk_sel(void)
{
	struct regmap *gpr;

	gpr = syscon_regmap_lookup_by_compatible("fsl,imx6q-iomuxc-gpr");
	if (!IS_ERR(gpr))
		regmap_update_bits(gpr, IOMUXC_GPR5,
				   IMX6Q_GPR5_ENET_TX_CLK_SEL, IMX6Q_GPR5_ENET_TX_CLK_SEL);
	else
		pr_err("failed to find fsl,imx6q-iomux-gpr regmap\n");
}

static inline void imx6q_enet_init(void)
{
	imx6_enet_mac_init("fsl,imx6q-fec", "fsl,imx6q-ocotp");
	imx6q_enet_phy_init();
	imx6q_1588_init();
	if (cpu_is_imx6q() && imx_get_soc_revision() == IMX_CHIP_REVISION_2_0)
		imx6q_enet_clk_sel();
}

static void __init imx6q_init_machine(void)
{
	struct device *parent;

	if (cpu_is_imx6q() && imx_get_soc_revision() == IMX_CHIP_REVISION_2_0)
		imx_print_silicon_rev("i.MX6QP", IMX_CHIP_REVISION_1_0);
	else
		imx_print_silicon_rev(cpu_is_imx6dl() ? "i.MX6DL" : "i.MX6Q",
				 imx_get_soc_revision());

	parent = imx_soc_device_init();
	if (parent == NULL)
		pr_warn("failed to initialize soc device\n");

	of_platform_populate(NULL, of_default_bus_match_table, NULL, parent);

	imx6q_enet_init();
	imx_anatop_init();
	imx6q_csi_mux_init();
	cpu_is_imx6q() ?  imx6q_pm_init() : imx6dl_pm_init();
	imx6q_axi_init();
}

#define OCOTP_CFG3			0x440
#define OCOTP_CFG3_SPEED_SHIFT		16
#define OCOTP_CFG3_SPEED_1P2GHZ		0x3
#define OCOTP_CFG3_SPEED_996MHZ		0x2
#define OCOTP_CFG3_SPEED_852MHZ		0x1

static void __init imx6q_opp_check_speed_grading(struct device *cpu_dev)
{
	struct device_node *np;
	void __iomem *base;
	u32 val;

	np = of_find_compatible_node(NULL, NULL, "fsl,imx6q-ocotp");
	if (!np) {
		pr_warn("failed to find ocotp node\n");
		return;
	}

	base = of_iomap(np, 0);
	if (!base) {
		pr_warn("failed to map ocotp\n");
		goto put_node;
	}

	/*
	 * SPEED_GRADING[1:0] defines the max speed of ARM:
	 * 2b'11: 1200000000Hz;
	 * 2b'10: 996000000Hz;
	 * 2b'01: 852000000Hz; -- i.MX6Q Only, exclusive with 996MHz.
	 * 2b'00: 792000000Hz;
	 * We need to set the max speed of ARM according to fuse map.
	 */
	val = readl_relaxed(base + OCOTP_CFG3);
	val >>= OCOTP_CFG3_SPEED_SHIFT;
	val &= 0x3;

	if ((val != OCOTP_CFG3_SPEED_1P2GHZ) && cpu_is_imx6q())
		if (dev_pm_opp_disable(cpu_dev, 1200000000))
			pr_warn("failed to disable 1.2 GHz OPP\n");
	if (val < OCOTP_CFG3_SPEED_996MHZ)
		if (dev_pm_opp_disable(cpu_dev, 996000000))
			pr_warn("failed to disable 996 MHz OPP\n");
	if (cpu_is_imx6q()) {
		if (val != OCOTP_CFG3_SPEED_852MHZ)
			if (dev_pm_opp_disable(cpu_dev, 852000000))
				pr_warn("failed to disable 852 MHz OPP\n");
	}
	iounmap(base);

	if (IS_ENABLED(CONFIG_MX6_VPU_352M)) {
		if (dev_pm_opp_disable(cpu_dev, 396000000))
			pr_warn("failed to disable 396MHz OPP\n");
		pr_info("remove 396MHz OPP for VPU running at 352MHz!\n");
	}

put_node:
	of_node_put(np);
}

static void __init imx6q_opp_init(void)
{
	struct device_node *np;
	struct device *cpu_dev = get_cpu_device(0);

	if (!cpu_dev) {
		pr_warn("failed to get cpu0 device\n");
		return;
	}
	np = of_node_get(cpu_dev->of_node);
	if (!np) {
		pr_warn("failed to find cpu0 node\n");
		return;
	}

	if (of_init_opp_table(cpu_dev)) {
		pr_warn("failed to init OPP table\n");
		goto put_node;
	}

	imx6q_opp_check_speed_grading(cpu_dev);

put_node:
	of_node_put(np);
}

static struct platform_device imx6q_cpufreq_pdev = {
	.name = "imx6q-cpufreq",
};

static void __init imx6q_init_late(void)
{
	/*
	 * WAIT mode is broken on TO 1.0 and 1.1, so there is no point
	 * to run cpuidle on them.
	 */
	if ((cpu_is_imx6q() && imx_get_soc_revision() > IMX_CHIP_REVISION_1_1)
		|| (cpu_is_imx6dl() && imx_get_soc_revision() >
		IMX_CHIP_REVISION_1_0))
		imx6q_cpuidle_init();

	if (IS_ENABLED(CONFIG_ARM_IMX6Q_CPUFREQ)) {
		imx6q_opp_init();
		platform_device_register(&imx6q_cpufreq_pdev);
	}
}

static void __init imx6q_map_io(void)
{
	debug_ll_io_init();
	imx_scu_map_io();
	imx6_pm_map_io();
	imx_busfreq_map_io();
}

static void __init imx6q_init_irq(void)
{
	imx_gpc_check_dt();
	imx_init_revision_from_anatop();
	imx_init_l2cache();
	imx_src_init();
	irqchip_init();
}

static const char * const imx6q_dt_compat[] __initconst = {
	"fsl,imx6dl",
	"fsl,imx6q",
	NULL,
};

#ifdef CONFIG_MX6_CLK_FOR_BOOTUI_TRANS
static void imx6q_init_reserve(void)
{
	phys_addr_t base, size;

/*
	 * Frame buffer base address.
	 * It is same as CONFIG_FB_BASE in Uboot.
	 */
	base = 0x27800000;

	/* printk("\n[imx6q_init_reserve] base:%x\n", base); */

	/*
	 * Reserved display memory size.
	 * It should be bigger than 3 x framer buffer size.
	 * For 1080P 32 bpp, 1920*1080*4*3 = 0x017BB000.
	 */
	size = 0x01800000;

	memblock_reserve(base, size);
	memblock_remove(base, size);
}
#endif


DT_MACHINE_START(IMX6Q, "Freescale i.MX6 Quad/DualLite (Device Tree)")
	.smp		= smp_ops(imx_smp_ops),
	.map_io		= imx6q_map_io,
	.init_irq	= imx6q_init_irq,
	.init_machine	= imx6q_init_machine,
	.init_late      = imx6q_init_late,
	.dt_compat	= imx6q_dt_compat,
#ifdef CONFIG_MX6_CLK_FOR_BOOTUI_TRANS
	.reserve        = imx6q_init_reserve,
#endif
MACHINE_END