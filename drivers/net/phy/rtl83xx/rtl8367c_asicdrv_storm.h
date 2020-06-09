#ifndef _RTL8367C_ASICDRV_STORM_H_
#define _RTL8367C_ASICDRV_STORM_H_

#include "rtl8367c_asicdrv.h"

extern ret_t rtl8367c_setAsicStormFilterBroadcastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicStormFilterBroadcastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsicStormFilterBroadcastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 meter);
extern ret_t rtl8367c_getAsicStormFilterBroadcastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pMeter);
extern ret_t rtl8367c_setAsicStormFilterMulticastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicStormFilterMulticastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsicStormFilterMulticastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 meter);
extern ret_t rtl8367c_getAsicStormFilterMulticastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pMeter);
extern ret_t rtl8367c_setAsicStormFilterUnknownMulticastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicStormFilterUnknownMulticastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsicStormFilterUnknownMulticastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 meter);
extern ret_t rtl8367c_getAsicStormFilterUnknownMulticastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pMeter);
extern ret_t rtl8367c_setAsicStormFilterUnknownUnicastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicStormFilterUnknownUnicastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsicStormFilterUnknownUnicastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 meter);
extern ret_t rtl8367c_getAsicStormFilterUnknownUnicastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pMeter);
extern ret_t rtl8367c_setAsicStormFilterExtBroadcastMeter(struct phy_device *phydev, rtk_uint32 meter);
extern ret_t rtl8367c_getAsicStormFilterExtBroadcastMeter(struct phy_device *phydev, rtk_uint32 *pMeter);
extern ret_t rtl8367c_setAsicStormFilterExtMulticastMeter(struct phy_device *phydev, rtk_uint32 meter);
extern ret_t rtl8367c_getAsicStormFilterExtMulticastMeter(struct phy_device *phydev, rtk_uint32 *pMeter);
extern ret_t rtl8367c_setAsicStormFilterExtUnknownMulticastMeter(struct phy_device *phydev, rtk_uint32 meter);
extern ret_t rtl8367c_getAsicStormFilterExtUnknownMulticastMeter(struct phy_device *phydev, rtk_uint32 *pMeter);
extern ret_t rtl8367c_setAsicStormFilterExtUnknownUnicastMeter(struct phy_device *phydev, rtk_uint32 meter);
extern ret_t rtl8367c_getAsicStormFilterExtUnknownUnicastMeter(struct phy_device *phydev, rtk_uint32 *pMeter);
extern ret_t rtl8367c_setAsicStormFilterExtBroadcastEnable(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicStormFilterExtBroadcastEnable(struct phy_device *phydev, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsicStormFilterExtMulticastEnable(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicStormFilterExtMulticastEnable(struct phy_device *phydev, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsicStormFilterExtUnknownMulticastEnable(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicStormFilterExtUnknownMulticastEnable(struct phy_device *phydev, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsicStormFilterExtUnknownUnicastEnable(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicStormFilterExtUnknownUnicastEnable(struct phy_device *phydev, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsicStormFilterExtEnablePortMask(struct phy_device *phydev, rtk_uint32 portmask);
extern ret_t rtl8367c_getAsicStormFilterExtEnablePortMask(struct phy_device *phydev, rtk_uint32 *pPortmask);


#endif /*_RTL8367C_ASICDRV_STORM_H_*/


