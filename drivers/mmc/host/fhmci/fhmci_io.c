//#define CONFIG_HIK_MACH

#include <mach/clock.h>

#define SIMPLE_0
//#define SIMPLE_90
//#define SIMPLE_180
//#define SIMPLE_270

static int fh_mci_sys_ctrl_init(struct fhmci_host *host)
{
#ifdef OLD
#define PMU_BASE_ADDR (0xfe900000)
#endif

	struct clk *sdc_clk;
	u32 pctrl_gpio;
	u32 drv_shift;
	u32 sam_shift;
	int ret;
	u32 reg;
#ifdef OLD
	u32 clk_cfg_offset;
	u32 oth_cfg_offset[5];
	u32 offset;
	int i;
#endif

	if (host->id == 0) {
		pctrl_gpio = 5;
		drv_shift = 20;
		sam_shift = 16;
#ifdef OLD
		clk_cfg_offset = PMU_BASE_ADDR + 0x170;
		oth_cfg_offset[0] = PMU_BASE_ADDR + 0x178; //D0
		oth_cfg_offset[1] = PMU_BASE_ADDR + 0x17c;//D1
		oth_cfg_offset[2] = PMU_BASE_ADDR + 0x180;//D2
		oth_cfg_offset[3] = PMU_BASE_ADDR + 0x184;//D3
		oth_cfg_offset[4] = PMU_BASE_ADDR + 0x174;//CMD
#endif
		sdc_clk = clk_get(NULL, "sdc0_clk");

#ifdef CONFIG_HIK_MACH
		pctrl_gpio = 53;
#ifdef OLD
		//mux to GPIO53
		reg = *(int *)(PMU_BASE_ADDR + 0x016c);
		reg |= (1 << 20);
		*(int *)(PMU_BASE_ADDR + 0x016c) = reg;
#endif
#endif
	} else {
		pctrl_gpio = 6;
		drv_shift = 12;
		sam_shift = 8;
#ifdef OLD
		clk_cfg_offset = PMU_BASE_ADDR + 0x188;
		oth_cfg_offset[0] = PMU_BASE_ADDR + 0x194; //D0
		oth_cfg_offset[1] = PMU_BASE_ADDR + 0x198;//D1
		oth_cfg_offset[2] = PMU_BASE_ADDR + 0x19c;//D2
		oth_cfg_offset[3] = PMU_BASE_ADDR + 0x1a0;//D3
		oth_cfg_offset[4] = PMU_BASE_ADDR + 0x1a4;//CMD
#endif
		sdc_clk = clk_get(NULL, "sdc1_clk");

#ifdef CONFIG_HIK_MACH
		pctrl_gpio = 59;
#ifdef OLD
		//mux to GPIO59
		reg = *(int *)(PMU_BASE_ADDR + 0x0190);
		reg |= (1 << 20);
		*(int *)(PMU_BASE_ADDR + 0x0190) = reg;
#endif
#endif
	}

	//Power on.
	ret = gpio_request(pctrl_gpio, NULL);
	if (ret) {
		fhmci_error("gpio requset err!");
		return ret;
	}
#ifdef CONFIG_HIK_MACH
	gpio_direction_output(pctrl_gpio, 1);
#else
	gpio_direction_output(pctrl_gpio, 0);
#endif
	gpio_free(pctrl_gpio);

	//adjust clock phase...
	clk_enable(sdc_clk);
	clk_set_rate(sdc_clk, 50000000);
	reg = clk_get_clk_sel();
	reg &= ~(3 << drv_shift);
	reg &= ~(3 << sam_shift);
	reg |= (2 << drv_shift); //now drv fixed to 180.
#ifdef SIMPLE_0
	reg |= (0 << sam_shift);
#endif
#ifdef SIMPLE_90
	reg |= (1 << sam_shift);
#endif
#ifdef SIMPLE_180
	reg |= (2 << sam_shift);
#endif
#ifdef SIMPLE_270
	reg |= (3 << sam_shift);
#endif
	clk_set_clk_sel(reg);
#ifdef OLD
	//adjust driver/SMT/Pull-UP...
	if (1) {
		//CLK
		reg = *((volatile int *)clk_cfg_offset);
		reg &= ~(3 << 4);
		reg |= (3 << 4);//drive
		*((volatile int *)clk_cfg_offset) = reg;

		//Data0~DATA3, CMD
		for (i = 0; i < sizeof(oth_cfg_offset) / sizeof(u32); i++) {
			offset = oth_cfg_offset[i];
			reg = *((volatile int *)offset);
			reg &= ~(3 << 4);
			reg |= (3 << 4); //drive
			reg |= (1 << 8);//SMT
			reg &= ~(3 << 16);
			reg |= (2 << 16);//PULL UP
			*((volatile int *)offset) = reg;
		}
	}
#endif
	return 0;
}
