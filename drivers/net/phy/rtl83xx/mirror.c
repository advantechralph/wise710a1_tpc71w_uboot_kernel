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
 * Feature : Here is a list of all functions and variables in Mirror module.
 *
 */

#include "rtk_switch.h"
#include "rtk_error.h"
#include "mirror.h"
#include <linux/string.h>
#include "rtl8367c_asicdrv.h"
#include "rtl8367c_asicdrv_mirror.h"

/* Function Name:
 *      rtk_mirror_portBased_set
 * Description:
 *      Set port mirror function.
 * Input:
 *      mirroring_port          - Monitor port.
 *      pMirrored_rx_portmask   - Rx mirror port mask.
 *      pMirrored_tx_portmask   - Tx mirror port mask.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_PORT_ID      - Invalid port number
 *      RT_ERR_PORT_MASK    - Invalid portmask.
 * Note:
 *      The API is to set mirror function of source port and mirror port.
 *      The mirror port can only be set to one port and the TX and RX mirror ports
 *      should be identical.
 */
rtk_api_ret_t rtk_mirror_portBased_set(struct phy_device *phydev, rtk_port_t mirroring_port, rtk_portmask_t *pMirrored_rx_portmask, rtk_portmask_t *pMirrored_tx_portmask)
{
    rtk_api_ret_t retVal;
    rtk_enable_t mirRx, mirTx;
    rtk_uint32 i, pmask;
    rtk_port_t source_port;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check port valid */
    RTK_CHK_PORT_VALID(phydev, mirroring_port);

    if(NULL == pMirrored_rx_portmask)
        return RT_ERR_NULL_POINTER;

    if(NULL == pMirrored_tx_portmask)
        return RT_ERR_NULL_POINTER;

    RTK_CHK_PORTMASK_VALID(phydev, pMirrored_rx_portmask);

    RTK_CHK_PORTMASK_VALID(phydev, pMirrored_tx_portmask);

    /*Mirror Sorce Port Mask Check*/
    if (pMirrored_tx_portmask->bits[0]!=pMirrored_rx_portmask->bits[0]&&pMirrored_tx_portmask->bits[0]!=0&&pMirrored_rx_portmask->bits[0]!=0)
        return RT_ERR_PORT_MASK;

     /*mirror port != source port*/
    if(RTK_PORTMASK_IS_PORT_SET((*pMirrored_tx_portmask), mirroring_port) || RTK_PORTMASK_IS_PORT_SET((*pMirrored_rx_portmask), mirroring_port))
        return RT_ERR_PORT_MASK;

    source_port = rtk_switch_maxLogicalPort_get(phydev);

    RTK_SCAN_ALL_LOG_PORT(phydev, i)
    {
        if (pMirrored_tx_portmask->bits[0]&(1<<i))
        {
            source_port = i;
            break;
        }

        if (pMirrored_rx_portmask->bits[0]&(1<<i))
        {
            source_port = i;
            break;
        }
    }

    if ((retVal = rtl8367c_setAsicPortMirror(phydev, rtk_switch_port_L2P_get(phydev, source_port), rtk_switch_port_L2P_get(phydev, mirroring_port))) != RT_ERR_OK)
        return retVal;
    if(pMirrored_rx_portmask->bits[0] != 0)
    {
        if ((retVal = rtk_switch_portmask_L2P_get(phydev, pMirrored_rx_portmask, &pmask)) != RT_ERR_OK)
            return retVal;
        if ((retVal = rtl8367c_setAsicPortMirrorMask(phydev, pmask)) != RT_ERR_OK)
            return retVal;
    }
    else
    {
        if ((retVal = rtk_switch_portmask_L2P_get(phydev, pMirrored_tx_portmask, &pmask)) != RT_ERR_OK)
            return retVal;
        if ((retVal = rtl8367c_setAsicPortMirrorMask(phydev, pmask)) != RT_ERR_OK)
            return retVal;
    }


    if (pMirrored_rx_portmask->bits[0])
        mirRx = ENABLED;
    else
        mirRx = DISABLED;

    if ((retVal = rtl8367c_setAsicPortMirrorRxFunction(phydev, mirRx)) != RT_ERR_OK)
        return retVal;

    if (pMirrored_tx_portmask->bits[0])
        mirTx = ENABLED;
    else
        mirTx = DISABLED;

    if ((retVal = rtl8367c_setAsicPortMirrorTxFunction(phydev, mirTx)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;

}

/* Function Name:
 *      rtk_mirror_portBased_get
 * Description:
 *      Get port mirror function.
 * Input:
 *      None
 * Output:
 *      pMirroring_port         - Monitor port.
 *      pMirrored_rx_portmask   - Rx mirror port mask.
 *      pMirrored_tx_portmask   - Tx mirror port mask.
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API is to get mirror function of source port and mirror port.
 */
rtk_api_ret_t rtk_mirror_portBased_get(struct phy_device *phydev, rtk_port_t *pMirroring_port, rtk_portmask_t *pMirrored_rx_portmask, rtk_portmask_t *pMirrored_tx_portmask)
{
    rtk_api_ret_t retVal;
    rtk_port_t source_port;
    rtk_enable_t mirRx, mirTx;
    rtk_uint32 sport, mport, pmask;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pMirrored_rx_portmask)
        return RT_ERR_NULL_POINTER;

    if(NULL == pMirrored_tx_portmask)
        return RT_ERR_NULL_POINTER;

    if(NULL == pMirroring_port)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicPortMirror(phydev, &sport, &mport)) != RT_ERR_OK)
        return retVal;
    source_port = rtk_switch_port_P2L_get(phydev, sport);
    *pMirroring_port = rtk_switch_port_P2L_get(phydev, mport);

    if ((retVal = rtl8367c_getAsicPortMirrorRxFunction(phydev, (rtk_uint32*)&mirRx)) != RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicPortMirrorTxFunction(phydev, (rtk_uint32*)&mirTx)) != RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicPortMirrorMask(phydev, &pmask)) != RT_ERR_OK)
        return retVal;

    if (DISABLED == mirRx)
        pMirrored_rx_portmask->bits[0]=0;
    else
    {
        if ((retVal = rtk_switch_portmask_P2L_get(phydev, pmask, pMirrored_rx_portmask)) != RT_ERR_OK)
            return retVal;
        pMirrored_rx_portmask->bits[0] |= 1<<source_port;
    }

     if (DISABLED == mirTx)
        pMirrored_tx_portmask->bits[0]=0;
    else
    {
        if ((retVal = rtk_switch_portmask_P2L_get(phydev, pmask, pMirrored_tx_portmask)) != RT_ERR_OK)
            return retVal;
        pMirrored_tx_portmask->bits[0] |= 1<<source_port;
    }

    return RT_ERR_OK;

}

/* Function Name:
 *      rtk_mirror_portIso_set
 * Description:
 *      Set mirror port isolation.
 * Input:
 *      enable |Mirror isolation status.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set mirror isolation function that prevent normal forwarding packets to miror port.
 */
rtk_api_ret_t rtk_mirror_portIso_set(struct phy_device *phydev, rtk_enable_t enable)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if (enable >= RTK_ENABLE_END)
        return RT_ERR_ENABLE;

    if ((retVal = rtl8367c_setAsicPortMirrorIsolation(phydev, enable)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_mirror_portIso_get
 * Description:
 *      Get mirror port isolation.
 * Input:
 *      None
 * Output:
 *      pEnable |Mirror isolation status.
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API is to get mirror isolation status.
 */
rtk_api_ret_t rtk_mirror_portIso_get(struct phy_device *phydev, rtk_enable_t *pEnable)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pEnable)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicPortMirrorIsolation(phydev, pEnable)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_mirror_vlanLeaky_set
 * Description:
 *      Set mirror VLAN leaky.
 * Input:
 *      txenable -TX leaky enable.
 *      rxenable - RX leaky enable.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set mirror VLAN leaky function forwarding packets to miror port.
 */
rtk_api_ret_t rtk_mirror_vlanLeaky_set(struct phy_device *phydev, rtk_enable_t txenable, rtk_enable_t rxenable)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if ((txenable >= RTK_ENABLE_END) ||(rxenable >= RTK_ENABLE_END))
        return RT_ERR_ENABLE;

    if ((retVal = rtl8367c_setAsicPortMirrorVlanTxLeaky(phydev, txenable)) != RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicPortMirrorVlanRxLeaky(phydev, rxenable)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_mirror_vlanLeaky_get
 * Description:
 *      Get mirror VLAN leaky.
 * Input:
 *      None
 * Output:
 *      pTxenable - TX leaky enable.
 *      pRxenable - RX leaky enable.
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API is to get mirror VLAN leaky status.
 */
rtk_api_ret_t rtk_mirror_vlanLeaky_get(struct phy_device *phydev, rtk_enable_t *pTxenable, rtk_enable_t *pRxenable)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if( (NULL == pTxenable) || (NULL == pRxenable) )
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicPortMirrorVlanTxLeaky(phydev, pTxenable)) != RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicPortMirrorVlanRxLeaky(phydev, pRxenable)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_mirror_isolationLeaky_set
 * Description:
 *      Set mirror Isolation leaky.
 * Input:
 *      txenable -TX leaky enable.
 *      rxenable - RX leaky enable.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set mirror VLAN leaky function forwarding packets to miror port.
 */
rtk_api_ret_t rtk_mirror_isolationLeaky_set(struct phy_device *phydev, rtk_enable_t txenable, rtk_enable_t rxenable)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if ((txenable >= RTK_ENABLE_END) ||(rxenable >= RTK_ENABLE_END))
        return RT_ERR_ENABLE;

    if ((retVal = rtl8367c_setAsicPortMirrorIsolationTxLeaky(phydev, txenable)) != RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicPortMirrorIsolationRxLeaky(phydev, rxenable)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_mirror_isolationLeaky_get
 * Description:
 *      Get mirror isolation leaky.
 * Input:
 *      None
 * Output:
 *      pTxenable - TX leaky enable.
 *      pRxenable - RX leaky enable.
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API is to get mirror isolation leaky status.
 */
rtk_api_ret_t rtk_mirror_isolationLeaky_get(struct phy_device *phydev, rtk_enable_t *pTxenable, rtk_enable_t *pRxenable)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if( (NULL == pTxenable) || (NULL == pRxenable) )
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicPortMirrorIsolationTxLeaky(phydev, pTxenable)) != RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicPortMirrorIsolationRxLeaky(phydev, pRxenable)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_mirror_keep_set
 * Description:
 *      Set mirror packet format keep.
 * Input:
 *      mode - -mirror keep mode.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The API is to set  -mirror keep mode.
 *      The mirror keep mode is as following:
 *      - MIRROR_FOLLOW_VLAN
 *      - MIRROR_KEEP_ORIGINAL
 *      - MIRROR_KEEP_END
 */
rtk_api_ret_t rtk_mirror_keep_set(struct phy_device *phydev, rtk_mirror_keep_t mode)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if (mode >= MIRROR_KEEP_END)
        return RT_ERR_ENABLE;

    if ((retVal = rtl8367c_setAsicPortMirrorRealKeep(phydev, mode)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_mirror_keep_get
 * Description:
 *      Get mirror packet format keep.
 * Input:
 *      None
 * Output:
 *      pMode -mirror keep mode.
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API is to get mirror keep mode.
  *      The mirror keep mode is as following:
 *      - MIRROR_FOLLOW_VLAN
 *      - MIRROR_KEEP_ORIGINAL
 *      - MIRROR_KEEP_END
 */
rtk_api_ret_t rtk_mirror_keep_get(struct phy_device *phydev, rtk_mirror_keep_t *pMode)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    if(NULL == pMode)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicPortMirrorRealKeep(phydev, pMode)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_mirror_override_set
 * Description:
 *      Set port mirror override function.
 * Input:
 *      rxMirror        - 1: output mirrored packet, 0: output normal forward packet
 *      txMirror        - 1: output mirrored packet, 0: output normal forward packet
 *      aclMirror       - 1: output mirrored packet, 0: output normal forward packet
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 * Note:
 *      The API is to set mirror override function.
 *      This function control the output format when a port output
 *      normal forward & mirrored packet at the same time.
 */
rtk_api_ret_t rtk_mirror_override_set(struct phy_device *phydev, rtk_enable_t rxMirror, rtk_enable_t txMirror, rtk_enable_t aclMirror)
{
    rtk_api_ret_t retVal;

    if( (rxMirror >= RTK_ENABLE_END) || (txMirror >= RTK_ENABLE_END) || (aclMirror >= RTK_ENABLE_END))
        return RT_ERR_ENABLE;

    if ((retVal = rtl8367c_setAsicPortMirrorOverride(phydev, (rtk_uint32)rxMirror, (rtk_uint32)txMirror, (rtk_uint32)aclMirror)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_mirror_override_get
 * Description:
 *      Get port mirror override function.
 * Input:
 *      None
 * Output:
 *      pRxMirror       - 1: output mirrored packet, 0: output normal forward packet
 *      pTxMirror       - 1: output mirrored packet, 0: output normal forward packet
 *      pAclMirror      - 1: output mirrored packet, 0: output normal forward packet
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_NULL_POINTER - Null Pointer
 * Note:
 *      The API is to Get mirror override function.
 *      This function control the output format when a port output
 *      normal forward & mirrored packet at the same time.
 */
rtk_api_ret_t rtk_mirror_override_get(struct phy_device *phydev, rtk_enable_t *pRxMirror, rtk_enable_t *pTxMirror, rtk_enable_t *pAclMirror)
{
    rtk_api_ret_t retVal;

    if( (pRxMirror == NULL) || (pTxMirror == NULL) || (pAclMirror == NULL))
        return RT_ERR_ENABLE;

    if ((retVal = rtl8367c_getAsicPortMirrorOverride(phydev, (rtk_uint32 *)pRxMirror, (rtk_uint32 *)pTxMirror, (rtk_uint32 *)pAclMirror)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}
