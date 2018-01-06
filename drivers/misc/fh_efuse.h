/*
 * fh_efuse.h
 *
 *  Created on: Aug 9, 2016
 *      Author: duobao
 */

#ifndef FH_EFUSE_H_
#define FH_EFUSE_H_


#include <linux/io.h>
#include <linux/scatterlist.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/scatterlist.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/bug.h>
#include <linux/completion.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

/****************************************************************************
 * #define section
 *	add constant #define here if any
 ***************************************************************************/
//#define FH_EFUSE_PROC_FILE    "driver/efuse"



#define IOCTL_EFUSE_CHECK_PRO						0
#define IOCTL_EFUSE_WRITE_KEY						1
#define IOCTL_EFUSE_CHECK_LOCK						2
#define IOCTL_EFUSE_TRANS_KEY						3
#define IOCTL_EFUSE_SWITCH_CPU_KEY_MODE				4
#define IOCTL_EFUSE_SWITCH_EFUSE_KEY_MODE			5
#define IOCTL_EFUSE_CHECK_ERROR						6
#define IOCTL_EFUSE_READ_KEY						7
#define IOCTL_EFUSE_SET_LOCK						8

/****************************************************************************
 * ADT section
 *	add Abstract Data Type definition here
 ***************************************************************************/

struct efuse_status{
	//bit 1 means could write..0 not write
	u32 protect_bits[2];
	//bit 1 means cpu couldn't read efuse entry data...
	u32	efuse_apb_lock;
	u32 aes_ahb_lock;
	u32 error;


};

typedef struct{
	//write key
	u32 efuse_entry_no;	//from 0 ~ 31
	u8 *key_buff;
	u32 key_size;
	//trans key
	u32 trans_key_start_no; //from 0 ~ 7
	u32 trans_key_size;		//max 8


	//status
	struct efuse_status status;


	//u32 status;			//process status back to app.

}EFUSE_INFO;


struct wrap_efuse_obj {
	void *regs;
	//write key
	u32 efuse_entry_no;	//from 0 ~ 31
	u8 *key_buff;
	u32 key_size;
	//trans key
	u32 trans_key_start_no; //from 0 ~ 7
	u32 trans_key_size;		//max 8

	//status
	struct efuse_status status;

#define USE_CPU_SET_KEY				0
#define USE_EFUSE_SET_KEY			1
	u32 open_flag;

};
/****************************************************************************
 *  extern variable declaration section
 ***************************************************************************/

/****************************************************************************
 *  section
 *	add function prototype here if any
 ***************************************************************************/
void efuse_trans_key(struct wrap_efuse_obj *obj,u32 start_no,u32 size);
#endif /* FH_EFUSE_H_ */
