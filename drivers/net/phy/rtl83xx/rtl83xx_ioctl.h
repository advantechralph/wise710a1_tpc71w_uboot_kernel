
#ifndef __RTL83XX_IOCTL_H__
#define __RTL83XX_IOCTL_H__

#include "trap.h"
#include "cpu.h"
#include "stat.h"
#include "qos.h"
#include "acl.h"
#include "rate.h"
#include "storm.h"
#include "l2.h"

typedef enum rtl83xx_sw_ioctl_e
{
    SIOCRTLCOMMAND = SIOCDEVPRIVATE,
    SIOCRTLEND
} rtl83xx_sw_ioctl;

typedef enum rtl83xx_sw_cmd_e
{
    RTLCMD_PHYID_GET                        = 0,
    RTLCMD_PHYSTATUS_GET,
    RTLCMD_PHYAUTOABILITY_GET,
    RTLCMD_PHYAUTOABILITY_SET,
    RTLCMD_PHYFORCEABILITY_GET,
    RTLCMD_PHYFORCEABILITY_SET,
    RTLCMD_PHYADMIN_GET,
    RTLCMD_PHYADMIN_SET,
    RTLCMD_TRAPRMAACTION_GET                = 10,
    RTLCMD_TRAPRMAACTION_SET,
    RTLCMD_LLDPENABLED_GET,
    RTLCMD_LLDPENABLED_SET,
    RTLCMD_CPUENABLED_GET,
    RTLCMD_CPUENABLED_SET,
    RTLCMD_CPUTAGPORT_GET,
    RTLCMD_CPUTAGPORT_SET,
    RTLCMD_CPUAWAREPORT_SET,
    RTLCMD_PORTSTATALL_GET                  = 20, /* MIB ALL*/
    RTLCMD_PORTSTAT_GET,                          /* MIB */
    RTLCMD_STATALL_RESET,
    RTLCMD_PORTSTAT_RESET,
    RTLCMD_IGRACL_SET                       = 40, /* ACL enable/disable*/
    RTLCMD_ACLRULE_SET,
    RTLCMD_L2FLOODINGPORTMASK_SET,
    RTLCMD_QOSINIT_SET                      = 70, /* QOS */
    RTLCMD_QOS1Q_SET,
    RTLCMD_RATE_SHAREDMETER_GET             = 80, /* Rate */
    RTLCMD_RATE_SHAREDMETER_SET,
    RTLCMD_RATE_SHAREDMETERBUCKET_GET,
    RTLCMD_RATE_SHAREDMETERBUCKET_SET,
    RTLCMD_RATE_IGR_BANDWIDTH_CTRLRATE_GET,
    RTLCMD_RATE_IGR_BANDWIDTH_CTRLRATE_SET,
    RTLCMD_RATE_EGR_BANDWIDTH_CTRLRATE_GET,
    RTLCMD_RATE_EGR_BANDWIDTH_CTRLRATE_SET,
    RTLCMD_RATE_EGR_QUEUEBW_CTRLENABLE_GET,
    RTLCMD_RATE_EGR_QUEUEBW_CTRLENABLE_SET,
    RTLCMD_RATE_EGR_QUEUEBW_CTRLRATE_GET,
    RTLCMD_RATE_EGR_QUEUEBW_CTRLRATE_SET,
    RTLCMD_END
} rtl83xx_cmd_s;

typedef enum rtlsw_aclrule_cmdtype_s {
    RTLSW_CMD_ACLRULE_DEL_ID,
    RTLSW_CMD_ACLRULE_DMAC_ETHTYPE,
    RTLSW_CMD_ACLRULE_DMAC_SMAC_ETHTYPE,
    RTLSW_CMD_ACLRULE_DMAC_SMAC,
    RTLSW_CMD_ACLRULE_DMAC,
    RTLSW_CMD_ACLRULE_DMAC_TCPUDP_ALL,
    RTLSW_CMD_ACLRULE_CTAG_PRIORITY,
    RTLSW_CMD_ACLRULE_DMAC_UDP_DPORT,
} rtlsw_aclrule_cmdtype_s;

typedef struct rtlsw_qnum_t {
    rtl83xx_cmd_s           cmd;
    rtk_queue_num_t         qnum;
}rtlsw_qnum_s;

typedef struct rtlsw_enabled_t {
    rtl83xx_cmd_s           cmd;
    rtk_enable_t            enabled;
}rtlsw_enabled_s;

typedef struct rtlsw_enabled_acl_t {
    rtl83xx_cmd_s           cmd;
    rtk_enable_t            enabled;
    unsigned char           mac[6];
    rtk_rate_t              shareMeterRate;
}rtlsw_enabled_acl_s;

typedef struct rtlsw_phyid_t {
    rtl83xx_cmd_s           cmd;
    unsigned int            phyid;
}rtlsw_phyid_s;

typedef struct phy_status_t {
    rtl83xx_cmd_s           cmd;
    rtk_port_t              Port;
    rtk_port_linkStatus_t   LinkStatus;
    rtk_port_speed_t        Speed;
    rtk_port_duplex_t       Duplex;
}phy_status_s;

typedef struct phy_ability_t {
    rtl83xx_cmd_s           cmd;
    rtk_port_t              Port;
    rtk_port_phy_ability_t  Ability;
}phy_ability_s;

typedef struct phy_admin_t {
    rtl83xx_cmd_s           cmd;
    rtk_port_t              Port;
    rtk_enable_t            enabled;
}phy_admin_s;

typedef struct rtlsw_trap_ramAction_t {
    rtl83xx_cmd_s           cmd;
    rtk_trap_type_t         type;
    rtk_trap_rma_action_t   rma_action;
}rtlsw_trap_ramAction_s;

typedef struct rtlsw_cpu_tagPort_t {
    rtl83xx_cmd_s           cmd;
    rtk_port_t              port;
    rtk_cpu_insert_t        mode;
}rtlsw_cpu_tagPort_s;

typedef struct rtlsw_port_stat_t {
    rtl83xx_cmd_s           cmd;
    rtk_port_t              port;
    rtk_stat_port_cntr_t    port_cntr;
}rtlsw_port_stat_s;

typedef struct rtlsw_port_stat_field_t {
    rtl83xx_cmd_s           cmd;
    rtk_port_t              port;
    rtk_stat_port_type_t    cntr_idx;
    rtk_stat_counter_t      cntr;
}rtlsw_port_stat_field_s;

typedef struct rtlsw_sharedMeter_t {
    rtl83xx_cmd_s           cmd;
    rtk_meter_id_t          index;
    rtk_meter_type_t        type;
    rtk_rate_t              rate;
    rtk_enable_t            ifg_include;
}rtlsw_sharedMeter_s;

typedef struct rtlsw_sharedMeterBucket_t {
    rtl83xx_cmd_s           cmd;
    rtk_meter_id_t          index;
    rtk_uint32              bucket_size;
}rtlsw_sharedMeterBucket_s;

typedef struct rtlsw_rate_igr_bw_ctrlrate_t {
    rtl83xx_cmd_s           cmd;
    rtk_port_t              port;
    rtk_rate_t              rate;
    rtk_enable_t            ifg_include;
    rtk_enable_t            fc_enable;
}rtlsw_rate_igr_bw_ctrlrate_s;

typedef struct rtlsw_rate_egr_bw_ctrlrate_t {
    rtl83xx_cmd_s           cmd;
    rtk_port_t              port;
    rtk_rate_t              rate;
    rtk_enable_t            ifg_include;
}rtlsw_rate_egr_bw_ctrlrate_s;

typedef struct rtlsw_rate_egr_queuebw_ctrlenable_t {
    rtl83xx_cmd_s           cmd;
    rtk_port_t              port;
    rtk_qid_t               queue;
    rtk_enable_t            enable;
}rtlsw_rate_egr_queuebw_ctrlenable_s;

typedef struct rtlsw_rate_egr_queuebw_ctrlrate_t {
    rtl83xx_cmd_s           cmd;
    rtk_port_t              port;
    rtk_qid_t               queue;
    rtk_meter_id_t          index;
}rtlsw_rate_egr_queuebw_ctrlrate_s;

typedef struct rtlsw_acl_rule_cmd_t {
    rtl83xx_cmd_s           cmd;

    rtlsw_aclrule_cmdtype_s rule_cmd;
    rtk_uint32              filter_id;
    rtk_filter_action_t     filter_action;
    rtk_uint8               dmac[ETHER_ADDR_LEN];
    rtk_uint8               dmask[ETHER_ADDR_LEN];
    rtk_uint8               smac[ETHER_ADDR_LEN];
    rtk_uint8               smask[ETHER_ADDR_LEN];
    union
    {
        rtk_uint32              ethtype;
        rtk_uint32              priority;
        rtk_uint32              dport;
    }pattern_union;
}rtlsw_acl_rule_cmd_s;

typedef struct rtlsw_flooding_cmd_t {
    rtl83xx_cmd_s           cmd;
    rtk_l2_flood_type_t     flood_type;
}rtlsw_flooding_cmd_s;

int rtl83xx_ioctl(struct phy_device *phydev, struct ifreq *ifr, int cmd);

#endif /* __RTL83XX_IOCTL_H__ */

