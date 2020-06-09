/*
 * Copyright (C) 2012-2016 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Freescale i.MX6Q SabreSD board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MX6QSABRESD_CONFIG_H
#define __MX6QSABRESD_CONFIG_H

#ifdef CONFIG_SPL
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_MMC_SUPPORT
#include "imx6_spl_advantech.h"
#endif

#define CONFIG_MACH_TYPE	3980
#define CONFIG_MXC_UART_BASE	UART1_BASE
#define CONFIG_CONSOLE_DEV		"ttymxc0"
#define CONFIG_MMCROOT			"/dev/mmcblk0p2"  /* SDHC3 */

#if defined(CONFIG_TARGET_MX6TPC71WN10PA)
#define PHYS_SDRAM_SIZE         (1u * 1024 * 1024 * 1024)
#endif

#if defined(CONFIG_MX6QP)
#define CONFIG_DEFAULT_FDT_FILE	"imx6qp-tpc71w-n10pa.dtb"
#elif defined(CONFIG_MX6Q)
#define CONFIG_DEFAULT_FDT_FILE	"imx6q-tpc71w-n10pa.dtb"
#elif defined(CONFIG_MX6DL)
#define CONFIG_DEFAULT_FDT_FILE	"imx6dl-tpc71w-n10pa.dtb"
#elif defined(CONFIG_MX6SOLO)
#define CONFIG_DEFAULT_FDT_FILE	"imx6dl-tpc71w-n10pa.dtb"
#endif
#define CONFIG_MMC_CLOCK_DOWNGRADE
#include "mx6advantech_common.h"
/* don't use pmic */
#undef CONFIG_LDO_BYPASS_CHECK

#define CONFIG_SYS_FSL_USDHC_NUM	3
#define CONFIG_SYS_MMC_ENV_DEV		1	/* SDHC3 */
#define CONFIG_SYS_MMC_ENV_PART                0       /* user partition */

#ifdef CONFIG_SYS_USE_SPINOR
#define CONFIG_SF_DEFAULT_CS   0
#endif
#undef CONFIG_FEC_MXC_PHYADDR
#define CONFIG_FEC_MXC_PHYADDR	1
#undef CONFIG_PHY_ATHEROS
#define CONFIG_PHY_REALTEK

/*
 * imx6 q/dl/solo pcie would be failed to work properly in kernel, if
 * the pcie module is iniialized/enumerated both in uboot and linux
 * kernel.
 * rootcause:imx6 q/dl/solo pcie don't have the reset mechanism.
 * it is only be RESET by the POR. So, the pcie module only be
 * initialized/enumerated once in one POR.
 * Set to use pcie in kernel defaultly, mask the pcie config here.
 * Remove the mask freely, if the uboot pcie functions, rather than
 * the kernel's, are required.
 */
/* #define CONFIG_CMD_PCI */
#ifdef CONFIG_CMD_PCI
#define CONFIG_PCI
#define CONFIG_PCI_PNP
#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_PCIE_IMX
#define CONFIG_PCIE_IMX_PERST_GPIO	IMX_GPIO_NR(7, 12)
#define CONFIG_PCIE_IMX_POWER_GPIO	IMX_GPIO_NR(3, 19)
#endif

/* USB Configs */
#define CONFIG_CMD_USB
#ifdef CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_STORAGE
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS		0
#define CONFIG_USB_MAX_CONTROLLER_COUNT	1 /* Enabled USB controller number */
#endif

/*#define CONFIG_SPLASH_SCREEN*/
/*#define CONFIG_MXC_EPDC*/

/*
 * SPLASH SCREEN Configs
 */
#ifndef CONFIG_ADVANTECH
#if defined(CONFIG_SPLASH_SCREEN) && defined(CONFIG_MXC_EPDC)
	/*
	 * Framebuffer and LCD
	 */
	#define CONFIG_CMD_BMP
	#define CONFIG_LCD
	#define CONFIG_SYS_CONSOLE_IS_IN_ENV
	#undef LCD_TEST_PATTERN
	/* #define CONFIG_SPLASH_IS_IN_MMC			1 */
	#define LCD_BPP					LCD_MONOCHROME
	/* #define CONFIG_SPLASH_SCREEN_ALIGN		1 */

	#define CONFIG_WAVEFORM_BUF_SIZE		0x200000
#endif /* CONFIG_SPLASH_SCREEN && CONFIG_MXC_EPDC */
#endif

#define CONFIG_UBOOT_LOGO_ENABLE

#ifdef CONFIG_UBOOT_LOGO_ENABLE
	/* Select one of the output mode */
	#define IPU_OUTPUT_MODE_LVDS

	#define CONFIG_FB_BASE	(CONFIG_SYS_TEXT_BASE + 0x1000000)
	#define UBOOT_LOGO_BMP_ADDR 0x00100000

	#define CONFIG_IMX_PWM
	#define IMX_PWM1_BASE	 PWM1_BASE_ADDR
	#define IMX_PWM2_BASE	 PWM2_BASE_ADDR

#ifdef CONFIG_MX6DL
	#define CONFIG_IPUV3_CLK 270000000
#else
	#define CONFIG_IPUV3_CLK 264000000
#endif

#ifdef IPU_OUTPUT_MODE_LVDS

#ifdef CONFIG_ADVANTECH
#if 1
	#define DISPLAY_WIDTH	1024
	#define DISPLAY_HEIGHT	600
	#define DISPLAY_BPP	32
	#define DISPLAY_IF_BPP	24  /* RGB888 interface */

	#define DISPLAY_HSYNC_START	0
	#define DISPLAY_HSYNC_END	35
	#define DISPLAY_HSYNC_WIDTH	72

	#define DISPLAY_VSYNC_START	0
	#define DISPLAY_VSYNC_END	320
	#define DISPLAY_VSYNC_WIDTH	7

	#define DISPLAY_PIX_CLOCK	29500000  /*(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)*/
#else
	#define DISPLAY_WIDTH	800
	#define DISPLAY_HEIGHT	480
	#define DISPLAY_BPP	32
	#define DISPLAY_IF_BPP	24  /* RGB888 interface */

	#define DISPLAY_HSYNC_START	127
	#define DISPLAY_HSYNC_END	127
	#define DISPLAY_HSYNC_WIDTH	2

	#define DISPLAY_VSYNC_START	13
	#define DISPLAY_VSYNC_END	13
	#define DISPLAY_VSYNC_WIDTH	2

	#define DISPLAY_PIX_CLOCK	33000000  /*(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz)*/
	/*	#define DISPLAY_PIX_CLOCK	24864000 */
#endif
#else   /* Demo board */
	/* For LVDS, 1024*768 resolution */
	#define DISPLAY_WIDTH	1024
	#define DISPLAY_HEIGHT	768
	#define DISPLAY_BPP	32
	#define DISPLAY_IF_BPP	18  /* RGB666 interface */

	#define DISPLAY_HSYNC_START	220
	#define DISPLAY_HSYNC_END	40
	#define DISPLAY_HSYNC_WIDTH	60

	#define DISPLAY_VSYNC_START	21
	#define DISPLAY_VSYNC_END	7
	#define DISPLAY_VSYNC_WIDTH	10

	#define DISPLAY_PIX_CLOCK	64000000  /*(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz) */

#endif
	#define DISPLAY_VSYNC_POLARITY			0
	#define DISPLAY_HSYNC_POLARITY			0
	#define DISPLAY_CLOCK_POLARITY			0
	#define DISPLAY_DATA_POLARITY			0
	#define DISPLAY_DATA_ENABLE_POLARITY		1
	#define IPU_NUM			1 /* 1 for IPU1, 2 for IPU2. */
	#define DI_NUM			0 /* 0 for DI0, 1 for DI1. */
	#define LVDS_PORT		0  /* 0 for LVDS0, 1 for LVDS1. Note: LVDS and DI should be same number. */
	#define DI_CLOCK_EXTERNAL_MODE  /* When clock external mode was defined, the DI clock root will be PLL3 PFD1, without this macro, the DI root clock is IPU internal clock. */
	/*#define LVDS_CLOCK_SRC_PLL5*/
#endif /* IPU_OUTPUT_MODE_LVDS */

#endif  /* CONFIG_UBOOT_LOGO_ENABLE */


/* uncomment for SECURE mode support */
/* #define CONFIG_SECURE_BOOT */

#ifdef CONFIG_SECURE_BOOT
#ifndef CONFIG_CSF_SIZE
#define CONFIG_CSF_SIZE 0x4000
#endif
#endif

/* #define CONFIG_MFG_IGNORE_CHECK_SECURE_BOOT */

#define CONFIG_SUPPORT_LVDS
#ifdef CONFIG_SUPPORT_LVDS
#define IOMUX_LCD_BKLT_PWM 	MX6_PAD_SD1_CMD__GPIO1_IO18
#define IOMUX_LCD_BKLT_EN	MX6_PAD_KEY_COL0__GPIO4_IO06
#define IOMUX_LCD_VDD_EN	MX6_PAD_KEY_ROW0__GPIO4_IO07
#define LCD_BKLT_PWM 		IMX_GPIO_NR(1, 18)
#define LCD_BKLT_EN 		IMX_GPIO_NR(4, 6)
#define LCD_VDD_EN 		IMX_GPIO_NR(4, 7)	
#endif

#define SPI1_CS0                IMX_GPIO_NR(2,30)
#define IOMUX_SPI_SCLK          MX6_PAD_EIM_D16__ECSPI1_SCLK
#define IOMUX_SPI_MISO          MX6_PAD_EIM_D17__ECSPI1_MISO
#define IOMUX_SPI_MOSI          MX6_PAD_EIM_D18__ECSPI1_MOSI
#define IOMUX_SPI_CS0           MX6_PAD_EIM_EB2__ECSPI1_SS0

#define USDHC2_CD_GPIO		IMX_GPIO_NR(1, 4)
#endif                         /* __MX6QSABRESD_CONFIG_H */
