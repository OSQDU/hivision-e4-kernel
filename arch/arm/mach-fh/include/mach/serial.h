/*
 * DaVinci serial device definitions
 *
 * Author: Kevin Hilman, MontaVista Software, Inc. <source@mvista.com>
 *
 * 2007 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */
#ifndef __ASM_ARCH_SERIAL_H
#define __ASM_ARCH_SERIAL_H

#define     REG_UART_RBR         	(0x0000)
#define     REG_UART_THR         	(0x0000)
#define     REG_UART_DLL         	(0x0000)
#define     REG_UART_DLH         	(0x0004)
#define     REG_UART_IER         	(0x0004)
#define     REG_UART_IIR         	(0x0008)
#define     REG_UART_FCR         	(0x0008)
#define     REG_UART_LCR         	(0x000c)
#define     REG_UART_MCR         	(0x0010)
#define     REG_UART_LSR         	(0x0014)
#define     REG_UART_MSR         	(0x0018)
#define     REG_UART_SCR         	(0x001c)
#define     REG_UART_FAR         	(0x0070)
#define     REG_UART_TFR         	(0x0074)
#define     REG_UART_RFW         	(0x0078)
#define     REG_UART_USR         	(0x007c)
#define     REG_UART_TFL         	(0x0080)
#define     REG_UART_RFL         	(0x0084)
#define     REG_UART_SRR         	(0x0088)
#define     REG_UART_SFE         	(0x0098)
#define     REG_UART_SRT         	(0x009c)
#define     REG_UART_STET        	(0x00a0)
#define     REG_UART_HTX         	(0x00a4)
#define     REG_UART_DMASA			(0x00a8)
#define     REG_UART_CPR         	(0x00f4)
#define     REG_UART_UCV         	(0x00f8)
#define     REG_UART_CTR         	(0x00fc)

#endif /* __ASM_ARCH_SERIAL_H */
