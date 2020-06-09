#ifndef _RTL8367C_ASICDRV_OAM_H_
#define _RTL8367C_ASICDRV_OAM_H_

#include "rtl8367c_asicdrv.h"

enum OAMPARACT
{
    OAM_PARFWD = 0,
    OAM_PARLB,
    OAM_PARDISCARD,
    OAM_PARFWDCPU
};

enum OAMMULACT
{
    OAM_MULFWD = 0,
    OAM_MULDISCARD,
    OAM_MULCPU
};

extern ret_t rtl8367c_setAsicOamParser(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 parser);
extern ret_t rtl8367c_getAsicOamParser(struct phy_device *phydev, rtk_uint32 port, rtk_uint32* pParser);
extern ret_t rtl8367c_setAsicOamMultiplexer(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 multiplexer);
extern ret_t rtl8367c_getAsicOamMultiplexer(struct phy_device *phydev, rtk_uint32 port, rtk_uint32* pMultiplexer);
extern ret_t rtl8367c_setAsicOamCpuPri(struct phy_device *phydev, rtk_uint32 priority);
extern ret_t rtl8367c_getAsicOamCpuPri(struct phy_device *phydev, rtk_uint32 *pPriority);
extern ret_t rtl8367c_setAsicOamEnable(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicOamEnable(struct phy_device *phydev, rtk_uint32 *pEnabled);
#endif /*_RTL8367C_ASICDRV_OAM_H_*/

