#!/bin/bash

app_create()
{
    nfs_dir=$1
	make ARCH=arm CROSS_COMPILE=arm-fullhan-linux-uclibcgnueabi- -C wlan_src/mapp/mlanutl
	make ARCH=arm CROSS_COMPILE=arm-fullhan-linux-uclibcgnueabi- -C wlan_src/mapp/uaputl
	
	cp -f wlan_src/mlan.ko $nfs_dir/
	cp -f wlan_src/sd8xxx.ko $nfs_dir/sd8801.ko
	cp -f wlan_src/mapp/uaputl/uaputl.exe $nfs_dir/
	cp -f wlan_src/mapp/mlanutl/mlanutl $nfs_dir/
    cp -f FwImage/sd8801_uapsta.bin $nfs_dir/	
}

#~/nfs/fh8810/ is the nfs_dir
app_create ~/nfs/fh8810/
