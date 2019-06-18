#
# Copyright (c) 2019 Dell Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may
# not use this file except in compliance with the License. You may obtain
# a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
# THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
# LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
# FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
#
# See the Apache Version 2.0 License for specific language governing
# permissions and limitations under the License.
#

ACLOCAL_AMFLAGS=-I m4
SUBDIRS = inc

bin_SCRIPTS = cfg/*.py cfg/*.sh bin/*

cfgdir = $(sysconfdir)/opx
cfg_SCRIPTS = cfg/*.xml

svcdir = $(sysconfdir)/sysctl.d
svc_SCRIPTS = cfg/etc/sysctl.d/*

bin_PROGRAMS=opx_nas_daemon

COMMON_HARDEN_FLAGS=-fexceptions -fstack-protector-strong -fstack-protector-all -D_FORTIFY_SOURCE=2 -Wall -Wformat -Wformat-security -Werror
C_HARDEN_FLAGS=-Wimplicit-function-declaration
LD_HARDEN_FLAGS=-Wl,-z,defs -Wl,-z,now

opx_nas_daemon_SOURCES=src/main.c src/hald_init.c

opx_nas_daemon_LDADD= -lopx_logging -lopx_cps_api_common \
                        -lpthread -lsystemd -lopx_nas_ndi \
                        -lopx_nas_common -lopx_nas_interface \
                        -lopx_hal_routing -lopx_nas_l2 -lopx_nas_qos \
			-lopx_nas_multicast \
                        -lopx_nas_packet_io -lopx_nas_linux -lopx_nas_acl

opx_nas_daemon_CPPFLAGS=-I$(top_srcdir)/inc/opx -I$(includedir)/opx $(COMMON_HARDEN_FLAGS)
opx_nas_daemon_CFLAGS=$(C_HARDEN_FLAGS)
opx_nas_daemon_LDFLAGS=-L/usr/lib/x86_64-linux-gnu -lopx_nas_ndi $(LD_HARDEN_FLAGS)

systemdconfdir=/lib/systemd/system
systemdconf_DATA = cfg/*.service

man1_MANS = man/man1/*
