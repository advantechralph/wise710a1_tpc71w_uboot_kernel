#ifndef _RTL8367C_ASICDRV_QOS_H_
#define _RTL8367C_ASICDRV_QOS_H_

#include "rtl8367c_asicdrv.h"

#define RTL8367C_DECISIONPRIMAX    0xFF

/* enum Priority Selection Types */
enum PRIDECISION
{
    PRIDEC_PORT = 0,
    PRIDEC_ACL,
    PRIDEC_DSCP,
    PRIDEC_1Q,
    PRIDEC_1AD,
    PRIDEC_CVLAN,
    PRIDEC_DA,
    PRIDEC_SA,
    PRIDEC_END,
};

/* enum Priority Selection Index */
enum RTL8367C_PRIDEC_TABLE
{
    PRIDEC_IDX0 = 0,
    PRIDEC_IDX1,
    PRIDEC_IDX_END,
};

enum RTL8367C_DOT1P_PRISEL
{
    DOT1P_PRISEL_USER =  0,
    DOT1P_PRISEL_TAG,
    DOT1P_PRISEL_END
};

enum RTL8367C_DSCP_PRISEL
{
    DSCP_PRISEL_INTERNAL =  0,
    DSCP_PRISEL_DSCP,
    DSCP_PRISEL_USER ,
    DSCP_PRISEL_END
};


extern ret_t rtl8367c_setAsicRemarkingDot1pAbility(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicRemarkingDot1pAbility(struct phy_device *phydev, rtk_uint32 port, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicRemarkingDot1pParameter(struct phy_device *phydev, rtk_uint32 priority, rtk_uint32 newPriority );
extern ret_t rtl8367c_getAsicRemarkingDot1pParameter(struct phy_device *phydev, rtk_uint32 priority, rtk_uint32 *pNewPriority );
extern  ret_t rtl8367c_setAsicRemarkingDot1pSrc(struct phy_device *phydev, rtk_uint32 type);
extern  ret_t rtl8367c_getAsicRemarkingDot1pSrc(struct phy_device *phydev, rtk_uint32 *pType);
extern ret_t rtl8367c_setAsicRemarkingDscpAbility(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicRemarkingDscpAbility(struct phy_device *phydev, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicRemarkingDscpParameter(struct phy_device *phydev, rtk_uint32 priority, rtk_uint32 newDscp );
extern ret_t rtl8367c_getAsicRemarkingDscpParameter(struct phy_device *phydev, rtk_uint32 priority, rtk_uint32* pNewDscp );

extern ret_t rtl8367c_setAsicPriorityDot1qRemapping(struct phy_device *phydev, rtk_uint32 srcpriority, rtk_uint32 priority );
extern ret_t rtl8367c_getAsicPriorityDot1qRemapping(struct phy_device *phydev, rtk_uint32 srcpriority, rtk_uint32 *pPriority );
extern ret_t rtl8367c_setAsicPriorityDscpBased(struct phy_device *phydev, rtk_uint32 dscp, rtk_uint32 priority );
extern ret_t rtl8367c_getAsicPriorityDscpBased(struct phy_device *phydev, rtk_uint32 dscp, rtk_uint32 *pPriority );
extern ret_t rtl8367c_setAsicPriorityPortBased(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 priority );
extern ret_t rtl8367c_getAsicPriorityPortBased(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pPriority );
extern ret_t rtl8367c_setAsicPriorityDecision(struct phy_device *phydev, rtk_uint32 index, rtk_uint32 prisrc, rtk_uint32 decisionPri);
extern ret_t rtl8367c_getAsicPriorityDecision(struct phy_device *phydev, rtk_uint32 index, rtk_uint32 prisrc, rtk_uint32* pDecisionPri);
extern ret_t rtl8367c_setAsicPriorityToQIDMappingTable(struct phy_device *phydev, rtk_uint32 qnum, rtk_uint32 priority, rtk_uint32 qid );
extern ret_t rtl8367c_getAsicPriorityToQIDMappingTable(struct phy_device *phydev, rtk_uint32 qnum, rtk_uint32 priority, rtk_uint32* pQid);
extern ret_t rtl8367c_setAsicOutputQueueMappingIndex(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 qnum );
extern ret_t rtl8367c_getAsicOutputQueueMappingIndex(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pQnum );

extern ret_t rtl8367c_setAsicRemarkingDscpSrc(struct phy_device *phydev, rtk_uint32 type);
extern ret_t rtl8367c_getAsicRemarkingDscpSrc(struct phy_device *phydev, rtk_uint32 *pType);
extern ret_t rtl8367c_setAsicRemarkingDscp2Dscp(struct phy_device *phydev, rtk_uint32 dscp, rtk_uint32 rmkDscp);
extern ret_t rtl8367c_getAsicRemarkingDscp2Dscp(struct phy_device *phydev, rtk_uint32 dscp, rtk_uint32 *pRmkDscp);

extern ret_t rtl8367c_setAsicPortPriorityDecisionIndex(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 index );
extern ret_t rtl8367c_getAsicPortPriorityDecisionIndex(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pIndex );

#endif /*#ifndef _RTL8367C_ASICDRV_QOS_H_*/

