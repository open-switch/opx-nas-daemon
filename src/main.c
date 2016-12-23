/*
 * Copyright (c) 2016 Dell Inc.
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

/*!
 * \file   main.c
 * \brief  Main file for hal daemon
 * \date   04-2014
 * \author Amazon Team
 */

#include "hald_init.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <systemd/sd-daemon.h>

#include <stdio.h>

int main(int argc, char **argv) {
    if (hald_init()!=STD_ERR_OK) exit (1);

    sd_notify(0,"READY=1");

    while (1) {
        sleep(1);
    }
    return 0;
}

