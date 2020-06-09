#ifndef _RTL8367C_ASICDRV_METER_H_
#define _RTL8367C_ASICDRV_METER_H_

#include "rtl8367c_asicdrv.h"


extern ret_t rtl8367c_setAsicShareMeter(struct phy_device *phydev, rtk_uint32 index, rtk_uint32 rate, rtk_uint32 ifg);
extern ret_t rtl8367c_getAsicShareMeter(struct phy_device *phydev, rtk_uint32 index, rtk_uint32 *pRate, rtk_uint32 *pIfg);
extern ret_t rtl8367c_setAsicShareMeterBucketSize(struct phy_device *phydev, rtk_uint32 index, rtk_uint32 lbThreshold);
extern ret_t rtl8367c_getAsicShareMeterBucketSize(struct phy_device *phydev, rtk_uint32 index, rtk_uint32 *pLbThreshold);
extern ret_t rtl8367c_setAsicShareMeterType(struct phy_device *phydev, rtk_uint32 index, rtk_uint32 type);
extern ret_t rtl8367c_getAsicShareMeterType(struct phy_device *phydev, rtk_uint32 index, rtk_uint32 *pType);
extern ret_t rtl8367c_setAsicMeterExceedStatus(struct phy_device *phydev, rtk_uint32 index);
extern ret_t rtl8367c_getAsicMeterExceedStatus(struct phy_device *phydev, rtk_uint32 index, rtk_uint32* pStatus);

#endif /*_RTL8367C_ASICDRV_FC_H_*/

