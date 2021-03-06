#!/bin/bash

function setupipv6
{
echo
myip=`ifconfig eth0 | awk '/inet / {print $2}' | awk -F ":" '{print $2}'`
/sbin/ip tunnel add is0 mode sit remote 10.10.5.56 local $myip
/sbin/ip link set is0 up
/sbin/ip addr add 2001:da8:e000:90:0:5efe:$myip/64 dev is0
/sbin/ip route add default via 2001:da8:e000:90::1 dev is0
echo
if ifconfig | grep is0 >/dev/null
then
echo "Success to set up IPv6 Tunnel !"
else
echo "Fail to set up IPv6 Tunnel !"
fi
echo
}

function resetipv6
{
/sbin/ip tunnel del is0
setupipv6
}

if [ $# -lt 1 ]; then
   setupipv6
elif [ "$1" = "-r" ]; then
   resetipv6
else
echo
echo "Usage: ipv6    : set up ipv6 tunnel"
echo "       ipv6 -r : reset up ipv6 tunnel"
echo
fi
