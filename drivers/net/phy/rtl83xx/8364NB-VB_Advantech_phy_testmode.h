#ifndef __8364NB_VB_ADVANTECH_PHY_TESTMODE_H__
#define __8364NB_VB_ADVANTECH_PHY_TESTMODE_H__

#include "port.h"

/* Function Name:
 *      rtk_port_phyTestMode_set
 * Description:
 *      Set PHY in test mode.
 * Input:
 * 		phydev  - PHY device description struct
 *      port    - port id.
 *      mode    - PHY test mode 0:normal 1:test mode 1 4:test mode 4
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK              	- OK
 *      RT_ERR_FAILED          	- Failed
 *      RT_ERR_SMI             	- SMI access error
 *      RT_ERR_PORT_ID 			- Invalid port number.
 *      RT_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 *      RT_ERR_NOT_ALLOWED      - The Setting is not allowed, caused by set more than 1 port in Test mode.
 * Note:
 *      Set PHY in test mode and only one PHY can be in test mode at the same time.
 *      It means API will return FAILED if other PHY is in test mode.
 *      This API only provide test mode 1 & 4 setup
 */
extern rtk_api_ret_t rtk_port_phyTestMode_set(struct phy_device *phydev, rtk_port_t port, rtk_port_phy_test_mode_t mode);

/* Function Name:
 *      rtk_port_phyTestMode_get
 * Description:
 *      Get PHY in which test mode.
 * Input:
 * 		phydev  - PHY device description struct
 *      port    - Port id.
 * Output:
 *      mode    - PHY test mode 0:normal 1:test mode 1 4:test mode 4
 * Return:
 *      RT_ERR_OK              	- OK
 *      RT_ERR_FAILED          	- Failed
 *      RT_ERR_SMI             	- SMI access error
 *      RT_ERR_PORT_ID 			- Invalid port number.
 *      RT_ERR_INPUT 			- Invalid input parameters.
 *      RT_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      Get test mode of PHY from register setting 9.15 to 9.13.
 */
extern rtk_api_ret_t rtk_port_phyTestMode_get(struct phy_device *phydev, rtk_port_t port, rtk_port_phy_test_mode_t *pMode);

#endif /* __8364NB_VB_ADVANTECH_PHY_TESTMODE_H__ */
