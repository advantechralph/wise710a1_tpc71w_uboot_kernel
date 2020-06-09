#ifndef _RTL8367C_ASICDRV_SCHEDULING_H_
#define _RTL8367C_ASICDRV_SCHEDULING_H_

#include "rtl8367c_asicdrv.h"

#define RTL8367C_QWEIGHTMAX    0x7F
#define RTL8367C_PORT_QUEUE_METER_INDEX_MAX    7

/* enum for queue type */
enum QUEUETYPE
{
    QTYPE_STRICT = 0,
    QTYPE_WFQ,
};
extern ret_t rtl8367c_setAsicLeakyBucketParameter(struct phy_device *phydev, rtk_uint32 tick, rtk_uint32 token);
extern ret_t rtl8367c_getAsicLeakyBucketParameter(struct phy_device *phydev, rtk_uint32 *tick, rtk_uint32 *token);
extern ret_t rtl8367c_setAsicAprMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 qid, rtk_uint32 apridx);
extern ret_t rtl8367c_getAsicAprMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 qid, rtk_uint32 *apridx);
extern ret_t rtl8367c_setAsicPprMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 qid, rtk_uint32 ppridx);
extern ret_t rtl8367c_getAsicPprMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 qid, rtk_uint32 *ppridx);
extern ret_t rtl8367c_setAsicAprEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 aprEnable);
extern ret_t rtl8367c_getAsicAprEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *aprEnable);
extern ret_t rtl8367c_setAsicPprEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 pprEnable);
extern ret_t rtl8367c_getAsicPprEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pprEnable);

extern ret_t rtl8367c_setAsicWFQWeight(struct phy_device *phydev, rtk_uint32, rtk_uint32 queueid, rtk_uint32 weight );
extern ret_t rtl8367c_getAsicWFQWeight(struct phy_device *phydev, rtk_uint32, rtk_uint32 queueid, rtk_uint32 *weight );
extern ret_t rtl8367c_setAsicWFQBurstSize(struct phy_device *phydev, rtk_uint32 burstsize);
extern ret_t rtl8367c_getAsicWFQBurstSize(struct phy_device *phydev, rtk_uint32 *burstsize);

extern ret_t rtl8367c_setAsicQueueType(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 qid, rtk_uint32 queueType);
extern ret_t rtl8367c_getAsicQueueType(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 qid, rtk_uint32 *queueType);
extern ret_t rtl8367c_setAsicQueueRate(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 qid, rtk_uint32 ppridx, rtk_uint32 apridx );
extern ret_t rtl8367c_getAsicQueueRate(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 qid, rtk_uint32* ppridx, rtk_uint32* apridx );
extern ret_t rtl8367c_setAsicPortEgressRate(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 rate);
extern ret_t rtl8367c_getAsicPortEgressRate(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *rate);
extern ret_t rtl8367c_setAsicPortEgressRateIfg(struct phy_device *phydev, rtk_uint32 ifg);
extern ret_t rtl8367c_getAsicPortEgressRateIfg(struct phy_device *phydev, rtk_uint32 *ifg);

#endif /*_RTL8367C_ASICDRV_SCHEDULING_H_*/

