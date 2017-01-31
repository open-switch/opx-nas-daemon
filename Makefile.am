ACLOCAL_AMFLAGS=-I m4
SUBDIRS = inc

bin_SCRIPTS = cfg/*.py cfg/*.sh

cfgdir = $(sysconfdir)/opx
cfg_SCRIPTS = cfg/*.xml

svcdir = $(sysconfdir)/sysctl.d
svc_SCRIPTS = cfg/etc/sysctl.d/*

bin_PROGRAMS=opx_nas_daemon

opx_nas_daemon_SOURCES=src/main.c src/hald_init.c

opx_nas_daemon_LDADD= -lopx_logging -lopx_cps_api_common \
                        -lpthread -lm -lsystemd  -lopx_nas_ndi \
                        -lopx_common -lopx_nas_common \
                        -lopx_nas_interface \
                        -lopx_hal_routing -lopx_nas_l2 -lopx_nas_qos \
                        -lopx_nas_packet_io -lopx_nas_linux -lopx_nas_acl

opx_nas_daemon_CPPFLAGS= -I$(top_srcdir)/inc/opx -I$(includedir)/opx
systemdconfdir=/lib/systemd/system
systemdconf_DATA = cfg/*.service
