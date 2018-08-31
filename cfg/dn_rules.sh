#!/bin/bash
#
# Copyright (c) 2015 Dell Inc.
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
#*******************FILTER table FORWARD chain rules****************
ebtables -I FORWARD -d BGA -j DROP
ebtables -A FORWARD -p ARP -j DROP
# stop FCoE and FIP Packets from forwarding on all ports.
ebtables -A FORWARD -p 0x8906 -j DROP
ebtables -A FORWARD -p 0x8914 -j DROP
# stop ICMPv6 neighbor solicitation & advertisement packets from forwarding on all ports.
ebtables -A FORWARD -p ipv6 --ip6-proto ipv6-icmp --ip6-icmp-type neighbour-solicitation -j DROP
ebtables -A FORWARD -p ipv6 --ip6-proto ipv6-icmp --ip6-icmp-type neighbour-advertisement -j DROP
#Mark all multicast and broadcast packets in forwarding chain
#and drop in the egress side. In some cases like
#IGMP and MLD snooping the control packets needs to be
#forwarded or flooded. So mark packets so that dropping
#in egress affects only the forwarded packets and not
#locally generated packets. Here packets can be tagged and untagged
#so there is no rule which can accept both tagged and untagged IGMP/MLD packets.
#So here IGMP/MLD rule is not added to accept it. The rule is added below in
#egress after POSTROUTING.
ebtables -A FORWARD -d Multicast -j mark --mark-set 0x1 --mark-target ACCEPT
ebtables -A FORWARD -d Broadcast -j mark --mark-set 0x2 --mark-target ACCEPT
ebtables -A FORWARD -p IPv4 -j DROP
ebtables -A FORWARD -p IPv6 -j DROP

#Accept/forward IGMP/MLD(Query - 130, Report - 131,Leave -132) packets
#for snooping and rest marked with "0x1" drop it.
ebtables -t nat -I POSTROUTING -p IPv4 --ip-proto igmp --mark 0x1 -j ACCEPT
ebtables -t nat -I POSTROUTING -p IPv6 --ip6-proto ipv6-icmp --ip6-icmp-type 130 --mark 0x1 -j ACCEPT
ebtables -t nat -I POSTROUTING -p IPv6 --ip6-proto ipv6-icmp --ip6-icmp-type 131 --mark 0x1 -j ACCEPT
ebtables -t nat -I POSTROUTING -p IPv6 --ip6-proto ipv6-icmp --ip6-icmp-type 132 --mark 0x1 -j ACCEPT
ebtables -t nat -A POSTROUTING -d Broadcast --mark 0x2 -j DROP
ebtables -t nat -A POSTROUTING -d Multicast --mark 0x1 -j DROP


#Accept ARP request on eth0 (untagged/tagged interfaces) and don't copy it to NFLOG
ebtables -A OUTPUT -p ARP -o eth0+ --arp-op Request -j ACCEPT

# Avoiding the VRRP MAC destined packets flooding in bridge level, redirecting the packets
# to the next layer (routing) for further lookup.
ebtables -t nat -N IPV4VRRP
ebtables -t nat -N IPV6VRRP
ebtables -t nat -A PREROUTING -d 00:00:5e:00:01:00/ff:ff:ff:ff:ff:00 -j IPV4VRRP
ebtables -t nat -A PREROUTING -d 00:00:5e:00:02:00/ff:ff:ff:ff:ff:00 -j IPV6VRRP
ebtables -t nat -A IPV4VRRP -p IPv4 -j redirect --redirect-target ACCEPT

ebtables -t nat -A IPV6VRRP -p IPv6 --ip6-protocol ipv6-icmp -j RETURN
ebtables -t nat -A IPV6VRRP -p IPv6 -j redirect --redirect-target ACCEPT


