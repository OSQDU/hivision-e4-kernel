/*
 * Local header file for DaVinci time code.
 *
 * Author: Kevin Hilman, MontaVista Software, Inc. <source@mvista.com>
 *
 * 2007 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */
#ifndef __ARCH_ARM_MACH_FH81_TIME_H
#define __ARCH_ARM_MACH_FH81_TIME_H

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

#ifdef FPGA_OLD
#define TIMER0_CLK 			(27000000)
#define TIMER1_CLK 			(27000000)
#else
#define TIMER0_CLK 			(1000000)
#define TIMER1_CLK 			(1000000)
#endif
#define PAE_PTS_CLK			(1000000)

#endif /* __ARCH_ARM_MACH_FH81_TIME_H */
