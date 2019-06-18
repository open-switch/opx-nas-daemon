#!/usr/bin/python
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

import cps_object
from xml.dom import minidom
import cps
import nas_os_utils as nu
import syslog
from time import sleep
import os

""" This file currently has only mirror and sflow code handling
If you add more config to nas_default_init_config.xml please
add the code to handle those either in this script if it is
small or add another script to handle those
"""

direction_map = {
    "ingress": 1,
    "egress": 2,
    "ingress_egress": 3
}

tm_config_file = "/etc/opx/dn_nas_default_init_config.xml"


def do_transaction(obj, op):
    """Perfrom the transaction
    @obj - cps object
    @op- opertaion to be perfromed
    @return true if successful otherwise false
    """
    tr_obj = {'change': obj.get(), 'operation': op}
    l = [tr_obj]
    if cps.transaction(l):
        return True
    else:
        return False


def create_mirror_session(src, dst, direction):
    """create a new mirror session
    @src - source mirror interface
    @dst - destiantion mirrorr interface
    @direction - direction in which packets needs to be mirrored
    @return true if mirror session was created successfully otherwise false
    """
    data_dict = {"dst-intf": nu.if_nametoindex(str(dst)), "type": 1}
    obj = cps_object.CPSObject(module="base-mirror/entry",
                               data=data_dict)
    src_intf_list = ["intf", "0", "src"]
    obj.add_embed_attr(src_intf_list, nu.if_nametoindex(str(src)))
    src_intf_list[2] = "direction"
    obj.add_embed_attr(src_intf_list, direction_map[str(direction)])
    return do_transaction(obj, "create")


def create_sflow_session(interface, direction, rate):
    """create a new sflow session
    @interface - interface on which sflow is to be enabled
    @direction - direction in which packet sampling is to be done
    @rate - sampling packet rate
    @return true if sflow session is created othersie false
    """
    data_dict = {"ifindex": nu.if_nametoindex(str(interface)),
                 "direction": direction_map[str(direction)],
                 "sampling-rate": str(rate)
                 }
    obj = cps_object.CPSObject(module="base-sflow/entry", data=data_dict)
    return do_transaction(obj, "create")

def parse_mirror_config(mirror_list):
    """parse the mirror config from xml and
       create mirror sessions based on that
       @mirror_list - xml tag list of mirror config
    """
    try:
        for m in mirror_list:
            src = m.getElementsByTagName("source_interface")[0].firstChild.nodeValue
            dst = m.getElementsByTagName("destination_interface")[0].firstChild.nodeValue
            dir = m.getElementsByTagName("direction")[0].firstChild.nodeValue
            if not create_mirror_session(src, dst, dir):
                syslog.syslog("Failed to create miiror session with source %s"
                              " destination %s and in direction %s " % (src,
                                                                dst, dir))
    except Exception as ex:
        syslog.syslog(str(ex))


def parse_sflow_config(sflow_list):
    """parse the sflow config from xml and
       create sflow sessions based on that
       @sflow_list - xml tag list of sflow config
   """
    try:
        for s in sflow_list:
            iface = s.getElementsByTagName("interface")[0].firstChild.nodeValue
            rate = s.getElementsByTagName("rate")[0].firstChild.nodeValue
            dir = s.getElementsByTagName("direction")[0].firstChild.nodeValue
            if not create_sflow_session(iface, dir, rate):
                syslog.syslog("Failed to create sflow session with interface %s"
                              " rate %s and in direction %s " % (iface,
                                                                 rate, dir))
    except Exception as ex:
        syslog.syslog(str(ex))


def parse_config_file():
    """parse the xml file which has the config for
    default mirror sflow session and apply them
    """
    if not os.path.isfile(tm_config_file):
        print "%s does not exist" % (tm_config_file)
    try:
        config_xml_handle = minidom.parse(tm_config_file)
    except Exception:
        print "%s Is not a valid xml file" % (tm_config_file)

    try:
        mirror_list = config_xml_handle.getElementsByTagName("mirror")
        parse_mirror_config(mirror_list)

        sflow_list = config_xml_handle.getElementsByTagName("sflow")
        parse_sflow_config(sflow_list)
    except Exception as ex:
        syslog.syslog(str(ex))


if __name__ == '__main__':

    # Wait till sflow and mirror services are ready
    sflow_obj = cps_object.CPSObject(module="base-sflow/entry")
    mirror_obj = cps_object.CPSObject(module="base-mirror/entry")
    while cps.get([sflow_obj.get(), mirror_obj.get()], []) != True:
        sleep(1)

    parse_config_file()
