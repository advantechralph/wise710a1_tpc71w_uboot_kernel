#ifndef _RTL8367C_ASICDRV_MIRROR_H_
#define _RTL8367C_ASICDRV_MIRROR_H_

#include "rtl8367c_asicdrv.h"

extern ret_t rtl8367c_setAsicPortMirror(struct phy_device *phydev, rtk_uint32 source, rtk_uint32 monitor);
extern ret_t rtl8367c_getAsicPortMirror(struct phy_device *phydev, rtk_uint32 *pSource, rtk_uint32 *pMonitor);
extern ret_t rtl8367c_setAsicPortMirrorRxFunction(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicPortMirrorRxFunction(struct phy_device *phydev, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicPortMirrorTxFunction(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicPortMirrorTxFunction(struct phy_device *phydev, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicPortMirrorIsolation(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicPortMirrorIsolation(struct phy_device *phydev, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicPortMirrorPriority(struct phy_device *phydev, rtk_uint32 priority);
extern ret_t rtl8367c_getAsicPortMirrorPriority(struct phy_device *phydev, rtk_uint32* pPriority);
extern ret_t rtl8367c_setAsicPortMirrorMask(struct phy_device *phydev, rtk_uint32 SourcePortmask);
extern ret_t rtl8367c_getAsicPortMirrorMask(struct phy_device *phydev, rtk_uint32 *pSourcePortmask);
extern ret_t rtl8367c_setAsicPortMirrorVlanRxLeaky(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicPortMirrorVlanRxLeaky(struct phy_device *phydev, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicPortMirrorVlanTxLeaky(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicPortMirrorVlanTxLeaky(struct phy_device *phydev, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicPortMirrorIsolationRxLeaky(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicPortMirrorIsolationRxLeaky(struct phy_device *phydev, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicPortMirrorIsolationTxLeaky(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicPortMirrorIsolationTxLeaky(struct phy_device *phydev, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicPortMirrorRealKeep(struct phy_device *phydev, rtk_uint32 mode);
extern ret_t rtl8367c_getAsicPortMirrorRealKeep(struct phy_device *phydev, rtk_uint32* pMode);
extern ret_t rtl8367c_setAsicPortMirrorOverride(struct phy_device *phydev, rtk_uint32 rxMirror, rtk_uint32 txMirror, rtk_uint32 aclMirror);
extern ret_t rtl8367c_getAsicPortMirrorOverride(struct phy_device *phydev, rtk_uint32 *pRxMirror, rtk_uint32 *pTxMirror, rtk_uint32 *pAclMirror);

#endif /*#ifndef _RTL8367C_ASICDRV_MIRROR_H_*/

