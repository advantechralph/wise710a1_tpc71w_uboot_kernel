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
 * Feature : Here is a list of all functions and variables in OAM(802.3ah)  module.
 *
 */

#include "rtk_switch.h"
#include "rtk_error.h"
#include "oam.h"
#include <linux/string.h>

#include "rtl8367c_asicdrv.h"
#include "rtl8367c_asicdrv_oam.h"


/* Module Name : OAM */

/* Function Name:
 *      rtk_oam_init
 * Description:
 *      Initialize oam module.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      Must initialize oam module before calling any oam APIs.
 */
rtk_api_ret_t rtk_oam_init(struct phy_device *phydev)
{
    return RT_ERR_OK;
} /* end of rtk_oam_init */


/* Function Name:
 *      rtk_oam_state_set
 * Description:
 *      This API set OAM state.
 * Input:
 *      enabled     -OAMstate
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error parameter
 * Note:
 *      This API set OAM state.
 */
rtk_api_ret_t rtk_oam_state_set(struct phy_device *phydev, rtk_enable_t enabled)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if (enabled >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicOamEnable(phydev, enabled))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_oam_state_get
 * Description:
 *      This API get OAM state.
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
 *      This API set current OAM state.
 */
rtk_api_ret_t rtk_oam_state_get(struct phy_device *phydev, rtk_enable_t *pEnabled)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if ((retVal = rtl8367c_getAsicOamEnable(phydev, pEnabled))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}



/* Function Name:
 *      rtk_oam_parserAction_set
 * Description:
 *      Set OAM parser action
 * Input:
 *      port    - port id
 *      action  - parser action
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
rtk_api_ret_t  rtk_oam_parserAction_set(struct phy_device *phydev, rtk_port_t port, rtk_oam_parser_act_t action)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(phydev, port);

    if (action >= OAM_PARSER_ACTION_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicOamParser(phydev, rtk_switch_port_L2P_get(phydev, port), action))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_oam_parserAction_set
 * Description:
 *      Get OAM parser action
 * Input:
 *      port    - port id
 * Output:
 *      pAction  - parser action
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
rtk_api_ret_t  rtk_oam_parserAction_get(struct phy_device *phydev, rtk_port_t port, rtk_oam_parser_act_t *pAction)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(phydev, port);

    if ((retVal = rtl8367c_getAsicOamParser(phydev, rtk_switch_port_L2P_get(phydev, port), pAction))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}


/* Function Name:
 *      rtk_oam_multiplexerAction_set
 * Description:
 *      Set OAM multiplexer action
 * Input:
 *      port    - port id
 *      action  - parser action
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
rtk_api_ret_t  rtk_oam_multiplexerAction_set(struct phy_device *phydev, rtk_port_t port, rtk_oam_multiplexer_act_t action)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(phydev, port);

    if (action >= OAM_MULTIPLEXER_ACTION_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicOamMultiplexer(phydev, rtk_switch_port_L2P_get(phydev, port), action))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_oam_parserAction_set
 * Description:
 *      Get OAM multiplexer action
 * Input:
 *      port    - port id
 * Output:
 *      pAction  - parser action
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
rtk_api_ret_t  rtk_oam_multiplexerAction_get(struct phy_device *phydev, rtk_port_t port, rtk_oam_multiplexer_act_t *pAction)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(phydev, port);

    if ((retVal = rtl8367c_getAsicOamMultiplexer(phydev, rtk_switch_port_L2P_get(phydev, port), pAction))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}


