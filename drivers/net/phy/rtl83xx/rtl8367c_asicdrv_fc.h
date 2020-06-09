#ifndef _RTL8367C_ASICDRV_FC_H_
#define _RTL8367C_ASICDRV_FC_H_

#include "rtl8367c_asicdrv.h"

#define RTL8367C_PAGE_NUMBER    0x600


enum FLOW_CONTROL_TYPE
{
    FC_EGRESS = 0,
    FC_INGRESS,
};

enum FC_JUMBO_SIZE
{
    FC_JUMBO_SIZE_3K = 0,
    FC_JUMBO_SIZE_4K,
    FC_JUMBO_SIZE_6K,
    FC_JUMBO_SIZE_9K,
    FC_JUMBO_SIZE_END,

};


extern ret_t rtl8367c_setAsicFlowControlSelect(struct phy_device *phydev, rtk_uint32 select);
extern ret_t rtl8367c_getAsicFlowControlSelect(struct phy_device *phydev, rtk_uint32 *pSelect);
extern ret_t rtl8367c_setAsicFlowControlJumboMode(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicFlowControlJumboMode(struct phy_device *phydev, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicFlowControlJumboModeSize(struct phy_device *phydev, rtk_uint32 size);
extern ret_t rtl8367c_getAsicFlowControlJumboModeSize(struct phy_device *phydev, rtk_uint32* pSize);
extern ret_t rtl8367c_setAsicFlowControlQueueEgressEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 qid, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicFlowControlQueueEgressEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 qid, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicFlowControlDropAll(struct phy_device *phydev, rtk_uint32 dropall);
extern ret_t rtl8367c_getAsicFlowControlDropAll(struct phy_device *phydev, rtk_uint32* pDropall);
extern ret_t rtl8367c_setAsicFlowControlPauseAllThreshold(struct phy_device *phydev, rtk_uint32 threshold);
extern ret_t rtl8367c_getAsicFlowControlPauseAllThreshold(struct phy_device *phydev, rtk_uint32 *pThreshold);
extern ret_t rtl8367c_setAsicFlowControlSystemThreshold(struct phy_device *phydev, rtk_uint32 onThreshold, rtk_uint32 offThreshold);
extern ret_t rtl8367c_getAsicFlowControlSystemThreshold(struct phy_device *phydev, rtk_uint32 *pOnThreshold, rtk_uint32 *pOffThreshold);
extern ret_t rtl8367c_setAsicFlowControlSharedThreshold(struct phy_device *phydev, rtk_uint32 onThreshold, rtk_uint32 offThreshold);
extern ret_t rtl8367c_getAsicFlowControlSharedThreshold(struct phy_device *phydev, rtk_uint32 *pOnThreshold, rtk_uint32 *pOffThreshold);
extern ret_t rtl8367c_setAsicFlowControlPortThreshold(struct phy_device *phydev, rtk_uint32 onThreshold, rtk_uint32 offThreshold);
extern ret_t rtl8367c_getAsicFlowControlPortThreshold(struct phy_device *phydev, rtk_uint32 *pOnThreshold, rtk_uint32 *pOffThreshold);
extern ret_t rtl8367c_setAsicFlowControlPortPrivateThreshold(struct phy_device *phydev, rtk_uint32 onThreshold, rtk_uint32 offThreshold);
extern ret_t rtl8367c_getAsicFlowControlPortPrivateThreshold(struct phy_device *phydev, rtk_uint32 *pOnThreshold, rtk_uint32 *pOffThreshold);
extern ret_t rtl8367c_setAsicFlowControlSystemDropThreshold(struct phy_device *phydev, rtk_uint32 onThreshold, rtk_uint32 offThreshold);
extern ret_t rtl8367c_getAsicFlowControlSystemDropThreshold(struct phy_device *phydev, rtk_uint32 *pOnThreshold, rtk_uint32 *pOffThreshold);
extern ret_t rtl8367c_setAsicFlowControlSharedDropThreshold(struct phy_device *phydev, rtk_uint32 onThreshold, rtk_uint32 offThreshold);
extern ret_t rtl8367c_getAsicFlowControlSharedDropThreshold(struct phy_device *phydev, rtk_uint32 *pOnThreshold, rtk_uint32 *pOffThreshold);
extern ret_t rtl8367c_setAsicFlowControlPortDropThreshold(struct phy_device *phydev, rtk_uint32 onThreshold, rtk_uint32 offThreshold);
extern ret_t rtl8367c_getAsicFlowControlPortDropThreshold(struct phy_device *phydev, rtk_uint32 *pOnThreshold, rtk_uint32 *pOffThreshold);
extern ret_t rtl8367c_setAsicFlowControlPortPrivateDropThreshold(struct phy_device *phydev, rtk_uint32 onThreshold, rtk_uint32 offThreshold);
extern ret_t rtl8367c_getAsicFlowControlPortPrivateDropThreshold(struct phy_device *phydev, rtk_uint32 *pOnThreshold, rtk_uint32 *pOffThreshold);
extern ret_t rtl8367c_setAsicFlowControlSystemJumboThreshold(struct phy_device *phydev, rtk_uint32 onThreshold, rtk_uint32 offThreshold);
extern ret_t rtl8367c_getAsicFlowControlSystemJumboThreshold(struct phy_device *phydev, rtk_uint32 *pOnThreshold, rtk_uint32 *pOffThreshold);
extern ret_t rtl8367c_setAsicFlowControlSharedJumboThreshold(struct phy_device *phydev, rtk_uint32 onThreshold, rtk_uint32 offThreshold);
extern ret_t rtl8367c_getAsicFlowControlSharedJumboThreshold(struct phy_device *phydev, rtk_uint32 *pOnThreshold, rtk_uint32 *pOffThreshold);
extern ret_t rtl8367c_setAsicFlowControlPortJumboThreshold(struct phy_device *phydev, rtk_uint32 onThreshold, rtk_uint32 offThreshold);
extern ret_t rtl8367c_getAsicFlowControlPortJumboThreshold(struct phy_device *phydev, rtk_uint32 *pOnThreshold, rtk_uint32 *pOffThreshold);
extern ret_t rtl8367c_setAsicFlowControlPortPrivateJumboThreshold(struct phy_device *phydev, rtk_uint32 onThreshold, rtk_uint32 offThreshold);
extern ret_t rtl8367c_getAsicFlowControlPortPrivateJumboThreshold(struct phy_device *phydev, rtk_uint32 *pOnThreshold, rtk_uint32 *pOffThreshold);

extern ret_t rtl8367c_setAsicEgressFlowControlPortDropGap(struct phy_device *phydev, rtk_uint32 gap);
extern ret_t rtl8367c_getAsicEgressFlowControlPortDropGap(struct phy_device *phydev, rtk_uint32 *pGap);
extern ret_t rtl8367c_setAsicEgressFlowControlQueueDropGap(struct phy_device *phydev, rtk_uint32 gap);
extern ret_t rtl8367c_getAsicEgressFlowControlQueueDropGap(struct phy_device *phydev, rtk_uint32 *pGap);
extern ret_t rtl8367c_setAsicEgressFlowControlPortDropThreshold(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 threshold);
extern ret_t rtl8367c_getAsicEgressFlowControlPortDropThreshold(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pThreshold);
extern ret_t rtl8367c_setAsicEgressFlowControlQueueDropThreshold(struct phy_device *phydev, rtk_uint32 qid, rtk_uint32 threshold);
extern ret_t rtl8367c_getAsicEgressFlowControlQueueDropThreshold(struct phy_device *phydev, rtk_uint32 qid, rtk_uint32 *pThreshold);
extern ret_t rtl8367c_getAsicEgressQueueEmptyPortMask(struct phy_device *phydev, rtk_uint32 *pPortmask);
extern ret_t rtl8367c_getAsicTotalPage(struct phy_device *phydev, rtk_uint32 *pPageCount);
extern ret_t rtl8367c_getAsicPulbicPage(struct phy_device *phydev, rtk_uint32 *pPageCount);
extern ret_t rtl8367c_getAsicMaxTotalPage(struct phy_device *phydev, rtk_uint32 *pPageCount);
extern ret_t rtl8367c_getAsicMaxPulbicPage(struct phy_device *phydev, rtk_uint32 *pPageCount);
extern ret_t rtl8367c_getAsicPortPage(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pPageCount);
extern ret_t rtl8367c_getAsicPortPageMax(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pPageCount);
extern ret_t rtl8367c_setAsicFlowControlEgressPortIndep(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enable);
extern ret_t rtl8367c_getAsicFlowControlEgressPortIndep(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pEnable);

#endif /*_RTL8367C_ASICDRV_FC_H_*/

