/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * $Revision: 68033 $
 * $Date: 2016-05-12 14:48:03 +0800 (週四, 12 五月 2016) $
 *
 * Purpose : Definition function prototype of RTK switch API.
 *
 * Feature : Function prototype definition
 *
 */

#ifndef __RTK_SWITCH_H__
#define __RTK_SWITCH_H__

#include "rtk_types.h"

#define MAX_PHYID	32
#define UNDEFINE_PHY_PORT   (0xFF)
#define RTK_SWITCH_PORT_NUM (32)

#define MAXPKTLEN_CFG_ID_MAX (1)

#define RTK_SWITCH_MAX_PKTLEN (0x3FFF)

typedef enum init_state_e
{
    INIT_NOT_COMPLETED = 0,
    INIT_COMPLETED,
    INIT_STATE_END
} init_state_t;

typedef enum switch_chip_e
{
    CHIP_RTL8367C = 0,
    CHIP_RTL8370B,
    CHIP_RTL8364B,
    CHIP_RTL8363SC_VB,
    CHIP_END
}switch_chip_t;

typedef enum port_type_e
{
    UTP_PORT = 0,
    EXT_PORT,
    UNKNOWN_PORT = 0xFF,
    PORT_TYPE_END
}port_type_t;

typedef struct rtk_switch_halCtrl_s
{
    switch_chip_t   switch_type;
    rtk_uint32      l2p_port[RTK_SWITCH_PORT_NUM];
    rtk_uint32      p2l_port[RTK_SWITCH_PORT_NUM];
    port_type_t     log_port_type[RTK_SWITCH_PORT_NUM];
    rtk_uint32      ptp_port[RTK_SWITCH_PORT_NUM];
    rtk_uint32      valid_portmask;
    rtk_uint32      valid_utp_portmask;
    rtk_uint32      valid_ext_portmask;
    rtk_uint32      valid_cpu_portmask;
    rtk_uint32      min_phy_port;
    rtk_uint32      max_phy_port;
    rtk_uint32      phy_portmask;
    rtk_uint32      combo_logical_port;
    rtk_uint32      hsg_logical_port;
    rtk_uint32      sg_logical_portmask;
    rtk_uint32      max_meter_id;
    rtk_uint32      max_lut_addr_num;
    rtk_uint32      trunk_group_mask;

}rtk_switch_halCtrl_t;

typedef enum rtk_switch_maxPktLen_linkSpeed_e {
     MAXPKTLEN_LINK_SPEED_FE = 0,
     MAXPKTLEN_LINK_SPEED_GE,
     MAXPKTLEN_LINK_SPEED_END,
} rtk_switch_maxPktLen_linkSpeed_t;


/* UTIL MACRO */
#define RTK_CHK_INIT_STATE(__phydev__)                          		\
    do                                                      			\
    {                                                      				\
        if(rtk_switch_initialState_get(__phydev__) != INIT_COMPLETED) 	\
        {                                                   			\
            return RT_ERR_NOT_INIT;                         			\
        }                                                   			\
    }while(0)

#define RTK_CHK_PORT_VALID(__phydev__, __port__)                    		\
    do                                                          			\
    {                                                           			\
        if(rtk_switch_logicalPortCheck(__phydev__, __port__) != RT_ERR_OK)  \
        {                                                       			\
            return RT_ERR_PORT_ID;                              			\
        }                                                       			\
    }while(0)

#define RTK_CHK_PORT_IS_UTP(__phydev__, __port__)                   \
    do                                                          	\
    {                                                           	\
        if(rtk_switch_isUtpPort(__phydev__, __port__) != RT_ERR_OK)	\
        {                                                       	\
            return RT_ERR_PORT_ID;                              	\
        }                                                       	\
    }while(0)

#define RTK_CHK_PORT_IS_EXT(__phydev__, __port__)					\
    do																\
    {                                                          		\
        if(rtk_switch_isExtPort(__phydev__, __port__) != RT_ERR_OK)	\
        {                                                      		\
            return RT_ERR_PORT_ID;                              	\
        }                                                       	\
    }while(0)

#define RTK_CHK_PORT_IS_COMBO(__phydev__, __port__)						\
    do                                                          		\
    {                                                           		\
        if(rtk_switch_isComboPort(__phydev__, __port__) != RT_ERR_OK)   \
        {                                                       		\
            return RT_ERR_PORT_ID;                            	 		\
        }																\
    }while(0)

#define RTK_CHK_PORT_IS_PTP(__phydev__, __port__)					\
    do																\
    {																\
        if(rtk_switch_isPtpPort(__phydev__, __port__) != RT_ERR_OK)	\
        {															\
            return RT_ERR_PORT_ID;									\
        }															\
    }while(0)

#define RTK_CHK_PORTMASK_VALID(__phydev__, __portmask__)						\
    do																			\
    {																			\
        if(rtk_switch_isPortMaskValid(__phydev__, __portmask__) != RT_ERR_OK)	\
        {																		\
            return RT_ERR_PORT_MASK;											\
        }																		\
    }while(0)

#define RTK_CHK_PORTMASK_VALID_ONLY_UTP(__phydev__, __portmask__)               \
    do                                                              			\
    {                                                               			\
        if(rtk_switch_isPortMaskUtp(__phydev__, __portmask__) != RT_ERR_OK)     \
        {                                                           			\
            return RT_ERR_PORT_MASK;                               				\
        }                                                           			\
    }while(0)

#define RTK_CHK_PORTMASK_VALID_ONLY_EXT(__phydev__, __portmask__)               \
    do                                                              			\
    {                                                               			\
        if(rtk_switch_isPortMaskExt(__phydev__, __portmask__) != RT_ERR_OK)     \
        {                                                           			\
            return RT_ERR_PORT_MASK;                                			\
        }                                                           			\
    }while(0)

#define RTK_CHK_TRUNK_GROUP_VALID(__phydev__, __grpId__)                        \
    do                                                              			\
    {                                                              				\
        if(rtk_switch_isValidTrunkGrpId(__phydev__, __grpId__) != RT_ERR_OK)    \
        {                                                           			\
            return RT_ERR_LA_TRUNK_ID;                              			\
        }                                                           			\
    }while(0)

#define RTK_PORTMASK_IS_PORT_SET(__portmask__, __port__)    (((__portmask__).bits[0] & (0x00000001 << __port__)) ? 1 : 0)
#define RTK_PORTMASK_IS_EMPTY(__portmask__)                 (((__portmask__).bits[0] == 0) ? 1 : 0)
#define RTK_PORTMASK_CLEAR(__portmask__)                    ((__portmask__).bits[0] = 0)
#define RTK_PORTMASK_PORT_SET(__portmask__, __port__)       ((__portmask__).bits[0] |= (0x00000001 << __port__))
#define RTK_PORTMASK_PORT_CLEAR(__portmask__, __port__)     ((__portmask__).bits[0] &= ~(0x00000001 << __port__))
#define RTK_PORTMASK_ALLPORT_SET(__portmask__)              (rtk_switch_logPortMask_get(phydev, &__portmask__))
#define RTK_PORTMASK_SCAN(__portmask__, __port__)           for(__port__ = 0; __port__ < RTK_SWITCH_PORT_NUM; __port__++)  if(RTK_PORTMASK_IS_PORT_SET(__portmask__, __port__))
#define RTK_PORTMASK_COMPARE(__portmask_A__, __portmask_B__)    ((__portmask_A__).bits[0] - (__portmask_B__).bits[0])

#define RTK_SCAN_ALL_PHY_PORTMASK(phydev, __port__)                 for(__port__ = 0; __port__ < RTK_SWITCH_PORT_NUM; __port__++)  if( (rtk_switch_phyPortMask_get(phydev) & (0x00000001 << __port__)))
#define RTK_SCAN_ALL_LOG_PORT(phydev, __port__)                     for(__port__ = 0; __port__ < RTK_SWITCH_PORT_NUM; __port__++)  if( rtk_switch_logicalPortCheck(phydev, __port__) == RT_ERR_OK)
#define RTK_SCAN_ALL_LOG_PORTMASK(__portmask__)             for((__portmask__).bits[0] = 0; (__portmask__).bits[0] < 0x7FFFF; (__portmask__).bits[0]++)  if( rtk_switch_isPortMaskValid(&__portmask__) == RT_ERR_OK)

/* Port mask defination */
#define RTK_PHY_PORTMASK_ALL                                (rtk_switch_phyPortMask_get(phydev))

/* Port defination*/
#define RTK_MAX_LOGICAL_PORT_ID                             (rtk_switch_maxLogicalPort_get(phydev))

/* Function Name:
 *      rtk_switch_probe
 * Description:
 *      Probe switch
 * Input:
 *      phydev	- PHY device
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Switch probed
 *      RT_ERR_FAILED   - Switch Unprobed.
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_probe(struct phy_device *phydev, switch_chip_t *pSwitchChip);

/* Function Name:
 *      rtk_switch_initialState_set
 * Description:
 *      Set initial status
 * Input:
 * 		phydev 	- PHY device
 *      state   - Initial state;
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Initialized
 *      RT_ERR_FAILED   - Uninitialized
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_initialState_set(struct phy_device *phydev, init_state_t state);

/* Function Name:
 *      rtk_switch_initialState_get
 * Description:
 *      Get initial status
 * Input:
 *      phydev	- PHY device
 * Output:
 *      None
 * Return:
 *      INIT_COMPLETED     - Initialized
 *      INIT_NOT_COMPLETED - Uninitialized
 * Note:
 *
 */
extern init_state_t rtk_switch_initialState_get(struct phy_device *phydev);

/* Function Name:
 *      rtk_switch_logicalPortCheck
 * Description:
 *      Check logical port ID.
 * Input:
 * 		phydev			- PHY device
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is correct
 *      RT_ERR_FAILED   - Port ID is not correct
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_logicalPortCheck(struct phy_device *phydev, rtk_port_t logicalPort);

/* Function Name:
 *      rtk_switch_isUtpPort
 * Description:
 *      Check is logical port a UTP port
 * Input:
 * 		phydev			- PHY device
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is a UTP port
 *      RT_ERR_FAILED   - Port ID is not a UTP port
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_isUtpPort(struct phy_device *phydev, rtk_port_t logicalPort);

/* Function Name:
 *      rtk_switch_isExtPort
 * Description:
 *      Check is logical port a Extension port
 * Input:
 * 		phydev			- PHY device
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is a EXT port
 *      RT_ERR_FAILED   - Port ID is not a EXT port
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_isExtPort(struct phy_device *phydev, rtk_port_t logicalPort);

/* Function Name:
 *      rtk_switch_isHsgPort
 * Description:
 *      Check is logical port a HSG port
 * Input:
 * 		phydev			- PHY device
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is a HSG port
 *      RT_ERR_FAILED   - Port ID is not a HSG port
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_isHsgPort(struct phy_device *phydev, rtk_port_t logicalPort);

/* Function Name:
 *      rtk_switch_isSgmiiPort
 * Description:
 *      Check is logical port a SGMII port
 * Input:
 * 		phydev			- PHY device
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is a SGMII port
 *      RT_ERR_FAILED   - Port ID is not a SGMII port
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_isSgmiiPort(struct phy_device *phydev, rtk_port_t logicalPort);

/* Function Name:
 *      rtk_switch_isCPUPort
 * Description:
 *      Check is logical port a CPU port
 * Input:
 * 		phydev			- PHY device
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is a CPU port
 *      RT_ERR_FAILED   - Port ID is not a CPU port
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_isCPUPort(struct phy_device *phydev, rtk_port_t logicalPort);

/* Function Name:
 *      rtk_switch_isComboPort
 * Description:
 *      Check is logical port a Combo port
 * Input:
 * 		phydev			- PHY device
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is a combo port
 *      RT_ERR_FAILED   - Port ID is not a combo port
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_isComboPort(struct phy_device *phydev, rtk_port_t logicalPort);

/* Function Name:
 *      rtk_switch_ComboPort_get
 * Description:
 *      Get Combo port ID
 * Input:
 *      phydev		- PHY device
 * Output:
 *      None
 * Return:
 *      Port ID of combo port
 * Note:
 *
 */
extern rtk_uint32 rtk_switch_ComboPort_get(struct phy_device *phydev);

/* Function Name:
 *      rtk_switch_isPtpPort
 * Description:
 *      Check is logical port a PTP port
 * Input:
 * 		phydev			- PHY device
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is a PTP port
 *      RT_ERR_FAILED   - Port ID is not a PTP port
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_isPtpPort(struct phy_device *phydev, rtk_port_t logicalPort);

/* Function Name:
 *      rtk_switch_port_L2P_get
 * Description:
 *      Get physical port ID
 * Input:
 * 		phydev			- PHY device
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      Physical port ID
 * Note:
 *
 */
extern rtk_uint32 rtk_switch_port_L2P_get(struct phy_device *phydev, rtk_port_t logicalPort);

/* Function Name:
 *      rtk_switch_port_P2L_get
 * Description:
 *      Get logical port ID
 * Input:
 * 		phydev			- PHY device
 *      physicalPort    - physical port ID
 * Output:
 *      None
 * Return:
 *      logical port ID
 * Note:
 *
 */
extern rtk_port_t rtk_switch_port_P2L_get(struct phy_device *phydev, rtk_uint32 physicalPort);

/* Function Name:
 *      rtk_switch_isPortMaskValid
 * Description:
 *      Check portmask is valid or not
 * Input:
 * 		phydev		- PHY device
 *      pPmask      - logical port mask
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - port mask is valid
 *      RT_ERR_FAILED       - port mask is not valid
 *      RT_ERR_NOT_INIT     - Not Initialize
 *      RT_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_isPortMaskValid(struct phy_device *phydev, rtk_portmask_t *pPmask);

/* Function Name:
 *      rtk_switch_isPortMaskUtp
 * Description:
 *      Check all ports in portmask are only UTP port
 * Input:
 * 		phydev		 - PHY device
 *      pPmask       - logical port mask
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - Only UTP port in port mask
 *      RT_ERR_FAILED       - Not only UTP port in port mask
 *      RT_ERR_NOT_INIT     - Not Initialize
 *      RT_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_isPortMaskUtp(struct phy_device *phydev, rtk_portmask_t *pPmask);

/* Function Name:
 *      rtk_switch_isPortMaskExt
 * Description:
 *      Check all ports in portmask are only EXT port
 * Input:
 * 		phydev		- PHY device
 *      pPmask      - logical port mask
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - Only EXT port in port mask
 *      RT_ERR_FAILED       - Not only EXT port in port mask
 *      RT_ERR_NOT_INIT     - Not Initialize
 *      RT_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_isPortMaskExt(struct phy_device *phydev, rtk_portmask_t *pPmask);

/* Function Name:
 *      rtk_switch_portmask_L2P_get
 * Description:
 *      Get physicl portmask from logical portmask
 * Input:
 * 		phydev				- PHY device
 *      pLogicalPmask       - logical port mask
 * Output:
 *      pPhysicalPortmask   - physical port mask
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_NOT_INIT     - Not Initialize
 *      RT_ERR_NULL_POINTER - Null pointer
 *      RT_ERR_PORT_MASK    - Error port mask
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_portmask_L2P_get(struct phy_device *phydev, rtk_portmask_t *pLogicalPmask, rtk_uint32 *pPhysicalPortmask);

/* Function Name:
 *      rtk_switch_portmask_P2L_get
 * Description:
 *      Get logical portmask from physical portmask
 * Input:
 * 		phydev				- PHY device
 *      physicalPortmask    - physical port mask
 * Output:
 *      pLogicalPmask       - logical port mask
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_NOT_INIT     - Not Initialize
 *      RT_ERR_NULL_POINTER - Null pointer
 *      RT_ERR_PORT_MASK    - Error port mask
 * Note:
 *
 */
extern rtk_api_ret_t rtk_switch_portmask_P2L_get(struct phy_device *phydev, rtk_uint32 physicalPortmask, rtk_portmask_t *pLogicalPmask);

/* Function Name:
 *      rtk_switch_phyPortMask_get
 * Description:
 *      Get physical portmask
 * Input:
 * 		phydev			- PHY device
 * Output:
 *      None
 * Return:
 *      0x00                - Not Initialize
 *      Other value         - Physical port mask
 * Note:
 *
 */
rtk_uint32 rtk_switch_phyPortMask_get(struct phy_device *phydev);

/* Function Name:
 *      rtk_switch_logPortMask_get
 * Description:
 *      Get Logical portmask
 * Input:
 * 		phydev			- PHY device
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_NOT_INIT     - Not Initialize
 *      RT_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
rtk_api_ret_t rtk_switch_logPortMask_get(struct phy_device *phydev, rtk_portmask_t *pPortmask);

/* Function Name:
 *      rtk_switch_init
 * Description:
 *      Set chip to default configuration enviroment
 * Input:
 *      phydev	- PHY device
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 * Note:
 *      The API can set chip registers to default configuration for different release chip model.
 */
extern rtk_api_ret_t rtk_switch_init(struct phy_device *phydev);

/* Function Name:
 *      rtk_switch_portMaxPktLen_set
 * Description:
 *      Set Max packet length
 * Input:
 * 		phydev	- PHY device
 *      port    - Port ID
 *      speed   - Speed
 *      cfgId   - Configuration ID
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_INPUT        - Error Input
 * Note:
 */
extern rtk_api_ret_t rtk_switch_portMaxPktLen_set(struct phy_device *phydev, rtk_port_t port, rtk_switch_maxPktLen_linkSpeed_t speed, rtk_uint32 cfgId);

/* Function Name:
 *      rtk_switch_portMaxPktLen_get
 * Description:
 *      Get Max packet length
 * Input:
 * 		phydev	- PHY device
 *      port    - Port ID
 *      speed   - Speed
 * Output:
 *      pCfgId  - Configuration ID
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_INPUT        - Error Input
 * Note:
 */
extern rtk_api_ret_t rtk_switch_portMaxPktLen_get(struct phy_device *phydev, rtk_port_t port, rtk_switch_maxPktLen_linkSpeed_t speed, rtk_uint32 *pCfgId);

/* Function Name:
 *      rtk_switch_maxPktLenCfg_set
 * Description:
 *      Set Max packet length configuration
 * Input:
 * 		phydev	- PHY device
 *      cfgId   - Configuration ID
 *      pktLen  - Max packet length
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_INPUT        - Error Input
 * Note:
 */
extern rtk_api_ret_t rtk_switch_maxPktLenCfg_set(struct phy_device *phydev, rtk_uint32 cfgId, rtk_uint32 pktLen);

/* Function Name:
 *      rtk_switch_maxPktLenCfg_get
 * Description:
 *      Get Max packet length configuration
 * Input:
 * 		phydev	- PHY device
 *      cfgId   - Configuration ID
 *      pPktLen - Max packet length
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_INPUT        - Error Input
 * Note:
 */
extern rtk_api_ret_t rtk_switch_maxPktLenCfg_get(struct phy_device *phydev, rtk_uint32 cfgId, rtk_uint32 *pPktLen);

/* Function Name:
 *      rtk_switch_greenEthernet_set
 * Description:
 *      Set all Ports Green Ethernet state.
 * Input:
 * 		phydev	- PHY device
 *      enable 	- Green Ethernet state.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - OK
 *      RT_ERR_FAILED   - Failed
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_ENABLE   - Invalid enable input.
 * Note:
 *      This API can set all Ports Green Ethernet state.
 *      The configuration is as following:
 *      - DISABLE
 *      - ENABLE
 */
extern rtk_api_ret_t rtk_switch_greenEthernet_set(struct phy_device *phydev, rtk_enable_t enable);

/* Function Name:
 *      rtk_switch_greenEthernet_get
 * Description:
 *      Get all Ports Green Ethernet state.
 * Input:
 * 		phydev			- PHY device
 * Output:
 *      pEnable - Green Ethernet state.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 * Note:
 *      This API can get Green Ethernet state.
 */
extern rtk_api_ret_t rtk_switch_greenEthernet_get(struct phy_device *phydev, rtk_enable_t *pEnable);

/* Function Name:
 *      rtk_switch_maxLogicalPort_get
 * Description:
 *      Get Max logical port ID
 * Input:
 * 		phydev	- PHY device
 * Output:
 *      None
 * Return:
 *      Max logical port
 * Note:
 *      This API can get max logical port
 */
extern rtk_port_t rtk_switch_maxLogicalPort_get(struct phy_device *phydev);

/* Function Name:
 *      rtk_switch_maxMeterId_get
 * Description:
 *      Get Max Meter ID
 * Input:
 * 		phydev	- PHY device
 * Output:
 *      None
 * Return:
 *      0x00                - Not Initialize
 *      Other value         - Max Meter ID
 * Note:
 *
 */
extern rtk_uint32 rtk_switch_maxMeterId_get(struct phy_device *phydev);

/* Function Name:
 *      rtk_switch_maxLutAddrNumber_get
 * Description:
 *      Get Max LUT Address number
 * Input:
 * 		phydev	- PHY device
 * Output:
 *      None
 * Return:
 *      0x00                - Not Initialize
 *      Other value         - Max LUT Address number
 * Note:
 *
 */
extern rtk_uint32 rtk_switch_maxLutAddrNumber_get(struct phy_device *phydev);

/* Function Name:
 *      rtk_switch_isValidTrunkGrpId
 * Description:
 *      Check if trunk group is valid or not
 * Input:
 * 		phydev		- PHY device
 *      grpId       - Group ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - Trunk Group ID is valid
 *      RT_ERR_LA_TRUNK_ID  - Trunk Group ID is not valid
 * Note:
 *
 */
rtk_uint32 rtk_switch_isValidTrunkGrpId(struct phy_device *phydev, rtk_uint32 grpId);

#endif
