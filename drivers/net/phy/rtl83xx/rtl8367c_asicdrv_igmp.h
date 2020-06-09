#ifndef _RTL8367C_ASICDRV_IGMP_H_
#define _RTL8367C_ASICDRV_IGMP_H_

/****************************************************************/
/* Header File inclusion                                        */
/****************************************************************/
#include "rtl8367c_asicdrv.h"

#define RTL8367C_MAX_LEAVE_TIMER        (7)
#define RTL8367C_MAX_QUERY_INT          (0xFFFF)
#define RTL8367C_MAX_ROB_VAR            (7)

#define RTL8367C_IGMP_GOUP_NO           (256)
#define RTL8367C_IGMP_MAX_GOUP          (0xFF)
#define RTL8367C_IGMP_GRP_BLEN          (3)
#define RTL8367C_ROUTER_PORT_INVALID    (0xF)

enum RTL8367C_IGMPTABLE_FULL_OP
{
    TABLE_FULL_FORWARD = 0,
    TABLE_FULL_DROP,
    TABLE_FULL_TRAP,
    TABLE_FULL_OP_END
};

enum RTL8367C_CRC_ERR_OP
{
    CRC_ERR_DROP = 0,
    CRC_ERR_TRAP,
    CRC_ERR_FORWARD,
    CRC_ERR_OP_END
};

enum RTL8367C_IGMP_MLD_PROTOCOL_OP
{
    PROTOCOL_OP_ASIC = 0,
    PROTOCOL_OP_FLOOD,
    PROTOCOL_OP_TRAP,
    PROTOCOL_OP_DROP,
    PROTOCOL_OP_END
};

enum RTL8367C_IGMP_MLD_BYPASS_GROUP
{
    BYPASS_224_0_0_X = 0,
    BYPASS_224_0_1_X,
    BYPASS_239_255_255_X,
    BYPASS_IPV6_00XX,
    BYPASS_GROUP_END
};

typedef struct
{
    rtk_uint32 p0_timer;
    rtk_uint32 p1_timer;
    rtk_uint32 p2_timer;
    rtk_uint32 p3_timer;
    rtk_uint32 p4_timer;
    rtk_uint32 p5_timer;
    rtk_uint32 p6_timer;
    rtk_uint32 p7_timer;
    rtk_uint32 p8_timer;
    rtk_uint32 p9_timer;
    rtk_uint32 p10_timer;
    rtk_uint32 report_supp_flag;

}rtl8367c_igmpgroup;
/*
 * Copyright (C) 2013 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 76306 $
 * $Date: 2017-03-08 15:13:58 +0800 (週三, 08 三月 2017) $
 *
 * Purpose : RTL8367C switch high-level API for RTL8367C
 * Feature : IGMP related functions
 *
 */
#include "rtl8367c_asicdrv_igmp.h"

ret_t rtl8367c_setAsicIgmp(struct phy_device *phydev, rtk_uint32 enabled);
ret_t rtl8367c_getAsicIgmp(struct phy_device *phydev, rtk_uint32 *pEnabled);
ret_t rtl8367c_setAsicIpMulticastVlanLeaky(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enabled );
ret_t rtl8367c_getAsicIpMulticastVlanLeaky(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pEnabled );
ret_t rtl8367c_setAsicIGMPTableFullOP(struct phy_device *phydev, rtk_uint32 operation);
ret_t rtl8367c_getAsicIGMPTableFullOP(struct phy_device *phydev, rtk_uint32 *pOperation);
ret_t rtl8367c_setAsicIGMPCRCErrOP(struct phy_device *phydev, rtk_uint32 operation);
ret_t rtl8367c_getAsicIGMPCRCErrOP(struct phy_device *phydev, rtk_uint32 *pOperation);
ret_t rtl8367c_setAsicIGMPFastLeaveEn(struct phy_device *phydev, rtk_uint32 enabled);
ret_t rtl8367c_getAsicIGMPFastLeaveEn(struct phy_device *phydev, rtk_uint32 *pEnabled);
ret_t rtl8367c_setAsicIGMPLeaveTimer(struct phy_device *phydev, rtk_uint32 leave_timer);
ret_t rtl8367c_getAsicIGMPLeaveTimer(struct phy_device *phydev, rtk_uint32 *pLeave_timer);
ret_t rtl8367c_setAsicIGMPQueryInterval(struct phy_device *phydev, rtk_uint32 interval);
ret_t rtl8367c_getAsicIGMPQueryInterval(struct phy_device *phydev, rtk_uint32 *pInterval);
ret_t rtl8367c_setAsicIGMPRobVar(struct phy_device *phydev, rtk_uint32 rob_var);
ret_t rtl8367c_getAsicIGMPRobVar(struct phy_device *phydev, rtk_uint32 *pRob_var);
ret_t rtl8367c_setAsicIGMPStaticRouterPort(struct phy_device *phydev, rtk_uint32 pmsk);
ret_t rtl8367c_getAsicIGMPStaticRouterPort(struct phy_device *phydev, rtk_uint32 *pMsk);
ret_t rtl8367c_setAsicIGMPAllowDynamicRouterPort(struct phy_device *phydev, rtk_uint32 pmsk);
ret_t rtl8367c_getAsicIGMPAllowDynamicRouterPort(struct phy_device *phydev, rtk_uint32 *pPmsk);
ret_t rtl8367c_getAsicIGMPdynamicRouterPort1(struct phy_device *phydev, rtk_uint32 *pPort, rtk_uint32 *pTimer);
ret_t rtl8367c_getAsicIGMPdynamicRouterPort2(struct phy_device *phydev, rtk_uint32 *pPort, rtk_uint32 *pTimer);
ret_t rtl8367c_setAsicIGMPSuppression(struct phy_device *phydev, rtk_uint32 report_supp_enabled, rtk_uint32 leave_supp_enabled);
ret_t rtl8367c_getAsicIGMPSuppression(struct phy_device *phydev, rtk_uint32 *pReport_supp_enabled, rtk_uint32 *pLeave_supp_enabled);
ret_t rtl8367c_setAsicIGMPQueryRX(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 allow_query);
ret_t rtl8367c_getAsicIGMPQueryRX(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pAllow_query);
ret_t rtl8367c_setAsicIGMPReportRX(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 allow_report);
ret_t rtl8367c_getAsicIGMPReportRX(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pAllow_report);
ret_t rtl8367c_setAsicIGMPLeaveRX(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 allow_leave);
ret_t rtl8367c_getAsicIGMPLeaveRX(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pAllow_leave);
ret_t rtl8367c_setAsicIGMPMRPRX(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 allow_mrp);
ret_t rtl8367c_getAsicIGMPMRPRX(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pAllow_mrp);
ret_t rtl8367c_setAsicIGMPMcDataRX(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 allow_mcdata);
ret_t rtl8367c_getAsicIGMPMcDataRX(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pAllow_mcdata);
ret_t rtl8367c_setAsicIGMPv1Opeartion(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 igmpv1_op);
ret_t rtl8367c_getAsicIGMPv1Opeartion(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pIgmpv1_op);
ret_t rtl8367c_setAsicIGMPv2Opeartion(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 igmpv2_op);
ret_t rtl8367c_getAsicIGMPv2Opeartion(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pIgmpv2_op);
ret_t rtl8367c_setAsicIGMPv3Opeartion(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 igmpv3_op);
ret_t rtl8367c_getAsicIGMPv3Opeartion(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pIgmpv3_op);
ret_t rtl8367c_setAsicMLDv1Opeartion(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 mldv1_op);
ret_t rtl8367c_getAsicMLDv1Opeartion(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pMldv1_op);
ret_t rtl8367c_setAsicMLDv2Opeartion(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 mldv2_op);
ret_t rtl8367c_getAsicMLDv2Opeartion(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pMldv2_op);
ret_t rtl8367c_setAsicIGMPPortMAXGroup(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 max_group);
ret_t rtl8367c_getAsicIGMPPortMAXGroup(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pMax_group);
ret_t rtl8367c_getAsicIGMPPortCurrentGroup(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pCurrent_group);
ret_t rtl8367c_getAsicIGMPGroup(struct phy_device *phydev, rtk_uint32 idx, rtk_uint32 *pValid, rtl8367c_igmpgroup *pGrp);
ret_t rtl8367c_setAsicIpMulticastPortIsoLeaky(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enabled);
ret_t rtl8367c_getAsicIpMulticastPortIsoLeaky(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pEnabled);
ret_t rtl8367c_setAsicIGMPReportLeaveFlood(struct phy_device *phydev, rtk_uint32 flood);
ret_t rtl8367c_getAsicIGMPReportLeaveFlood(struct phy_device *phydev, rtk_uint32 *pFlood);
ret_t rtl8367c_setAsicIGMPDropLeaveZero(struct phy_device *phydev, rtk_uint32 drop);
ret_t rtl8367c_getAsicIGMPDropLeaveZero(struct phy_device *phydev, rtk_uint32 *pDrop);
ret_t rtl8367c_setAsicIGMPBypassStormCTRL(struct phy_device *phydev, rtk_uint32 bypass);
ret_t rtl8367c_getAsicIGMPBypassStormCTRL(struct phy_device *phydev, rtk_uint32 *pBypass);
ret_t rtl8367c_setAsicIGMPIsoLeaky(struct phy_device *phydev, rtk_uint32 leaky);
ret_t rtl8367c_getAsicIGMPIsoLeaky(struct phy_device *phydev, rtk_uint32 *pLeaky);
ret_t rtl8367c_setAsicIGMPVLANLeaky(struct phy_device *phydev, rtk_uint32 leaky);
ret_t rtl8367c_getAsicIGMPVLANLeaky(struct phy_device *phydev, rtk_uint32 *pLeaky);
ret_t rtl8367c_setAsicIGMPBypassGroup(struct phy_device *phydev, rtk_uint32 bypassType, rtk_uint32 enabled);
ret_t rtl8367c_getAsicIGMPBypassGroup(struct phy_device *phydev, rtk_uint32 bypassType, rtk_uint32 *pEnabled);

#endif /*#ifndef _RTL8367C_ASICDRV_IGMP_H_*/

