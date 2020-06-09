#ifndef _RTL8367C_ASICDRV_I2C_H_
#define _RTL8367C_ASICDRV_I2C_H_
#include "rtk_types.h"
#include "rtl8367c_asicdrv.h"


#define TIMEROUT_FOR_MICROSEMI (0x400)

#define GPIO_INPUT 1
#define GPIO_OUTPUT 2

extern ret_t rtl8367c_setAsicI2C_checkBusIdle(struct phy_device *phydev);
extern ret_t rtl8367c_setAsicI2CStartCmd(struct phy_device *phydev);
extern ret_t rtl8367c_setAsicI2CStopCmd(struct phy_device *phydev);
extern ret_t rtl8367c_setAsicI2CTxOneCharCmd(struct phy_device *phydev, rtk_uint8 oneChar);
extern ret_t rtl8367c_setAsicI2CcheckRxAck(struct phy_device *phydev);
extern ret_t rtl8367c_setAsicI2CRxOneCharCmd(struct phy_device *phydev, rtk_uint8 *pValue);
extern ret_t rtl8367c_setAsicI2CTxAckCmd(struct phy_device *phydev);
extern ret_t rtl8367c_setAsicI2CTxNoAckCmd(struct phy_device *phydev);
extern ret_t rtl8367c_setAsicI2CSoftRSTseqCmd(struct phy_device *phydev);
extern ret_t rtl8367c_setAsicI2CGpioPinGroup(struct phy_device *phydev, rtk_uint32 pinGroup_ID);
extern ret_t rtl8367c_getAsicI2CGpioPinGroup(struct phy_device *phydev, rtk_uint32 * pPinGroup_ID);





#endif /*#ifndef _RTL8367C_ASICDRV_I2C_H_*/

