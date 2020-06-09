#ifndef _RTL8367C_ASICDRV_TRUNKING_H_
#define _RTL8367C_ASICDRV_TRUNKING_H_

#include "rtl8367c_asicdrv.h"

#define RTL8367C_MAX_TRUNK_GID              (2)
#define RTL8367C_TRUNKING_PORTNO            (4)
#define RTL8367C_TRUNKING1_PORTN0           (2)
#define RTL8367C_TRUNKING_HASHVALUE_MAX     (15)

extern ret_t rtl8367c_setAsicTrunkingGroup(struct phy_device *phydev, rtk_uint32 group, rtk_uint32 portmask);
extern ret_t rtl8367c_getAsicTrunkingGroup(struct phy_device *phydev, rtk_uint32 group, rtk_uint32* pPortmask);
extern ret_t rtl8367c_setAsicTrunkingFlood(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicTrunkingFlood(struct phy_device *phydev, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicTrunkingHashSelect(struct phy_device *phydev, rtk_uint32 hashsel);
extern ret_t rtl8367c_getAsicTrunkingHashSelect(struct phy_device *phydev, rtk_uint32* pHashsel);

extern ret_t rtl8367c_getAsicQeueuEmptyStatus(struct phy_device *phydev, rtk_uint32* pPortmask);

extern ret_t rtl8367c_setAsicTrunkingMode(struct phy_device *phydev, rtk_uint32 mode);
extern ret_t rtl8367c_getAsicTrunkingMode(struct phy_device *phydev, rtk_uint32* pMode);
extern ret_t rtl8367c_setAsicTrunkingFc(struct phy_device *phydev, rtk_uint32 group, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicTrunkingFc(struct phy_device *phydev, rtk_uint32 group, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicTrunkingHashTable(struct phy_device *phydev, rtk_uint32 hashval, rtk_uint32 portId);
extern ret_t rtl8367c_getAsicTrunkingHashTable(struct phy_device *phydev, rtk_uint32 hashval, rtk_uint32* pPortId);
extern ret_t rtl8367c_setAsicTrunkingHashTable1(struct phy_device *phydev, rtk_uint32 hashval, rtk_uint32 portId);
extern ret_t rtl8367c_getAsicTrunkingHashTable1(struct phy_device *phydev, rtk_uint32 hashval, rtk_uint32* pPortId);

#endif /*_RTL8367C_ASICDRV_TRUNKING_H_*/

