#!/bin/sh

ln -s /mnt/bcmdhd /bcmdhd
cd /bcmdhd

insmod bcmdhd.ko
#dhd_watchdog_ms=1000 //1000 is default
#dhd_msg_level=0xffff

./wpa_supplicant -Dwext -iwlan0 -c./wpa_supplicant.conf -B
sleep 7

ifconfig wlan0 192.168.1.229 netmask 255.255.255.0
sleep 5

./iperf -c 192.168.1.239 -i 1 -m -f M -w 256k -t 7200 &
