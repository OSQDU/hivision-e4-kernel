/*
 * Fullhan FH81 clock definitions
 *
 * Copyright (C) 2014 Fullhan Microelectronics Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ARCH_FH81_CLOCK_H
#define __ARCH_FH81_CLOCK_H


#ifndef __ASSEMBLER__

#include <linux/list.h>
#include <linux/clkdev.h>

#define OSC_FREQUENCY		(24000000)

#define CLOCK_FIXED         	(1<<0)
#define CLOCK_NOGATE        	(1<<1)
#define CLOCK_NODIV         	(1<<2)
#define CLOCK_NORESET       	(1<<3)
#define CLOCK_MULTI_PARENT  	(1<<4)
#define CLOCK_PLL		(1<<5)

#define CLOCK_MAX_PARENT    4

struct clk {
	struct list_head    list;
	//struct clk_ops      *ops;
	const char          *name;
	unsigned long       frequency;
	unsigned int        flag;
	int                 select;
	struct clk         *parent[CLOCK_MAX_PARENT];
	int                 prediv;
	int                 divide;
	unsigned int        div_reg_offset;
	unsigned int        div_reg_mask;
	unsigned int        en_reg_offset;
	unsigned int        en_reg_mask;
	unsigned int        rst_reg_offset;
	unsigned int        rst_reg_mask;
	unsigned int        sel_reg_offset;
	unsigned int        sel_reg_mask;
	unsigned int        def_rate;
};

#define CLK(dev, con, ck) 	\
		{					\
			.dev_id = dev,	\
			.con_id = con,	\
			.clk = ck,		\
		}					\

int fh_clk_init(void);
int fh_clk_procfs_init(void);

extern struct clk_lookup fh_clks[];

#endif
#endif
