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

/*!
 * \file   main.c
 * \brief  Main file for hal daemon
 * \date   04-2014
 * \author Amazon Team
 */

#include "hald_init.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>             /* signal() */
#include <systemd/sd-daemon.h>  /* sd_notify() */
#include <stdlib.h>             /* exit(), EXIT_SUCCESS */
#include <stdbool.h>            /* bool, true, false */

volatile static bool shutdwn = false;

static void sigterm_hdlr(int signo)
{
    shutdwn = true;
}

int main(int argc, char **argv) {

    // signal must install before service init
    (void)signal(SIGTERM, sigterm_hdlr);

    if (hald_init()!=STD_ERR_OK) exit (1);

    sd_notify(0,"READY=1");

    while (!shutdwn) {
        pause();
    }

    /* Let systemd know we got the shutdown request
     * and that we're in the process of shutting down */
    sd_notify(0, "STOPPING=1");

    /************************************
     *                                  *
     * Shutdown clean up code goes here *
     *                                  *
     ************************************/

    exit(EXIT_SUCCESS);
}

