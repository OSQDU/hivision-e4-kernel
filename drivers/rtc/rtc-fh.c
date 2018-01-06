/*
 * rtc-fh.c
 *
 *  Created on: Jul 5, 2016
 *      Author: duobao
 */
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/rtc.h>
#include <linux/bcd.h>
#include <linux/clk.h>
#include <linux/log2.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <mach/rtc.h>
struct fh_rtc_controller
{
    // vadd
	void * __iomem regs; /* vaddr of the control registers */
	u32 irq;
	u32 paddr;
	u32 base_year;
	u32 base_month;
	u32 base_day;
	struct rtc_device *rtc;
	struct clk *clk;
};


enum{

	TIME_FUNC = 0,
	ALARM_FUNC,

};


//#define FH_RTC_DEBUG_PRINT

#ifdef FH_RTC_DEBUG_PRINT
#define RTC_PRINT_DBG(fmt, args...) \
	printk("[FH_RTC_DEBUG]: "); \
	printk(fmt, ## args)
#else
#define RTC_PRINT_DBG(fmt, args...)  do { } while (0)
#endif



static u32 fh_rtc_get_hw_sec_data(struct fh_rtc_controller *fh_rtc,u32 func_switch){

	u32 ret_sec;
	u32 raw_value;
	u32 sec_value;
	u32 min_value;
	u32 hour_value;
	u32 day_value;

	if(func_switch == TIME_FUNC)
		raw_value = fh_rtc_get_time(fh_rtc->regs);
	else
		raw_value = fh_rtc_get_alarm_time(fh_rtc->regs);


	sec_value = FH_GET_RTC_SEC(raw_value);
	min_value = FH_GET_RTC_MIN(raw_value);
	hour_value = FH_GET_RTC_HOUR(raw_value);
	day_value = FH_GET_RTC_DAY(raw_value);
	ret_sec = (day_value * 86400) + (hour_value * 3600) + (min_value * 60) + sec_value;
	return ret_sec;

}

static void fh_rtc_set_hw_sec_data(struct fh_rtc_controller *fh_rtc,struct rtc_time *rtc_tm,u32 func_switch){
	u32 raw_value;

	u32 sec_value;
	u32 min_value;
	u32 hour_value;
	u32 day_value;

	day_value = rtc_year_days(rtc_tm->tm_mday,rtc_tm->tm_mon,rtc_tm->tm_yday);
	day_value += (rtc_tm->tm_year-70)*365 + ELAPSED_LEAP_YEARS(rtc_tm->tm_year);
	hour_value = rtc_tm->tm_hour;
	min_value = rtc_tm->tm_min;
	sec_value = rtc_tm->tm_sec;

	raw_value = (day_value << DAY_BIT_START) | (hour_value << HOUR_BIT_START)
			| (min_value << MIN_BIT_START) | (sec_value << MIN_BIT_START);

	if(func_switch == TIME_FUNC)
		fh_rtc_set_time(fh_rtc->regs,raw_value);
	else
		fh_rtc_set_alarm_time(fh_rtc->regs,raw_value);

}


/*******************************/

static int fh_rtc_open(struct device *dev)
{

	pr_info("%s\n",__func__);
	return 0;
}

static void fh_rtc_release(struct device *dev)
{
	pr_info("%s\n",__func__);
}

// return hw total sec value..
static u32 fh_rtc_get_hw_time_sec_data(struct fh_rtc_controller *fh_rtc){

	return fh_rtc_get_hw_sec_data(fh_rtc,TIME_FUNC);
}

static void fh_rtc_set_hw_time_data(struct fh_rtc_controller *fh_rtc,struct rtc_time *rtc_tm){

	fh_rtc_set_hw_sec_data(fh_rtc,rtc_tm,TIME_FUNC);
}

static u32 fh_rtc_get_hw_alarm_sec_data(struct fh_rtc_controller *fh_rtc){
	return fh_rtc_get_hw_sec_data(fh_rtc,ALARM_FUNC);
}

static void fh_rtc_set_hw_alarm_data(struct fh_rtc_controller *fh_rtc,struct rtc_time *rtc_tm){
	fh_rtc_set_hw_sec_data(fh_rtc,rtc_tm,ALARM_FUNC);
}



static int fh_rtc_gettime(struct device *dev, struct rtc_time *rtc_tm)
{

	struct platform_device *pdev = to_platform_device(dev);
	struct fh_rtc_controller *fh_rtc = platform_get_drvdata(pdev);

	pr_info("%s\n",__func__);

	rtc_time_to_tm(fh_rtc_get_hw_time_sec_data(fh_rtc),rtc_tm);

//debug
	//fh_rtc_dump_current_data(rtc_tm);

	return 0;
}

static int fh_rtc_settime(struct device *dev, struct rtc_time *tm)
{

	struct platform_device *pdev = to_platform_device(dev);
	struct fh_rtc_controller *fh_rtc = platform_get_drvdata(pdev);

	//debug
	//fh_rtc_dump_current_data(tm);

	fh_rtc_set_hw_time_data(fh_rtc,tm);

	return 0;
}

static int fh_rtc_getalarm(struct device *dev, struct rtc_wkalrm *alrm)
{

	struct platform_device *pdev = to_platform_device(dev);
	struct fh_rtc_controller *fh_rtc = platform_get_drvdata(pdev);
	struct rtc_time *rtc_tm = &alrm->time;
	pr_info("%s\n",__func__);
	//fh_rtc_dump_current_data(rtc_tm);
	rtc_time_to_tm(fh_rtc_get_hw_alarm_sec_data(fh_rtc),rtc_tm);

	return 0;
}

static int fh_rtc_setalarm(struct device *dev, struct rtc_wkalrm *alrm)
{

	struct platform_device *pdev = to_platform_device(dev);
	struct fh_rtc_controller *fh_rtc = platform_get_drvdata(pdev);
	struct rtc_time *rtc_tm = &alrm->time;
	pr_info("%s\n",__func__);
	//debug
	//fh_rtc_dump_current_data(rtc_tm);
	fh_rtc_set_hw_alarm_data(fh_rtc,rtc_tm);

	return 0;
}



static int fh_rtc_irq_enable(struct device *dev, unsigned int enabled)
{

	struct platform_device *pdev = to_platform_device(dev);
	struct fh_rtc_controller *fh_rtc = platform_get_drvdata(pdev);
	pr_info("%s\n",__func__);

	if(enabled){
		fh_rtc_enable_interrupt(fh_rtc->regs,FH_RTC_ISR_SEC_POS|FH_RTC_ISR_ALARM_POS);
	}
	else{

		fh_rtc_disenable_interrupt(fh_rtc->regs,FH_RTC_ISR_SEC_POS|FH_RTC_ISR_ALARM_POS);
	}

	//fh_rtc_dump_regs(fh_rtc);
	return 0;
}




static irqreturn_t fh_rtc_irq(int irq, void *dev_id)
{

	struct fh_rtc_controller *fh_rtc = (struct fh_rtc_controller *)dev_id;
	unsigned long events = 0;
	u32 isr_status;
	struct rtc_device *rtc = fh_rtc->rtc;

	//fh_rtc_dump_regs(fh_rtc);

	isr_status = fh_rtc_get_enabled_interrupt(fh_rtc->regs);
	//clear isr status
	fh_rtc_clear_interrupt_status(fh_rtc->regs,isr_status);

	//check which kind of isr....
	if(isr_status & FH_RTC_ISR_SEC_POS){

		events |= RTC_IRQF | RTC_UF;
	}
	else if(isr_status & FH_RTC_ISR_ALARM_POS){
		events |= RTC_IRQF | RTC_AF;
	}
	else{
		pr_info("rtc unknown isr...\n");
		return IRQ_HANDLED;
	}
	rtc_update_irq(rtc, 1, events);

	return IRQ_HANDLED;

}



static const struct rtc_class_ops fh_rtcops = {
	.open		= fh_rtc_open,
	.release	= fh_rtc_release,
	.read_time	= fh_rtc_gettime,
	.set_time	= fh_rtc_settime,
	.read_alarm	= fh_rtc_getalarm,
	.set_alarm	= fh_rtc_setalarm,
	.alarm_irq_enable = fh_rtc_irq_enable,
};

static void create_proc_rtc(struct fh_rtc_controller *rtc);
static void remove_proc(void);
static int __devinit fh_rtc_probe(struct platform_device *pdev)
{
	int err = 0;
	struct fh_rtc_controller *fh_rtc;
	struct resource *ioarea;
	struct fh_rtc_platform_data * rtc_platform_info;
	struct rtc_device *rtc;
	struct resource *res;


	//first malloc rtc controller....
	fh_rtc = kzalloc(sizeof(struct fh_rtc_controller), GFP_KERNEL);
	if (!fh_rtc)
		return -ENOMEM;

	memset(fh_rtc,0,sizeof(struct fh_rtc_controller));
	//board info below
	rtc_platform_info = (struct fh_rtc_platform_data *)pdev->dev.platform_data;
	if(rtc_platform_info == NULL){
		dev_err(&pdev->dev, "%s, rtc platform error.\n",
			__func__);
		err = -ENODEV;
		goto err_nores;
	}
	fh_rtc->base_year = rtc_platform_info->base_year;
	fh_rtc->base_month = rtc_platform_info->base_month;
	fh_rtc->base_day = rtc_platform_info->base_day;
	/* find the IRQs */
	fh_rtc->irq = platform_get_irq(pdev, 0);
	if (fh_rtc->irq < 0) {
		dev_err(&pdev->dev, "%s, rtc irq error.\n",
			__func__);
		err = fh_rtc->irq;
		goto err_nores;
	}

	/* get the memory region */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		dev_err(&pdev->dev, "failed to get memory region resource\n");
		err = -ENOENT;
		goto err_nores;
	}

	fh_rtc->paddr = res->start;
	ioarea = request_mem_region(res->start, resource_size(res),
			pdev->name);
	if(!ioarea) {
		dev_err(&pdev->dev, "rtc region already claimed\n");
		err = -EBUSY;
		goto err_nores;
	}

	fh_rtc->regs = ioremap(res->start, resource_size(res));
	if (!fh_rtc->regs) {
		dev_err(&pdev->dev, "rtc already mapped\n");
		err = -EINVAL;
		//free mem region
		goto err_nores;
	}

	/* register RTC and exit */
	platform_set_drvdata(pdev, fh_rtc);
	rtc = rtc_device_register(rtc_platform_info->dev_name, &pdev->dev, &fh_rtcops,
				  THIS_MODULE);

	if (IS_ERR(rtc)) {
		dev_err(&pdev->dev, "cannot attach rtc\n");
		err = PTR_ERR(rtc);
		goto err_nores;
	}
	fh_rtc->rtc = rtc;


	err = request_irq(fh_rtc->irq , fh_rtc_irq, 0,
			  dev_name(&pdev->dev), fh_rtc);
	if (err) {
		dev_dbg(&pdev->dev, "request_irq failed, %d\n", err);
		goto err_nores;
	}

	create_proc_rtc(fh_rtc);
err_nores:
	return err;

	return 0;
}

static int __devexit fh_rtc_remove(struct platform_device *dev)
{
	return 0;
}



#ifdef CONFIG_PM

/* RTC Power management control */

static int fh_rtc_suspend(struct platform_device *pdev, pm_message_t state)
{
	return 0;
}

static int fh_rtc_resume(struct platform_device *pdev)
{
	return 0;
}
#else
#define fh_rtc_suspend NULL
#define fh_rtc_resume  NULL
#endif



static struct platform_driver fh_rtc_driver = {
	.probe		= fh_rtc_probe,
	.remove		= __devexit_p(fh_rtc_remove),
	.suspend	= fh_rtc_suspend,
	.resume		= fh_rtc_resume,
	.driver		= {
		.name	= "fh_rtc",
		.owner	= THIS_MODULE,
	},
};


static int __init fh_rtc_init(void)
{

	return platform_driver_register(&fh_rtc_driver);
}

static void __exit fh_rtc_exit(void)
{
	remove_proc();
	platform_driver_unregister(&fh_rtc_driver);
}

module_init(fh_rtc_init);
module_exit(fh_rtc_exit);

MODULE_DESCRIPTION("FH SOC RTC Driver");
MODULE_AUTHOR("yu.zhang <zhangy@fullhan.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:fh-rtc");
/****************************debug proc*****************************/
#include <linux/proc_fs.h>

struct proc_dir_entry *proc_rtc_entry;


ssize_t proc_read(char *page, char **start, off_t off, int count, int *eof, struct fh_rtc_controller *data)
{
    ssize_t len = 0;
  //  struct fh_rtc_controller *data;
	printk(KERN_INFO"------------- dump register -------------\n");
	printk("cnt:0x%x\n",fh_rtc_get_time(data->regs)  );
	printk("offset:0x%x\n",fh_rtc_get_offset(data->regs));
	printk("fail:0x%x\n",fh_rtc_get_power_fail(data->regs));
	printk("alarm_cnt:0x%x\n",fh_rtc_get_alarm_time(data->regs));
	printk("int stat:0x%x\n",fh_rtc_get_int_status(data->regs));
	printk("int en:0x%x\n",fh_rtc_get_enabled_interrupt(data->regs));
	printk("sync:0x%x\n",fh_rtc_get_sync(data->regs));
	printk("debug:0x%x\n",fh_rtc_get_debug(data->regs));
	printk(KERN_INFO"-------------------------------------------\n");

    return len;
}

static void create_proc_rtc(struct fh_rtc_controller *rtc)
{
	proc_rtc_entry = create_proc_entry("fh_rtc", S_IRUGO, NULL);
    if (!proc_rtc_entry)
    {
        printk(KERN_ERR"create proc failed\n");
    }
    else
    {
    	proc_rtc_entry->read_proc = (read_proc_t *)proc_read;
    	proc_rtc_entry->data = rtc;
    }
}

static void remove_proc(void)
{
    remove_proc_entry("fh_rtc", NULL);
}


