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

#ifndef __ASM_ARCH_FH81_CLOCK_H
#define __ASM_ARCH_FH81_CLOCK_H

struct clk;

extern int clk_register(struct clk *clk);
extern void clk_unregister(struct clk *clk);

void clk_set_clk_sel(unsigned int reg);
unsigned int clk_get_clk_sel(void);

#endif
