#ifndef _RTL8367C_ASICDRV_CPUTAG_H_
#define _RTL8367C_ASICDRV_CPUTAG_H_

#include "rtl8367c_asicdrv.h"

enum CPUTAG_INSERT_MODE
{
    CPUTAG_INSERT_TO_ALL = 0,
    CPUTAG_INSERT_TO_TRAPPING,
    CPUTAG_INSERT_TO_NO,
    CPUTAG_INSERT_END
};

extern ret_t rtl8367c_setAsicCputagEnable(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicCputagEnable(struct phy_device *phydev, rtk_uint32 *pEnabled);
extern ret_t rtl8367c_setAsicCputagTrapPort(struct phy_device *phydev, rtk_uint32 port);
extern ret_t rtl8367c_getAsicCputagTrapPort(struct phy_device *phydev, rtk_uint32 *pPort);
extern ret_t rtl8367c_setAsicCputagPortmask(struct phy_device *phydev, rtk_uint32 portmask);
extern ret_t rtl8367c_getAsicCputagPortmask(struct phy_device *phydev, rtk_uint32 *pPmsk);
extern ret_t rtl8367c_setAsicCputagInsertMode(struct phy_device *phydev, rtk_uint32 mode);
extern ret_t rtl8367c_getAsicCputagInsertMode(struct phy_device *phydev, rtk_uint32 *pMode);
extern ret_t rtl8367c_setAsicCputagPriorityRemapping(struct phy_device *phydev, rtk_uint32 srcPri, rtk_uint32 newPri);
extern ret_t rtl8367c_getAsicCputagPriorityRemapping(struct phy_device *phydev, rtk_uint32 srcPri, rtk_uint32 *pNewPri);
extern ret_t rtl8367c_setAsicCputagPosition(struct phy_device *phydev, rtk_uint32 postion);
extern ret_t rtl8367c_getAsicCputagPosition(struct phy_device *phydev, rtk_uint32* pPostion);
extern ret_t rtl8367c_setAsicCputagMode(struct phy_device *phydev, rtk_uint32 mode);
extern ret_t rtl8367c_getAsicCputagMode(struct phy_device *phydev, rtk_uint32 *pMode);
extern ret_t rtl8367c_setAsicCputagRxMinLength(struct phy_device *phydev, rtk_uint32 mode);
extern ret_t rtl8367c_getAsicCputagRxMinLength(struct phy_device *phydev, rtk_uint32 *pMode);

#endif /*#ifndef _RTL8367C_ASICDRV_CPUTAG_H_*/

