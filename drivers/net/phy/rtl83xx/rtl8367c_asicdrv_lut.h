#ifndef _RTL8367C_ASICDRV_LUT_H_
#define _RTL8367C_ASICDRV_LUT_H_

#include "rtl8367c_asicdrv.h"

#define RTL8367C_LUT_AGETIMERMAX        (7)
#define RTL8367C_LUT_AGESPEEDMAX        (3)
#define RTL8367C_LUT_LEARNLIMITMAX      (0x1040)
#define RTL8367C_LUT_ADDRMAX            (0x103F)
#define RTL8367C_LUT_IPMCGRP_TABLE_MAX  (0x3F)
#define RTL8367C_LUT_ENTRY_SIZE         (6)
#define RTL8367C_LUT_BUSY_CHECK_NO      (10)

#define RTL8367C_LUT_TABLE_SIZE         (6)

enum RTL8367C_LUTHASHMETHOD{

    LUTHASHMETHOD_SVL=0,
    LUTHASHMETHOD_IVL,
    LUTHASHMETHOD_END,
};


enum RTL8367C_LRNOVERACT{

    LRNOVERACT_FORWARD=0,
    LRNOVERACT_DROP,
    LRNOVERACT_TRAP,
    LRNOVERACT_END,
};

enum RTL8367C_LUTREADMETHOD{

    LUTREADMETHOD_MAC =0,
    LUTREADMETHOD_ADDRESS,
    LUTREADMETHOD_NEXT_ADDRESS,
    LUTREADMETHOD_NEXT_L2UC,
    LUTREADMETHOD_NEXT_L2MC,
    LUTREADMETHOD_NEXT_L3MC,
    LUTREADMETHOD_NEXT_L2L3MC,
    LUTREADMETHOD_NEXT_L2UCSPA,
};

enum RTL8367C_FLUSHMODE
{
    FLUSHMDOE_PORT = 0,
    FLUSHMDOE_VID,
    FLUSHMDOE_FID,
    FLUSHMDOE_END,
};

enum RTL8367C_FLUSHTYPE
{
    FLUSHTYPE_DYNAMIC = 0,
    FLUSHTYPE_BOTH,
    FLUSHTYPE_END,
};


typedef struct LUTTABLE{

    ipaddr_t sip;
    ipaddr_t dip;
    ether_addr_t mac;
    rtk_uint16 ivl_svl:1;
    rtk_uint16 cvid_fid:12;
    rtk_uint16 fid:4;
    rtk_uint16 efid:3;

    rtk_uint16 nosalearn:1;
    rtk_uint16 da_block:1;
    rtk_uint16 sa_block:1;
    rtk_uint16 auth:1;
    rtk_uint16 lut_pri:3;
    rtk_uint16 sa_en:1;
    rtk_uint16 fwd_en:1;
    rtk_uint16 mbr:11;
    rtk_uint16 spa:4;
    rtk_uint16 age:3;
    rtk_uint16 l3lookup:1;
    rtk_uint16 igmp_asic:1;
    rtk_uint16 igmpidx:8;

    rtk_uint16 lookup_hit:1;
    rtk_uint16 lookup_busy:1;
    rtk_uint16 address:13;

    rtk_uint16 l3vidlookup:1;
    rtk_uint16 l3_vid:12;

    rtk_uint16 wait_time;

}rtl8367c_luttb;

extern ret_t rtl8367c_setAsicLutIpMulticastLookup(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicLutIpMulticastLookup(struct phy_device *phydev, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicLutIpMulticastVidLookup(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicLutIpMulticastVidLookup(struct phy_device *phydev, rtk_uint32* pEnabled);
extern ret_t rtl8367c_setAsicLutAgeTimerSpeed(struct phy_device *phydev, rtk_uint32 timer, rtk_uint32 speed);
extern ret_t rtl8367c_getAsicLutAgeTimerSpeed(struct phy_device *phydev, rtk_uint32* pTimer, rtk_uint32* pSpeed);
extern ret_t rtl8367c_setAsicLutCamTbUsage(struct phy_device *phydev, rtk_uint32 enabled);
extern ret_t rtl8367c_getAsicLutCamTbUsage(struct phy_device *phydev, rtk_uint32* pEnabled);
extern ret_t rtl8367c_getAsicLutCamType(struct phy_device *phydev, rtk_uint32* pType);
extern ret_t rtl8367c_setAsicLutLearnLimitNo(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 number);
extern ret_t rtl8367c_getAsicLutLearnLimitNo(struct phy_device *phydev, rtk_uint32 port, rtk_uint32* pNumber);
extern ret_t rtl8367c_setAsicSystemLutLearnLimitNo(struct phy_device *phydev, rtk_uint32 number);
extern ret_t rtl8367c_getAsicSystemLutLearnLimitNo(struct phy_device *phydev, rtk_uint32 *pNumber);
extern ret_t rtl8367c_setAsicLutLearnOverAct(struct phy_device *phydev, rtk_uint32 action);
extern ret_t rtl8367c_getAsicLutLearnOverAct(struct phy_device *phydev, rtk_uint32* pAction);
extern ret_t rtl8367c_setAsicSystemLutLearnOverAct(struct phy_device *phydev, rtk_uint32 action);
extern ret_t rtl8367c_getAsicSystemLutLearnOverAct(struct phy_device *phydev, rtk_uint32 *pAction);
extern ret_t rtl8367c_setAsicSystemLutLearnPortMask(struct phy_device *phydev, rtk_uint32 portmask);
extern ret_t rtl8367c_getAsicSystemLutLearnPortMask(struct phy_device *phydev, rtk_uint32 *pPortmask);
extern ret_t rtl8367c_setAsicL2LookupTb(struct phy_device *phydev, rtl8367c_luttb *pL2Table);
extern ret_t rtl8367c_getAsicL2LookupTb(struct phy_device *phydev, rtk_uint32 method, rtl8367c_luttb *pL2Table);
extern ret_t rtl8367c_getAsicLutLearnNo(struct phy_device *phydev, rtk_uint32 port, rtk_uint32* pNumber);
extern ret_t rtl8367c_setAsicLutIpLookupMethod(struct phy_device *phydev, rtk_uint32 type);
extern ret_t rtl8367c_getAsicLutIpLookupMethod(struct phy_device *phydev, rtk_uint32* pType);
extern ret_t rtl8367c_setAsicLutForceFlush(struct phy_device *phydev, rtk_uint32 portmask);
extern ret_t rtl8367c_getAsicLutForceFlushStatus(struct phy_device *phydev, rtk_uint32 *pPortmask);
extern ret_t rtl8367c_setAsicLutFlushMode(struct phy_device *phydev, rtk_uint32 mode);
extern ret_t rtl8367c_getAsicLutFlushMode(struct phy_device *phydev, rtk_uint32* pMode);
extern ret_t rtl8367c_setAsicLutFlushType(struct phy_device *phydev, rtk_uint32 type);
extern ret_t rtl8367c_getAsicLutFlushType(struct phy_device *phydev, rtk_uint32* pType);
extern ret_t rtl8367c_setAsicLutFlushVid(struct phy_device *phydev, rtk_uint32 vid);
extern ret_t rtl8367c_getAsicLutFlushVid(struct phy_device *phydev, rtk_uint32* pVid);
extern ret_t rtl8367c_setAsicLutFlushFid(struct phy_device *phydev, rtk_uint32 fid);
extern ret_t rtl8367c_getAsicLutFlushFid(struct phy_device *phydev, rtk_uint32* pFid);
extern ret_t rtl8367c_setAsicLutDisableAging(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 disabled);
extern ret_t rtl8367c_getAsicLutDisableAging(struct phy_device *phydev, rtk_uint32 port, rtk_uint32 *pDisabled);
extern ret_t rtl8367c_setAsicLutIPMCGroup(struct phy_device *phydev, rtk_uint32 index, ipaddr_t group_addr, rtk_uint32 vid, rtk_uint32 pmask, rtk_uint32 valid);
extern ret_t rtl8367c_getAsicLutIPMCGroup(struct phy_device *phydev, rtk_uint32 index, ipaddr_t *pGroup_addr, rtk_uint32 *pVid, rtk_uint32 *pPmask, rtk_uint32 *pValid);
extern ret_t rtl8367c_setAsicLutLinkDownForceAging(struct phy_device *phydev, rtk_uint32 enable);
extern ret_t rtl8367c_getAsicLutLinkDownForceAging(struct phy_device *phydev, rtk_uint32 *pEnable);
extern ret_t rtl8367c_setAsicLutFlushAll(struct phy_device *phydev);
extern ret_t rtl8367c_getAsicLutFlushAllStatus(struct phy_device *phydev, rtk_uint32 *pBusyStatus);
extern ret_t rtl8367c_setAsicLutIpmcFwdRouterPort(struct phy_device *phydev, rtk_uint32 enable);
extern ret_t rtl8367c_getAsicLutIpmcFwdRouterPort(struct phy_device *phydev, rtk_uint32 *pEnable);

#endif /*_RTL8367C_ASICDRV_LUT_H_*/

