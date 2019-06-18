/*
 * Copyright (c) 2019 Dell Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 * FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 * See the Apache Version 2.0 License for specific language governing
 * permissions and limitations under the License.
 */

/*
 * filename: hald_init.c
 */


#include "hald_init.h"
#include "event_log.h"

#include "nas_ndi_init.h"
#include "db_linux_event_register.h"
#include "nas_qos_init.h"
#include "hal_rt_extn.h"
#include "nas_vrf_extn.h"

#include "hal_interface.h"

#include "cps_api_events.h"
#include "db_api_linux_init.h"
#include "nas_l2_init.h"
#include "nas_mc_l3_main.h"
#include "nas_acl_init.h"
#include "nas_switch.h"
#include <stdio.h>


t_std_error cps_init_functions() {
    //! @TODO come back and fix the ordering/initialization of the CPS api
    t_std_error rc = cps_api_linux_init();
    if (rc!=STD_ERR_OK) return rc;

    if (cps_api_event_service_init()!=cps_api_ret_code_OK) {
        return STD_ERR(COM,FAIL,0);
    }
    if (cps_api_event_thread_init()==cps_api_ret_code_ERR) {
        return STD_ERR(COM,FAIL,0);
    }
    return STD_ERR_OK;
}

typedef struct T_hald_init_functions {
    t_std_error (*init_fun_void)(void);
    t_std_error (*init_fun_param)(void *param);
    void *param;
} t_hald_init_list;

static t_hald_init_list hald_init_functions [] = {
//base infrastructure
    { cps_init_functions, NULL,NULL },

//Switch
    { nas_switch_init, NULL, NULL },

//NPU
    { nas_ndi_init, NULL, NULL },

// nas services
    { hal_interface_init, NULL, NULL },
    { nas_l2_init,  NULL, NULL },
    { nas_mc_l3_init,  NULL, NULL },
    { nas_vrf_init, NULL, NULL },
    { hal_rt_init, NULL, NULL },
    { hal_packet_io_init, NULL, NULL },
    { nas_acl_init, NULL, NULL },
    { nas_qos_init, NULL, NULL },

//hal event gerators
    { cps_api_net_notify_init, NULL, NULL },
};

t_std_error hald_init() {
    int ix = 0;
    int mx = sizeof (hald_init_functions) / sizeof(*hald_init_functions);
    t_std_error er = STD_ERR_OK;
    for ( ; ix < mx ; ++ix ) {
        if (hald_init_functions[ix].init_fun_void!=NULL) {
            if ((er=hald_init_functions[ix].init_fun_void())!=STD_ERR_OK) {
                EV_LOG_TRACE(ev_log_t_NPU,0,"NPU-INIT","Failed to init at ix %d",
                    ix);
                return er;
            }
        }
        if (hald_init_functions[ix].init_fun_param!=NULL) {
            if ((er=hald_init_functions[ix].init_fun_param(
                            hald_init_functions[ix].param))!=STD_ERR_OK) {
                EV_LOG_TRACE(ev_log_t_NPU,0,"NPU-INIT","Failed to init at ix %d",
                    ix);
                return er;
            }
        }
    }
    return STD_ERR_OK;
}


