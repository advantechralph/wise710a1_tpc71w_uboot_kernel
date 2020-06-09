#ifndef _RTL8367C_ASICDRV_DOT1X_H_
#define _RTL8367C_ASICDRV_DOT1X_H_

#include "rtl8367c_asicdrv.h"

enum DOT1X_UNAUTH_BEHAV
{
    DOT1X_UNAUTH_DROP = 0,
    DOT1X_UNAUTH_TRAP,
    DOT1X_UNAUTH_GVLAN,
    DOT1X_UNAUTH_END
};

extern ret_t rtl8367c_setAsic1xPBEnConfig(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsic1xPBEnConfig(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsic1xPBAuthConfig(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 auth);
extern ret_t rtl8367c_getAsic1xPBAuthConfig(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pAuth);
extern ret_t rtl8367c_setAsic1xPBOpdirConfig(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 opdir);
extern ret_t rtl8367c_getAsic1xPBOpdirConfig(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pOpdir);
extern ret_t rtl8367c_setAsic1xMBEnConfig(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsic1xMBEnConfig(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsic1xMBOpdirConfig(struct phy_device *phydev, rtk_uint32 opdir);
extern ret_t rtl8367c_getAsic1xMBOpdirConfig(struct phy_device *phydev, rtk_uint32 *pOpdir);
extern ret_t rtl8367c_setAsic1xProcConfig(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 proc);
extern ret_t rtl8367c_getAsic1xProcConfig(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pProc);
extern ret_t rtl8367c_setAsic1xGuestVidx(struct phy_device *phydev, rtk_uint32 index);
extern ret_t rtl8367c_getAsic1xGuestVidx(struct phy_device *phydev, rtk_uint32 *pIndex);
extern ret_t rtl8367c_setAsic1xGVOpdir(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsic1xGVOpdir(struct phy_device *phydev, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsic1xTrapPriority(struct phy_device *phydev, rtk_uint32 priority);
extern ret_t rtl8367c_getAsic1xTrapPriority(struct phy_device *phydev, rtk_uint32 *pPriority);


#endif /*_RTL8367C_ASICDRV_DOT1X_H_*/

