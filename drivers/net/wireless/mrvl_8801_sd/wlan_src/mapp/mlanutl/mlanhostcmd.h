/** @file  mlanhostcmd.h
  *
  * @brief This file contains command structures for mlanutl application
  *
  * (C) Copyright 2008-2016 Marvell International Ltd. All Rights Reserved
  *
  * MARVELL CONFIDENTIAL
  * The source code contained or described herein and all documents related to
  * the source code ("Material") are owned by Marvell International Ltd or its
  * suppliers or licensors. Title to the Material remains with Marvell International Ltd
  * or its suppliers and licensors. The Material contains trade secrets and
  * proprietary and confidential information of Marvell or its suppliers and
  * licensors. The Material is protected by worldwide copyright and trade secret
  * laws and treaty provisions. No part of the Material may be used, copied,
  * reproduced, modified, published, uploaded, posted, transmitted, distributed,
  * or disclosed in any way without Marvell's prior express written permission.
  *
  * No license under any patent, copyright, trade secret or other intellectual
  * property right is granted to or conferred upon you by disclosure or delivery
  * of the Materials, either expressly, by implication, inducement, estoppel or
  * otherwise. Any license under such intellectual property rights must be
  * express and approved by Marvell in writing.
  *
  */
/************************************************************************
Change log:
     11/26/2008: initial version
************************************************************************/
#ifndef _MLANHOSTCMD_H_
#define _MLANHOSTCMD_H_

/** Find number of elements */
#define NELEMENTS(x) (sizeof(x)/sizeof(x[0]))

/** Size of command buffer */
#define MRVDRV_SIZE_OF_CMD_BUFFER		(2 * 1024)

/** Host Command ID : Memory access */
#define HostCmd_CMD_MEM_ACCESS                0x0086

/** Pre-Authenticate - 11r only */
#define HostCmd_CMD_802_11_AUTHENTICATE       0x0011

/** Read/Write Mac register */
#define HostCmd_CMD_MAC_REG_ACCESS            0x0019
/** Read/Write BBP register */
#define HostCmd_CMD_BBP_REG_ACCESS            0x001a
/** Read/Write RF register */
#define HostCmd_CMD_RF_REG_ACCESS             0x001b
/** Get TX Power data */
#define HostCmd_CMD_802_11_RF_TX_POWER        0x001e
/** Get the current TSF */
#define HostCmd_CMD_GET_TSF                   0x0080
/** Host Command ID : CAU register access */
#define HostCmd_CMD_CAU_REG_ACCESS            0x00ed

/** Host Command ID : 802.11 BG scan configuration */
#define HostCmd_CMD_802_11_BG_SCAN_CONFIG     0x006b
/** Host Command ID : Configuration data */
#define HostCmd_CMD_CFG_DATA                  0x008f
/** Host Command ID : 802.11 crypto */
#define HostCmd_CMD_802_11_CRYPTO             0x0078
/** Host Command ID : 802.11 auto Tx */
#define HostCmd_CMD_802_11_AUTO_TX      	0x0082

/** Host Command ID : Channel TRPC config */
#define HostCmd_CMD_CHAN_TRPC_CONFIG                0x00fb

/** TLV  type ID definition */
#define PROPRIETARY_TLV_BASE_ID     0x0100
/** TLV type : Auto Tx */
#define TLV_TYPE_AUTO_TX            (PROPRIETARY_TLV_BASE_ID + 0x18)	/* 0x0118
									 */

/** TLV type : Channel TRPC */
#define TLV_TYPE_CHAN_TRPC              (PROPRIETARY_TLV_BASE_ID + 0x89)	/* 0x0189
										 */

/* Define general hostcmd data structure */

/** Convert String to integer */
t_u32 a2hex_or_atoi(char *value);
char *mlan_config_get_line(FILE * fp, char *str, t_s32 size, int *lineno);

int prepare_host_cmd_buffer(FILE * fp, char *cmd_name, t_u8 *buf);
int prepare_hostcmd_regrdwr(t_u32 type, t_u32 offset, t_u32 *value, t_u8 *buf);

#endif /* _MLANHOSTCMD_H_ */
