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

#ifndef __ARCH_ARM_MACH_FH81_COMMON_H
#define __ARCH_ARM_MACH_FH81_COMMON_H

#include <linux/compiler.h>
#include <linux/types.h>
#include <asm/mach/time.h>

extern struct sys_timer fh_timer;

extern void fh_intc_init(void);
void fh_irq_suspend(void);
void fh_irq_resume(void);

#endif /* __ARCH_ARM_MACH_FH81_COMMON_H */
