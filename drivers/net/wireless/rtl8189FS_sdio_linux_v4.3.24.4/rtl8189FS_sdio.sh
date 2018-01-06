#!/bin/bash

hostapd_and_rtl_hostapd_2G()
{
	cd wpa_supplicant_hostapd-0.8_rtw_r7475.20130812/hostapd
	make
	cp hostapd ~/nfs/fh8810/
	cd ../..
	cp rtl_hostapd_2G.conf ~/nfs/fh8810/
	cp rtl8189FS_linux_v4.3.24.4_18988.20160812/8189fs.ko ~/nfs/fh8810/
}

# ~/nfs/fh8810/ is the nfs dir in pc-linux, please modify it if need
hostapd_and_rtl_hostapd_2G