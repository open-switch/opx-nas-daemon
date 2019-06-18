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

/**
 * filename: hald_init.h
 */



#ifndef __HALD_INIT__H
#define __HALD_INIT__H

#include "std_error_codes.h"

/**
 * @brief   the initializaiton function for the HAL daemon.  All applicaitons
 *          and components will be added to the initializaiton list.
 *          Additional plugins will also be loaded at this time.
 * @return  Error code indicating that a problem has occured.  HALD will not
 *          start if there is an error at initializaiton time.
 */
t_std_error hald_init();


#endif
