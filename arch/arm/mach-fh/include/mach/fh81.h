/*
 *	Copyright (c) 2010 Shanghai Fullhan Microelectronics Co., Ltd.
 *				All Rights Reserved. Confidential.
 *
 *This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef __ASM_ARCH_FH81_H
#define __ASM_ARCH_FH81_H

#ifdef CONFIG_ARCH_FH81

#include <linux/init.h>

#define FH81_VBUS_ENABLE

#define		RAM_BASE		(0x10000000)
#define		DDR_BASE		(0xA0000000)

// module register base address: <moduleName>_REG_BASE
#define		PMU_REG_BASE		(0xF0000000)
#define		TIMER_REG_BASE		(0xf0c00000)
#define		GPIO0_REG_BASE		(0xf0300000)
#define		GPIO1_REG_BASE		(0xf4000000)
#define		UART0_REG_BASE		(0xf0700000)
#define		UART1_REG_BASE		(0xf0800000)
#define		SPI0_REG_BASE		(0xf0500000)
#define		SPI1_REG_BASE		(0xf0600000)
#define		SPI2_REG_BASE		(0x99300000)
#define		INTC_REG_BASE		(0xE0200000)
#define		GMAC_REG_BASE		(0xEF000000)
#define		DMAC_REG_BASE		(0xEE000000)
#define		I2C1_REG_BASE		(0xf0b00000)
#define		I2C0_REG_BASE		(0xf0200000)
#define		SDC0_REG_BASE		(0xe2000000)
#define		SDC1_REG_BASE		(0xe2100000)
#define		WDT_REG_BASE		(0xf0d00000)
#define		PWM_REG_BASE		(0xf0400000)
#define		PAE_REG_BASE		(0xec100000)
#define		I2S_REG_BASE		(0xf0900000)
#define		ACW_REG_BASE		(0xf0a00000)
#define		SADC_REG_BASE		(0xf1200000)
#define		AES_REG_BASE		(0xe8200000)
#define		DDRC_REG_BASE		(0xED000000)
/* Public function ----------------------------------------------------------*/
#ifdef CONFIG_ARCH_FH81
#define CONSOLE_REG_BASE		UART1_REG_BASE
#elif defined(CONFIG_ARCH_WUDANG)
#define CONSOLE_REG_BASE		UART1_REG_BASE
#else
#define CONSOLE_REG_BASE		UART0_REG_BASE
#endif

#define FH_UART_NUMBER		2

#define PMU_REG_SIZE		0x200
#define PMU_DEBUG

#define REG_PMU_CHIP_ID	 	 (0x000)
#define REG_PMU_IP_VER		 (0x004)
#define REG_PMU_FW_VER		 (0x008)
#define REG_PMU_SYS_CTRL	 (0x00c)
#define REG_PMU_PLL0_CTRL	 (0x010)
#define REG_PMU_PLL1_CTRL	 (0x014)
#define REG_PMU_ARC_CLK_GATE	 (0x018)
#define REG_PMU_CLK_GATE	 (0x01c)
#define REG_PMU_CLK_SEL		 (0x020)
#define REG_PMU_CLK_DIV0	 (0x024)
#define REG_PMU_CLK_DIV1	 (0x028)
#define REG_PMU_CLK_DIV2	 (0x02c)
#define REG_PMU_CLK_DIV3	 (0x030)
#define REG_PMU_CLK_DIV4	 (0x034)
#define REG_PMU_CLK_DIV5	 (0x038)
#define REG_PMU_CLK_DIV6	 (0x03c)
#define REG_PMU_SWRST_MAIN_CTRL (0x040)
#define REG_PMU_SWRST_AXI_CTRL  (0x044)
#define REG_PMU_SWRST_AHB_CTRL  (0x048)
#define REG_PMU_SWRST_APB_CTRL  (0x04c)
#define REG_PMU_VDAC_CTRL	 (0x050)
#define REG_PMU_MAC_REF_CLK_CFG (0x0a4)

#ifdef FH81_VBUS_ENABLE
/*ATTENTION: written by ARC */
#define PMU_ARM_INT_MASK	(0x020)
#define PMU_ARM_INT_RAWSTAT	(0x020)
#define PMU_ARM_INT_STAT	(0x020)

#define PMU_A625_INT_MASK	(0x030)
#define PMU_A625_INT_RAWSTAT	(0x02c)
#define PMU_A625_INT_STAT	(0x028)
#endif

#define FH81_N_AINTC_IRQ	64

#define PAE_IRQ			0
#define VPU_IRQ			1
#define ISP_F_IRQ		2
#define ISP_B_IRQ		3
#define VOU_IRQ			4
#define JPEG_IRQ		5
#define TVE_IRQ			6
#define TOE_IRQ			7
#define DDRC_IRQ		8
#define DMAC_IRQ		9
#define AES_IRQ			10
#define MIPIC_IRQ		11
#define MIPI_WRAP_IRQ		12
#define PMU_IRQ			13
#define GMAC_IRQ		14
#define Reserved1_IRQ		15
#define AXIC0_IRQ		16
#define AXIC1_IRQ		17
#define X2H0_IRQ		18
#define X2H1_IRQ		19
#define AHBC0_IRQ		20
#define AHBC1_IRQ		21
#define RESERVED2_IRQ		22
#define SADC_IRQ		23
#define SDC0_IRQ		24
#define SDC1_IRQ		25
#define ACW_IRQ			26
#define WDT_IRQ			27
#define SPI0_IRQ		28
#define SPI1_IRQ		29
#define UART0_IRQ		30
#define UART1_IRQ		31
#define I2S0_IRQ		32
#define I2S1_IRQ		33
#define RTC_IRQ			34
#define PWM_IRQ			35
#define TMR0_IRQ		36
#define TMR1_IRQ		37
#define USB0_IRQ		38
#define USB1_IRQ		39
#define GPIO0_IRQ		40
#define GPIO1_IRQ		41
#define I2C0_IRQ		42
#define I2C1_IRQ		43
#define GPIO_N1_IRQ		44		/* External Interruppt */
#define GPIO_N2_IRQ		45
#define GPIO_N3_IRQ		46
#define GPIO_N4_IRQ		47
#define INTC_INTRA_IRQ		48
#define INTC_INTRB_IRQ		49
#define INTC_INTRC_IRQ		50
#define INTC_INTRD_IRQ		51
#define ARM_PMU_IRQ		52		/* arm performance monitor uint */

#ifdef FH81_VBUS_ENABLE		/* vbus definition */
#define ARC_SW_IRQ		PAE_IRQ
#define ARM_SW_IRQ		6

#define FH_MAX_IRQ		ARM_PMU_IRQ

#define NR_INTERNAL_IRQS	64
#define NR_EXTERNAL_IRQS	64
#define NR_IRQS			(NR_INTERNAL_IRQS+NR_EXTERNAL_IRQS)

#define INTNR_IRQ_START		0
#define INTNR_IRQ_END		FH_MAX_IRQ		/* set max irq value */
#else			/* normal definition */
#define NR_INTERNAL_IRQS	56
#define NR_EXTERNAL_IRQS	64

#define INTNR_IRQ_START		0
#define INTNR_IRQ_END		ARM_PMU_IRQ		/* set max irq value */

#define FH81_IRQS_L	(1<<GMAC_IRQ \
		| 1<<UART0_IRQ \
		| 1<<DMAC_IRQ \
		| 1<<SPI0_IRQ \
		| 1<<SDC0_IRQ \
		| 1<<SDC1_IRQ \
		) //0x40004000)
#define FH81_IRQS_H		(1<<(TMR0_IRQ-32)\
		| 1<<(I2C0_IRQ-32)\
		| 1<<(GPIO0_IRQ-32)\
		)		//0x10
#endif
/* FH81 currently has the most gpio pins (144) */
///#define FH81_N_GPIO			144
/* FH81 currently has the most irqs so use FH81_N_CP_INTC_IRQ */
///#define NR_IRQS				(FH81_N_AINTC_IRQ + FH81_N_GPIO)
#endif /* #ifdef CONFIG_ARCH_FH81 */

#endif /* __ASM_ARCH_FH81_H */
