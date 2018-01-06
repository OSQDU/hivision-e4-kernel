/*
 * Copyright 2010-2011 Picochip Ltd., Jamie Iles
 * http://www.picochip.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * This file implements a driver for the Synopsys DesignWare watchdog device
 * in the many ARM subsystems. The watchdog has 16 different timeout periods
 * and these are a function of the input clock frequency.
 *
 * The DesignWare watchdog cannot be stopped once it has been started so we
 * use a software timer to implement a ping that will keep the watchdog alive.
 * If we receive an expected close for the watchdog then we keep the timer
 * running, otherwise the timer is stopped and the watchdog will expire.
 */
#define pr_fmt(fmt) "fh_wdt: " fmt
#include <linux/irqreturn.h>
#include <linux/bitops.h>
#include <linux/clk.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/file.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/pm.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/uaccess.h>
#include <linux/watchdog.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include "fh_audio_misc.h"
#include <linux/i2c.h>

static struct {
	spinlock_t		lock;
	void __iomem		*regs;
	struct clk		*clk;
	unsigned long		in_use;
	unsigned long		next_heartbeat;
	struct timer_list	timer;
	int			expect_close;
} fh_i2s;

static const struct watchdog_info fh_wdt_ident = {
	.options	= WDIOF_KEEPALIVEPING | WDIOF_SETTIMEOUT |
			  WDIOF_MAGICCLOSE,
	.identity	= "Synopsys DesignWare Watchdog",
};
#define fh_I2S
#define	I2S_IOCTL_BASE	'W'
#define	I2S_GETSUPPORT	_IOR(I2S_IOCTL_BASE, 3, int)
typedef unsigned short UINT16;
typedef unsigned int    uint32;

#define	OFFSET_I2S_IER                      0x0000
#define	OFFSET_I2S_IRER                     0x0004
#define	OFFSET_I2S_ITER                     0x0008
#define	OFFSET_I2S_CER                      0x000c
#define	OFFSET_I2S_CCR                      0x0010
#define	OFFSET_I2S_RXFFR                    0x0014
#define	OFFSET_I2S_TXFFR                    0x0018
#define	OFFSET_I2S_LRBR0                    0x0020
#define	OFFSET_I2S_LRBR1                    0x0060
#define	OFFSET_I2S_LRBR2                    0x00A0
#define	OFFSET_I2S_LRBR3                    0x00E0
#define	OFFSET_I2S_LTHR0                    0x0020
#define	OFFSET_I2S_LTHR1                    0x0060
#define	OFFSET_I2S_LTHR2                    0x00A0
#define OFFSET_I2S_LTHR3                    0x00E0
#define	OFFSET_I2S_RRBR0                    0x0024
#define	OFFSET_I2S_RRBR1                    0x0064
#define	OFFSET_I2S_RRBR2                    0x00A4
#define	OFFSET_I2S_RRBR3                    0x00E4
#define	OFFSET_I2S_RTHR0                    0x0024
#define	OFFSET_I2S_RTHR1                    0x0064
#define	OFFSET_I2S_RTHR2                    0x00A4
#define	OFFSET_I2S_RTHR3                    0x00E4
#define	OFFSET_I2S_RER0                     0x0028
#define	OFFSET_I2S_RER1                     0x0068
#define OFFSET_I2S_RER2                     0x00A8
#define OFFSET_I2S_RER3                     0x00E8
#define	OFFSET_I2S_TER0                     0x002C
#define	OFFSET_I2S_TER1                     0x006C
#define	OFFSET_I2S_TER2                     0x00AC
#define	OFFSET_I2S_TER3                     0x00EC
#define	OFFSET_I2S_RCR0                     0x0030
#define	OFFSET_I2S_RCR1                     0x0070
#define	OFFSET_I2S_RCR2                     0x00B0
#define	OFFSET_I2S_RCR3                     0x00F0
#define	OFFSET_I2S_TCR0                     0x0034
#define	OFFSET_I2S_TCR1                     0x0074
#define	OFFSET_I2S_TCR2                     0x00B4
#define	OFFSET_I2S_TCR3                     0x00F4
#define	OFFSET_I2S_ISR0                     0x0038
#define	OFFSET_I2S_ISR1                     0x0078
#define	OFFSET_I2S_ISR2                     0x00B8
#define	OFFSET_I2S_ISR3                     0x00F8
#define	OFFSET_I2S_IMR0                     0x003C
#define	OFFSET_I2S_IMR1                     0x007C
#define	OFFSET_I2S_IMR2                     0x00BC
#define	OFFSET_I2S_IMR3                     0x00FC
#define	OFFSET_I2S_ROR0                     0x0040
#define OFFSET_I2S_ROR1                     0x0080
#define	OFFSET_I2S_ROR2                     0x00C0
#define	OFFSET_I2S_ROR3                     0x0100
#define	OFFSET_I2S_TOR0                     0x0044
#define	OFFSET_I2S_TOR1                     0x0084
#define	OFFSET_I2S_TOR2                     0x00C4
#define	OFFSET_I2S_TOR3                     0x0104
#define	OFFSET_I2S_RFCR0                    0x0048
#define	OFFSET_I2S_RFCR1                    0x0088
#define	OFFSET_I2S_RFCR2                    0x00C8
#define	OFFSET_I2S_RFCR3                    0x0108
#define OFFSET_I2S_TFCR0                    0x004C
#define OFFSET_I2S_TFCR1                    0x008C
#define	OFFSET_I2S_TFCR2                    0x00CC
#define	OFFSET_I2S_TFCR3                    0x010C
#define	OFFSET_I2S_RFF0                     0x0050
#define	OFFSET_I2S_RFF1                     0x0090
#define	OFFSET_I2S_RFF2                     0x00D4
#define	OFFSET_I2S_RFF3                     0x0110
#define	OFFSET_I2S_TFF0                     0x0054
#define OFFSET_I2S_TFF1                     0x0094
#define OFFSET_I2S_TFF2                     0x00D4
#define	OFFSET_I2S_TFF3                     0x0114
#define	OFFSET_I2S_RXDMA                    0x01C0
#define	OFFSET_I2S_RRXDMA                   0x01C4
#define	OFFSET_I2S_TXDMA                    0x01C8
#define	OFFSET_I2S_RTXDMA                   0x01CC
#define	OFFSET_I2S_COMP_PARAM_2             0x01f0
#define	OFFSET_I2S_COMP_PARAM_1             0x01f4
#define	OFFSET_I2S_COMP_VERSION             0x01f8
#define	OFFSET_I2S_COMP_TYPE                0x01fc
#define RESOLUTION12
#define write_reg(addr,reg)  (*((volatile uint32 *)(addr)))=(uint32)(reg)
#define read_reg(addr)  (*((volatile uint32 *)(addr)))



int open_count =0;
static int fh_set_rate(int rate){
return 0;
}
static int fh_set_channel(int channels){
	return 0;
}
static int fh_set_bit(int bit){
	return 0;
}

static long fh_i2s_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{


struct fh_audio_cfg *data;
data = filp->private_data;
	switch (cmd) {

	case FH_AUDIO_WRITE_RATE:
		data->rate = (int ) arg;
		fh_set_rate(data->rate);
		break;
	case FH_AUDIO_WRITE_CHANNELS:
		data->channels = (int ) arg;
		fh_set_channel(data->channels);
		break;
	case FH_AUDIO_WRITE_BIT:
		data->frame_bit = (int ) arg;
		fh_set_bit(data->frame_bit);
		break;

	default:
		return -ENOTTY;
	}
	return 0;
}
static inline long bytes_to_frames(int frame_bit, int bytes)
{
	return bytes * 8 /frame_bit;
}
static inline long  frames_to_bytes(int frame_bit, int frames)
{
	return frames * frame_bit / 8;
}
int avail_data_len(enum audio_type type,struct fh_audio_cfg *stream)
{
	int delta;
	if (capture == type) {
		delta = stream->capture.hw_ptr - stream->capture.appl_ptr;
		if (delta < 0) {
			delta += stream->capture.size;
		}
		return delta;
	} else {
		delta = stream->playback.appl_ptr - stream->playback.hw_ptr;
		if (delta < 0) {
			delta += stream->playback.size;
		}
		return stream->playback.size - delta;
	}
}

enum audio_state check_audio_state(enum audio_type type, struct fh_audio_cfg *stream)
{
#if 0
	if (capture == type) {
		if (stream->capture.appl_ptr >= stream->capture.hw_ptr) {
			printk("capture xrun\n");
			return xrun;
		}else {
			return normal;
		}
	} else {
		if (stream->playback.hw_ptr >= stream->playback.appl_ptr) {
					printk("capture xrun\n");
					return xrun;
				}else {
					return normal;
				}
	}
#endif
	return normal;
}

/*
 *1.   I2S---->FIFO
 *2.  FIFO---->I2S
 *
 */
static int update_hw_ptr(enum audio_type type,struct fh_audio_cfg *stream,u8* data,int len)
{
	int after,left;
	if (!stream){
		printk("%s %d stream not init\n",__FUNCTION__,__LINE__);
		return -EINVAL;
	}
	if(capture == type){
		after = len+stream->capture.hw_ptr;
		if(after  > stream->capture.size){
			left = len - (stream->capture.size-stream->capture.hw_ptr);
			memcpy(stream->capture.buffer_base+stream->capture.hw_ptr,data,stream->capture.size-stream->capture.hw_ptr);
			memcpy(stream->capture.buffer_base,data+stream->capture.size-stream->capture.hw_ptr,left);
			stream->capture.hw_ptr = left;
		}else {
			memcpy(stream->capture.buffer_base+stream->capture.hw_ptr,data,len);
			stream->capture.hw_ptr += len;
		}

	} else {
				after = len+stream->playback.hw_ptr;
				if(after  > stream->playback.size){
					left = len - (stream->playback.size-stream->playback.hw_ptr);
					memcpy(data,stream->playback.buffer_base+stream->playback.hw_ptr,stream->playback.size-stream->playback.hw_ptr);
					memcpy(data+stream->playback.size-stream->playback.hw_ptr,stream->playback.buffer_base,left);
					stream->playback.hw_ptr = left;
				}else {
					memcpy(data,stream->playback.buffer_base+stream->playback.hw_ptr,len);
					stream->playback.hw_ptr += len;
				}
	}
	return 0;

}


static void capture_work_func(struct fh_audio_cfg *stream)
{
	uint32 count_data,i,audio_data;
	uint32 buff[I2S_FIFO_LEN_RX];

	for(count_data=0;count_data<I2S_FIFO_LEN_RX;count_data++) {
			audio_data = read_reg(OFFSET_I2S_RRBR0 + fh_i2s.regs);
			i = read_reg(OFFSET_I2S_LRBR0 + fh_i2s.regs);
			buff[count_data] =audio_data ;
	}
	 update_hw_ptr(capture,stream,(u8 *)buff,count_data<<2);
	 if (waitqueue_active(&stream->readqueue)) {
		 int avail = avail_data_len(capture,stream);
		if (avail > frames_to_bytes(stream->frame_bit,stream->period_size)) {
			wake_up_interruptible(&stream->readqueue);
		}
	 }
}
static void playback_work_func(struct fh_audio_cfg *stream)
{
	uint32 count_data;
	uint32 buff[I2S_FIFO_LEN_TX];
	update_hw_ptr(playback,stream,(u8 *)buff,I2S_FIFO_LEN_TX<<2);

	for(count_data=0;count_data<I2S_FIFO_LEN_TX;count_data++) {
			write_reg(OFFSET_I2S_RTHR0 + fh_i2s.regs,buff[count_data]);
			write_reg(OFFSET_I2S_LTHR0 + fh_i2s.regs,buff[count_data]);
	}
}
static irqreturn_t davinci_i2s_irq(int irq, void *data) {
	uint32 irq_data,temp;
	struct fh_audio_cfg *stream;
	stream = (struct fh_audio_cfg *)data;
	irq_data = read_reg(OFFSET_I2S_ISR0 + fh_i2s.regs );
	temp = read_reg(OFFSET_I2S_IMR0 + fh_i2s.regs) ;
	if ( (irq_data & 0x10)&&( !(temp&(1<<4))) ) {
		playback_work_func(stream);
	}
	if ( (irq_data & 0x01)&&( !(temp&(1<<0))) ) {
		capture_work_func(stream);
	}
		return IRQ_HANDLED;
	}
struct i2c_adapter *codec_i2c_adapter;
void set_i2c_codec_adapter(struct i2c_adapter * adapter) {
	codec_i2c_adapter = adapter;
}
EXPORT_SYMBOL(set_i2c_codec_adapter);
int i2c_write_codec(u8 addr, u8 data) {
	int rval;
	struct i2c_msg msgs[1];
	u8 send[2];
	msgs[0].len = 2;
	msgs[0].addr = 0x1a;
	msgs[0].flags = 0;
	msgs[0].buf = send;
	send[0] = addr;
	send[1] = data;
	rval = i2c_transfer(codec_i2c_adapter, msgs, 1);
	return rval;
}
static void codec_config(void)
{
	    i2c_write_codec(0x0, 0xff);//set 8K sample
		i2c_write_codec(0x9, 0x2);
		i2c_write_codec(0x4, 0x10);
		i2c_write_codec(0x1, 0x3c);
		i2c_write_codec(0x5, 0x5);
		i2c_write_codec(0x7, 0xe6);
		i2c_write_codec(0x2, 0x14);
		i2c_write_codec(0x8, 0x38);
		i2c_write_codec(0xf, 0x1b);
		i2c_write_codec(0x10, 0x1b);
}

static int fh_audio_open(struct inode *ip, struct file *fp){
struct fh_i2s_dev irq_handle;
struct fh_audio_cfg *data;
int period_bytes,buffer_bytes;
u8 * ptr;
int rc;
codec_config();
spin_lock(&fh_i2s.lock);
if(open_count){
	spin_unlock(&fh_i2s.lock);
	return -EBUSY;
}
open_count++;

spin_unlock(&fh_i2s.lock);
data = kzalloc(sizeof(*data), GFP_KERNEL);
if (!data)
	return -ENOMEM;
fp->private_data=data;
data->frame_bit=16;
data->buffer_size=1024*8;
data->period_size=1024;
data->rate=8000;
data->capture.hw_ptr =0;
data->capture.appl_ptr =0;
data->playback.appl_ptr=0;
data->playback.hw_ptr=0;
data->capture.state = stopping;
data->playback.state = stopping;
data->capture.mmap_addr = NULL;
data->playback.mmap_addr = NULL;

	    write_reg(  OFFSET_I2S_CCR +   fh_i2s.regs  , 0x8  );
	    write_reg(  OFFSET_I2S_TCR0  +   fh_i2s.regs   , 0x4 );
	    write_reg( OFFSET_I2S_RCR0 +   fh_i2s.regs    , 0x4  );
	    write_reg(  OFFSET_I2S_TFCR0  +   fh_i2s.regs  , I2S_FIFO_LEN_TX  );
	    write_reg(  OFFSET_I2S_RFCR0  +   fh_i2s.regs  , I2S_FIFO_LEN_RX );
	    write_reg(  OFFSET_I2S_ITER +   fh_i2s.regs    , 0x01  );
	    write_reg(  OFFSET_I2S_IRER  +   fh_i2s.regs   , 0x01  );
	    write_reg(  OFFSET_I2S_CER  +   fh_i2s.regs    , 0x01  );
	    write_reg(  OFFSET_I2S_IER  +   fh_i2s.regs    , 0x01  );

buffer_bytes = frames_to_bytes(data->frame_bit,data->buffer_size);
period_bytes = frames_to_bytes(data->frame_bit,data->period_size);
ptr  = (unsigned char *)kzalloc(buffer_bytes, GFP_KERNEL);
if (!ptr) {
	printk("no enough mem for capture or playback buffer alloc\n");
	return -ENOMEM;
}
data->capture.buffer_base = ptr;
data->capture.size = buffer_bytes;
ptr  = (unsigned char *)kzalloc(period_bytes, GFP_KERNEL);
if (!ptr) {
	return -ENOMEM;
}
data->capture.rw_buff = ptr;

ptr  = (unsigned char *)kzalloc(buffer_bytes, GFP_KERNEL);
if (!ptr) {
	return -ENOMEM;
}
data->playback.buffer_base = ptr;
data->playback.size = buffer_bytes;
ptr  = (unsigned char *)kzalloc(period_bytes, GFP_KERNEL);
if (!ptr) {
	return -ENOMEM;
}
data->playback.rw_buff = ptr;
data->start_threshold = buffer_bytes;
data->stop_threshold = buffer_bytes;
init_waitqueue_head(&data->readqueue);
init_waitqueue_head(&data->writequeue);

irq_handle.irq = 32;
strcpy(irq_handle.name,"i2s irq");
rc = request_irq(irq_handle.irq, davinci_i2s_irq,
IRQF_DISABLED, irq_handle.name, data);
if (rc) {
	free_irq(irq_handle.irq,data);
}

return 0;

}
static int fh_audio_read(struct file *fp, char __user *buf,
		size_t len, loff_t *off){
	struct fh_audio_cfg *stream = fp->private_data;
	int isr_status;
	int after,left,avail,user_status;
	if (stream->capture.state != running) {
		stream->capture.state = running;
		isr_status = read_reg(OFFSET_I2S_IMR0 +   fh_i2s.regs);
		isr_status&=~(1<<0);
	    write_reg(  OFFSET_I2S_IMR0 +   fh_i2s.regs    , isr_status  );
	}
	 avail = avail_data_len(capture,stream);
	if (avail > len) {
		avail = len;
	}
	after = avail+stream->capture.appl_ptr;
	if(after  > stream->capture.size){
		left = avail - (stream->capture.size-stream->capture.appl_ptr);
		user_status = copy_to_user(buf,stream->capture.buffer_base+stream->capture.appl_ptr,stream->capture.size-stream->capture.appl_ptr);
		user_status = copy_to_user(buf+stream->capture.size-stream->capture.appl_ptr,stream->capture.buffer_base,left);
		stream->capture.appl_ptr = left;
	}else {
		user_status=copy_to_user(buf,stream->capture.buffer_base+stream->capture.appl_ptr,avail);
		stream->capture.appl_ptr += avail;
	}
	if(user_status != 0)
		return -ENOMEM;
	return avail;

}
static int fh_audio_write(struct file *fp, const char __user *buf,
		 size_t len, loff_t *off){
	struct fh_audio_cfg *stream = fp->private_data;
	int isr_status;
	int after,left,user_status;
	int avail = avail_data_len(playback,stream);
	if (0 == avail) {
		return 0;
	}
		if (avail > len) {
			avail = len;
		}
		after = avail+stream->playback.appl_ptr;
		if(after  > stream->playback.size){
			left = avail - (stream->playback.size-stream->playback.appl_ptr);
			user_status=copy_from_user(stream->playback.buffer_base+stream->playback.appl_ptr,buf,stream->playback.size-stream->playback.appl_ptr);
			user_status=copy_from_user(stream->playback.buffer_base,buf+stream->playback.size-stream->playback.appl_ptr,left);
			stream->playback.appl_ptr = left;
		}else {
			user_status=copy_from_user(stream->playback.buffer_base+stream->playback.appl_ptr,buf,avail);
			stream->playback.appl_ptr += avail;
		}
		if ((stream->playback.state != running)&&(stream->playback.appl_ptr > frames_to_bytes(stream->frame_bit,stream->buffer_size)-frames_to_bytes(stream->frame_bit,stream->period_size))) {
					/*avail ==0 means santisfy start_threshold*/
					stream->playback.state = running;
					isr_status = read_reg(OFFSET_I2S_IMR0 +   fh_i2s.regs);
					isr_status&=~(1<<4);
				    write_reg(  OFFSET_I2S_IMR0 +   fh_i2s.regs    , isr_status  );
				}
		if(user_status != 0)
			return -ENOMEM;
		return avail;

}


static int fh_audio_close(struct inode *ip, struct file *fp){
	struct fh_audio_cfg *data;
	data = fp->private_data;
	spin_lock(&fh_i2s.lock);
	open_count--;
	spin_unlock(&fh_i2s.lock);
	 write_reg(  OFFSET_I2S_IMR0 +   fh_i2s.regs    , 0x33  );
	 free_irq(32, data);
	data->capture.state = stopping;
	data->playback.state = stopping;
	kfree(data->capture.buffer_base);
	kfree(data->capture.rw_buff);
	kfree(data->playback.buffer_base);
	kfree(data->playback.rw_buff);
	kfree(data);
	return 0;
}
static const struct file_operations wdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.open		= fh_audio_open,
	.read      =fh_audio_read,
	.write		= fh_audio_write,
	.unlocked_ioctl	= fh_i2s_ioctl,
   .release =fh_audio_close,


};

static struct miscdevice fh_i2s_miscdev = {
	.fops		= &wdt_fops,
	.name		= "fh_i2s",
	.minor		= WATCHDOG_MINOR,
};



static int __devinit fh_i2s_drv_probe(struct platform_device *pdev)
{
	int ret;

	struct resource *mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (!mem)
		return -EINVAL;

	if (!devm_request_mem_region(&pdev->dev, mem->start, resource_size(mem),
				     "fh_i2s"))
		return -ENOMEM;

	fh_i2s.regs = devm_ioremap(&pdev->dev, mem->start, resource_size(mem));

	if (!fh_i2s.regs)
		return -ENOMEM;



	spin_lock_init(&fh_i2s.lock);

	ret = misc_register(&fh_i2s_miscdev);
	if (ret)
		goto out_disable_clk;

	return 0;

out_disable_clk:
	clk_disable(fh_i2s.clk);


	return ret;
}

static int __devexit fh_wdt_drv_remove(struct platform_device *pdev)
{
	misc_deregister(&fh_i2s_miscdev);

	return 0;
}

static struct platform_driver fh_i2s_driver = {
	.probe		= fh_i2s_drv_probe,
	.remove		= __devexit_p(fh_wdt_drv_remove),
	.driver		= {
		.name	= "fh_i2s",
		.owner	= THIS_MODULE,

	},
};

static int __init fh_i2s_init(void)
{
	return platform_driver_register(&fh_i2s_driver);
}
module_init(fh_i2s_init);

static void __exit fh_i2s_exit(void)
{
	platform_driver_unregister(&fh_i2s_driver);
}
module_exit(fh_i2s_exit);

MODULE_AUTHOR("Jamie Iles");
MODULE_DESCRIPTION("Synopsys DesignWare Watchdog Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
