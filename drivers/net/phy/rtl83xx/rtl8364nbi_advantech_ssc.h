#ifndef __RTK_API_RTL8364NBI_ADVANTECH_SSC_H__
#define __RTK_API_RTL8364NBI_ADVANTECH_SSC_H__

#include "rtk_types.h"

/* Function Name:
 *      rtk_port_macForceLinkExtSSC_set
 * Description:
 *      Fixed WISE-710 EMI test bug.
 * Input:
 *      port        - Port ID
 *      state       - Nway state
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK                   - OK
 *      RT_ERR_FAILED               - Failed
 *      RT_ERR_SMI                  - SMI access error
 *      RT_ERR_PORT_ID              - Invalid port ID.
 */
extern rtk_api_ret_t rtk_port_macForceLinkExtSSC_set(
		struct phy_device *phydev, 
		rtk_port_t port, 
		rtk_enable_t state
		);

/* Function Name:
 *      rtk_port_macForceLinkExtSSC_get
 * Description:
 *      Fixed WISE-710 EMI test bug.
 * Input:
 *      port        - Port ID
 * Output:
 *      pState      - Nway state
 * Return:
 *      RT_ERR_OK                   - OK
 *      RT_ERR_FAILED               - Failed
 *      RT_ERR_SMI                  - SMI access error
 *      RT_ERR_PORT_ID              - Invalid port ID.
 */
extern rtk_api_ret_t rtk_port_macForceLinkExtSSC_get(
		struct phy_device *phydev, 
		rtk_port_t port, 
		rtk_enable_t *pState
		);

#endif /* __RTK_API_RTL8364NBI_ADVANTECH_SSC_H__ */
