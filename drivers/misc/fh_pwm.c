#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/pwm.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include <asm/uaccess.h>
#include "fh_pwm.h"

#define FH_PWM_PROC_FILE "driver/pwm"

#undef  FH_PWM_DEBUG
#ifdef FH_PWM_DEBUG
#define PRINT_DBG(fmt,args...)  printk(fmt,##args)
#else
#define PRINT_DBG(fmt,args...)  do{} while(0)
#endif

static struct pwm_ctrl fh_pwm_ctrl =
{
	.base = 0,
};

#ifdef CONFIG_FH_PWM_DEBUG_FS
static unsigned long pwm_get_feq(int pwm_id)
{
	unsigned long rate = clk_get_rate(fh_pwm_ctrl.clk);
	return rate;
}

static int pwm_set_feq(int pwm_id, unsigned long feq)
{
	clk_set_rate(fh_pwm_ctrl.clk, feq);
	return 0;
}

static u32 pwm_get_period(int pwm_id)
{
	u32 period = readl(fh_pwm_ctrl.base + REG_PWM_CMD(pwm_id)) & 0xfff;
	return period;
}

static int pwm_set_period(int pwm_id, u32 period)
{
	u32 reg;

	if(period < 8)
	{
		pr_err("PWM: min period is 8\n");
		return -EINVAL;
	}

	reg = readl(fh_pwm_ctrl.base + REG_PWM_CMD(pwm_id));

	reg &= ~(0xfff);
	reg |= period & 0xfff;

	writel(reg, fh_pwm_ctrl.base + REG_PWM_CMD(pwm_id));
	return 0;
}

static u32 pwm_get_counter(int pwm_id)
{
	u32 count = (readl(fh_pwm_ctrl.base + REG_PWM_CTRL) >> 16) & 0xfff;
	return count;
}

static int pwm_set_counter(int pwm_id, u32 counter)
{
	u32 reg;
	reg = readl(fh_pwm_ctrl.base + REG_PWM_CMD(pwm_id));

	reg &= ~(0xfff0000);
	reg |= (counter & 0xfff) << 16;

	writel(reg, fh_pwm_ctrl.base + REG_PWM_CMD(pwm_id));

	return 0;
}
#endif

static int pwm_get_duty_cycle_ns(struct pwm_device* pwm)
{
    u32 reg, period, duty;
    u32 clk_rate = clk_get_rate(fh_pwm_ctrl.clk);

    reg = readl(fh_pwm_ctrl.base + REG_PWM_CMD(pwm->id));
    period = reg & 0x0fff;
    duty = (reg >> 16) & 0xfff;
    duty = period - duty;       //reverse duty cycle

    if(period == 0)
    {
        period = duty;
    }

    pwm->counter_ns = duty * 1000000000 / clk_rate;
    pwm->period_ns = period * 1000000000 / clk_rate;

    PRINT_DBG("get duty: %d, period: %d, reg: 0x%x\n", duty, period, reg);

    return 0;
}

static int pwm_set_duty_cycle_ns(struct pwm_device* pwm)
{
    u32 period, duty, reg, clk_rate, duty_revert;
    clk_rate = clk_get_rate(fh_pwm_ctrl.clk);
    if(!clk_rate)
    {
        pr_err("PWM: clock rate is 0\n");
        return -EINVAL;
    }
    period = pwm->period_ns / (1000000000 / clk_rate);

    if(period < 8)
    {
        pr_err("PWM: min period is 8\n");
        return -EINVAL;
    }

    duty = pwm->counter_ns / (1000000000 / clk_rate);

    if(period < duty)
    {
        pr_err("PWM: period < duty\n");
        return -EINVAL;
    }

    duty_revert = period - duty;

    if(duty == period)
    {
        reg = (duty & 0xfff) << 16 | (0 & 0xfff);
    }
    else
    {
        reg = (duty_revert & 0xfff) << 16 | (period & 0xfff);
    }

    PRINT_DBG("set duty_revert: %d, period: %d, reg: 0x%x\n", duty_revert, period, reg);

    writel(reg, fh_pwm_ctrl.base + REG_PWM_CMD(pwm->id));
    return 0;
}


static int pwm_set_duty_cycle_percent(struct pwm_device* pwm)
{
    u32 period, duty, reg, clk_rate, duty_revert;
    clk_rate = clk_get_rate(fh_pwm_ctrl.clk);
    if(!clk_rate)
    {
        pr_err("PWM: clock rate is 0\n");
        return -EINVAL;
    }

    if(pwm->percent > 100 || pwm->percent < 0)
    {
        pr_err("PWM: pwm->percent is out of range\n");
        return -EINVAL;
    }

    period = pwm->period_ns / (1000000000 / clk_rate);

    if(period < 8)
    {
        pr_err("PWM: min period is 8\n");
        return -EINVAL;
    }

    duty = period * 100 / pwm->percent;

    if(period < duty)
    {
        pr_err("PWM: period < duty\n");
        return -EINVAL;
    }

    duty_revert = period - duty;

    if(duty == period)
    {
        reg = (duty & 0xfff) << 16 | (0 & 0xfff);
    }
    else
    {
        reg = (duty_revert & 0xfff) << 16 | (period & 0xfff);
    }

    PRINT_DBG("set duty_revert: %d, period: %d, reg: 0x%x\n", duty_revert, period, reg);

    writel(reg, fh_pwm_ctrl.base + REG_PWM_CMD(pwm->id));
    return 0;
}

struct pwm_device *pwm_request(int pwm_id, const char *label)
{

    if(fh_pwm_ctrl.pwm[pwm_id].working == 0)
    {
        pwm_enable(&fh_pwm_ctrl.pwm[pwm_id]);
        return &fh_pwm_ctrl.pwm[pwm_id];
    }
    else
    {
        return NULL;
    }
};
EXPORT_SYMBOL(pwm_request);

void pwm_free(struct pwm_device *pwm)
{
    pwm_disable(pwm);
    pwm->working = 0;
    return;
}
EXPORT_SYMBOL(pwm_free);

int pwm_config(struct pwm_device *pwm, int duty_ns, int period_ns)
{
    pwm->counter_ns = duty_ns;
    pwm->period_ns = period_ns;
    pwm_set_duty_cycle_ns(pwm);
    return 0;
}
EXPORT_SYMBOL(pwm_config);

int pwm_enable(struct pwm_device *pwm)
{
    pwm->working = 1;
    writel(0x1, fh_pwm_ctrl.base + REG_PWM_CTRL);
    return 0;
}
EXPORT_SYMBOL(pwm_enable);

void pwm_disable(struct pwm_device *pwm)
{
    pwm->working = 0;
    writel(0x0, fh_pwm_ctrl.base + REG_PWM_CTRL);
}
EXPORT_SYMBOL(pwm_disable);


#ifdef CONFIG_FH_PWM_DEBUG_FS
/*
 * /sys/devices/platform/ep93xx-pwm.N
 *   /freq          read-write  pwm output frequency (0 = disable output)
 *   /duty_percent  read-write  pwm duty cycle percent (1..99)
 */

static ssize_t fh_pwm_get_clk_freq(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	unsigned long rate = pwm_get_feq(0);

	return sprintf(buf, "%ld\n", rate);

}

static ssize_t fh_pwm_set_clk_freq(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	long val;
	int err;

	err = strict_strtol(buf, 10, &val);
	if (err)
		return -EINVAL;

	err = pwm_set_feq(0, val);
	return err;
}

static ssize_t fh_pwm_get_timer_period(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	u32 period = pwm_get_period(0);

	return sprintf(buf, "%d\n", period);

}

static ssize_t fh_pwm_set_timer_period(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	long val;
	int err;

	err = strict_strtol(buf, 10, &val);
	if (err)
		return -EINVAL;

	err = pwm_set_period(0, val);

	return err;
}

static ssize_t fh_pwm_get_pwm_counter(struct device *dev,
		struct device_attribute *attr, char *buf)
{

	u32 count = pwm_get_counter(0);

	return sprintf(buf, "%d\n", count);
}

static ssize_t fh_pwm_set_pwm_counter(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{

	long val;
	int err;

	err = strict_strtol(buf, 10, &val);
	if (err)
		return -EINVAL;

	err = pwm_set_counter(0, val);

	return err;
}

static DEVICE_ATTR(clk_freq, S_IWUSR | S_IRUGO,
		fh_pwm_get_clk_freq, fh_pwm_set_clk_freq);
static DEVICE_ATTR(pwm_counter, S_IWUSR | S_IRUGO,
		fh_pwm_get_pwm_counter, fh_pwm_set_pwm_counter);
static DEVICE_ATTR(timer_period, S_IWUSR | S_IRUGO,
		fh_pwm_get_timer_period, fh_pwm_set_timer_period);


static struct attribute *fh_pwm_attrs[] =
{
	&dev_attr_clk_freq.attr,
	&dev_attr_pwm_counter.attr,
	&dev_attr_timer_period.attr,
	NULL
};

static const struct attribute_group fh_pwm_sysfs_files =
{
	.attrs	= fh_pwm_attrs,
};

#endif

static int fh_pwm_open(struct inode *inode, struct file *file)
{
	writel(0x1, fh_pwm_ctrl.base + REG_PWM_CTRL);
	return 0;
}

static int fh_pwm_release(struct inode *inode, struct file *filp)
{
	writel(0x0, fh_pwm_ctrl.base + REG_PWM_CTRL);
	return 0;
}


static long fh_pwm_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	struct pwm_device *pwm;

	if (unlikely(_IOC_TYPE(cmd) != PWM_IOCTL_MAGIC))
	{
		pr_err("%s: ERROR: incorrect magic num %d (error: %d)\n",
			   __func__, _IOC_TYPE(cmd), -ENOTTY);
		return -ENOTTY;
	}

	if (unlikely(_IOC_NR(cmd) > PWM_IOCTL_MAXNR))
	{
		pr_err("%s: ERROR: incorrect cmd num %d (error: %d)\n",
			   __func__, _IOC_NR(cmd), -ENOTTY);
		return -ENOTTY;
	}

	if (_IOC_DIR(cmd) & _IOC_READ)
	{
		ret = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	}
	else if(_IOC_DIR(cmd) & _IOC_WRITE)
	{
		ret = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	}

	if(ret)
	{
		pr_err("%s: ERROR: user space access is not permitted %d (error: %d)\n",
			   __func__, _IOC_NR(cmd), -EACCES);
		return -EACCES;
	}

	switch(cmd)
	{
	case ENABLE_PWM:
		writel(0x1, fh_pwm_ctrl.base + REG_PWM_CTRL);
		break;
	case DISABLE_PWM:
		writel(0x0, fh_pwm_ctrl.base + REG_PWM_CTRL);
		break;
	case SET_PWM_DUTY_CYCLE:
	    pwm = (struct pwm_device __user *)arg;
	    printk("ioctl: pwm addr: %p, pwm->period: %d ns\n", pwm, pwm->period_ns);
	    pwm_set_duty_cycle_ns(pwm);
		break;
	case GET_PWM_DUTY_CYCLE:
	    pwm = (struct pwm_device __user *)arg;
	    printk("ioctl: pwm->id: %d, pwm->counter: %d, pwm->period: %d\n", pwm->id, pwm->counter_ns, pwm->period_ns);
	    pwm_get_duty_cycle_ns(pwm);
	    break;
    case SET_PWM_DUTY_CYCLE_PERCENT:
        pwm = (struct pwm_device __user *)arg;
        printk("ioctl: pwm->id: %d, pwm->counter: %d, pwm->period: %d\n", pwm->id, pwm->counter_ns, pwm->period_ns);
        pwm_set_duty_cycle_percent(pwm);
        break;
	}


	return ret;
}

static const struct file_operations fh_pwm_fops =
{
	.owner 			= THIS_MODULE,
	.open 			= fh_pwm_open,
	.release 		= fh_pwm_release,
	.unlocked_ioctl = fh_pwm_ioctl,
};

static struct miscdevice fh_pwm_misc =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &fh_pwm_fops,
};

static void del_char(char* str,char ch)
{
   char *p = str;
   char *q = str;
   while(*q)
   {
       if (*q !=ch)
       {
           *p++ = *q;
       }
       q++;
   }
   *p='\0';
}

static ssize_t fh_pwm_proc_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
    int i;
    char message[32] = {0};
    char * const delim = ",";
    char *cur = message;
    char* param_str[4];
    unsigned int param[4];

    len = (len > 32) ? 32 : len;

    if (copy_from_user(message, buf, len))
        return -EFAULT;

    for(i=0; i<4; i++)
    {
        param_str[i] = strsep(&cur, delim);
        if(!param_str[i])
        {
            pr_err("%s: ERROR: parameter[%d] is empty\n", __func__, i);
            return -EINVAL;
        }
        else
        {
            del_char(param_str[i], ' ');
            del_char(param_str[i], '\n');
            param[i] = (u32)simple_strtoul(param_str[i], NULL, 10);
            if(param[i] < 0)
            {
                pr_err("%s: ERROR: parameter[%d] is incorrect\n", __func__, i);
                return -EINVAL;
            }
        }
    }

    printk("set pwm %d to %s, duty cycle: %u ns, period cycle: %u\n", param[0], param[1] ? "enable" : "disable", param[2], param[3]);
    fh_pwm_ctrl.pwm[param[0]].counter_ns = param[2];
    fh_pwm_ctrl.pwm[param[0]].period_ns = param[3];

    param[1] ? pwm_enable(&fh_pwm_ctrl.pwm[param[0]]) : pwm_disable(&fh_pwm_ctrl.pwm[param[0]]);
    pwm_set_duty_cycle_ns(&fh_pwm_ctrl.pwm[param[0]]);

    return len;
}

static void *v_seq_start(struct seq_file *s, loff_t *pos)
{
    static unsigned long counter = 0;
    if (*pos == 0)
        return &counter;
    else
    {
        *pos = 0;
        return NULL;
    }
}

static void *v_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
    (*pos)++;
    return NULL;
}

static void v_seq_stop(struct seq_file *s, void *v)
{

}

static int v_seq_show(struct seq_file *sfile, void *v)
{
    int i;
    seq_printf(sfile, "\nPWM Status:\n");

    for(i=0; i<3; i++)
    {
        seq_printf(sfile, "id: %d \t%s, duty_ns: %u, period_ns: %u\n",
                fh_pwm_ctrl.pwm[i].id,
                (fh_pwm_ctrl.pwm[i].working) ? "ENABLE" : "DISABLE",
                 fh_pwm_ctrl.pwm[i].counter_ns,
                 fh_pwm_ctrl.pwm[i].period_ns);
    }
    return 0;
}

static const struct seq_operations isp_seq_ops =
{
    .start = v_seq_start,
    .next = v_seq_next,
    .stop = v_seq_stop,
    .show = v_seq_show
};

static int fh_pwm_proc_open(struct inode *inode, struct file *file)
{
    return seq_open(file, &isp_seq_ops);
}

static struct file_operations fh_pwm_proc_ops =
{
    .owner = THIS_MODULE,
    .open = fh_pwm_proc_open,
    .read = seq_read,
    .write  = fh_pwm_proc_write,
};


static int __devinit fh_pwm_probe(struct platform_device *pdev)
{
	struct resource *res;
	int err;

	fh_pwm_ctrl.pwm[0].id = 0;
	fh_pwm_ctrl.pwm[1].id = 1;
	fh_pwm_ctrl.pwm[2].id = 2;

	fh_pwm_ctrl.pwm[0].working = 0;
	fh_pwm_ctrl.pwm[1].working = 0;
	fh_pwm_ctrl.pwm[2].working = 0;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL)
	{
		err = -ENXIO;
		goto fail_no_mem_resource;
	}

	res = request_mem_region(res->start, resource_size(res), pdev->name);
	if (res == NULL)
	{
		err = -EBUSY;
		goto fail_no_mem_resource;
	}

	fh_pwm_ctrl.base = ioremap(res->start, resource_size(res));
	if (fh_pwm_ctrl.base == NULL)
	{
		err = -ENXIO;
		goto fail_no_ioremap;
	}
#ifdef CONFIG_FH_PWM_DEBUG_FS
	err = sysfs_create_group(&pdev->dev.kobj, &fh_pwm_sysfs_files);
	if (err)
		goto fail_no_sysfs;
#endif

	fh_pwm_ctrl.clk = clk_get(&pdev->dev, "pwm_clk");

	if (IS_ERR(fh_pwm_ctrl.clk))
	{
		err = PTR_ERR(fh_pwm_ctrl.clk);
		goto fail_no_clk;
	}

	platform_set_drvdata(pdev, &fh_pwm_ctrl);

	/* disable pwm at startup. Avoids zero value. */
	writel(0x0, fh_pwm_ctrl.base + REG_PWM_CTRL);

	clk_enable(fh_pwm_ctrl.clk);

	err = misc_register(&fh_pwm_misc);

	if(err < 0)
	{
		pr_err("%s: ERROR: %s registration failed",
			   __func__, DEVICE_NAME);
		return -ENXIO;
	}

	pr_info("PWM driver: IO base addr: 0x%p\n", fh_pwm_ctrl.base);

	fh_pwm_ctrl.proc_file = create_proc_entry(FH_PWM_PROC_FILE, 0644, NULL);

    if (fh_pwm_ctrl.proc_file)
        fh_pwm_ctrl.proc_file->proc_fops = &fh_pwm_proc_ops;
    else
        pr_err("%s: ERROR: %s proc file create failed",
               __func__, DEVICE_NAME);

	return 0;

fail_no_clk:
#ifdef CONFIG_FH_PWM_DEBUG_FS
	sysfs_remove_group(&pdev->dev.kobj, &fh_pwm_sysfs_files);
fail_no_sysfs:
#endif
	iounmap(fh_pwm_ctrl.base);
fail_no_ioremap:
	release_mem_region(res->start, resource_size(res));
fail_no_mem_resource:
	return err;
}

static int __exit fh_pwm_remove(struct platform_device *pdev)
{
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	writel(0x0, fh_pwm_ctrl.base + REG_PWM_CTRL);
	clk_disable(fh_pwm_ctrl.clk);

#ifdef CONFIG_FH_PWM_DEBUG_FS
	sysfs_remove_group(&pdev->dev.kobj, &fh_pwm_sysfs_files);

#endif
	iounmap(fh_pwm_ctrl.base);
	release_mem_region(res->start, resource_size(res));
	platform_set_drvdata(pdev, NULL);
	misc_deregister(&fh_pwm_misc);
	return 0;
}

static struct platform_driver fh_pwm_driver =
{
	.driver	=
	{
		.name	= DEVICE_NAME,
		.owner	= THIS_MODULE,
	},
	.probe 		= fh_pwm_probe,
	.remove		= __exit_p(fh_pwm_remove),
};

static int __init fh_pwm_init(void)
{
	return platform_driver_register(&fh_pwm_driver);
}

static void __exit fh_pwm_exit(void)
{

	platform_driver_unregister(&fh_pwm_driver);

}

module_init(fh_pwm_init);
module_exit(fh_pwm_exit);


MODULE_AUTHOR("fullhan");

MODULE_DESCRIPTION("FH PWM driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV( MISC_DYNAMIC_MINOR	);
