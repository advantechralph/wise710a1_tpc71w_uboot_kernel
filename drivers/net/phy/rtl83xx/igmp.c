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
 * Purpose : RTK switch high-level API for RTL8367/RTL8367C
 * Feature : Here is a list of all functions and variables in IGMP module.
 *
 */

#include "rtk_switch.h"
#include "rtk_error.h"
#include "igmp.h"
#include <linux/string.h>

#include "rtl8367c_asicdrv.h"
#include "rtl8367c_asicdrv_igmp.h"
#include "rtl8367c_asicdrv_lut.h"


/* Function Name:
 *      rtk_igmp_init
 * Description:
 *      This API enables H/W IGMP and set a default initial configuration.
 * Input:
 *      None.
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 *      This API enables H/W IGMP and set a default initial configuration.
 */
rtk_api_ret_t rtk_igmp_init(struct phy_device *phydev)
{
    rtk_api_ret_t retVal;
    rtk_port_t port;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if ((retVal = rtl8367c_setAsicLutIpMulticastLookup(phydev, ENABLED))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicLutIpLookupMethod(phydev, 1))!=RT_ERR_OK)
        return retVal;

    RTK_SCAN_ALL_PHY_PORTMASK(phydev, port)
    {
        if ((retVal = rtl8367c_setAsicIGMPv1Opeartion(phydev, port, PROTOCOL_OP_ASIC))!=RT_ERR_OK)
            return retVal;

        if ((retVal = rtl8367c_setAsicIGMPv2Opeartion(phydev, port, PROTOCOL_OP_ASIC))!=RT_ERR_OK)
            return retVal;

        if ((retVal = rtl8367c_setAsicIGMPv3Opeartion(phydev, port, PROTOCOL_OP_FLOOD))!=RT_ERR_OK)
            return retVal;

        if ((retVal = rtl8367c_setAsicMLDv1Opeartion(phydev, port, PROTOCOL_OP_ASIC))!=RT_ERR_OK)
            return retVal;

        if ((retVal = rtl8367c_setAsicMLDv2Opeartion(phydev, port, PROTOCOL_OP_FLOOD))!=RT_ERR_OK)
            return retVal;
    }

    if ((retVal = rtl8367c_setAsicIGMPAllowDynamicRouterPort(phydev, rtk_switch_phyPortMask_get(phydev)))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicIGMPFastLeaveEn(phydev, ENABLED))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicIGMPReportLeaveFlood(phydev, 1))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicIgmp(phydev, ENABLED))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_state_set
 * Description:
 *      This API set H/W IGMP state.
 * Input:
 *      enabled     - H/W IGMP state
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error parameter
 * Note:
 *      This API set H/W IGMP state.
 */
rtk_api_ret_t rtk_igmp_state_set(struct phy_device *phydev, rtk_enable_t enabled)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if (enabled >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicIgmp(phydev, enabled))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_state_get
 * Description:
 *      This API get H/W IGMP state.
 * Input:
 *      None.
 * Output:
 *      pEnabled        - H/W IGMP state
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error parameter
 * Note:
 *      This API set current H/W IGMP state.
 */
rtk_api_ret_t rtk_igmp_state_get(struct phy_device *phydev, rtk_enable_t *pEnabled)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(pEnabled == NULL)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIgmp(phydev, pEnabled))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_static_router_port_set
 * Description:
 *      Configure static router port
 * Input:
 *      pPortmask    - Static Port mask
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_PORT_MASK       - Error parameter
 * Note:
 *      This API set static router port
 */
rtk_api_ret_t rtk_igmp_static_router_port_set(struct phy_device *phydev, rtk_portmask_t *pPortmask)
{
    rtk_api_ret_t retVal;
    rtk_uint32 pmask;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check Valid port mask */
    if(pPortmask == NULL)
        return RT_ERR_NULL_POINTER;

    RTK_CHK_PORTMASK_VALID(phydev, pPortmask);

    if ((retVal = rtk_switch_portmask_L2P_get(phydev, pPortmask, &pmask))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicIGMPStaticRouterPort(phydev, pmask))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_static_router_port_get
 * Description:
 *      Get static router port
 * Input:
 *      None.
 * Output:
 *      pPortmask       - Static port mask
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_PORT_MASK       - Error parameter
 * Note:
 *      This API get static router port
 */
rtk_api_ret_t rtk_igmp_static_router_port_get(struct phy_device *phydev, rtk_portmask_t *pPortmask)
{
    rtk_api_ret_t retVal;
    rtk_uint32 pmask;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(pPortmask == NULL)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPStaticRouterPort(phydev, &pmask))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtk_switch_portmask_P2L_get(phydev, pmask, pPortmask))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_protocol_set
 * Description:
 *      set IGMP/MLD protocol action
 * Input:
 *      port        - Port ID
 *      protocol    - IGMP/MLD protocol
 *      action      - Per-port and per-protocol IGMP action seeting
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_PORT_MASK       - Error parameter
 * Note:
 *      This API set IGMP/MLD protocol action
 */
rtk_api_ret_t rtk_igmp_protocol_set(struct phy_device *phydev, rtk_port_t port, rtk_igmp_protocol_t protocol, rtk_igmp_action_t action)
{
    rtk_uint32      operation;
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check port valid */
    RTK_CHK_PORT_VALID(phydev, port);

    if(protocol >= PROTOCOL_END)
        return RT_ERR_INPUT;

    if(action >= IGMP_ACTION_END)
        return RT_ERR_INPUT;

    switch(action)
    {
        case IGMP_ACTION_FORWARD:
            operation = PROTOCOL_OP_FLOOD;
            break;
        case IGMP_ACTION_TRAP2CPU:
            operation = PROTOCOL_OP_TRAP;
            break;
        case IGMP_ACTION_DROP:
            operation = PROTOCOL_OP_DROP;
            break;
        case IGMP_ACTION_ASIC:
            operation = PROTOCOL_OP_ASIC;
            break;
        default:
            return RT_ERR_INPUT;
    }

    switch(protocol)
    {
        case PROTOCOL_IGMPv1:
            if ((retVal = rtl8367c_setAsicIGMPv1Opeartion(phydev, rtk_switch_port_L2P_get(phydev, port), operation))!=RT_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_IGMPv2:
            if ((retVal = rtl8367c_setAsicIGMPv2Opeartion(phydev, rtk_switch_port_L2P_get(phydev, port), operation))!=RT_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_IGMPv3:
            if ((retVal = rtl8367c_setAsicIGMPv3Opeartion(phydev, rtk_switch_port_L2P_get(phydev, port), operation))!=RT_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_MLDv1:
            if ((retVal = rtl8367c_setAsicMLDv1Opeartion(phydev, rtk_switch_port_L2P_get(phydev, port), operation))!=RT_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_MLDv2:
            if ((retVal = rtl8367c_setAsicMLDv2Opeartion(phydev, rtk_switch_port_L2P_get(phydev, port), operation))!=RT_ERR_OK)
                return retVal;

            break;
        default:
            return RT_ERR_INPUT;

    }

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_protocol_get
 * Description:
 *      set IGMP/MLD protocol action
 * Input:
 *      port        - Port ID
 *      protocol    - IGMP/MLD protocol
 *      action      - Per-port and per-protocol IGMP action seeting
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_PORT_MASK       - Error parameter
 * Note:
 *      This API set IGMP/MLD protocol action
 */
rtk_api_ret_t rtk_igmp_protocol_get(struct phy_device *phydev, rtk_port_t port, rtk_igmp_protocol_t protocol, rtk_igmp_action_t *pAction)
{
    rtk_uint32      operation;
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check port valid */
    RTK_CHK_PORT_VALID(phydev, port);

    if(protocol >= PROTOCOL_END)
        return RT_ERR_INPUT;

    if(pAction == NULL)
        return RT_ERR_NULL_POINTER;

    switch(protocol)
    {
        case PROTOCOL_IGMPv1:
            if ((retVal = rtl8367c_getAsicIGMPv1Opeartion(phydev, rtk_switch_port_L2P_get(phydev, port), &operation))!=RT_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_IGMPv2:
            if ((retVal = rtl8367c_getAsicIGMPv2Opeartion(phydev, rtk_switch_port_L2P_get(phydev, port), &operation))!=RT_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_IGMPv3:
            if ((retVal = rtl8367c_getAsicIGMPv3Opeartion(phydev, rtk_switch_port_L2P_get(phydev, port), &operation))!=RT_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_MLDv1:
            if ((retVal = rtl8367c_getAsicMLDv1Opeartion(phydev, rtk_switch_port_L2P_get(phydev, port), &operation))!=RT_ERR_OK)
                return retVal;

            break;
        case PROTOCOL_MLDv2:
            if ((retVal = rtl8367c_getAsicMLDv2Opeartion(phydev, rtk_switch_port_L2P_get(phydev, port), &operation))!=RT_ERR_OK)
                return retVal;

            break;
        default:
            return RT_ERR_INPUT;

    }

    switch(operation)
    {
        case PROTOCOL_OP_FLOOD:
            *pAction = IGMP_ACTION_FORWARD;
            break;
        case PROTOCOL_OP_TRAP:
            *pAction = IGMP_ACTION_TRAP2CPU;
            break;
        case PROTOCOL_OP_DROP:
            *pAction = IGMP_ACTION_DROP;
            break;
        case PROTOCOL_OP_ASIC:
            *pAction = IGMP_ACTION_ASIC;
            break;
        default:
            return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_fastLeave_set
 * Description:
 *      set IGMP/MLD FastLeave state
 * Input:
 *      state       - ENABLED: Enable FastLeave, DISABLED: disable FastLeave
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_INPUT           - Error Input
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 *      This API set IGMP/MLD FastLeave state
 */
rtk_api_ret_t rtk_igmp_fastLeave_set(struct phy_device *phydev, rtk_enable_t state)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(state >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicIGMPFastLeaveEn(phydev, (rtk_uint32)state))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_fastLeave_get
 * Description:
 *      get IGMP/MLD FastLeave state
 * Input:
 *      None
 * Output:
 *      pState      - ENABLED: Enable FastLeave, DISABLED: disable FastLeave
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_NULL_POINTER    - NULL pointer
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 *      This API get IGMP/MLD FastLeave state
 */
rtk_api_ret_t rtk_igmp_fastLeave_get(struct phy_device *phydev, rtk_enable_t *pState)
{
    rtk_uint32      fast_leave;
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(pState == NULL)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPFastLeaveEn(phydev, &fast_leave))!=RT_ERR_OK)
        return retVal;

    *pState = ((fast_leave == 1) ? ENABLED : DISABLED);
    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_maxGroup_set
 * Description:
 *      Set per port multicast group learning limit.
 * Input:
 *      port        - Port ID
 *      group       - The number of multicast group learning limit.
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_PORT_ID         - Error Port ID
 *      RT_ERR_OUT_OF_RANGE    - parameter out of range
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 *      This API set per port multicast group learning limit.
 */
rtk_api_ret_t rtk_igmp_maxGroup_set(struct phy_device *phydev, rtk_port_t port, rtk_uint32 group)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check port valid */
    RTK_CHK_PORT_VALID(phydev, port);

    if(group > RTL8367C_IGMP_MAX_GOUP)
        return RT_ERR_OUT_OF_RANGE;

    if ((retVal = rtl8367c_setAsicIGMPPortMAXGroup(phydev, rtk_switch_port_L2P_get(phydev, port), group))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_maxGroup_get
 * Description:
 *      Get per port multicast group learning limit.
 * Input:
 *      port        - Port ID
 * Output:
 *      pGroup      - The number of multicast group learning limit.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_PORT_ID         - Error Port ID
 *      RT_ERR_NULL_POINTER    - Null pointer
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 *      This API get per port multicast group learning limit.
 */
rtk_api_ret_t rtk_igmp_maxGroup_get(struct phy_device *phydev, rtk_port_t port, rtk_uint32 *pGroup)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check port valid */
    RTK_CHK_PORT_VALID(phydev, port);

    if(pGroup == NULL)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPPortMAXGroup(phydev, rtk_switch_port_L2P_get(phydev, port), pGroup))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_currentGroup_get
 * Description:
 *      Get per port multicast group learning count.
 * Input:
 *      port        - Port ID
 * Output:
 *      pGroup      - The number of multicast group learning count.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_PORT_ID         - Error Port ID
 *      RT_ERR_NULL_POINTER    - Null pointer
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 *      This API get per port multicast group learning count.
 */
rtk_api_ret_t rtk_igmp_currentGroup_get(struct phy_device *phydev, rtk_port_t port, rtk_uint32 *pGroup)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check port valid */
    RTK_CHK_PORT_VALID(phydev, port);

    if(pGroup == NULL)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPPortCurrentGroup(phydev, rtk_switch_port_L2P_get(phydev, port), pGroup))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_tableFullAction_set
 * Description:
 *      set IGMP/MLD Table Full Action
 * Input:
 *      action      - Table Full Action
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_INPUT           - Error Input
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 */
rtk_api_ret_t rtk_igmp_tableFullAction_set(struct phy_device *phydev, rtk_igmp_tableFullAction_t action)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(action >= IGMP_TABLE_FULL_OP_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicIGMPTableFullOP(phydev, (rtk_uint32)action))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_tableFullAction_get
 * Description:
 *      get IGMP/MLD Table Full Action
 * Input:
 *      None
 * Output:
 *      pAction     - Table Full Action
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_NULL_POINTER    - Null pointer
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 */
rtk_api_ret_t rtk_igmp_tableFullAction_get(struct phy_device *phydev, rtk_igmp_tableFullAction_t *pAction)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pAction)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPTableFullOP(phydev, (rtk_uint32 *)pAction))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_checksumErrorAction_set
 * Description:
 *      set IGMP/MLD Checksum Error Action
 * Input:
 *      action      - Checksum error Action
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_INPUT           - Error Input
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 */
rtk_api_ret_t rtk_igmp_checksumErrorAction_set(struct phy_device *phydev, rtk_igmp_checksumErrorAction_t action)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(action >= IGMP_CRC_ERR_OP_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicIGMPCRCErrOP(phydev, (rtk_uint32)action))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_igmp_checksumErrorAction_get
 * Description:
 *      get IGMP/MLD Checksum Error Action
 * Input:
 *      None
 * Output:
 *      pAction     - Checksum error Action
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_NULL_POINTER    - Null pointer
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 */
rtk_api_ret_t rtk_igmp_checksumErrorAction_get(struct phy_device *phydev, rtk_igmp_checksumErrorAction_t *pAction)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pAction)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPCRCErrOP(phydev, (rtk_uint32 *)pAction))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_leaveTimer_set
 * Description:
 *      set IGMP/MLD Leave timer
 * Input:
 *      timer       - Leave timer
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_INPUT           - Error Input
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 */
rtk_api_ret_t rtk_igmp_leaveTimer_set(struct phy_device *phydev, rtk_uint32 timer)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(timer > RTL8367C_MAX_LEAVE_TIMER)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicIGMPLeaveTimer(phydev, timer))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_leaveTimer_get
 * Description:
 *      get IGMP/MLD Leave timer
 * Input:
 *      None
 * Output:
 *      pTimer      - Leave Timer.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_NULL_POINTER    - Null pointer
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 */
rtk_api_ret_t rtk_igmp_leaveTimer_get(struct phy_device *phydev, rtk_uint32 *pTimer)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pTimer)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPLeaveTimer(phydev, pTimer))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_queryInterval_set
 * Description:
 *      set IGMP/MLD Query Interval
 * Input:
 *      interval     - Query Interval
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_INPUT           - Error Input
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 */
rtk_api_ret_t rtk_igmp_queryInterval_set(struct phy_device *phydev, rtk_uint32 interval)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(interval > RTL8367C_MAX_QUERY_INT)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicIGMPQueryInterval(phydev, interval))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_queryInterval_get
 * Description:
 *      get IGMP/MLD Query Interval
 * Input:
 *      None.
 * Output:
 *      pInterval   - Query Interval
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_NULL_POINTER    - Null pointer
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 */
rtk_api_ret_t rtk_igmp_queryInterval_get(struct phy_device *phydev, rtk_uint32 *pInterval)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pInterval)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPQueryInterval(phydev, pInterval))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_robustness_set
 * Description:
 *      set IGMP/MLD Robustness value
 * Input:
 *      robustness     - Robustness value
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_INPUT           - Error Input
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 */
rtk_api_ret_t rtk_igmp_robustness_set(struct phy_device *phydev, rtk_uint32 robustness)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(robustness > RTL8367C_MAX_ROB_VAR)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicIGMPRobVar(phydev, robustness))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_robustness_get
 * Description:
 *      get IGMP/MLD Robustness value
 * Input:
 *      None
 * Output:
 *      pRobustness     - Robustness value.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_NULL_POINTER    - Null pointer
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 */
rtk_api_ret_t rtk_igmp_robustness_get(struct phy_device *phydev, rtk_uint32 *pRobustness)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pRobustness)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPRobVar(phydev, pRobustness))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_dynamicRouterRortAllow_set
 * Description:
 *      Configure dynamic router port allow option
 * Input:
 *      pPortmask    - Dynamic Port allow mask
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_PORT_MASK       - Error parameter
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_dynamicRouterPortAllow_set(struct phy_device *phydev, rtk_portmask_t *pPortmask)
{
    rtk_api_ret_t   retVal;
    rtk_uint32 pmask;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pPortmask)
        return RT_ERR_NULL_POINTER;

    RTK_CHK_PORTMASK_VALID(phydev, pPortmask);

    if ((retVal = rtk_switch_portmask_L2P_get(phydev, pPortmask, &pmask))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicIGMPAllowDynamicRouterPort(phydev, pmask))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_dynamicRouterRortAllow_get
 * Description:
 *      Get dynamic router port allow option
 * Input:
 *      None.
 * Output:
 *      pPortmask    - Dynamic Port allow mask
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_NULL_POINTER    - Null pointer
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_PORT_MASK       - Error parameter
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_dynamicRouterPortAllow_get(struct phy_device *phydev, rtk_portmask_t *pPortmask)
{
    rtk_api_ret_t   retVal;
    rtk_uint32 pmask;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pPortmask)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPAllowDynamicRouterPort(phydev, &pmask))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtk_switch_portmask_P2L_get(phydev, pmask, pPortmask))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_dynamicRouterPort_get
 * Description:
 *      Get dynamic router port
 * Input:
 *      None.
 * Output:
 *      pDynamicRouterPort    - Dynamic Router Port
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_NULL_POINTER    - Null pointer
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_PORT_MASK       - Error parameter
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_dynamicRouterPort_get(struct phy_device *phydev, rtk_igmp_dynamicRouterPort_t *pDynamicRouterPort)
{
    rtk_api_ret_t   retVal;
    rtk_uint32 port;
    rtk_uint32 timer;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pDynamicRouterPort)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPdynamicRouterPort1(phydev, &port, &timer))!= RT_ERR_OK)
        return retVal;

    if (port == RTL8367C_ROUTER_PORT_INVALID)
    {
        pDynamicRouterPort->dynamicRouterPort0Valid = DISABLED;
        pDynamicRouterPort->dynamicRouterPort0      = 0;
        pDynamicRouterPort->dynamicRouterPort0Timer = 0;
    }
    else
    {
        pDynamicRouterPort->dynamicRouterPort0Valid = ENABLED;
        pDynamicRouterPort->dynamicRouterPort0      = rtk_switch_port_P2L_get(phydev, port);
        pDynamicRouterPort->dynamicRouterPort0Timer = timer;
    }

    if ((retVal = rtl8367c_getAsicIGMPdynamicRouterPort2(phydev, &port, &timer))!= RT_ERR_OK)
        return retVal;

    if (port == RTL8367C_ROUTER_PORT_INVALID)
    {
        pDynamicRouterPort->dynamicRouterPort1Valid = DISABLED;
        pDynamicRouterPort->dynamicRouterPort1      = 0;
        pDynamicRouterPort->dynamicRouterPort1Timer = 0;
    }
    else
    {
        pDynamicRouterPort->dynamicRouterPort1Valid = ENABLED;
        pDynamicRouterPort->dynamicRouterPort1      = rtk_switch_port_P2L_get(phydev, port);
        pDynamicRouterPort->dynamicRouterPort1Timer = timer;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_suppressionEnable_set
 * Description:
 *      Configure IGMPv1/v2 & MLDv1 Report/Leave/Done suppression
 * Input:
 *      reportSuppression   - Report suppression
 *      leaveSuppression    - Leave suppression
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error Input
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_suppressionEnable_set(struct phy_device *phydev, rtk_enable_t reportSuppression, rtk_enable_t leaveSuppression)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(reportSuppression >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if(leaveSuppression >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicIGMPSuppression(phydev, (rtk_uint32)reportSuppression, (rtk_uint32)leaveSuppression))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_suppressionEnable_get
 * Description:
 *      Get IGMPv1/v2 & MLDv1 Report/Leave/Done suppression
 * Input:
 *      None
 * Output:
 *      pReportSuppression  - Report suppression
 *      pLeaveSuppression   - Leave suppression
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_NULL_POINTER    - Null pointer
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_suppressionEnable_get(struct phy_device *phydev, rtk_enable_t *pReportSuppression, rtk_enable_t *pLeaveSuppression)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pReportSuppression)
        return RT_ERR_NULL_POINTER;

    if(NULL == pLeaveSuppression)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPSuppression(phydev, (rtk_uint32 *)pReportSuppression, (rtk_uint32 *)pLeaveSuppression))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_portRxPktEnable_set
 * Description:
 *      Configure IGMP/MLD RX Packet configuration
 * Input:
 *      port       - Port ID
 *      pRxCfg     - RX Packet Configuration
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error Input
 *      RT_ERR_NULL_POINTER    - Null pointer
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_portRxPktEnable_set(struct phy_device *phydev, rtk_port_t port, rtk_igmp_rxPktEnable_t *pRxCfg)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check port valid */
    RTK_CHK_PORT_VALID(phydev, port);

    if(NULL == pRxCfg)
        return RT_ERR_NULL_POINTER;

    if(pRxCfg->rxQuery >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if(pRxCfg->rxReport >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if(pRxCfg->rxLeave >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if(pRxCfg->rxMRP >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if(pRxCfg->rxMcast >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicIGMPQueryRX(phydev, rtk_switch_port_L2P_get(phydev, port), (rtk_uint32)pRxCfg->rxQuery))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicIGMPReportRX(phydev, rtk_switch_port_L2P_get(phydev, port), (rtk_uint32)pRxCfg->rxReport))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicIGMPLeaveRX(phydev, rtk_switch_port_L2P_get(phydev, port), (rtk_uint32)pRxCfg->rxLeave))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicIGMPMRPRX(phydev, rtk_switch_port_L2P_get(phydev, port), (rtk_uint32)pRxCfg->rxMRP))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicIGMPMcDataRX(phydev, rtk_switch_port_L2P_get(phydev, port), (rtk_uint32)pRxCfg->rxMcast))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_portRxPktEnable_get
 * Description:
 *      Get IGMP/MLD RX Packet configuration
 * Input:
 *      port       - Port ID
 *      pRxCfg     - RX Packet Configuration
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error Input
 *      RT_ERR_NULL_POINTER    - Null pointer
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_portRxPktEnable_get(struct phy_device *phydev, rtk_port_t port, rtk_igmp_rxPktEnable_t *pRxCfg)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check port valid */
    RTK_CHK_PORT_VALID(phydev, port);

    if(NULL == pRxCfg)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPQueryRX(phydev, rtk_switch_port_L2P_get(phydev, port), (rtk_uint32 *)&(pRxCfg->rxQuery)))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicIGMPReportRX(phydev, rtk_switch_port_L2P_get(phydev, port), (rtk_uint32 *)&(pRxCfg->rxReport)))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicIGMPLeaveRX(phydev, rtk_switch_port_L2P_get(phydev, port), (rtk_uint32 *)&(pRxCfg->rxLeave)))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicIGMPMRPRX(phydev, rtk_switch_port_L2P_get(phydev, port), (rtk_uint32 *)&(pRxCfg->rxMRP)))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicIGMPMcDataRX(phydev, rtk_switch_port_L2P_get(phydev, port), (rtk_uint32 *)&(pRxCfg->rxMcast)))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_groupInfo_get
 * Description:
 *      Get IGMP/MLD Group database
 * Input:
 *      indes       - Index (0~255)
 * Output:
 *      pGroup      - Group database information.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error Input
 *      RT_ERR_NULL_POINTER    - Null pointer
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_groupInfo_get(struct phy_device *phydev, rtk_uint32 index, rtk_igmp_groupInfo_t *pGroup)
{
    rtk_api_ret_t   retVal;
    rtk_uint32      valid;
    rtl8367c_igmpgroup  grp;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check index */
    if(index > RTL8367C_IGMP_MAX_GOUP)
        return RT_ERR_INPUT;

    if(NULL == pGroup)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPGroup(phydev, index, &valid, &grp))!=RT_ERR_OK)
        return retVal;

    memset(pGroup, 0x00, sizeof(rtk_igmp_groupInfo_t));
    pGroup->valid = valid;
    pGroup->reportSuppFlag = grp.report_supp_flag;

    if(grp.p0_timer != 0)
    {
        RTK_PORTMASK_PORT_SET((pGroup->member), rtk_switch_port_P2L_get(phydev, 0));
        pGroup->timer[rtk_switch_port_P2L_get(phydev, 0)] = grp.p0_timer;
    }

    if(grp.p1_timer != 0)
    {
        RTK_PORTMASK_PORT_SET((pGroup->member), rtk_switch_port_P2L_get(phydev, 1));
        pGroup->timer[rtk_switch_port_P2L_get(phydev, 1)] = grp.p1_timer;
    }

    if(grp.p2_timer != 0)
    {
        RTK_PORTMASK_PORT_SET((pGroup->member), rtk_switch_port_P2L_get(phydev, 2));
        pGroup->timer[rtk_switch_port_P2L_get(phydev, 2)] = grp.p2_timer;
    }

    if(grp.p3_timer != 0)
    {
        RTK_PORTMASK_PORT_SET((pGroup->member), rtk_switch_port_P2L_get(phydev, 3));
        pGroup->timer[rtk_switch_port_P2L_get(phydev, 3)] = grp.p3_timer;
    }

    if(grp.p4_timer != 0)
    {
        RTK_PORTMASK_PORT_SET((pGroup->member), rtk_switch_port_P2L_get(phydev, 4));
        pGroup->timer[rtk_switch_port_P2L_get(phydev, 4)] = grp.p4_timer;
    }

    if(grp.p5_timer != 0)
    {
        RTK_PORTMASK_PORT_SET((pGroup->member), rtk_switch_port_P2L_get(phydev, 5));
        pGroup->timer[rtk_switch_port_P2L_get(phydev, 5)] = grp.p5_timer;
    }

    if(grp.p6_timer != 0)
    {
        RTK_PORTMASK_PORT_SET((pGroup->member), rtk_switch_port_P2L_get(phydev, 6));
        pGroup->timer[rtk_switch_port_P2L_get(phydev, 6)] = grp.p6_timer;
    }

    if(grp.p7_timer != 0)
    {
        RTK_PORTMASK_PORT_SET((pGroup->member), rtk_switch_port_P2L_get(phydev, 7));
        pGroup->timer[rtk_switch_port_P2L_get(phydev, 7)] = grp.p7_timer;
    }

    if(grp.p8_timer != 0)
    {
        RTK_PORTMASK_PORT_SET((pGroup->member), rtk_switch_port_P2L_get(phydev, 8));
        pGroup->timer[rtk_switch_port_P2L_get(phydev, 8)] = grp.p8_timer;
    }

    if(grp.p9_timer != 0)
    {
        RTK_PORTMASK_PORT_SET((pGroup->member), rtk_switch_port_P2L_get(phydev, 9));
        pGroup->timer[rtk_switch_port_P2L_get(phydev, 9)] = grp.p9_timer;
    }

    if(grp.p10_timer != 0)
    {
        RTK_PORTMASK_PORT_SET((pGroup->member), rtk_switch_port_P2L_get(phydev, 10));
        pGroup->timer[rtk_switch_port_P2L_get(phydev, 10)] = grp.p10_timer;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_ReportLeaveFwdAction_set
 * Description:
 *      Set Report Leave packet forwarding action
 * Input:
 *      action      - Action
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error Input
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_ReportLeaveFwdAction_set(struct phy_device *phydev, rtk_igmp_ReportLeaveFwdAct_t action)
{
    rtk_api_ret_t   retVal;
    rtk_uint32      regData;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    switch(action)
    {
        case IGMP_REPORT_LEAVE_TO_ROUTER:
            regData = 1;
            break;
        case IGMP_REPORT_LEAVE_TO_ALLPORT:
            regData = 2;
            break;
        case IGMP_REPORT_LEAVE_TO_ROUTER_PORT_ADV:
            regData = 3;
            break;
        default:
            return RT_ERR_INPUT;
    }

    if ((retVal = rtl8367c_setAsicIGMPReportLeaveFlood(phydev, regData))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_ReportLeaveFwdAction_get
 * Description:
 *      Get Report Leave packet forwarding action
 * Input:
 *      action      - Action
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error Input
 *      RT_ERR_NULL_POINTER    - Null Pointer
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_ReportLeaveFwdAction_get(struct phy_device *phydev, rtk_igmp_ReportLeaveFwdAct_t *pAction)
{
    rtk_api_ret_t   retVal;
    rtk_uint32      regData;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pAction)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPReportLeaveFlood(phydev, &regData))!=RT_ERR_OK)
        return retVal;

    switch(regData)
    {
        case 1:
            *pAction = IGMP_REPORT_LEAVE_TO_ROUTER;
            break;
        case 2:
            *pAction = IGMP_REPORT_LEAVE_TO_ALLPORT;
            break;
        case 3:
            *pAction = IGMP_REPORT_LEAVE_TO_ROUTER_PORT_ADV;
            break;
        default:
            return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_dropLeaveZeroEnable_set
 * Description:
 *      Set the function of droppping Leave packet with group IP = 0.0.0.0
 * Input:
 *      enabled      - Action 1: drop, 0:pass
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error Input
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_dropLeaveZeroEnable_set(struct phy_device *phydev, rtk_enable_t enabled)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(enabled >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicIGMPDropLeaveZero(phydev, enabled))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;

}

/* Function Name:
 *      rtk_igmp_dropLeaveZeroEnable_get
 * Description:
 *      Get the function of droppping Leave packet with group IP = 0.0.0.0
 * Input:
 *      None
 * Output:
 *      pEnabled.   - Action 1: drop, 0:pass
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error Input
 *      RT_ERR_NULL_POINTER    - Null Pointer
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_dropLeaveZeroEnable_get(struct phy_device *phydev, rtk_enable_t *pEnabled)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pEnabled)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPDropLeaveZero(phydev, (rtk_uint32 *)pEnabled))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;

}

/* Function Name:
 *      rtk_igmp_bypassGroupRange_set
 * Description:
 *      Set Bypass group
 * Input:
 *      group       - bypassed group
 *      enabled     - enabled 1: Bypassed, 0: not bypass
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error Input
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_bypassGroupRange_set(struct phy_device *phydev, rtk_igmp_bypassGroup_t group, rtk_enable_t enabled)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(group >= IGMP_BYPASS_GROUP_END)
        return RT_ERR_INPUT;

    if(enabled >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicIGMPBypassGroup(phydev, group, enabled))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_igmp_bypassGroupRange_get
 * Description:
 *      get Bypass group
 * Input:
 *      group       - bypassed group
 * Output:
 *      pEnable     - enabled 1: Bypassed, 0: not bypass
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error Input
 *      RT_ERR_NULL_POINTER    - Null Pointer
 * Note:
 *
 */
rtk_api_ret_t rtk_igmp_bypassGroupRange_get(struct phy_device *phydev, rtk_igmp_bypassGroup_t group, rtk_enable_t *pEnable)
{
    rtk_api_ret_t   retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(group >= IGMP_BYPASS_GROUP_END)
        return RT_ERR_INPUT;

    if(NULL == pEnable)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicIGMPBypassGroup(phydev, group, pEnable))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}
