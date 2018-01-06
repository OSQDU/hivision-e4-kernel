#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/io.h>

#include <mach/io.h>
#include <mach/chip.h>
#include <mach/chip_reg.h>
//#include <mach/pmu.h>

static int fh_pmu_flag_stop = 0;

void fh_pmu_stop(void)
{
	fh_pmu_flag_stop = 1;
}
EXPORT_SYMBOL(fh_pmu_stop);

void fh_pmu_set_reg(u32 offset, u32 data)
{
	if (fh_pmu_flag_stop)
		return;

	if (offset > PMU_REG_SIZE) {
		pr_err("fh_pmu_set_reg: offset is out of range");
		return;
	}
	writel(data, VPMU(PMU_REG_BASE + offset));
}
EXPORT_SYMBOL(fh_pmu_set_reg);

u32 fh_pmu_get_reg(u32 offset)
{
	if (fh_pmu_flag_stop)
		return 0;

	if (offset > PMU_REG_SIZE) {
		pr_err("fh81_pmu_get_reg: offset is out of range");
		return 0;
	}
	return readl(VPMU(PMU_REG_BASE + offset));
}
EXPORT_SYMBOL(fh_pmu_get_reg);

void fh_pae_set_reg(u32 offset, u32 data)
{
	if (offset > 0x60) {
		pr_err("fh_pae_set_reg: offset is out of range");
		return;
	}
	writel(data, VPAE(PAE_REG_BASE + offset));
}
EXPORT_SYMBOL(fh_pae_set_reg);

int fh_pmu_init(void)
{
	fh_pmu_set_reg(REG_PMU_VDAC_CTRL, 0x2);
	return 0;
}
