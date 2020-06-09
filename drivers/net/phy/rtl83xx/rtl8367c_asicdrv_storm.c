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
 * Feature : Storm control filtering related functions
 *
 */

#include "rtl8367c_asicdrv_storm.h"
/* Function Name:
 *      rtl8367c_setAsicStormFilterBroadcastEnable
 * Description:
 *      Set per-port broadcast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterBroadcastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enabled)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_setAsicRegBit(phydev, RTL8367C_STORM_BCAST_REG, port, enabled);
}
/* Function Name:
 *      rtl8367c_getAsicStormFilterBroadcastEnable
 * Description:
 *      Get per-port broadcast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterBroadcastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pEnabled)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_getAsicRegBit(phydev, RTL8367C_STORM_BCAST_REG, port, pEnabled);
}
/* Function Name:
 *      rtl8367c_setAsicStormFilterBroadcastMeter
 * Description:
 *      Set per-port broadcast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_PORT_ID          - Invalid port number
 *      RT_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterBroadcastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 meter)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if(meter > RTL8367C_METERMAX)
        return RT_ERR_FILTER_METER_ID;

    return rtl8367c_setAsicRegBits(phydev, RTL8367C_STORM_BCAST_METER_CTRL_REG(port), RTL8367C_STORM_BCAST_METER_CTRL_MASK(port), meter);
}
/* Function Name:
 *      rtl8367c_getAsicStormFilterBroadcastMeter
 * Description:
 *      Get per-port broadcast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      pMeter  - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterBroadcastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pMeter)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_getAsicRegBits(phydev, RTL8367C_STORM_BCAST_METER_CTRL_REG(port), RTL8367C_STORM_BCAST_METER_CTRL_MASK(port), pMeter);
}
/* Function Name:
 *      rtl8367c_setAsicStormFilterMulticastEnable
 * Description:
 *      Set per-port multicast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterMulticastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enabled)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_setAsicRegBit(phydev, RTL8367C_STORM_MCAST_REG, port, enabled);
}
/* Function Name:
 *      rtl8367c_getAsicStormFilterMulticastEnable
 * Description:
 *      Get per-port multicast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterMulticastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pEnabled)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_getAsicRegBit(phydev, RTL8367C_STORM_MCAST_REG, port, pEnabled);
}
/* Function Name:
 *      rtl8367c_setAsicStormFilterMulticastMeter
 * Description:
 *      Set per-port multicast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_PORT_ID          - Invalid port number
 *      RT_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterMulticastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 meter)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if(meter > RTL8367C_METERMAX)
        return RT_ERR_FILTER_METER_ID;

    return rtl8367c_setAsicRegBits(phydev, RTL8367C_STORM_MCAST_METER_CTRL_REG(port), RTL8367C_STORM_MCAST_METER_CTRL_MASK(port), meter);
}
/* Function Name:
 *      rtl8367c_getAsicStormFilterMulticastMeter
 * Description:
 *      Get per-port multicast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      pMeter  - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterMulticastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pMeter)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_getAsicRegBits(phydev, RTL8367C_STORM_MCAST_METER_CTRL_REG(port), RTL8367C_STORM_MCAST_METER_CTRL_MASK(port), pMeter);
}
/* Function Name:
 *      rtl8367c_setAsicStormFilterUnknownMulticastEnable
 * Description:
 *      Set per-port unknown multicast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterUnknownMulticastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enabled)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_setAsicRegBit(phydev, RTL8367C_STORM_UNKNOWN_MCAST_REG, port, enabled);
}
/* Function Name:
 *      rtl8367c_getAsicStormFilterUnknownMulticastEnable
 * Description:
 *      Get per-port unknown multicast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterUnknownMulticastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pEnabled)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_getAsicRegBit(phydev, RTL8367C_STORM_UNKNOWN_MCAST_REG, port, pEnabled);
}
/* Function Name:
 *      rtl8367c_setAsicStormFilterUnknownMulticastMeter
 * Description:
 *      Set per-port unknown multicast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_PORT_ID          - Invalid port number
 *      RT_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterUnknownMulticastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 meter)
{
    ret_t retVal;

    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if(meter > RTL8367C_METERMAX)
        return RT_ERR_FILTER_METER_ID;

    if(port < 8)
    {
        retVal = rtl8367c_setAsicRegBits(phydev, RTL8367C_STORM_UNMC_METER_CTRL_REG(port), RTL8367C_STORM_UNMC_METER_CTRL_MASK(port), meter);
        if(retVal != RT_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = rtl8367c_setAsicRegBits(phydev, RTL8367C_REG_STORM_UNMC_METER_CTRL4 + ((port - 8) >> 1), RTL8367C_STORM_UNMC_METER_CTRL_MASK(port), meter);
        if(retVal != RT_ERR_OK)
            return retVal;
    }

    return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367c_getAsicStormFilterUnknownMulticastMeter
 * Description:
 *      Get per-port unknown multicast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      pMeter  - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterUnknownMulticastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pMeter)
{
    ret_t retVal;

    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if(port < 8)
    {
        retVal = rtl8367c_getAsicRegBits(phydev, RTL8367C_STORM_UNMC_METER_CTRL_REG(port), RTL8367C_STORM_UNMC_METER_CTRL_MASK(port), pMeter);
        if(retVal != RT_ERR_OK)
            return retVal;
    }
    else
    {
        retVal = rtl8367c_getAsicRegBits(phydev, RTL8367C_REG_STORM_UNMC_METER_CTRL4 + ((port - 8) >> 1), RTL8367C_STORM_UNMC_METER_CTRL_MASK(port), pMeter);
        if(retVal != RT_ERR_OK)
            return retVal;
    }

    return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367c_setAsicStormFilterUnknownUnicastEnable
 * Description:
 *      Set per-port unknown unicast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      enabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterUnknownUnicastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 enabled)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_setAsicRegBit(phydev, RTL8367C_STORM_UNKNOWN_UCAST_REG, port, enabled);
}
/* Function Name:
 *      rtl8367c_getAsicStormFilterUnknownUnicastEnable
 * Description:
 *      get per-port unknown unicast storm filter enable/disable
 * Input:
 *      port    - Physical port number (0~7)
 *      pEnabled - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterUnknownUnicastEnable(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pEnabled)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_getAsicRegBit(phydev, RTL8367C_STORM_UNKNOWN_UCAST_REG, port, pEnabled);
}
/* Function Name:
 *      rtl8367c_setAsicStormFilterUnknownUnicastMeter
 * Description:
 *      Set per-port unknown unicast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_PORT_ID          - Invalid port number
 *      RT_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterUnknownUnicastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 meter)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if(meter > RTL8367C_METERMAX)
        return RT_ERR_FILTER_METER_ID;

    return rtl8367c_setAsicRegBits(phydev, RTL8367C_STORM_UNDA_METER_CTRL_REG(port), RTL8367C_STORM_UNDA_METER_CTRL_MASK(port), meter);
}
/* Function Name:
 *      rtl8367c_getAsicStormFilterUnknownUnicastMeter
 * Description:
 *      Get per-port unknown unicast storm filter meter
 * Input:
 *      port    - Physical port number (0~7)
 *      pMeter  - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterUnknownUnicastMeter(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pMeter)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_getAsicRegBits(phydev, RTL8367C_STORM_UNDA_METER_CTRL_REG(port), RTL8367C_STORM_UNDA_METER_CTRL_MASK(port), pMeter);
}

/* Function Name:
 *      rtl8367c_setAsicStormFilterExtBroadcastMeter
 * Description:
 *      Set extension broadcast storm filter meter
 * Input:
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterExtBroadcastMeter(struct phy_device *phydev, rtk_uint32 meter)
{
    if(meter > RTL8367C_METERMAX)
        return RT_ERR_FILTER_METER_ID;

    return rtl8367c_setAsicRegBits(phydev, RTL8367C_REG_STORM_EXT_MTRIDX_CFG0, RTL8367C_BC_STORM_EXT_METERIDX_MASK, meter);
}

/* Function Name:
 *      rtl8367c_getAsicStormFilterExtBroadcastMeter
 * Description:
 *      get extension broadcast storm filter meter
 * Input:
 *      None
 * Output:
 *      pMeter  - meter index (0~31)
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_NULL_POINTER     - Invalid meter index
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterExtBroadcastMeter(struct phy_device *phydev, rtk_uint32 *pMeter)
{
    if(NULL == pMeter)
        return RT_ERR_NULL_POINTER;

    return rtl8367c_getAsicRegBits(phydev, RTL8367C_REG_STORM_EXT_MTRIDX_CFG0, RTL8367C_BC_STORM_EXT_METERIDX_MASK, pMeter);
}

/* Function Name:
 *      rtl8367c_setAsicStormFilterExtMulticastMeter
 * Description:
 *      Set extension multicast storm filter meter
 * Input:
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterExtMulticastMeter(struct phy_device *phydev, rtk_uint32 meter)
{
    if(meter > RTL8367C_METERMAX)
        return RT_ERR_FILTER_METER_ID;

    return rtl8367c_setAsicRegBits(phydev, RTL8367C_REG_STORM_EXT_MTRIDX_CFG0, RTL8367C_MC_STORM_EXT_METERIDX_MASK, meter);
}

/* Function Name:
 *      rtl8367c_getAsicStormFilterExtMulticastMeter
 * Description:
 *      get extension multicast storm filter meter
 * Input:
 *      None
 * Output:
 *      pMeter  - meter index (0~31)
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_NULL_POINTER     - Invalid meter index
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterExtMulticastMeter(struct phy_device *phydev, rtk_uint32 *pMeter)
{
    if(NULL == pMeter)
        return RT_ERR_NULL_POINTER;

    return rtl8367c_getAsicRegBits(phydev, RTL8367C_REG_STORM_EXT_MTRIDX_CFG0, RTL8367C_MC_STORM_EXT_METERIDX_MASK, pMeter);
}

/* Function Name:
 *      rtl8367c_setAsicStormFilterExtUnknownMulticastMeter
 * Description:
 *      Set extension unknown multicast storm filter meter
 * Input:
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterExtUnknownMulticastMeter(struct phy_device *phydev, rtk_uint32 meter)
{
    if(meter > RTL8367C_METERMAX)
        return RT_ERR_FILTER_METER_ID;

    return rtl8367c_setAsicRegBits(phydev, RTL8367C_REG_STORM_EXT_MTRIDX_CFG1, RTL8367C_UNMC_STORM_EXT_METERIDX_MASK, meter);
}

/* Function Name:
 *      rtl8367c_getAsicStormFilterExtUnknownMulticastMeter
 * Description:
 *      get extension unknown multicast storm filter meter
 * Input:
 *      None
 * Output:
 *      pMeter  - meter index (0~31)
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_NULL_POINTER     - Invalid meter index
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterExtUnknownMulticastMeter(struct phy_device *phydev, rtk_uint32 *pMeter)
{
    if(NULL == pMeter)
        return RT_ERR_NULL_POINTER;

    return rtl8367c_getAsicRegBits(phydev, RTL8367C_REG_STORM_EXT_MTRIDX_CFG1, RTL8367C_UNMC_STORM_EXT_METERIDX_MASK, pMeter);
}

/* Function Name:
 *      rtl8367c_setAsicStormFilterExtUnknownUnicastMeter
 * Description:
 *      Set extension unknown unicast storm filter meter
 * Input:
 *      meter   - meter index (0~31)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_FILTER_METER_ID  - Invalid meter index
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterExtUnknownUnicastMeter(struct phy_device *phydev, rtk_uint32 meter)
{
    if(meter > RTL8367C_METERMAX)
        return RT_ERR_FILTER_METER_ID;

    return rtl8367c_setAsicRegBits(phydev, RTL8367C_REG_STORM_EXT_MTRIDX_CFG1, RTL8367C_UNUC_STORM_EXT_METERIDX_MASK, meter);
}

/* Function Name:
 *      rtl8367c_getAsicStormFilterExtUnknownUnicastMeter
 * Description:
 *      get extension unknown unicast storm filter meter
 * Input:
 *      None
 * Output:
 *      pMeter  - meter index (0~31)
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_NULL_POINTER     - Invalid meter index
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterExtUnknownUnicastMeter(struct phy_device *phydev, rtk_uint32 *pMeter)
{
    if(NULL == pMeter)
        return RT_ERR_NULL_POINTER;

    return rtl8367c_getAsicRegBits(phydev, RTL8367C_REG_STORM_EXT_MTRIDX_CFG1, RTL8367C_UNUC_STORM_EXT_METERIDX_MASK, pMeter);
}

/* Function Name:
 *      rtl8367c_setAsicStormFilterExtBroadcastEnable
 * Description:
 *      Set extension broadcast storm filter state
 * Input:
 *      enabled     - state
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterExtBroadcastEnable(struct phy_device *phydev, rtk_uint32 enabled)
{
    return rtl8367c_setAsicRegBit(phydev, RTL8367C_REG_STORM_EXT_CFG, RTL8367C_STORM_BCAST_EXT_EN_OFFSET, enabled);
}

/* Function Name:
 *      rtl8367c_getAsicStormFilterExtBroadcastEnable
 * Description:
 *      Get extension broadcast storm filter state
 * Input:
 *      None
 * Output:
 *      pEnabled    - state
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_NULL_POINTER     - Null pointer
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterExtBroadcastEnable(struct phy_device *phydev, rtk_uint32 *pEnabled)
{
    if(NULL == pEnabled)
        return RT_ERR_NULL_POINTER;

    return rtl8367c_getAsicRegBit(phydev, RTL8367C_REG_STORM_EXT_CFG, RTL8367C_STORM_BCAST_EXT_EN_OFFSET, pEnabled);
}

/* Function Name:
 *      rtl8367c_setAsicStormFilterExtMulticastEnable
 * Description:
 *      Set extension multicast storm filter state
 * Input:
 *      enabled     - state
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterExtMulticastEnable(struct phy_device *phydev, rtk_uint32 enabled)
{
    return rtl8367c_setAsicRegBit(phydev, RTL8367C_REG_STORM_EXT_CFG, RTL8367C_STORM_MCAST_EXT_EN_OFFSET, enabled);
}

/* Function Name:
 *      rtl8367c_getAsicStormFilterExtMulticastEnable
 * Description:
 *      Get extension multicast storm filter state
 * Input:
 *      None
 * Output:
 *      pEnabled    - state
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_NULL_POINTER     - Null pointer
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterExtMulticastEnable(struct phy_device *phydev, rtk_uint32 *pEnabled)
{
    if(NULL == pEnabled)
        return RT_ERR_NULL_POINTER;

    return rtl8367c_getAsicRegBit(phydev, RTL8367C_REG_STORM_EXT_CFG, RTL8367C_STORM_MCAST_EXT_EN_OFFSET, pEnabled);
}

/* Function Name:
 *      rtl8367c_setAsicStormFilterExtUnknownMulticastEnable
 * Description:
 *      Set extension unknown multicast storm filter state
 * Input:
 *      enabled     - state
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterExtUnknownMulticastEnable(struct phy_device *phydev, rtk_uint32 enabled)
{
    return rtl8367c_setAsicRegBit(phydev, RTL8367C_REG_STORM_EXT_CFG, RTL8367C_STORM_UNKNOWN_MCAST_EXT_EN_OFFSET, enabled);
}

/* Function Name:
 *      rtl8367c_getAsicStormFilterExtUnknownMulticastEnable
 * Description:
 *      Get extension unknown multicast storm filter state
 * Input:
 *      None
 * Output:
 *      pEnabled    - state
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_NULL_POINTER     - Null pointer
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterExtUnknownMulticastEnable(struct phy_device *phydev, rtk_uint32 *pEnabled)
{
    if(NULL == pEnabled)
        return RT_ERR_NULL_POINTER;

    return rtl8367c_getAsicRegBit(phydev, RTL8367C_REG_STORM_EXT_CFG, RTL8367C_STORM_UNKNOWN_MCAST_EXT_EN_OFFSET, pEnabled);
}

/* Function Name:
 *      rtl8367c_setAsicStormFilterExtUnknownUnicastEnable
 * Description:
 *      Set extension unknown unicast storm filter state
 * Input:
 *      enabled     - state
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterExtUnknownUnicastEnable(struct phy_device *phydev, rtk_uint32 enabled)
{
    return rtl8367c_setAsicRegBit(phydev, RTL8367C_REG_STORM_EXT_CFG, RTL8367C_STORM_UNKNOWN_UCAST_EXT_EN_OFFSET, enabled);
}

/* Function Name:
 *      rtl8367c_getAsicStormFilterExtUnknownUnicastEnable
 * Description:
 *      Get extension unknown unicast storm filter state
 * Input:
 *      None
 * Output:
 *      pEnabled    - state
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_NULL_POINTER     - Null pointer
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterExtUnknownUnicastEnable(struct phy_device *phydev, rtk_uint32 *pEnabled)
{
    if(NULL == pEnabled)
        return RT_ERR_NULL_POINTER;

    return rtl8367c_getAsicRegBit(phydev, RTL8367C_REG_STORM_EXT_CFG, RTL8367C_STORM_UNKNOWN_UCAST_EXT_EN_OFFSET, pEnabled);
}

/* Function Name:
 *      rtl8367c_setAsicStormFilterExtEnablePortMask
 * Description:
 *      Set extension storm filter port mask
 * Input:
 *      portmask    - port mask
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicStormFilterExtEnablePortMask(struct phy_device *phydev, rtk_uint32 portmask)
{
    ret_t retVal;

    retVal = rtl8367c_setAsicRegBits(phydev, RTL8367C_REG_STORM_EXT_CFG, RTL8367C_STORM_EXT_EN_PORTMASK_MASK, portmask & 0x3FF);
    if(retVal != RT_ERR_OK)
        return retVal;

    retVal = rtl8367c_setAsicRegBits(phydev, RTL8367C_REG_STORM_EXT_CFG, RTL8367C_STORM_EXT_EN_PORTMASK_EXT_MASK, (portmask >> 10)&1);
    if(retVal != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_getAsicStormFilterExtEnablePortMask
 * Description:
 *      Get extension storm filter port mask
 * Input:
 *      portmask    - port mask
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_NULL_POINTER     - Null pointer
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicStormFilterExtEnablePortMask(struct phy_device *phydev, rtk_uint32 *pPortmask)
{
    rtk_uint32 tmpPmsk;
    ret_t retVal;

    if(NULL == pPortmask)
        return RT_ERR_NULL_POINTER;

    retVal = rtl8367c_getAsicRegBits(phydev, RTL8367C_REG_STORM_EXT_CFG, RTL8367C_STORM_EXT_EN_PORTMASK_MASK, &tmpPmsk);
    if(retVal != RT_ERR_OK)
        return retVal;
    *pPortmask = tmpPmsk & 0x3ff;

    retVal = rtl8367c_getAsicRegBits(phydev, RTL8367C_REG_STORM_EXT_CFG, RTL8367C_STORM_EXT_EN_PORTMASK_EXT_MASK, &tmpPmsk);
    if(retVal != RT_ERR_OK)
        return retVal;
    *pPortmask |= (tmpPmsk & 1) << 10;

    return RT_ERR_OK;
}


