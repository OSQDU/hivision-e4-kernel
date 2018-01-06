/** @file  mlanhostcmd.h
  *
  * @brief This file contains command structures for mlanconfig application
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

/** MLAN MAC Address Length */
#define MLAN_MAC_ADDR_LENGTH     (6)
/** Size of command buffer */
#define MRVDRV_SIZE_OF_CMD_BUFFER		(2 * 1024)

/** Command RET code, MSB is set to 1 */
#define HostCmd_RET_BIT					0x8000
/** General purpose action : Get */
#define HostCmd_ACT_GEN_GET				0x0000
/** General purpose action : Set */
#define HostCmd_ACT_GEN_SET				0x0001
/** General purpose action : Clear */
#define HostCmd_ACT_GEN_CLEAR				0x0004
/** General purpose action : Remove */
#define HostCmd_ACT_GEN_REMOVE          0x0004

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
#define TLV_TYPE_AUTO_TX            (PROPRIETARY_TLV_BASE_ID + 0x18)	// 0x0118

/** TLV type : Channel TRPC */
#define TLV_TYPE_CHAN_TRPC              (PROPRIETARY_TLV_BASE_ID + 0x89)	// 0x0189

/* Define general hostcmd data structure */
/** HostCmd_DS_GEN */
typedef struct MAPP_HostCmd_DS_GEN {
    /** Command */
	t_u16 command;
    /** Size */
	t_u16 size;
    /** Sequence number */
	t_u16 seq_num;
    /** Result */
	t_u16 result;
} __ATTRIB_PACK__ HostCmd_DS_GEN;

typedef struct _HostCmd_DS_MEF_CFG {
    /** Criteria */
	t_u32 Criteria;
    /** Number of entries */
	t_u16 NumEntries;
} __ATTRIB_PACK__ HostCmd_DS_MEF_CFG;

typedef struct _MEF_CFG_DATA {
    /** Size */
	t_u16 size;
    /** Data */
	HostCmd_DS_MEF_CFG data;
} __ATTRIB_PACK__ MEF_CFG_DATA;

/** Size of HostCmd_DS_GEN */
#define S_DS_GEN    sizeof(HostCmd_DS_GEN)

/** HostCmd_DS_REG */
typedef struct MAPP_HostCmd_DS_REG {
    /** Read or write */
	t_u16 action;
    /** Register offset */
	t_u16 offset;
    /** Value */
	t_u32 value;
} __ATTRIB_PACK__ HostCmd_DS_REG;

/** HostCmd_DS_MEM */
typedef struct MAPP_HostCmd_DS_MEM {
    /** Read or write */
	t_u16 action;
    /** Reserved */
	t_u16 reserved;
    /** Address */
	t_u32 addr;
    /** Value */
	t_u32 value;
} __ATTRIB_PACK__ HostCmd_DS_MEM;

/** HostCmd_DS_802_11_CFG_DATA */
typedef struct MAPP_HostCmd_DS_802_11_CFG_DATA {
    /** Action */
	t_u16 action;
    /** Type */
	t_u16 type;
    /** Data length */
	t_u16 data_len;
    /** Data */
	t_u8 data[1];
} __ATTRIB_PACK__ HostCmd_DS_802_11_CFG_DATA;

/** MrvlIEtypesHeader_t */
typedef struct MrvlIEtypesHeader {
    /** Header type */
	t_u16 type;
    /** Header length */
	t_u16 len;
} __ATTRIB_PACK__ MrvlIEtypesHeader_t;

/** MrvlIEtypes_Data_t */
typedef struct MrvlIEtypes_Data_t {
    /** Header */
	MrvlIEtypesHeader_t header;
    /** Data */
	t_u8 data[1];
} __ATTRIB_PACK__ MrvlIEtypes_Data_t;

/** HostCmd_DS_802_11_CRYPTO */
typedef struct MAPP_HostCmd_DS_802_11_CRYPTO {
	t_u16 encdec;	  /**< Decrypt=0, Encrypt=1 */
	t_u16 algorithm;  /**< RC4=1 AES=2 , AES_KEY_WRAP=3 */
	t_u16 key_IV_length;/**< Length of Key IV (bytes)   */
	t_u8 keyIV[32];	  /**< Key IV */
	t_u16 key_length;  /**< Length of Key (bytes) */
	t_u8 key[32];	  /**< Key */
	MrvlIEtypes_Data_t data;   /**< Plain text if encdec=Encrypt, Ciphertext data if encdec=Decrypt*/
} __ATTRIB_PACK__ HostCmd_DS_802_11_CRYPTO;

/** HostCmd_DS_802_11_CRYPTO_AES_CCM */
typedef struct MAPP_HostCmd_DS_802_11_CRYPTO_AES_CCM {
	t_u16 encdec;	  /**< Decrypt=0, Encrypt=1 */
	t_u16 algorithm;  /**< AES_CCM=4 */
	t_u16 key_length;  /**< Length of Key (bytes)  */
	t_u8 key[32];	  /**< Key  */
	t_u16 nonce_length;/**< Length of Nonce (bytes) */
	t_u8 nonce[14];	  /**< Nonce */
	t_u16 AAD_length;  /**< Length of AAD (bytes) */
	t_u8 AAD[32];	  /**< AAD */
	MrvlIEtypes_Data_t data;   /**< Plain text if encdec=Encrypt, Ciphertext data if encdec=Decrypt*/
} __ATTRIB_PACK__ HostCmd_DS_802_11_CRYPTO_AES_CCM;

/** AES CCM cipher test */
#define CIPHER_TEST_AES_CCM (4)
/** AutoTx_MacFrame_t */
typedef struct AutoTx_MacFrame {
	t_u16 interval;	      /**< in seconds */
	t_u8 priority;	      /**< User Priority: 0~7, ignored if non-WMM */
	t_u8 reserved;	      /**< set to 0 */
	t_u16 frame_len;       /**< Length of MAC frame payload */
	t_u8 dest_mac_addr[MLAN_MAC_ADDR_LENGTH];	/**< Destination MAC address */
	t_u8 src_mac_addr[MLAN_MAC_ADDR_LENGTH];	/**< Source MAC address */
	t_u8 payload[];			  /**< Payload */
} __ATTRIB_PACK__ AutoTx_MacFrame_t;

/** MrvlIEtypes_AutoTx_t */
typedef struct MrvlIEtypes_AutoTx {
	MrvlIEtypesHeader_t header;	    /**< Header */
	AutoTx_MacFrame_t auto_tx_mac_frame;  /**< Auto Tx MAC frame */
} __ATTRIB_PACK__ MrvlIEtypes_AutoTx_t;

/** HostCmd_DS_802_11_AUTO_TX */
typedef struct MAPP_HostCmd_DS_802_11_AUTO_TX {
    /** Action */
	t_u16 action;		/* 0 = ACT_GET; 1 = ACT_SET; */
	MrvlIEtypes_AutoTx_t auto_tx;	 /**< Auto Tx */
} __ATTRIB_PACK__ HostCmd_DS_802_11_AUTO_TX;

/* String helper functions */
/** Convert char to hex integer */
int hexval(t_s32 chr);
/** Convert char to hex integer */
t_u8 hexc2bin(t_s8 chr);
/** Convert string to hex integer */
t_u32 a2hex(t_s8 *s);
/** Check the Hex String */
int ishexstring(t_s8 *s);
/** Convert String to integer */
t_u32 a2hex_or_atoi(t_s8 *value);
/** Convert String to Integer */
int atoval(t_s8 *buf);
/** Hump hex data */
void hexdump(t_s8 *prompt, void *p, t_s32 len, t_s8 delim);
/** Convert String to Hex */
t_s8 *convert2hex(t_s8 *ptr, t_u8 *chr);

int process_host_cmd_resp(t_u8 *buf);
int prepare_host_cmd_buffer(FILE * fp, char *cmd_name, t_u8 *buf);
int prepare_arp_filter_buffer(FILE * fp, t_u8 *buf, t_u16 *length);
int prepare_cfg_data_buffer(int argc, char *argv[], FILE * fp, t_u8 *buf);
int prepare_hostcmd_regrdwr(t_u32 type, t_u32 offset, t_u32 *value, t_u8 *buf);

#endif /* _MLANHOSTCMD_H_ */
