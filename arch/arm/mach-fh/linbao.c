/*
 * Fullhan FH81 board support
 *
 * Copyright (C) 2014 Fullhan Microelectronics Co., Ltd.
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
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/serial_8250.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/gpio.h>
#include <linux/spi/spi.h>

#include <asm/mach/map.h>

#include <mach/chip.h>
#include <mach/cputype.h>
#include <mach/irqs.h>
#include <mach/time.h>
#include <mach/serial.h>
#include <mach/pmu.h>
#include "clock.h"

/*
 * external oscillator
 * fixed to 24M
 */
static struct clk osc_clk = {
	.name               = "osc_clk",
	.frequency          = OSC_FREQUENCY,
	.flag               = CLOCK_FIXED,
};

/*
 * phase-locked-loop device,
 * generates a higher frequency clock
 * from the external oscillator reference
 */
static struct clk pll0_clk = {
	.name               = "pll0_clk",
	.flag               = CLOCK_PLL,
	.parent             = {&osc_clk},
	.div_reg_offset     = REG_PMU_PLL0,
};

static struct clk pll1_clk = {
	.name               = "pll1_clk",
	.flag               = CLOCK_PLL,
	.parent             = {&osc_clk},
	.div_reg_offset     = REG_PMU_PLL1,
};

static struct clk pll2_clk = {
	.name               = "pll2_clk",
	.flag               = CLOCK_PLL,
	.parent             = {&osc_clk},
	.div_reg_offset     = REG_PMU_PLL2,
};

/*
 * CPU
 */
static struct clk arm_clk = {
	.name               = "arm_clk",
	.flag               = CLOCK_NOGATE,
	.parent             = {&pll0_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV0,
	.div_reg_mask       = 0xf,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x1,
};

static struct clk arc_clk = {
	.name               = "arc_clk",
	.flag               = CLOCK_NOGATE | CLOCK_NODIV,
	.parent             = {&arm_clk},
	.prediv             = 2,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x400000,
};

/*
 * BUS
 */
static struct clk axi_clk = {
	.name               = "axi_clk",
	.flag               = CLOCK_NOGATE | CLOCK_NODIV | CLOCK_NORESET,
	.parent             = {&arm_clk},
	.prediv             = 2,
};

static struct clk ahb_clk = {
	.name               = "ahb_clk",
	.flag               = CLOCK_NOGATE | CLOCK_NORESET,
	.parent             = {&pll0_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV0,
	.div_reg_mask       = 0xf0000,
};

static struct clk eahb_clk = {
	.name               = "eahb_clk",
	.flag               = CLOCK_NORESET,
	.parent             = {&pll0_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV0,
	.div_reg_mask       = 0xf0000000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x8,
};

static struct clk apb_clk = {
	.name               = "apb_clk",
	.flag               = CLOCK_NOGATE | CLOCK_NORESET | CLOCK_NODIV,
	.parent             = {&ahb_clk},
	.prediv             = 2,
};

static struct clk eapb_clk = {
	.name               = "eapb_clk",
	.flag               = CLOCK_NOGATE | CLOCK_NORESET | CLOCK_NODIV,
	.parent             = {&eahb_clk},
	.prediv             = 2,
};

/*
 * ip
 */


static struct clk ddr_clk = {
	.name               = "ddr_clk",
	.parent             = {&pll2_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV1,
	.div_reg_mask       = 0xf,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x40,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x8,
};

static struct clk isp_clk = {
	.name               = "isp_clk",
	.flag               = CLOCK_NORESET,
	.parent             = {&pll0_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV0,
	.div_reg_mask       = 0x700,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x1,
};

static struct clk pae_clk = {
	.name               = "pae_clk",
	.flag               = CLOCK_NORESET,
	.parent             = {&pll0_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV0,
	.div_reg_mask       = 0x7000000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x10,
};

static struct clk vou_clk = {
	.name               = "vou_clk",
	.parent             = {&pll2_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV1,
	.div_reg_mask       = 0x3f00,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x20,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x2,
};

static struct clk fd_clk = {
	.name               = "fd_clk",
	.flag               = CLOCK_FIXED|CLOCK_NORESET,
	.en_reg_offset      = REG_PMU_CLK_GATE_S,
	.en_reg_mask        = 0x1,
};

static struct clk bgm_clk = {
	.name               = "bgm_clk",
	.flag               = CLOCK_FIXED|CLOCK_NORESET,
	.en_reg_offset      = REG_PMU_CLK_GATE_S,
	.en_reg_mask        = 0x2,
};

static struct clk cis_clk = {
	.name               = "cis_clk",
	.flag               = CLOCK_NORESET | CLOCK_MULTI_PARENT,
	.parent             = {&pll2_clk, &pll0_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV1,
	.div_reg_mask       = 0xff0000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x800000,
	.sel_reg_offset     = REG_PMU_CLK_SEL,
	.sel_reg_mask       = 0x1,
};

static struct clk cis_revert_clk = {
	.name               = "cis_revert_clk",
	.flag               = CLOCK_NOGATE | CLOCK_NORESET | CLOCK_NODIV,
	.parent             = {&cis_clk},
	.prediv             = 1,
};

static struct clk mipi_dphy_clk = {
	.name               = "mipi_dphy_clk",
	.flag               = CLOCK_NORESET,
	.parent             = {&pll0_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV2,
	.div_reg_mask       = 0x1f0000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x100000,
};

static struct clk mipi_pix_clk = {
	.name               = "mipi_pix_clk",
	.flag               = CLOCK_NORESET | CLOCK_NOGATE,
	.parent             = {&pll0_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV2,
	.div_reg_mask       = 0xf000000,
};

static struct clk pix_clk = {
	.name               = "pix_clk",
	.flag               = CLOCK_NORESET | CLOCK_NOGATE | CLOCK_NODIV | CLOCK_MULTI_PARENT,
	.parent             = {&cis_clk, &cis_revert_clk, &mipi_pix_clk},
	.prediv             = 1,
	.sel_reg_offset     = REG_PMU_CLK_SEL,
	.sel_reg_mask       = 0x30,
};

static struct clk pts_clk = {
	.name               = "pts_clk",
	.parent             = {&pll1_clk},
	.prediv             = 10,
	.div_reg_offset     = REG_PMU_CLK_DIV2,
	.div_reg_mask       = 0x1ff,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x80000,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x20000,
};

static struct clk spi0_clk = {
	.name               = "spi0_clk",
	.parent             = {&pll1_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV3,
	.div_reg_mask       = 0xff,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x80,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x100,
};

static struct clk spi1_clk = {
	.name               = "spi1_clk",
	.parent             = {&pll1_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV3,
	.div_reg_mask       = 0xff0000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x100,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x200,
};

static struct clk spi2_clk = {
	.name               = "spi2_clk",
	.parent             = {&pll1_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV3,
	.div_reg_mask       = 0xf000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x2,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x100000,
};

static struct clk spi3_clk = {
	.name               = "spi3_clk",
	.parent             = {&pll1_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV3,
	.div_reg_mask       = 0xf0000000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x4,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x200000,
};

static struct clk sdc0_clk = {
	.name               = "sdc0_clk",
	.parent             = {&pll1_clk},
	.prediv             = 2,
	.div_reg_offset     = REG_PMU_CLK_DIV3,
	.div_reg_mask       = 0xf00,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x200,
	.rst_reg_offset     = REG_PMU_SWRST_AHB_CTRL,
	.rst_reg_mask       = 0x4,
};

static struct clk sdc1_clk = {
	.name               = "sdc1_clk",
	.parent             = {&pll1_clk},
	.prediv             = 2,
	.div_reg_offset     = REG_PMU_CLK_DIV3,
	.div_reg_mask       = 0xf000000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x400,
	.rst_reg_offset     = REG_PMU_SWRST_AHB_CTRL,
	.rst_reg_mask       = 0x2,
};

static struct clk uart0_clk = {
	.name               = "uart0_clk",
	.parent             = {&pll1_clk},
	.prediv             = 10,
	.div_reg_offset     = REG_PMU_CLK_DIV4,
	.div_reg_mask       = 0x1f,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x2000,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x4000,
};

static struct clk uart1_clk = {
	.name               = "uart1_clk",
	.parent             = {&pll1_clk},
	.prediv             = 10,
	.div_reg_offset     = REG_PMU_CLK_DIV4,
	.div_reg_mask       = 0x1f00,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x4000,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x8000,
};

static struct clk uart2_clk = {
	.name               = "uart2_clk",
	.parent             = {&pll1_clk},
	.prediv             = 10,
	.div_reg_offset     = REG_PMU_CLK_DIV7,
	.div_reg_mask       = 0x1f,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x8000,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x800000,
};

static struct clk i2c0_clk = {
	.name               = "i2c0_clk",
	.parent             = {&pll1_clk},
	.prediv             = 20,
	.div_reg_offset     = REG_PMU_CLK_DIV4,
	.div_reg_mask       = 0x3f0000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x1000,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x400,
};

static struct clk i2c1_clk = {
	.name               = "i2c1_clk",
	.parent             = {&pll1_clk},
	.prediv             = 20,
	.div_reg_offset     = REG_PMU_CLK_DIV4,
	.div_reg_mask       = 0x3f000000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x800000,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x800,
};

static struct clk pwm_clk = {
	.name               = "pwm_clk",
	.parent             = {&pll1_clk},
	.prediv             = 20,
	.div_reg_offset     = REG_PMU_CLK_DIV5,
	.div_reg_mask       = 0x3f,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x10000,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x80,
};

static struct clk wdt_clk = {
	.name               = "wdt_clk",
	.flag               = CLOCK_NOGATE,
	.parent             = {&apb_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV5,
	.div_reg_mask       = 0xff00,
	.rst_reg_offset     = REG_PMU_SWRST_APB_CTRL,
	.rst_reg_mask       = 0x100000,
};


static struct clk tmr0_clk = {
	.name               = "tmr0_clk",
	.parent             = {&apb_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV5,
	.div_reg_mask       = 0xff0000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x20000,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x40000,
};

static struct clk tmr1_clk = {
	.name               = "tmr1_clk",
	.parent             = {&apb_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV5,
	.div_reg_mask       = 0xff000000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x40000,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x80000,
};

static struct clk ac_clk = {
	.name               = "ac_clk",
	.parent             = {&pll1_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV6,
	.div_reg_mask       = 0x3f,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x800,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x1000,
};

static struct clk i2s_clk = {
	.name               = "i2s_clk",
	.parent             = {&ac_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV6,
	.div_reg_mask       = 0x3f00,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x1000000,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x2000,
};

static struct clk sadc_clk = {
	.name               = "sadc_clk",
	.parent             = {&pll1_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV6,
	.div_reg_mask       = 0x7f0000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x4000000,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x10000,
};

static struct clk eth_clk = {
	.name               = "eth_clk",
	.parent             = {&pll1_clk},
	.prediv             = 2,
	.div_reg_offset     = REG_PMU_CLK_DIV6,
	.div_reg_mask       = 0xf000000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x72000000,
	.rst_reg_offset     = REG_PMU_SWRST_AHB_CTRL,
	.rst_reg_mask       = 0x20000,
};

static struct clk efuse_clk = {
	.name               = "efuse_clk",
	.parent             = {&pll1_clk},
	.prediv             = 1,
	.div_reg_offset     = REG_PMU_CLK_DIV7,
	.div_reg_mask       = 0x3f0000,
	.en_reg_offset      = REG_PMU_CLK_GATE,
	.en_reg_mask        = 0x200000,
	.rst_reg_offset     = REG_PMU_SWRST_MAIN_CTRL,
	.rst_reg_mask       = 0x1000000,
};

struct clk_lookup fh_clks[] = {
	CLK(NULL, "osc_clk", &osc_clk),
	CLK(NULL, "pll0_clk", &pll0_clk),
	CLK(NULL, "pll1_clk", &pll1_clk),
	CLK(NULL, "pll2_clk", &pll2_clk),
	CLK(NULL, "arm_clk", &arm_clk),
	CLK(NULL, "arc_clk", &arc_clk),
	CLK(NULL, "axi_clk", &axi_clk),
	CLK(NULL, "ahb_clk", &ahb_clk),
	CLK(NULL, "eahb_clk", &eahb_clk),

	CLK(NULL, "apb_clk", &apb_clk),
	CLK(NULL, "eapb_clk", &eapb_clk),
	CLK(NULL, "ddr_clk", &ddr_clk),
	CLK(NULL, "isp_clk", &isp_clk),
	CLK(NULL, "pae_clk", &pae_clk),
	CLK(NULL, "bgm_clk", &bgm_clk),
	CLK(NULL, "fd_clk", &fd_clk),

	CLK(NULL, "vou_clk", &vou_clk),
	CLK(NULL, "cis_clk", &cis_clk),
	CLK(NULL, "cis_revert_clk", &cis_revert_clk),
	CLK(NULL, "mipi_dphy_clk", &mipi_dphy_clk),
	CLK(NULL, "mipi_pix_clk", &mipi_pix_clk),
	CLK(NULL, "pix_clk", &pix_clk),
	CLK(NULL, "pts_clk", &pts_clk),

	CLK(NULL, "spi0_clk", &spi0_clk),
	CLK(NULL, "spi1_clk", &spi1_clk),
	CLK(NULL, "spi2_clk", &spi2_clk),
	CLK(NULL, "spi3_clk", &spi3_clk),
	CLK(NULL, "sdc0_clk", &sdc0_clk),
	CLK(NULL, "sdc1_clk", &sdc1_clk),
	CLK(NULL, "uart0_clk", &uart0_clk),
	CLK(NULL, "uart1_clk", &uart1_clk),
	CLK(NULL, "uart2_clk", &uart2_clk),
	CLK(NULL, "i2c0_clk", &i2c0_clk),
	CLK(NULL, "i2c1_clk", &i2c1_clk),
	CLK(NULL, "pwm_clk", &pwm_clk),
	CLK(NULL, "wdt_clk", &wdt_clk),
	CLK(NULL, "tmr0_clk", &tmr0_clk),
	CLK(NULL, "tmr1_clk", &tmr1_clk),
	CLK(NULL, "ac_clk", &ac_clk),
	CLK(NULL, "i2s_clk", &i2s_clk),
	CLK(NULL, "sadc_clk", &sadc_clk),
	CLK(NULL, "eth_clk", &eth_clk),
	CLK(NULL, "efuse_clk", &efuse_clk),

	CLK(NULL, NULL, NULL),
};

EXPORT_SYMBOL(fh_clks);
