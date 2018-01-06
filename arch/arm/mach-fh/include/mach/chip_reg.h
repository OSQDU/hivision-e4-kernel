/*****************************************************************************
*
*                                  chip_reg.h
*
*   Copyright (c) 2010 Shanghai Fullhan Microelectronics Co., Ltd.
*                       All Rights Reserved. Confidential.
*
*   File Description:
*       Chip register definition of the whole chip.
*       Include the address of each hardware register
*
*   Modification History:
*
******************************************************************************/
#ifndef _FH81_CHIP_REG_H_
#define _FH81_CHIP_REG_H_

#include <mach/chip.h>
#include <mach/io.h>

//#define		REG_PAE_PTS_REG				(PAE_REG_BASE + 0x040)


/*
//TMR
#define	REG_TIMER0_LOADCNT				(TIMER_REG_BASE + 0x00)
#define	REG_TIMER0_CUR_VAL				(TIMER_REG_BASE + 0x04)
#define	REG_TIMER0_CTRL_REG				(TIMER_REG_BASE + 0x08)
#define	REG_TIMER0_EOI_REG				(TIMER_REG_BASE + 0x0C)
#define	REG_TIMER0_INTSTATUS			(TIMER_REG_BASE + 0x10)

#define	REG_TIMER1_LOADCNT				(TIMER_REG_BASE + 0x14)
#define	REG_TIMER1_CUR_VAL				(TIMER_REG_BASE + 0x18)
#define	REG_TIMER1_CTRL_REG				(TIMER_REG_BASE + 0x1C)
#define	REG_TIMER1_EOI_REG				(TIMER_REG_BASE + 0x20)
#define	REG_TIMER1_INTSTATUS			(TIMER_REG_BASE + 0x24)

//intc
#define		REG_IRQ_EN_LOW				(INTC_REG_BASE + 0x0000)
#define		REG_IRQ_EN_HIGH				(INTC_REG_BASE + 0x0004)
#define		REG_IRQ_IRQMASK_LOW			(INTC_REG_BASE + 0x0008)
#define		REG_IRQ_IRQMASK_HIGH		(INTC_REG_BASE + 0x000C)
#define		REG_IRQ_IRQFORCE_LOW		(INTC_REG_BASE + 0x0010)
#define		REG_IRQ_IRQFORCE_HIGH		(INTC_REG_BASE + 0x0014)
#define		REG_IRQ_RAWSTATUS_LOW		(INTC_REG_BASE + 0x0018)
#define		REG_IRQ_RAWSTATUS_HIGH		(INTC_REG_BASE + 0x001C)
#define		REG_IRQ_STATUS_LOW			(INTC_REG_BASE + 0x0020)
#define		REG_IRQ_STATUS_HIGH			(INTC_REG_BASE + 0x0024)
#define		REG_IRQ_MASKSTATUS_LOW		(INTC_REG_BASE + 0x0028)
#define		REG_IRQ_MASKSTATUS_HIGH		(INTC_REG_BASE + 0x002C)
#define		REG_IRQ_FINSTATUS_LOW		(INTC_REG_BASE + 0x0030)
#define		REG_IRQ_FINSTATUS_HIGH		(INTC_REG_BASE + 0x0034)
#define		REG_FIQ_EN_LOW				(INTC_REG_BASE + 0x02C0)
#define		REG_FIQ_EN_HIGH				(INTC_REG_BASE + 0x02C4)
#define		REG_FIQ_FIQMASK_LOW			(INTC_REG_BASE + 0x02C8)
#define		REG_FIQ_FIQMASK_HIGH		(INTC_REG_BASE + 0x02CC)
#define		REG_FIQ_FIQFORCE_LOW		(INTC_REG_BASE + 0x02D0)
#define		REG_FIQ_FIQFORCE_HIGH		(INTC_REG_BASE + 0x02D4)
#define		REG_FIQ_RAWSTATUS_LOW		(INTC_REG_BASE + 0x02D8)
#define		REG_FIQ_RAWSTATUS_HIGH		(INTC_REG_BASE + 0x02DC)
#define		REG_FIQ_STATUS_LOW			(INTC_REG_BASE + 0x02E0)
#define		REG_FIQ_STATUS_HIGH			(INTC_REG_BASE + 0x02E4)
#define		REG_FIQ_FINSTATUS_LOW		(INTC_REG_BASE + 0x02E8)
#define		REG_FIQ_FINSTATUS_HIGH		(INTC_REG_BASE + 0x02EC)

//GMAC-MAC
#define		REG_GMAC_CONFIG				(0x0000)
#define		REG_GMAC_FRAME_FILTER		(0x0004)
#define		REG_GMAC_HASH_HIGH			(0x0008)
#define		REG_GMAC_HASH_LOW			(0x000C)
#define		REG_GMAC_GMII_ADDRESS		(0x0010)
#define		REG_GMAC_GMII_DATA			(0x0014)
#define		REG_GMAC_DEBUG				(0x0024)
#define		REG_GMAC_MAC_HIGH 			(0x0040)
#define		REG_GMAC_MAC_LOW 			(0x0044)

#define		REG_GMAC_BUS_MODE			(0x1000)
#define		REG_GMAC_TX_POLL_DEMAND		(0x1004)
#define		REG_GMAC_RX_POLL_DEMAND		(0x1008)
#define		REG_GMAC_RX_DESC_ADDR		(0x100C)
#define		REG_GMAC_TX_DESC_ADDR		(0x1010)
#define		REG_GMAC_STATUS				(0x1014)
#define		REG_GMAC_OP_MODE			(0x1018)
#define		REG_GMAC_INTR_EN			(0x101C)
#define		REG_GMAC_ERROR_COUNT		(0x1020)
#define		REG_GMAC_AXI_BUS_MODE		(0x1028)
#define		REG_GMAC_AXI_STATUS			(0x102C)
#define 	REG_GMAC_CURR_TX_DESC		(0x1048)
#define 	REG_GMAC_CURR_RX_DESC		(0x104C)
//GMAC-MMC
#define		REG_GMAC_MMC_HIGH_INTR		(0x10C)
#define		REG_GMAC_MMC_LOW_INTR		(0x110)

#define		SDC_INTERVAL			(0x100000)
#define		REG_SDC_CTRL(n)			(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0000)
#define		REG_SDC_PWREN(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0004)
#define		REG_SDC_CLKDIV(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0008)
#define		REG_SDC_CLKSRC(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x000C)
#define		REG_SDC_CLKENA(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0010)
#define		REG_SDC_TMOUT(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0014)
#define		REG_SDC_CTYPE(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0018)
#define		REG_SDC_BLKSIZ(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x001C)
#define		REG_SDC_BYTCNT(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0020)
#define		REG_SDC_INTMASK(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0024)
#define		REG_SDC_CMDARG(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0028)
#define		REG_SDC_CMD(n)			(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x002C)
#define		REG_SDC_RESP0(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0030)
#define		REG_SDC_RESP1(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0034)
#define		REG_SDC_RESP2(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0038)
#define		REG_SDC_RESP3(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x003C)
#define		REG_SDC_MINTSTS(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0040)
#define		REG_SDC_RINTSTS(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0044)
#define		REG_SDC_STATUS(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0048)
#define		REG_SDC_FIFOTH(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x004C)
#define		REG_SDC_CDETECT(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0050)
#define		REG_SDC_WRTPRT(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0054)
#define		REG_SDC_GPIO(n)			(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0058)
#define		REG_SDC_TCBCNT(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x005C)
#define		REG_SDC_TBBCNT(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0060)
#define		REG_SDC_DEBNCE(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0064)
#define		REG_SDC_USRID(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0068)
#define		REG_SDC_VERID(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x006C)
#define		REG_SDC_HCON(n)			(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0070)
#define		REG_SDC_UHS_REG(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0074)
#define		REG_SDC_RST_N(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0078)
#define		REG_SDC_BMOD(n)			(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0080)
#define		REG_SDC_PLDMND(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0084)
#define		REG_SDC_DBADDR(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0088)
#define		REG_SDC_IDSTS(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x008C)
#define		REG_SDC_IDINTEN(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0090)
#define		REG_SDC_DSCADDR(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0094)
#define		REG_SDC_BUFADDR(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0098)
#define		REG_SDC_CARDTHRCTL(n)	(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0100)
#define		REG_SDC_BACK_END_POWER(n)		(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0104)
#define		REG_SDC_FIFO(n)			(SDC0_REG_BASE + ((n) * SDC_INTERVAL) + 0x0200)
*/

#endif //_FH81_CHIP_REG_H_
