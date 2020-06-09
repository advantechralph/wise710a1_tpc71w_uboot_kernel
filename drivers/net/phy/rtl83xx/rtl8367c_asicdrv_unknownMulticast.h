#ifndef _RTL8367C_ASICDRV_UNKNOWNMULTICAST_H_
#define _RTL8367C_ASICDRV_UNKNOWNMULTICAST_H_

#include "rtl8367c_asicdrv.h"

enum L2_UNKOWN_MULTICAST_BEHAVE
{
    L2_UNKOWN_MULTICAST_FLOODING = 0,
    L2_UNKOWN_MULTICAST_DROP,
    L2_UNKOWN_MULTICAST_TRAP,
    L2_UNKOWN_MULTICAST_DROP_EXCLUDE_RMA,
    L2_UNKOWN_MULTICAST_END
};

enum L3_UNKOWN_MULTICAST_BEHAVE
{
    L3_UNKOWN_MULTICAST_FLOODING = 0,
    L3_UNKOWN_MULTICAST_DROP,
    L3_UNKOWN_MULTICAST_TRAP,
    L3_UNKOWN_MULTICAST_ROUTER,
    L3_UNKOWN_MULTICAST_END
};

enum MULTICASTTYPE{
    MULTICAST_TYPE_IPV4 = 0,
    MULTICAST_TYPE_IPV6,
    MULTICAST_TYPE_L2,
    MULTICAST_TYPE_END
};

extern ret_t rtl8367c_setAsicUnknownL2MulticastBehavior(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 behave);
extern ret_t rtl8367c_getAsicUnknownL2MulticastBehavior(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pBehave);
extern ret_t rtl8367c_setAsicUnknownIPv4MulticastBehavior(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 behave);
extern ret_t rtl8367c_getAsicUnknownIPv4MulticastBehavior(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pBehave);
extern ret_t rtl8367c_setAsicUnknownIPv6MulticastBehavior(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 behave);
extern ret_t rtl8367c_getAsicUnknownIPv6MulticastBehavior(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pBehave);
extern ret_t rtl8367c_setAsicUnknownMulticastTrapPriority(struct phy_device *phydev, rtk_uint32 priority);
extern ret_t rtl8367c_getAsicUnknownMulticastTrapPriority(struct phy_device *phydev, rtk_uint32 *pPriority);

#endif /*_RTL8367C_ASICDRV_UNKNOWNMULTICAST_H_*/


