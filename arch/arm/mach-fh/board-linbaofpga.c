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
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/io.h>
#include <linux/mmc/dw_mmc.h>
#include <linux/clk.h>
#include <linux/i2c/at24.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/phy.h>
#include <linux/dma-mapping.h>
#include <linux/spi/eeprom.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/pmu.h>

#include <mach/common.h>
#include <mach/serial.h>
#include <mach/mmc.h>
#include <mach/chip.h>
#include <mach/iomux.h>
#include <mach/irqs.h>
#include <mach/pmu.h>
#include <mach/fh_dmac.h>
#include <mach/fh_gmac.h>
#include <mach/gpio.h>
#include <mach/spi.h>
#include "clock.h"
#include <mach/rtc.h>
static struct map_desc linbao_io_desc[] = {
	{
		.virtual    = VA_RAM_REG_BASE,
		.pfn        = __phys_to_pfn(RAM_BASE),
		.length     = SZ_16K,
		.type       = MT_MEMORY,
	},
	{
		.virtual    = VA_DDRC_REG_BASE,
		.pfn        = __phys_to_pfn(DDRC_REG_BASE),
		.length     = SZ_16K,
		.type       = MT_DEVICE,
	},
	{
		.virtual	= VA_INTC_REG_BASE,
		.pfn		= __phys_to_pfn(INTC_REG_BASE),
		.length		= SZ_16K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= VA_TIMER_REG_BASE,
		.pfn		= __phys_to_pfn(TIMER_REG_BASE),
		.length		= SZ_16K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= VA_PMU_REG_BASE,
		.pfn		= __phys_to_pfn(PMU_REG_BASE),
		.length		= SZ_16K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= VA_UART0_REG_BASE,
		.pfn		= __phys_to_pfn(UART0_REG_BASE),
		.length		= SZ_16K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= VA_UART1_REG_BASE,
		.pfn		= __phys_to_pfn(UART1_REG_BASE),
		.length		= SZ_16K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= VA_UART2_REG_BASE,
		.pfn		= __phys_to_pfn(UART2_REG_BASE),
		.length		= SZ_16K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= VA_PAE_REG_BASE,
		.pfn		= __phys_to_pfn(PAE_REG_BASE),
		.length		= SZ_16K,
		.type		= MT_DEVICE,
	},
};


static struct resource fh_gpio0_resources[] = {
	{
		.start		= GPIO0_REG_BASE,
		.end		= GPIO0_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},

	{
		.start		= GPIO0_IRQ,
		.end		= GPIO0_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource fh_gpio1_resources[] = {
	{
		.start		= GPIO1_REG_BASE,
		.end		= GPIO1_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},

	{
		.start		= GPIO1_IRQ,
		.end		= GPIO1_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};


static struct resource fh_uart0_resources[] = {
	{
		.start		= (UART0_REG_BASE),
		.end		= (UART0_REG_BASE) + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},

	{
		.start		= UART0_IRQ,
		.end		= UART0_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource fh_uart1_resources[] = {
	{
		.start		= (UART1_REG_BASE),
		.end		= (UART1_REG_BASE) + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},

	{
		.start		= UART1_IRQ,
		.end		= UART1_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource fh_uart2_resources[] = {
	{
		.start		= (UART2_REG_BASE),
		.end		= (UART2_REG_BASE) + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},

	{
		.start		= UART2_IRQ,
		.end		= UART2_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource fh_dma_resources[] = {
	{
		.start		= (DMAC_REG_BASE),
		.end		= (DMAC_REG_BASE) + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},

	{
		.start		= DMAC_IRQ,
		.end		= DMAC_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};
static struct resource fh_i2c_resources_0[] = {
	{
		.start		= I2C0_REG_BASE,
		.end		= I2C0_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},

	{
		.start		= I2C0_IRQ,
		.end		= I2C0_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};
static struct resource fh_i2c_resources_1[] = {
	{
		.start		= I2C1_REG_BASE,
		.end		= I2C1_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},

	{
		.start		= I2C1_IRQ,
		.end		= I2C1_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource fh_sdc0_resources[] = {
	{
		.start		= SDC0_REG_BASE,
		.end		= SDC0_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= SDC0_IRQ,
		.end		= SDC0_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};
static struct resource fh_sdc1_resources[] = {
	{
		.start		= SDC1_REG_BASE,
		.end		= SDC1_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= SDC1_IRQ,
		.end		= SDC1_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};
static struct resource fh_wdt_resources[] = {
	{
		.start		= WDT_REG_BASE,
		.end		= WDT_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= WDT_IRQ,
		.end		= WDT_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};
static struct resource fh_i2s_resources[] = {
	{
		.start		= I2S_REG_BASE,
		.end		= I2S_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= I2S0_IRQ,
		.end		= I2S0_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource fh_spi0_resources[] = {
	{
		.start		= SPI0_REG_BASE,
		.end		= SPI0_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
		.name = "fh spi0 mem",
	},
	{
		.start		= SPI0_IRQ,
		.end		= SPI0_IRQ,
		.flags		= IORESOURCE_IRQ,
		.name = "fh spi0 irq",
	},
};

static struct resource fh_spi1_resources[] = {
	{
		.start		= SPI1_REG_BASE,
		.end		= SPI1_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
		.name = "fh spi1 mem",
	},
	{
		.start		= SPI1_IRQ,
		.end		= SPI1_IRQ,
		.flags		= IORESOURCE_IRQ,
		.name = "fh spi1 irq",
	},
};


static struct resource fh_spi2_resources[] = {
	{
		.start		= SPI2_REG_BASE,
		.end		= SPI2_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
		.name = "fh spi2 mem",
	},
	{
		.start		= SPI2_IRQ,
		.end		= SPI2_IRQ,
		.flags		= IORESOURCE_IRQ,
		.name = "fh spi2 irq",
	},
};



static struct resource fh_spi3_resources[] = {
	{
		.start		= SPI3_REG_BASE,
		.end		= SPI3_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
		.name = "fh spi3 mem",
	},
	{
		.start		= SPI3_IRQ,
		.end		= SPI3_IRQ,
		.flags		= IORESOURCE_IRQ,
		.name = "fh spi3 irq",
	},
};


static struct resource fh_gmac_resources[] = {
	{
		.start		= GMAC_REG_BASE,
		.end		= GMAC_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},

	{
		.start		= GMAC_IRQ,
		.end		= GMAC_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource fh_pwm_resources[] = {
	{
		.start		= PWM_REG_BASE,
		.end		= PWM_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},
};

static struct resource fh_sadc_resources[] = {
	{
		.start		= SADC_REG_BASE,
		.end		= SADC_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
		.name = "fh sadc mem",
	},
	{
		.start		= SADC_IRQ,
		.end		= SADC_IRQ,
		.flags		= IORESOURCE_IRQ,
		.name = "fh sadc irq",
	},
};

static struct resource fh_aes_resources[] = {
	{
		.start		= AES_REG_BASE,
		.end		= AES_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
		.name = "fh aes mem",
	},
	{
		.start		= AES_IRQ,
		.end		= AES_IRQ,
		.flags		= IORESOURCE_IRQ,
		.name = "fh aes irq",
	},
};

static struct resource fh_acw_resources[] = {
	{
		.start      = ACW_REG_BASE,
		.end        = ACW_REG_BASE + SZ_16K - 1,
		.flags      = IORESOURCE_MEM,
	},
	{
		.start      = ACW_IRQ,
		.end        = ACW_IRQ,
		.flags      = IORESOURCE_IRQ,
	},
};



static struct resource fh_rtc_resources[] = {
	{
		.start      = RTC_REG_BASE,
		.end        = RTC_REG_BASE + SZ_16K - 1,
		.flags      = IORESOURCE_MEM,
	},
	{
		.start      = RTC_IRQ,
		.end        = RTC_IRQ,
		.flags      = IORESOURCE_IRQ,
	},
};


static struct resource fh_efuse_resources[] = {
	{
		.start		= EFUSE_REG_BASE,
		.end		= EFUSE_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
		.name = "fh efuse mem",
	},

};



static void fh_gmac_early_init(struct fh_gmac_platform_data *plat_data)
{
	if (plat_data->interface == PHY_INTERFACE_MODE_RMII)
		fh_pmu_set_reg(REG_PMU_MAC_REF_CLK_CFG, 0x3130);
	else if (plat_data->interface == PHY_INTERFACE_MODE_MII)
		fh_pmu_set_reg(REG_PMU_MAC_REF_CLK_CFG, 0x1100);
}

static void fh_gmac_plat_init(struct fh_gmac_platform_data *plat_data)
{
	u32 reg;

	if (plat_data->interface == PHY_INTERFACE_MODE_RMII) {
		//mac speed: 1-100M
		//rmii mode: rmii
		//phy interface select: 1-rmii
		reg = fh_pmu_get_reg(REG_PMU_SYS_CTRL);
		reg |= 0x7000000;
		fh_pmu_set_reg(REG_PMU_SYS_CTRL, reg);

		//pmu reset
		fh_pmu_set_reg(REG_PMU_SWRST_AHB_CTRL, 0xfffdffff);
		while (fh_pmu_get_reg(REG_PMU_SWRST_AHB_CTRL) != 0xffffffff) {
		}
	} else if (plat_data->interface == PHY_INTERFACE_MODE_MII) {
		//mac speed: 1-100M
		//mii mode: mii
		//phy interface select: 1-mii
		reg = fh_pmu_get_reg(REG_PMU_SYS_CTRL);
		reg &= ~(0x7000000);
		reg |= 0x1000000;
		fh_pmu_set_reg(REG_PMU_SYS_CTRL, reg);

		//pmu reset
		fh_pmu_set_reg(REG_PMU_SWRST_AHB_CTRL, 0xfffdffff);
		while (fh_pmu_get_reg(REG_PMU_SWRST_AHB_CTRL) != 0xffffffff) {
		}
	}
}

static void fh_set_rmii_speed(int speed)
{
	u32 reg;

	if (speed == gmac_speed_10m) {
		reg = fh_pmu_get_reg(REG_PMU_SYS_CTRL);
		reg &= ~(0x1000000);
		fh_pmu_set_reg(REG_PMU_SYS_CTRL, reg);
	} else {
		reg = fh_pmu_get_reg(REG_PMU_SYS_CTRL);
		reg |= 0x1000000;
		fh_pmu_set_reg(REG_PMU_SYS_CTRL, reg);
	}
}

static struct fh_gmac_platform_data fh_gmac_data = {
	.early_init = fh_gmac_early_init,
	.plat_init = fh_gmac_plat_init,
	.set_rmii_speed = fh_set_rmii_speed,
	.phyid = -1,
};

static struct fh_gpio_chip fh_gpio0_chip = {
	.chip = {
		.owner = THIS_MODULE,
		.label = "FH_GPIO0",
		.base = 0,
		.ngpio = 32,
		.names = fh_gpio0_names,
	},
};

static struct fh_gpio_chip fh_gpio1_chip = {
	.chip = {
		.owner = THIS_MODULE,
		.label = "FH_GPIO1",
		.base = 32,
		.ngpio = 32,
		.names = fh_gpio1_names,
	},
};

static int fh_buswd(u32 slot_id)
{
	return 4;
}

static int sd_init(unsigned int slot_id, void *data, void *v)
{
	//pmu reset
	u32 reg;

	reg = slot_id ? 0xfffffffd : 0xfffffffb;
	fh_pmu_set_reg(REG_PMU_SWRST_AHB_CTRL, reg);
	while (fh_pmu_get_reg(REG_PMU_SWRST_AHB_CTRL) != 0xffffffff) {
	}
	return 0;
}

struct dw_mci_board fh_mci = {
	.init = sd_init,
	.num_slots = 1,
	.bus_hz = 50000000,
	.detect_delay_ms = 200,
	.get_bus_wd = fh_buswd,
	.caps = MMC_CAP_4_BIT_DATA
	| MMC_CAP_SD_HIGHSPEED
	| MMC_CAP_MMC_HIGHSPEED
	| MMC_CAP_NEEDS_POLL
	/*  | MMC_CAP_SDIO_IRQ  */,
};

struct dw_mci_board fh_mci_sd = {
	.init = sd_init,
	.num_slots = 1,
	.bus_hz = 50000000,
	.detect_delay_ms = 200,
	.get_bus_wd = fh_buswd,
	.caps = MMC_CAP_4_BIT_DATA
	| MMC_CAP_SD_HIGHSPEED
	| MMC_CAP_MMC_HIGHSPEED
	| MMC_CAP_NEEDS_POLL
	/*  | MMC_CAP_SDIO_IRQ  */,
};

static struct fh_dma_platform_data fh_dma_data = {
	.chan_priority  = CHAN_PRIORITY_DESCENDING,
	.nr_channels    = 8,
};

static struct at24_platform_data at24c02 = {
	.byte_len   = SZ_2K / 8,
	.page_size  = 8,
	.flags      = AT24_FLAG_TAKE8ADDR,
};

static struct platform_device fh_gmac_device = {
	.name           = "fh_gmac",
	.id             = 0,
	.num_resources  = ARRAY_SIZE(fh_gmac_resources),
	.resource       = fh_gmac_resources,
	.dev = {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &fh_gmac_data,
	},
};

static struct platform_device fh_i2s_misc_device = {
	.name           = "fh_i2s",
	.id             = 0,
	.num_resources  = ARRAY_SIZE(fh_i2s_resources),
	.resource       = fh_i2s_resources,
};

static struct platform_device fh_gpio0_device = {
	.name           = GPIO_NAME,
	.id             = 0,
	.num_resources  = ARRAY_SIZE(fh_gpio0_resources),
	.resource       = fh_gpio0_resources,
	.dev            = {
		.platform_data = &fh_gpio0_chip,
	},
};

static struct platform_device fh_gpio1_device = {
	.name           = GPIO_NAME,
	.id             = 1,
	.num_resources  = ARRAY_SIZE(fh_gpio1_resources),
	.resource       = fh_gpio1_resources,
	.dev            = {
		.platform_data = &fh_gpio1_chip,
	},
};

struct platform_device fh_sd0_device = {
	.name = "fh_mci",
	.id = 0,
	.num_resources = ARRAY_SIZE(fh_sdc0_resources),
	.resource = fh_sdc0_resources,
	.dev =
	{
		.platform_data = &fh_mci,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};

struct platform_device fh_sd1_device = {
	.name = "fh_mci",
	.id = 1,
	.num_resources = ARRAY_SIZE(fh_sdc1_resources),
	.resource = fh_sdc1_resources,
	.dev =
	{
		.platform_data = &fh_mci_sd,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};

struct platform_device fh_wdt_device = {
	.name = "fh_wdt",
	.id = 0,
	.num_resources = ARRAY_SIZE(fh_wdt_resources),
	.resource = fh_wdt_resources,
};

static struct platform_device fh_uart0_device = {
	.name			= "ttyS",
	.id				= 0,
	.num_resources	= ARRAY_SIZE(fh_uart0_resources),
	.resource		= fh_uart0_resources,
};

static struct platform_device fh_uart1_device = {
	.name			= "ttyS",
	.id				= 1,
	.num_resources	= ARRAY_SIZE(fh_uart1_resources),
	.resource		= fh_uart1_resources,
};

static struct platform_device fh_uart2_device = {
	.name			= "ttyS",
	.id				= 2,
	.num_resources	= ARRAY_SIZE(fh_uart2_resources),
	.resource		= fh_uart2_resources,
};

static struct platform_device fh_dma_device = {

	.name = "fh_dmac",
	.id				= 0,
	.num_resources	= ARRAY_SIZE(fh_dma_resources),
	.resource		= fh_dma_resources,
	.dev.platform_data  = &fh_dma_data,
};

static struct platform_device fh_i2c0_device = {
	.name	= "fh_i2c",
	.id				= 0,
	.num_resources	= ARRAY_SIZE(fh_i2c_resources_0),
	.resource		= fh_i2c_resources_0,
};

static struct platform_device fh_i2c1_device = {
	.name	= "fh_i2c",
	.id				= 1,
	.num_resources	= ARRAY_SIZE(fh_i2c_resources_1),
	.resource		= fh_i2c_resources_1,
};

static struct i2c_board_info __initdata fh_i2c_devices[] = {
	{
		I2C_BOARD_INFO("24c02", 0x50),
		.platform_data = &at24c02,
	},
	{
		I2C_BOARD_INFO("pcf8563", 0x51)
	}
};

#define FH_SPI0_CS0	(54)
#define FH_SPI0_CS1	(55)

#define FH_SPI1_CS0	(56)
#define FH_SPI1_CS1	(57)

#define FH_SPI3_CS0	(58)
#define FH_SPI3_CS1	(59)

//spi0
#define SPI0_FIFO_DEPTH				(64)
#define SPI0_CLK_IN					(100000000)
#define SPI0_MAX_SLAVE_NO			(2)

#define SPI1_FIFO_DEPTH				(32)
#define SPI1_CLK_IN					(100000000)
#define SPI1_MAX_SLAVE_NO			(2)

#define SPI2_FIFO_DEPTH				(64)
#define SPI2_CLK_IN				(100000000)

#define SPI3_FIFO_DEPTH				(32)
#define SPI3_CLK_IN					(100000000)
#define SPI3_MAX_SLAVE_NO			(2)

//SPI_TRANSFER_USE_DMA
//below is the control info
static struct fh_spi_platform_data fh_spi0_data = {
	.apb_clock_in = SPI0_CLK_IN,
	.fifo_len = SPI0_FIFO_DEPTH,
	.slave_max_num = SPI0_MAX_SLAVE_NO,
	.cs_data[0].GPIO_Pin = FH_SPI0_CS0,
	.cs_data[0].name = "spi0_cs0",
	.cs_data[1].GPIO_Pin = FH_SPI0_CS1,
	.cs_data[1].name = "spi0_cs1",
	.dma_transfer_enable = 0,
	.rx_handshake_num = 2,
	.tx_handshake_num = 3,
	.clk_name = "spi0_clk",
};

static struct fh_spi_platform_data fh_spi1_data = {
	.apb_clock_in = SPI1_CLK_IN,
	.fifo_len = SPI1_FIFO_DEPTH,
	.slave_max_num = SPI1_MAX_SLAVE_NO,
	.cs_data[0].GPIO_Pin = FH_SPI1_CS0,
	.cs_data[0].name = "spi1_cs0",
	.cs_data[1].GPIO_Pin = FH_SPI1_CS1,
	.cs_data[1].name = "spi1_cs1",
	.dma_transfer_enable = 0,
	.rx_handshake_num = 4,
	.tx_handshake_num = 5,
	.clk_name = "spi1_clk",
};




static struct fh_spi_platform_data fh_spi2_data = {
	.apb_clock_in = SPI2_CLK_IN,
	.fifo_len = SPI2_FIFO_DEPTH,
	.dma_transfer_enable = 0,
	.rx_handshake_num = 12,
	.tx_handshake_num = 13,
	.clk_name = "spi2_clk",
};

static struct fh_spi_platform_data fh_spi3_data = {
	.apb_clock_in = SPI3_CLK_IN,
	.fifo_len = SPI3_FIFO_DEPTH,
	.slave_max_num = SPI3_MAX_SLAVE_NO,
	.cs_data[0].GPIO_Pin = FH_SPI3_CS0,
	.cs_data[0].name = "spi3_cs0",
	.cs_data[1].GPIO_Pin = FH_SPI3_CS1,
	.cs_data[1].name = "spi3_cs1",
	.dma_transfer_enable = 0,
	.rx_handshake_num = 14,
	.tx_handshake_num = 15,
	.clk_name = "spi3_clk",
};


static struct fh_rtc_platform_data fh_rtc_data = {
	.clock_in = 32768,
	.dev_name = "rtc",
	.clk_name = "rtc_clk",
	.base_year = 2000,
	.base_month = 1,
	.base_day = 1,
};



static struct platform_device fh_spi0_device = {
	.name			= "fh_spi",
	.id				= 0,
	.num_resources	= ARRAY_SIZE(fh_spi0_resources),
	.resource		= fh_spi0_resources,
	.dev			= {
		.platform_data = &fh_spi0_data,
	},
};

static struct platform_device fh_spi1_device = {
	.name			= "fh_spi",
	.id				= 1,
	.num_resources	= ARRAY_SIZE(fh_spi1_resources),
	.resource		= fh_spi1_resources,
	.dev			= {
		.platform_data = &fh_spi1_data,
	},
};



static struct platform_device fh_spi2_device = {
	.name			= "fh_spi_slave",
	.id				= 0,
	.num_resources	= ARRAY_SIZE(fh_spi2_resources),
	.resource		= fh_spi2_resources,
	.dev			= {
		.platform_data = &fh_spi2_data,
	},
};


static struct platform_device fh_spi3_device = {
	.name			= "fh_spi",
	.id				= 3,
	.num_resources	= ARRAY_SIZE(fh_spi3_resources),
	.resource		= fh_spi3_resources,
	.dev			= {
		.platform_data = &fh_spi3_data,
	},
};

static struct platform_device fh_pwm_device = {
	.name			= "fh_pwm",
	.id				= 0,
	.num_resources	= ARRAY_SIZE(fh_pwm_resources),
	.resource		= fh_pwm_resources,

};

static struct platform_device fh_sadc_device = {
	.name			= "fh_sadc",
	.id				= 0,
	.num_resources	= ARRAY_SIZE(fh_sadc_resources),
	.resource		= fh_sadc_resources,
	.dev			= {
		.platform_data = NULL,
	},
};

static struct platform_device fh_aes_device = {
	.name			= "fh_aes",
	.id				= 0,
	.num_resources	= ARRAY_SIZE(fh_aes_resources),
	.resource		= fh_aes_resources,
	.dev			= {
		.platform_data = NULL,
	},
};

static struct platform_device fh_acw_misc_device = {
	.name			= "fh_acw",
	.id				= 0,
	.num_resources	= ARRAY_SIZE(fh_acw_resources),
	.resource		= fh_acw_resources,
};

static struct platform_device fh_ac97_device = {
	.name			= "fh-acodec",
	.id				= -1,
};

static struct platform_device fh_pcm_device = {
	.name			= "fh-pcm-audio",
	.id				= -1,
};



static struct platform_device fh_rtc_device = {
	.name			= "fh_rtc",
	.id				= 0,
	.num_resources	= ARRAY_SIZE(fh_rtc_resources),
	.resource		= fh_rtc_resources,
	.dev			= {
		.platform_data = &fh_rtc_data,
	},
};


static struct platform_device fh_efuse_device = {
	.name			= "fh_efuse",
	.id				= 0,
	.num_resources	= ARRAY_SIZE(fh_efuse_resources),
	.resource		= fh_efuse_resources,

};



static struct platform_device *linbao_devices[] __initdata = {
	&fh_gmac_device,
	&fh_uart0_device,
	&fh_uart1_device,
	&fh_uart2_device,
	&fh_dma_device,
	&fh_i2c0_device,
	&fh_i2c1_device,
	&fh_sd0_device,
	&fh_sd1_device,
	&fh_spi0_device,
	&fh_spi1_device,
	&fh_spi2_device,
	&fh_spi3_device,
	&fh_gpio0_device,
	&fh_gpio1_device,
	&fh_wdt_device,
	&fh_pwm_device,
	&fh_sadc_device,
	&fh_aes_device,
	&fh_pcm_device,
	&fh_ac97_device,
	&fh_acw_misc_device,
	&fh_i2s_misc_device,

	&fh_rtc_device,

	&fh_efuse_device,

};

static struct mtd_partition fh_sf_parts[] = {
	{
		/* head & Ramboot */
		.name		= "bootstrap",
		.offset		= 0,
		.size		= SZ_64K,
		.mask_flags	= MTD_WRITEABLE, /* force read-only */
	}, {
		/* Ramboot & U-Boot environment */
		.name		= "uboot-env",
		.offset		= MTDPART_OFS_APPEND,
		.size		= SZ_64K,
		.mask_flags	= MTD_WRITEABLE, /* force read-only */
	}, {
		/* U-Boot */
		.name		= "uboot",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 3 * SZ_64K,
		.mask_flags	= MTD_WRITEABLE, /* force read-only */
	}, {
		.name		= "kernel",
		.offset		= MTDPART_OFS_APPEND,
		.size		= SZ_4M,
		.mask_flags	= 0,
	}, {
		.name		= "rootfs",
		.offset		= MTDPART_OFS_APPEND,
		.size		= SZ_8M,
		.mask_flags	= 0,
	}, {
		.name		= "app",
		.offset		= MTDPART_OFS_APPEND,
		.size		= MTDPART_SIZ_FULL,
		.mask_flags	= 0,
	}//mtdparts=spi0.0:64k(bootstrap),64k(u-boot-env),192k(u-boot),4M(kernel),8M(rootfs),-(app)
	/* two blocks with bad block table (and mirror) at the end */
};

static struct flash_platform_data fh_flash_platform_data  = {
	.name		= "spi_flash",
	.parts		= fh_sf_parts,
	.nr_parts	= ARRAY_SIZE(fh_sf_parts),
};

static struct spi_board_info fh_spi_devices[] = {
	{
		.modalias        = "m25p80",
		.bus_num         = 0,
		.chip_select     = 0,
		.mode            = SPI_MODE_3,
		.max_speed_hz    = 25000000,
		.platform_data   = &fh_flash_platform_data,
	},
	{
		// Try spi nand flash
		.modalias       = "gd5f", //"spi-nand",
		.bus_num        = 0,
		.chip_select    = 1,
		.max_speed_hz   = 2500000,
		.mode           = SPI_MODE_3,
	},
};

static void __init linbao_map_io(void)
{
	iotable_init(linbao_io_desc, ARRAY_SIZE(linbao_io_desc));
}

static __init void linbao_board_init(void)
{
	printk("Linbao board init \n");
	platform_add_devices(linbao_devices, ARRAY_SIZE(linbao_devices));
	i2c_register_board_info(1, fh_i2c_devices, ARRAY_SIZE(fh_i2c_devices));
	spi_register_board_info(fh_spi_devices, ARRAY_SIZE(fh_spi_devices));
	fh_clk_procfs_init();
	fh_pmu_init();
}

static void __init linbao_init_early(void)
{
	u32 ccdate;
	u32 cctime;
	u32 y, m, d, h, mm, s;
	fh_clk_init();
//TODO:	linbao_iomux_init(&linbao_iomux);

	/*fpga version*/
	ccdate = fh_pmu_get_reg(REG_PMU_CHIP_ID);
	cctime = fh_pmu_get_reg(REG_PMU_IP_VER);
	y = ccdate & 0xff;
	m = (ccdate & 0xff00) >> 8;
	d = (ccdate & 0xff0000) >> 16;
	h = cctime & 0xff;
	mm = (cctime & 0xff00) >> 8;
	s = (cctime & 0xff0000) >> 16;
	printk("Linbao FPGA version: %x-%x-%x %x:%x:%x ", y, m, d, h, mm, s);
}

MACHINE_START(LINBAO, "LINBAO")
	.boot_params	= DDR_BASE + 0x100,
	.map_io		= linbao_map_io,
	.init_irq	= fh_intc_init,
	.timer		= &fh_timer,
	.init_machine	= linbao_board_init,
	.init_early	= linbao_init_early,
MACHINE_END
