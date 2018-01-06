/*
* Customer code to add GPIO control during WLAN start/stop
* Copyright (C) 1999-2011, Broadcom Corporation
* 
*         Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2 (the "GPL"),
* available at http://www.broadcom.com/licenses/GPLv2.php, with the
* following added to such license:
* 
*      As a special exception, the copyright holders of this software give you
* permission to link this software with independent modules, and to copy and
* distribute the resulting executable under terms of your choice, provided that
* you also meet, for each linked independent module, the terms and conditions of
* the license of that module.  An independent module is a module which is not
* derived from this software.  The special exception does not apply to any
* modifications of the software.
* 
*      Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a license
* other than the GPL, without Broadcom's express prior written consent.
*
* $Id: dhd_custom_gpio.c,v 1.2.42.1 2010-10-19 00:41:09 Exp $
*/

#include <osl.h>

#ifdef CUSTOMER_HW

#include <linux/gpio.h>

#define WIFI_IRQ_GPIO	7

#ifdef CUSTOMER_OOB
int bcm_wlan_get_oob_irq(void)
{
	int host_oob_irq = 0;
	printk("GPIO(WL_HOST_WAKE) = %d\n", WIFI_IRQ_GPIO);
	gpio_request(WIFI_IRQ_GPIO, NULL);
	host_oob_irq = gpio_to_irq(WIFI_IRQ_GPIO);
	gpio_direction_input(WIFI_IRQ_GPIO);
/*	gpio_free(7);*/
	printk("host_oob_irq: %d \r\n", host_oob_irq);

	return host_oob_irq;
}
#endif

void bcm_wlan_power_on(int flag)
{
	if (flag == 1) {
		printk("======== PULL WL_REG_ON HIGH! ========\n");
	} else {
		printk("======== PULL WL_REG_ON HIGH! (flag = %d) ========\n", flag);
	}
}

void bcm_wlan_power_off(int flag)
{
	if (flag == 1) {
		printk("======== Card detection to remove SDIO card! ========\n");
	} else {
		printk("======== PULL WL_REG_ON LOW! (flag = %d) ========\n", flag);
	}
}

#endif /* CUSTOMER_HW */
