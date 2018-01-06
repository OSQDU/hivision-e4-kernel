/*
 *
 * Copyright (C) 2015 Fullhan.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef ARCH_ARM_MACH_FH_INCLUDE_MACH_LINBAO_H_
#define ARCH_ARM_MACH_FH_INCLUDE_MACH_LINBAO_H_

#include <linux/init.h>

#define RAM_BASE                      (0x10000000)
#define DDR_BASE                      (0xA0000000)

#define PMU_REG_BASE                  (0xF0000000)
#define TIMER_REG_BASE                (0xf0c00000)
#define GPIO0_REG_BASE                (0xf0300000)
#define GPIO1_REG_BASE                (0xf4000000)
#define UART0_REG_BASE                (0xf0700000)
#define UART1_REG_BASE                (0xf0800000)
#define UART2_REG_BASE                (0xf0880000)
#define SPI0_REG_BASE                 (0xf0500000)
#define SPI1_REG_BASE                 (0xf0600000)
#define SPI2_REG_BASE                 (0xf0640000)
#define SPI3_REG_BASE                 (0xf0680000)
#define INTC_REG_BASE                 (0xE0200000)
#define GMAC_REG_BASE                 (0xEF000000)
#define DMAC_REG_BASE                 (0xEE000000)
#define I2C1_REG_BASE                 (0xf0b00000)
#define I2C0_REG_BASE                 (0xf0200000)
#define SDC0_REG_BASE                 (0xe2000000)
#define SDC1_REG_BASE                 (0xef300000)
#define WDT_REG_BASE                  (0xf0d00000)
#define PWM_REG_BASE                  (0xf0400000)
#define PAE_REG_BASE                  (0xec100000)
#define I2S_REG_BASE                  (0xf0900000)
#define ACW_REG_BASE                  (0xf0a00000)
#define SADC_REG_BASE                 (0xf1200000)
#define EFUSE_REG_BASE				  (0xf1600000)
#define AES_REG_BASE                  (0xe8200000)
#define RTC_REG_BASE                  (0xf1500000)
#define DDRC_REG_BASE                 (0xED000000)
#define CONSOLE_REG_BASE              UART0_REG_BASE

#define FH_UART_NUMBER                3

#define PMU_REG_SIZE                  0x220
#define PMU_DEBUG

#define REG_PMU_CHIP_ID                  (0x0000)
#define REG_PMU_IP_VER                   (0x0004)
#define REG_PMU_FW_VER                   (0x0008)
#define REG_PMU_SYS_CTRL                 (0x000c)
#define REG_PMU_PLL0                     (0x0010)
#define REG_PMU_PLL1                     (0x0014)
#define REG_PMU_PLL2                     (0x0018)
#define REG_PMU_CLK_GATE                 (0x001c)
#define REG_PMU_CLK_SEL                  (0x0020)
#define REG_PMU_CLK_DIV0                 (0x0024)
#define REG_PMU_CLK_DIV1                 (0x0028)
#define REG_PMU_CLK_DIV2                 (0x002c)
#define REG_PMU_CLK_DIV3                 (0x0030)
#define REG_PMU_CLK_DIV4                 (0x0034)
#define REG_PMU_CLK_DIV5                 (0x0038)
#define REG_PMU_CLK_DIV6                 (0x003c)
#define REG_PMU_SWRST_MAIN_CTRL          (0x0040)
#define REG_PMU_SWRST_AXI_CTRL           (0x0044)
#define REG_PMU_SWRST_AHB_CTRL           (0x0048)
#define REG_PMU_SWRST_APB_CTRL           (0x004c)
#define REG_PMU_SPC_IO_STATUS            (0x0054)
#define REG_PMU_SPC_FUN                  (0x0058)
#define REG_PMU_CLK_DIV7                 (0x005c)
#define REG_PMU_CLK_GATE_S               (0x0060)
#define REG_PMU_DBG_SPOT0                (0x0064)
#define REG_PMU_DBG_SPOT1                (0x0068)
#define REG_PMU_DBG_SPOT2                (0x006c)
#define REG_PMU_DBG_SPOT3                (0x0070)

#define REG_PMU_PAD_CIS_HSYNC_CFG        (0x0080)
#define REG_PMU_PAD_CIS_VSYNC_CFG        (0x0084)
#define REG_PMU_PAD_CIS_PCLK_CFG         (0x0088)
#define REG_PMU_PAD_CIS_D_0_CFG          (0x008c)
#define REG_PMU_PAD_CIS_D_1_CFG          (0x0090)
#define REG_PMU_PAD_CIS_D_2_CFG          (0x0094)
#define REG_PMU_PAD_CIS_D_3_CFG          (0x0098)
#define REG_PMU_PAD_CIS_D_4_CFG          (0x009c)
#define REG_PMU_PAD_CIS_D_5_CFG          (0x00a0)
#define REG_PMU_PAD_CIS_D_6_CFG          (0x00a4)
#define REG_PMU_PAD_CIS_D_7_CFG          (0x00a8)
#define REG_PMU_PAD_CIS_D_8_CFG          (0x00ac)
#define REG_PMU_PAD_CIS_D_9_CFG          (0x00b0)
#define REG_PMU_PAD_CIS_D_10_CFG         (0x00b4)
#define REG_PMU_PAD_CIS_D_11_CFG         (0x00b8)
#define REG_PMU_PAD_MAC_RMII_CLK_CFG     (0x00bc)
#define REG_PMU_MAC_REF_CLK_CFG          (0x00c0)
#define REG_PMU_PAD_MAC_MDC_CFG          (0x00c4)
#define REG_PMU_PAD_MAC_MDIO_CFG         (0x00c8)
#define REG_PMU_PAD_MAC_COL_CFG          (0x00cc)
#define REG_PMU_PAD_MAC_CRS_CFG          (0x00d0)
#define REG_PMU_PAD_MAC_RXCK_CFG         (0x00d4)
#define REG_PMU_PAD_MAC_RXD0_CFG         (0x00d8)
#define REG_PMU_PAD_MAC_RXD1_CFG         (0x00dc)
#define REG_PMU_PAD_MAC_RXD2_CFG         (0x00e0)
#define REG_PMU_PAD_MAC_RXD3_CFG         (0x00e4)
#define REG_PMU_PAD_MAC_RXDV_CFG         (0x00e8)
#define REG_PMU_PAD_MAC_TXCK_CFG         (0x00ec)
#define REG_PMU_PAD_MAC_TXD0_CFG         (0x00f0)
#define REG_PMU_PAD_MAC_TXD1_CFG         (0x00f4)
#define REG_PMU_PAD_MAC_TXD2_CFG         (0x00f8)
#define REG_PMU_PAD_MAC_TXD3_CFG         (0x00fc)
#define REG_PMU_PAD_MAC_TXEN_CFG         (0x0100)
#define REG_PMU_PAD_MAC_RXER_CFG         (0x0104)
#define REG_PMU_PAD_MAC_TXER_CFG         (0x0108)
#define REG_PMU_PAD_GPIO_0_CFG           (0x010c)
#define REG_PMU_PAD_GPIO_1_CFG           (0x0110)
#define REG_PMU_PAD_GPIO_2_CFG           (0x0114)
#define REG_PMU_PAD_GPIO_3_CFG           (0x0118)
#define REG_PMU_PAD_GPIO_4_CFG           (0x011c)
#define REG_PMU_PAD_GPIO_5_CFG           (0x0120)
#define REG_PMU_PAD_GPIO_6_CFG           (0x0124)
#define REG_PMU_PAD_GPIO_7_CFG           (0x0128)
#define REG_PMU_PAD_GPIO_8_CFG           (0x012c)
#define REG_PMU_PAD_GPIO_9_CFG           (0x0130)
#define REG_PMU_PAD_GPIO_10_CFG          (0x0134)
#define REG_PMU_PAD_GPIO_11_CFG          (0x0138)
#define REG_PMU_PAD_GPIO_12_CFG          (0x013c)
#define REG_PMU_PAD_GPIO_13_CFG          (0x0140)
#define REG_PMU_PAD_GPIO_14_CFG          (0x0144)
#define REG_PMU_PAD_UART_RX_CFG          (0x0148)
#define REG_PMU_PAD_UART_TX_CFG          (0x014c)
#define REG_PMU_PAD_CIS_SCL_CFG          (0x0150)
#define REG_PMU_PAD_CIS_SDA_CFG          (0x0154)
#define REG_PMU_PAD_I2C_SCL_CFG          (0x0158)
#define REG_PMU_PAD_I2C_SDA_CFG          (0x015c)
#define REG_PMU_PAD_SSI0_CLK_CFG         (0x0160)
#define REG_PMU_PAD_SSI0_TXD_CFG         (0x0164)
#define REG_PMU_PAD_SSI0_CSN_0_CFG       (0x0168)
#define REG_PMU_PAD_SSI0_CSN_1_CFG       (0x016c)
#define REG_PMU_PAD_SSI0_RXD_CFG         (0x0170)
#define REG_PMU_PAD_SD0_CD_CFG           (0x0174)
#define REG_PMU_PAD_SD0_WP_CFG           (0x0178)
#define REG_PMU_PAD_SD0_CLK_CFG          (0x017c)
#define REG_PMU_PAD_SD0_CMD_RSP_CFG      (0x0180)
#define REG_PMU_PAD_SD0_DATA_0_CFG       (0x0184)
#define REG_PMU_PAD_SD0_DATA_1_CFG       (0x0188)
#define REG_PMU_PAD_SD0_DATA_2_CFG       (0x018c)
#define REG_PMU_PAD_SD0_DATA_3_CFG       (0x0190)
#define REG_PMU_PAD_SD1_CD_CFG           (0x0194)
#define REG_PMU_PAD_SD1_WP_CFG           (0x0198)
#define REG_PMU_PAD_SD1_CLK_CFG          (0x019c)
#define REG_PMU_PAD_SD1_CMD_RSP_CFG      (0x01a0)
#define REG_PMU_PAD_SD1_DATA_0_CFG       (0x01a4)
#define REG_PMU_PAD_SD1_DATA_1_CFG       (0x01a8)
#define REG_PMU_PAD_SD1_DATA_2_CFG       (0x01ac)
#define REG_PMU_PAD_SD1_DATA_3_CFG       (0x01b0)
#define REG_PMU_AXI0_PRIO_CFG0           (0x01b4)
#define REG_PMU_AXI0_PRIO_CFG1           (0x01b8)
#define REG_PMU_AXI1_PRIO_CFG0           (0x01bc)
#define REG_PMU_AXI1_PRIO_CFG1           (0x01c0)
#define REG_PMU_SWRSTN_NSR               (0x01c4)
#define REG_PMU_REMAP                    (0x01c8)
#define REG_PMU_A625_START_CTRL          (0x01cc)
#define REG_PMU_A625BOOT0                (0x01d0)
#define REG_PMU_A625BOOT1                (0x01d4)
#define REG_PMU_A625BOOT2                (0x01d8)
#define REG_PMU_A625BOOT3                (0x01dc)
#define REG_PMU_ARM_INT_0                (0x01e0)
#define REG_PMU_ARM_INT_1                (0x01e4)
#define REG_PMU_ARM_INT_2                (0x01e8)
#define REG_PMU_A625_INT_0               (0x01ec)
#define REG_PMU_A625_INT_1               (0x01f0)
#define REG_PMU_A625_INT_2               (0x01f4)
#define REG_PMU_DMA                      (0x01f8)
#define REG_PMU_WDT_CTRL                 (0x01fc)
#define REG_PMU_VDAC_CTRL                (0x0200)
#define REG_PMU_VDAC_STAT                (0x0204)
#define REG_PMU_DBG_STAT0                (0x0208)
#define REG_PMU_DBG_STAT1                (0x020c)
#define REG_PMU_DBG_STAT2                (0x0210)
#define REG_PMU_DBG_STAT3                (0x0214)
#define REG_PMU_AXI2_PRIO_CFG            (0x0218)

/*ATTENTION: written by ARC */
#define PMU_ARM_INT_MASK             (0x01e0)
#define PMU_ARM_INT_RAWSTAT          (0x01e4)
#define PMU_ARM_INT_STAT             (0x01e8)

#define PMU_A625_INT_MASK             (0x01ec)
#define PMU_A625_INT_RAWSTAT          (0x01f0)
#define PMU_A625_INT_STAT             (0x01f4)

#define FH81_N_AINTC_IRQ	64

#define PAE_IRQ		0
#define VPU_IRQ		1
#define FD_IRQ		2
#define ISP_IRQ		3
#define VOU_IRQ		4
#define JPEG_IRQ	5
#define BGM_IRQ		6
#define TOE_IRQ		7
#define DDRC_IRQ	8
#define DMAC_IRQ	9
#define AES_IRQ		10
#define MIPIC_IRQ	11
#define MIPI_WRAP_IRQ	12
#define PMU_IRQ		13
#define GMAC_IRQ	14
#define AXIC2_IRQ	15
#define AXIC0_IRQ	16
#define AXIC1_IRQ	17
#define X2H0_IRQ	18
#define X2H1_IRQ	19
#define AHBC0_IRQ	20
#define AHBC1_IRQ	21
#define AHBC2_IRQ	22
#define SADC_IRQ	23
#define SDC0_IRQ	24
#define SDC1_IRQ	25
#define ACW_IRQ		26
#define WDT_IRQ		27
#define SPI0_IRQ	28
#define SPI1_IRQ	29
#define UART0_IRQ	30
#define UART1_IRQ	31

#define I2S0_IRQ	32
#define SPI2_IRQ	33
#define SPI3_IRQ	34
#define UART2_IRQ	35
#define TMR0_IRQ	36
#define TMR1_IRQ	37
#define RESERVE_USB_IRQ	38
#define DMAC1_IRQ	39
#define GPIO0_IRQ	40
#define GPIO1_IRQ	41
#define I2C0_IRQ	42
#define I2C1_IRQ	43
#define GPIO_N1_IRQ	44	/* External Interruppt */
#define GPIO_N2_IRQ	45
#define GPIO_N3_IRQ	46
#define GPIO_N4_IRQ	47
#define INTC_INTRA_IRQ	48
#define INTC_INTRB_IRQ	49
#define INTC_INTRC_IRQ	50
#define INTC_INTRD_IRQ	51
#define ARM_PMU_IRQ	52	/* arm performance monitor uint */
#define RTC_IRQ		53

#define WDT1_IRQ	55
#define ARC_SW_IRQ	60
#define ARM_SW_IRQ	61
/* */
#define FH_MAX_IRQ	ARM_SW_IRQ

#define NR_INTERNAL_IRQS 64
#define NR_EXTERNAL_IRQS 64
#define NR_IRQS	(NR_INTERNAL_IRQS+NR_EXTERNAL_IRQS)

#define INTNR_IRQ_START 	0
#define INTNR_IRQ_END 		FH_MAX_IRQ		/* set max irq value */

/*
#define FH81_IRQS_L 	(1<<GMAC_IRQ \
		| 1<<UART0_IRQ \
		| 1<<DMAC0_IRQ \
		| 1<<SPI0_IRQ \
		| 1<<SDC0_IRQ \
		| 1<<SDC1_IRQ \
		) //0x40004000)
#define FH81_IRQS_H	(1<<(TMR0_IRQ-32)\
		| 1<<(I2C0_IRQ-32)\
		| 1<<(GPIO0_IRQ-32)\
		| 1<<(ARC_SW_IRQ-32)\
		)		//0x10
*/

#endif /* ARCH_ARM_MACH_FH_INCLUDE_MACH_LINBAO_H_ */
