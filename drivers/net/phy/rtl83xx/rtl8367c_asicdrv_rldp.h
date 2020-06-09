#ifndef _RTL8367C_ASICDRV_RLDP_H_
#define _RTL8367C_ASICDRV_RLDP_H_

#include "rtl8367c_asicdrv.h"
#include <linux/string.h>

extern ret_t rtl8367c_setAsicRldp(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicRldp(struct phy_device *phydev, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsicRldpEnable8051(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicRldpEnable8051(struct phy_device *phydev, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsicRldpCompareRandomNumber(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicRldpCompareRandomNumber(struct phy_device *phydev, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsicRldpIndicatorSource(struct phy_device *phydev, rtk_uint32 src);
extern ret_t rtl8367c_getAsicRldpIndicatorSource(struct phy_device *phydev, rtk_uint32 *pSrc);
extern ret_t rtl8367c_setAsicRldpCheckingStatePara(struct phy_device *phydev, rtk_uint32 retryCount, rtk_uint32 retryPeriod);
extern ret_t rtl8367c_getAsicRldpCheckingStatePara(struct phy_device *phydev, rtk_uint32 *pRetryCount, rtk_uint32 *pRetryPeriod);
extern ret_t rtl8367c_setAsicRldpLoopStatePara(struct phy_device *phydev, rtk_uint32 retryCount, rtk_uint32 retryPeriod);
extern ret_t rtl8367c_getAsicRldpLoopStatePara(struct phy_device *phydev, rtk_uint32 *pRetryCount, rtk_uint32 *pRetryPeriod);
extern ret_t rtl8367c_setAsicRldpTxPortmask(struct phy_device *phydev, rtk_uint32 portmask);
extern ret_t rtl8367c_getAsicRldpTxPortmask(struct phy_device *phydev, rtk_uint32 *pPortmask);
extern ret_t rtl8367c_setAsicRldpMagicNum(struct phy_device *phydev, ether_addr_t seed);
extern ret_t rtl8367c_getAsicRldpMagicNum(struct phy_device *phydev, ether_addr_t *pSeed);
extern ret_t rtl8367c_getAsicRldpLoopedPortmask(struct phy_device *phydev, rtk_uint32 *pPortmask);
extern ret_t rtl8367c_setAsicRldp8051Portmask(struct phy_device *phydev, rtk_uint32 portmask);
extern ret_t rtl8367c_getAsicRldp8051Portmask(struct phy_device *phydev, rtk_uint32 *pPortmask);


extern ret_t rtl8367c_getAsicRldpRandomNumber(struct phy_device *phydev, ether_addr_t *pRandNumber);
extern ret_t rtl8367c_getAsicRldpLoopedPortPair(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pLoopedPair);
extern ret_t rtl8367c_setAsicRlppTrap8051(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicRlppTrap8051(struct phy_device *phydev, rtk_uint32 *pEnabled);

extern ret_t rtl8367c_setAsicRldpLeaveLoopedPortmask(struct phy_device *phydev, rtk_uint32 portmask);
extern ret_t rtl8367c_getAsicRldpLeaveLoopedPortmask(struct phy_device *phydev, rtk_uint32 *pPortmask);

extern ret_t rtl8367c_setAsicRldpEnterLoopedPortmask(struct phy_device *phydev, rtk_uint32 portmask);
extern ret_t rtl8367c_getAsicRldpEnterLoopedPortmask(struct phy_device *phydev, rtk_uint32 *pPortmask);

extern ret_t rtl8367c_setAsicRldpTriggerMode(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicRldpTriggerMode(struct phy_device *phydev, rtk_uint32 *pEnabled);

#endif /*_RTL8367C_ASICDRV_RLDP_H_*/

