/*
 * FH81 timer subsystem
 *
 * Copyright (C) 2014 Fullhan Microelectronics Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/irqreturn.h>

#include <asm/system.h>
#include <asm/mach-types.h>
#include <asm/sched_clock.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>

#include <mach/hardware.h>
#include <mach/cputype.h>
#include <mach/time.h>
#include <mach/chip.h>
#include <mach/chip_reg.h>
#include <mach/fh_predefined.h>
#include <mach/irqs.h>
#include <mach/pmu.h>

#include "clock.h"

//#define USE_PTS_AS_CLOCKSOURCE

#define CHECK_TIMER_READING

static struct clock_event_device clockevent_fh81;
static struct clocksource clocksource_fh81;
#ifndef USE_PTS_AS_CLOCKSOURCE
static unsigned int prev_cycle = 0;
#endif

struct clk *timer0_clk, *timer1_clk, *pts_clk;

/*
 * clockevent
 */

static int fh_set_next_event(unsigned long cycles,
			     struct clock_event_device *evt)
{
	unsigned int curr_val;

	SET_REG_M(VTIMER(REG_TIMER1_CTRL_REG), 0x00, 0x0);
	SET_REG(VTIMER(REG_TIMER1_LOADCNT), cycles);
	SET_REG_M(VTIMER(REG_TIMER1_CTRL_REG), 0x01, 0x1);

	curr_val = GET_REG(VTIMER(REG_TIMER1_CUR_VAL)) ;
	if (curr_val >  0x80000000) { ///0xffff0000)
		panic("timer curr %u, want cycles %lu\n", curr_val, cycles);

		SET_REG_M(VTIMER(REG_TIMER1_CTRL_REG), 0x01, 0x1);
		SET_REG(VTIMER(REG_TIMER1_LOADCNT), cycles);

		//pmu reset
		fh_pmu_set_reg(REG_PMU_SWRST_MAIN_CTRL, 0xfff7ffff);
		while (fh_pmu_get_reg(REG_PMU_SWRST_MAIN_CTRL) != 0xffffffff) {

		}
	}
//#endif

	return 0;
}

static void fh_set_mode(enum clock_event_mode mode,
			struct clock_event_device *evt)
{
	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		SET_REG(VTIMER(REG_TIMER1_CTRL_REG), 0x3);
		SET_REG(VTIMER(REG_TIMER1_LOADCNT), TIMER1_CLK / HZ);

		//pmu reset
		fh_pmu_set_reg(REG_PMU_SWRST_MAIN_CTRL, 0xfff7ffff);
		while (fh_pmu_get_reg(REG_PMU_SWRST_MAIN_CTRL) != 0xffffffff) {

		}

		break;
	case CLOCK_EVT_MODE_ONESHOT:
		break;
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
		SET_REG(VTIMER(REG_TIMER1_CTRL_REG), 0x0);
		break;
	case CLOCK_EVT_MODE_RESUME:
		SET_REG(VTIMER(REG_TIMER1_CTRL_REG), 0x3);
		break;
	}
}


static irqreturn_t fh_clock_timer_interrupt_handle(int irq, void *dev_id)
{
	GET_REG(VTIMER(REG_TIMER1_EOI_REG));
	clockevent_fh81.event_handler(&clockevent_fh81);
	return IRQ_HANDLED;
}

static struct irqaction fh_eventtimer_irq = {
	.name           = "System Timer Tick",
	.flags          =  IRQF_SHARED | IRQF_DISABLED | IRQF_TIMER,
	.handler        = fh_clock_timer_interrupt_handle,
	.dev_id			= &clockevent_fh81,
};


static void fh_timer_resources(void)
{

}
static DEFINE_CLOCK_DATA(cd);

static void notrace fh_update_sched_clock(void)
{
	const cycle_t cyc = clocksource_fh81.read(&clocksource_fh81);
	update_sched_clock(&cd, cyc, (u32)~0);
}

unsigned long long notrace sched_clock(void)
{
	const cycle_t cyc = clocksource_fh81.read(&clocksource_fh81);

	return cyc_to_sched_clock(&cd, cyc, (u32)~0);
}

static void fh_clocksource_init(void)
{
#ifdef USE_PTS_AS_CLOCKSOURCE
	unsigned long clock_tick_rate = pts_clk->frequency;
#else
	unsigned long clock_tick_rate = timer0_clk->frequency;
	prev_cycle = 0;
#endif

	if (clocksource_register_hz(&clocksource_fh81, clock_tick_rate))
		panic("register clocksouce :%s error\n", clocksource_fh81.name);

	printk("timer mult: 0x%x, timer shift: 0x%x\n", clocksource_fh81.mult,
	       clocksource_fh81.shift);
	/* force check the mult/shift of clocksource */
	init_fixed_sched_clock(&cd, fh_update_sched_clock, 32, clock_tick_rate,
			       clocksource_fh81.mult, clocksource_fh81.shift);
}

static cycle_t fh_clocksource_read(struct clocksource *cs)
{
#ifdef USE_PTS_AS_CLOCKSOURCE
	return GET_REG(VPAE(REG_PAE_PTS_REG));
#else
	unsigned int cycle;
	cycle = ~GET_REG(VTIMER(REG_TIMER0_CUR_VAL));
	if (unlikely(prev_cycle > cycle))
		cycle = ~GET_REG(VTIMER(REG_TIMER0_CUR_VAL));
	prev_cycle = cycle;
	return cycle;
#endif
}

static void fh_clockevent_init(void)
{
	setup_irq(TMR0_IRQ, &fh_eventtimer_irq);
	clockevent_fh81.mult = div_sc(timer1_clk->frequency, NSEC_PER_SEC, clockevent_fh81.shift);
	clockevent_fh81.max_delta_ns = clockevent_delta2ns(0xffffffff,
				       &clockevent_fh81);

	clockevent_fh81.min_delta_ns = clockevent_delta2ns(0xf, &clockevent_fh81);

	clockevent_fh81.cpumask = cpumask_of(0);
	clockevents_register_device(&clockevent_fh81);
}


static void __init fh_timer_init(void)
{

	timer0_clk = clk_get(NULL, "tmr0_clk");
	timer1_clk = clk_get(NULL, "tmr1_clk");
	pts_clk = clk_get(NULL, "pts_clk");

#ifdef USE_PTS_AS_CLOCKSOURCE
	clk_set_rate(pts_clk, PAE_PTS_CLK);
	clk_enable(pts_clk);
#else
	clk_set_rate(timer0_clk, TIMER0_CLK);
	clk_enable(timer0_clk);
#endif

	clk_set_rate(timer1_clk, TIMER1_CLK);
	clk_enable(timer1_clk);

	if (IS_ERR(timer0_clk) || IS_ERR(timer1_clk) || IS_ERR(pts_clk))
		pr_err("fh_timer: clock is not defined\n");


	fh_timer_resources();
	fh_clocksource_init();
	fh_clockevent_init();
}



static struct clocksource clocksource_fh81 = {
	.name		= "fh_clocksource",
	.rating		= 300,
	.read		= fh_clocksource_read,
	.mask		= CLOCKSOURCE_MASK(32),
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};


static struct clock_event_device clockevent_fh81 = {
	.name			= "fh_clockevent",
	.features       = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.shift			= 32,
	.set_next_event	= fh_set_next_event,
	.set_mode		= fh_set_mode,
};

struct sys_timer fh_timer = {
	.init   = fh_timer_init,
};
