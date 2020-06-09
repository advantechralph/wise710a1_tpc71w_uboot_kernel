#include "rtk_switch.h"
#include "rtk_error.h"
#include "rtl8364nbi_advantech_ssc.h"

#include "rtl8367c_asicdrv.h"
#include "rtl8367c_asicdrv_port.h"
#include "rtl8367c_asicdrv_misc.h"
#include "rtl8367c_asicdrv_portIsolation.h"

rtk_api_ret_t rtk_port_macForceLinkExtSSC_set(struct phy_device *phydev, rtk_port_t port, rtk_enable_t state)
{
    rtk_api_ret_t retVal;
    rtk_uint32 regData;
    rtk_uint32 regValue;
    rtk_uint32 type;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check Port Valid */
    RTK_CHK_PORT_IS_EXT(phydev, port);

    if(state >= RTK_ENABLE_END)
        return RT_ERR_ENABLE;

    if((retVal = rtl8367c_setAsicReg(phydev, 0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(phydev, 0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(phydev, 0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 1;
            break;
        case 0x0801:
        case 0x6511:
            type = 3;
            break;
        default:
            return RT_ERR_FAILED;
    }

    if(type == 1)
    {
        if(port == EXT_PORT0)
        {
            if(state == ENABLED)
            {
                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d53, 0x0001)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d55, 0x000f)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d54, 0x05fa)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d52, 0x2473)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d70, 0x0000)) != RT_ERR_OK)
                    return retVal;

            }
            else
            {
                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d53, 0x8000)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d55, 0x000f)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d54, 0x0008)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d52, 0x2411)) != RT_ERR_OK)
                    return retVal;
            }
        }
        else if(port == EXT_PORT1)
        {
            if(state == ENABLED)
            {
                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d5a, 0x0001)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d5c, 0x000f)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d5b, 0x05fa)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d59, 0x2473)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d70, 0x0000)) != RT_ERR_OK)
                    return retVal;

            }
            else
            {
                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d5a, 0x8000)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d5c, 0x000f)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d5b, 0x0008)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d59, 0x2411)) != RT_ERR_OK)
                    return retVal;
            }
        }
        else
            return RT_ERR_PORT_ID;
    }
    else if(type == 3)
    {
        if(port == EXT_PORT0)
        {
            if(state == ENABLED)
            {
                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d54, 0x05fa)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d53, 0x0001)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d52, 0x2472)) != RT_ERR_OK)
                    return retVal;

            }
            else
            {
                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d52, 0x2470)) != RT_ERR_OK)
                    return retVal;
            }
        }
        else if(port == EXT_PORT1)
        {
            if(state == ENABLED)
            {
                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d5b, 0x05fa)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d5a, 0x0001)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d59, 0x2472)) != RT_ERR_OK)
                    return retVal;

            }
            else
            {
                if ((retVal = rtl8367c_setAsicReg(phydev, 0x1d52, 0x2470)) != RT_ERR_OK)
                    return retVal;
            }
        }
        else
            return RT_ERR_PORT_ID;
    }

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_port_macForceLinkExtSSC_get(struct phy_device *phydev, rtk_port_t port, rtk_enable_t *pState)
{
    rtk_api_ret_t retVal;
    rtk_uint32 data;

    /* Check initialization state */
    RTK_CHK_INIT_STATE(phydev);

    /* Check Port Valid */
    RTK_CHK_PORT_IS_EXT(phydev, port);

    if(NULL == pState)
        return RT_ERR_NULL_POINTER;

    if(port == EXT_PORT0)
    {
        if ((retVal = rtl8367c_getAsicReg(phydev, 0x1d52, &data)) != RT_ERR_OK)
            return retVal;
    }
    else if(port == EXT_PORT1)
    {
        if ((retVal = rtl8367c_getAsicReg(phydev, 0x1d59, &data)) != RT_ERR_OK)
            return retVal;
    }

    *pState = (data & 0x0002) ? ENABLED : DISABLED;
    return RT_ERR_OK;
}
