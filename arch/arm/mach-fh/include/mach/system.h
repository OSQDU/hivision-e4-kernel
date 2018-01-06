/*
 * DaVinci system defines
 *
 * Author: Kevin Hilman, MontaVista Software, Inc. <source@mvista.com>
 *
 * 2007 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */
#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

#include <mach/common.h>
#include <mach/io.h>
#include <mach/fh_predefined.h>
#include <mach/pmu.h>

static inline void arch_idle(void)
{
	cpu_do_idle();
}

static inline void arch_reset(char mode, const char *cmd)
{
	fh_pmu_set_reg(REG_PMU_SWRST_MAIN_CTRL, 0x7fffffff);
}

#endif /* __ASM_ARCH_SYSTEM_H */
