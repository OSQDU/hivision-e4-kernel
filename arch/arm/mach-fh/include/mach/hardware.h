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

#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

/*
 * Before you add anything to ths file:
 *
 * This header is for defines common to ALL DaVinci family chips.
 * Anything that is chip specific should go in <chipname>.h,
 * and the chip/board init code should then explicitly include
 * <chipname>.h
 */
//#define DAVINCI_SYSTEM_MODULE_BASE        0x01C40000

/*
 * I/O mapping
 */
//#define IO_PHYS				0x01c00000UL
//#define IO_OFFSET			0xfd000000 /* Virtual IO = 0xfec00000 */
//#define IO_SIZE				0x00400000
//#define IO_VIRT				(IO_PHYS + IO_OFFSET)
//#define io_v2p(va)			((va) - IO_OFFSET)
//#define __IO_ADDRESS(x)			((x) + IO_OFFSET)

#ifdef __ASSEMBLER__
#define IOMEM(x)                	x
#else
#define IOMEM(x)                	((void __force __iomem *)(x))
#endif

#endif /* __ASM_ARCH_HARDWARE_H */
