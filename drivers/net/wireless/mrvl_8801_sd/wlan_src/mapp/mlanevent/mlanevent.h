/** @file  mlanevent.h
 *
 *  @brief Header file for mlanevent application
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
 *  All Rights Reserved
 */
/************************************************************************
Change log:
    03/18/08: Initial creation
************************************************************************/

#ifndef _MLAN_EVENT_H
#define _MLAN_EVENT_H

/** Character, 1 byte */
typedef signed char t_s8;
/** Unsigned character, 1 byte */
typedef unsigned char t_u8;

/** Short integer */
typedef signed short t_s16;
/** Unsigned short integer */
typedef unsigned short t_u16;

/** Integer */
typedef signed int t_s32;
/** Unsigned integer */
typedef unsigned int t_u32;

/** Long long integer */
typedef long long t_s64;
/** Unsigned long long integer */
typedef unsigned long long t_u64;

#if (BYTE_ORDER == LITTLE_ENDIAN)
#undef BIG_ENDIAN_SUPPORT
#endif

/** 16 bits byte swap */
#define swap_byte_16(x) \
  ((t_u16)((((t_u16)(x) & 0x00ffU) << 8) | \
         (((t_u16)(x) & 0xff00U) >> 8)))

/** 32 bits byte swap */
#define swap_byte_32(x) \
  ((t_u32)((((t_u32)(x) & 0x000000ffUL) << 24) | \
         (((t_u32)(x) & 0x0000ff00UL) <<  8) | \
         (((t_u32)(x) & 0x00ff0000UL) >>  8) | \
         (((t_u32)(x) & 0xff000000UL) >> 24)))

/** 64 bits byte swap */
#define swap_byte_64(x) \
  ((t_u64)((t_u64)(((t_u64)(x) & 0x00000000000000ffULL) << 56) | \
         (t_u64)(((t_u64)(x) & 0x000000000000ff00ULL) << 40) | \
         (t_u64)(((t_u64)(x) & 0x0000000000ff0000ULL) << 24) | \
         (t_u64)(((t_u64)(x) & 0x00000000ff000000ULL) <<  8) | \
         (t_u64)(((t_u64)(x) & 0x000000ff00000000ULL) >>  8) | \
         (t_u64)(((t_u64)(x) & 0x0000ff0000000000ULL) >> 24) | \
         (t_u64)(((t_u64)(x) & 0x00ff000000000000ULL) >> 40) | \
         (t_u64)(((t_u64)(x) & 0xff00000000000000ULL) >> 56) ))

#ifdef BIG_ENDIAN_SUPPORT
/** Convert from 16 bit little endian format to CPU format */
#define uap_le16_to_cpu(x) swap_byte_16(x)
/** Convert from 32 bit little endian format to CPU format */
#define uap_le32_to_cpu(x) swap_byte_32(x)
/** Convert from 64 bit little endian format to CPU format */
#define uap_le64_to_cpu(x) swap_byte_64(x)
/** Convert to 16 bit little endian format from CPU format */
#define uap_cpu_to_le16(x) swap_byte_16(x)
/** Convert to 32 bit little endian format from CPU format */
#define uap_cpu_to_le32(x) swap_byte_32(x)
/** Convert to 64 bit little endian format from CPU format */
#define uap_cpu_to_le64(x) swap_byte_64(x)
#else /* BIG_ENDIAN_SUPPORT */
/** Do nothing */
#define uap_le16_to_cpu(x) x
/** Do nothing */
#define uap_le32_to_cpu(x) x
/** Do nothing */
#define uap_le64_to_cpu(x) x
/** Do nothing */
#define uap_cpu_to_le16(x) x
/** Do nothing */
#define uap_cpu_to_le32(x) x
/** Do nothing */
#define uap_cpu_to_le64(x) x
#endif /* BIG_ENDIAN_SUPPORT */

/** Convert WPS TLV header from network to host order */
#define endian_convert_tlv_wps_header_in(t,l)       \
    {                                               \
        (t) = ntohs(t);       \
        (l) = ntohs(l);       \
    }

/**
 * Hex or Decimal to Integer
 * @param   num string to convert into decimal or hex
 */
#define A2HEXDECIMAL(num)  \
    (strncasecmp("0x", (num), 2)?(unsigned int) strtoll((num),NULL,0):a2hex((num)))\

/**
 * Check of decimal or hex string
 * @param   num string
 */
#define IS_HEX_OR_DIGIT(num) \
    (strncasecmp("0x", (num), 2)?ISDIGIT((num)):ishexstring((num)))\

/** MLan Event application version string */
#define MLAN_EVENT_VERSION         "MlanEvent 2.0"

/** Failure */
#define MLAN_EVENT_FAILURE     -1

#ifdef __GNUC__
/** Structure packing begins */
#define PACK_START
/** Structure packeing end */
#define PACK_END  __attribute__ ((packed))
#else
/** Structure packing begins */
#define PACK_START   __packed
/** Structure packeing end */
#define PACK_END
#endif

#ifndef ETH_ALEN
/** MAC address length */
#define ETH_ALEN    6
#endif

/** Netlink protocol number */
#define NETLINK_MARVELL         (MAX_LINKS - 1)
/** Netlink maximum payload size */
#define NL_MAX_PAYLOAD          1024
/** Netlink multicast group number */
#define NL_MULTICAST_GROUP      1
/** Default wait time in seconds for events */
#define UAP_RECV_WAIT_DEFAULT   10
/** Maximum number of devices */
#define MAX_NO_OF_DEVICES       4

#ifndef NLMSG_HDRLEN
/** NL message header length */
#define NLMSG_HDRLEN            ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))
#endif

/** Event ID mask */
#define EVENT_ID_MASK                   0xffff

/** BSS number mask */
#define BSS_NUM_MASK                    0xf

/** Get BSS number from event cause (bit 23:16) */
#define EVENT_GET_BSS_NUM(event_cause)          \
    (((event_cause) >> 16) & BSS_NUM_MASK)

/** Invitation Flag mask */
#define INVITATION_FLAG_MASK            0x01

/* Event buffer */
typedef PACK_START struct _evt_buf {
    /** Flag to check if event data is present in the buffer or not  */
	int flag;
    /** Event length */
	int length;
    /** Event data */
	t_u8 buffer[NL_MAX_PAYLOAD];
} PACK_END evt_buf;

/** Event header */
typedef PACK_START struct _event_header {
    /** Event ID */
	t_u32 event_id;
    /** Event data */
	t_u8 event_data[0];
} PACK_END event_header;

/** Event ID length */
#define EVENT_ID_LEN    4

/** Event ID : WMM status change */
#define MICRO_AP_EV_WMM_STATUS_CHANGE       0x00000017

/** Event ID: STA deauth */
#define MICRO_AP_EV_ID_STA_DEAUTH           0x0000002c

/** Event ID: STA associated */
#define MICRO_AP_EV_ID_STA_ASSOC            0x0000002d

/** Event ID: BSS started */
#define MICRO_AP_EV_ID_BSS_START            0x0000002e

/** Event ID: Debug event */
#define MICRO_AP_EV_ID_DEBUG                0x00000036

/** Event ID: BSS idle event */
#define MICRO_AP_EV_BSS_IDLE                0x00000043

/** Event ID: BSS active event */
#define MICRO_AP_EV_BSS_ACTIVE              0x00000044

/** Event ID: WEP ICV error */
#define EVENT_WEP_ICV_ERROR                 0x00000046

/** Event ID: RSN Connect event */
#define MICRO_AP_EV_RSN_CONNECT             0x00000051

/** HS WAKE UP event id */
#define UAP_EVENT_ID_HS_WAKEUP             0x80000001
/** HS_ACTIVATED event id */
#define UAP_EVENT_ID_DRV_HS_ACTIVATED      0x80000002
/** HS DEACTIVATED event id */
#define UAP_EVENT_ID_DRV_HS_DEACTIVATED    0x80000003
/** HOST SLEEP AWAKE  event id in legacy PS*/
#define UAP_EVENT_HOST_SLEEP_AWAKE         0x00000012

/** HS WAKE UP event id */
#define UAP_EVENT_ID_DRV_MGMT_FRAME        0x80000005

/** SCAN REPORT Event id */
#define EVENT_ID_DRV_SCAN_REPORT           0x80000009

/** WPA IE Tag */
#define  IEEE_WPA_IE                 221
/** RSN IE Tag */
#define  IEEE_RSN_IE                 48

/** TLV ID : WAPI Information */
#define MRVL_WAPI_INFO_TLV_ID        0x0167

/** TLV ID : Management Frame */
#define MRVL_MGMT_FRAME_TLV_ID       0x0168

/** TLV Id : Channel Config */
#define MRVL_CHANNELCONFIG_TLV_ID    0x012a

/** Assoc Request */
#define SUBTYPE_ASSOC_REQUEST        0
/** Assoc Response */
#define SUBTYPE_ASSOC_RESPONSE       1
/** ReAssoc Request */
#define SUBTYPE_REASSOC_REQUEST      2
/** ReAssoc Response */
#define SUBTYPE_REASSOC_RESPONSE     3
/** WEP key user input length */
#define WEP_KEY_USER_INPUT           13

/** Band config ACS mode */
#define BAND_CONFIG_ACS_MODE    0x40
/** Band Config 2.4GHz */
#define BAND_CONFIG_2_4GHZ      0x00
/** Band config 5GHz */
#define BAND_CONFIG_5GHZ        0x01
/** secondary channel is below */
#define SECOND_CHANNEL_BELOW    0x30
/** secondary channel is above */
#define SECOND_CHANNEL_ABOVE    0x10

/** TLV buffer header*/
typedef PACK_START struct _tlvbuf_header {
    /** Header type */
	t_u16 type;
    /** Header length */
	t_u16 len;
} PACK_END tlvbuf_header;

/** Event body : STA deauth */
typedef PACK_START struct _eventbuf_sta_deauth {
    /** Deauthentication reason */
	t_u16 reason_code;
    /** MAC address of deauthenticated STA */
	t_u8 sta_mac_address[ETH_ALEN];
} PACK_END eventbuf_sta_deauth;

/** Event body : WEP ICV error */
typedef PACK_START struct _eventbuf_wep_icv_error {
    /** Deauthentication reason */
	t_u16 reason_code;
    /** MAC address of deauthenticated STA */
	t_u8 sta_mac_address[ETH_ALEN];
    /** WEP key index */
	t_u8 wep_key_index;
    /** WEP key length */
	t_u8 wep_key_length;
    /** WEP key */
	t_u8 wep_key[WEP_KEY_USER_INPUT];
} PACK_END eventbuf_wep_icv_error;

/** Event body : STA associated */
typedef PACK_START struct _eventbuf_sta_assoc {
    /** Reserved */
	t_u8 reserved[2];
    /** MAC address of associated STA */
	t_u8 sta_mac_address[ETH_ALEN];
    /** Assoc request/response buffer */
	t_u8 assoc_payload[0];
} PACK_END eventbuf_sta_assoc;

/** Event body : RSN Connect */
typedef PACK_START struct _eventbuf_rsn_connect {
    /** Reserved */
	t_u8 reserved[2];
    /** MAC address of Station */
	t_u8 sta_mac_address[ETH_ALEN];
    /** WPA/WPA2 TLV IEs */
	t_u8 tlv_list[0];
} PACK_END eventbuf_rsn_connect;

/** Event body : BSS started */
typedef PACK_START struct _eventbuf_bss_start {
    /** Reserved */
	t_u8 reserved[2];
    /** MAC address of BSS */
	t_u8 ap_mac_address[ETH_ALEN];
} PACK_END eventbuf_bss_start;

/**
 *                 IEEE 802.11 MAC Message Data Structures
 *
 * Each IEEE 802.11 MAC message includes a MAC header, a frame body (which
 * can be empty), and a frame check sequence field. This section gives the
 * structures that used for the MAC message headers and frame bodies that
 * can exist in the three types of MAC messages - 1) Control messages,
 * 2) Data messages, and 3) Management messages.
 */
#ifdef BIG_ENDIAN_SUPPORT
typedef PACK_START struct _IEEEtypes_FrameCtl_t {
    /** Order */
	t_u8 order:1;
    /** Wep */
	t_u8 wep:1;
    /** More Data */
	t_u8 more_data:1;
    /** Power Mgmt */
	t_u8 pwr_mgmt:1;
    /** Retry */
	t_u8 retry:1;
    /** More Frag */
	t_u8 more_frag:1;
    /** From DS */
	t_u8 from_ds:1;
    /** To DS */
	t_u8 to_ds:1;
    /** Sub Type */
	t_u8 sub_type:4;
    /** Type */
	t_u8 type:2;
    /** Protocol Version */
	t_u8 protocol_version:2;
} PACK_END IEEEtypes_FrameCtl_t;
#else
typedef PACK_START struct _IEEEtypes_FrameCtl_t {
    /** Protocol Version */
	t_u8 protocol_version:2;
    /** Type */
	t_u8 type:2;
    /** Sub Type */
	t_u8 sub_type:4;
    /** To DS */
	t_u8 to_ds:1;
    /** From DS */
	t_u8 from_ds:1;
    /** More Frag */
	t_u8 more_frag:1;
    /** Retry */
	t_u8 retry:1;
    /** Power Mgmt */
	t_u8 pwr_mgmt:1;
    /** More Data */
	t_u8 more_data:1;
    /** Wep */
	t_u8 wep:1;
    /** Order */
	t_u8 order:1;
} PACK_END IEEEtypes_FrameCtl_t;
#endif

/** IEEEtypes_AssocRqst_t */
typedef PACK_START struct _IEEEtypes_AssocRqst_t {
    /** Capability Info */
	t_u16 cap_info;
    /** Listen Interval */
	t_u16 listen_interval;
    /** IE Buffer */
	t_u8 ie_buffer[0];
} PACK_END IEEEtypes_AssocRqst_t;

/** IEEEtypes_AssocRsp_t */
typedef PACK_START struct _IEEEtypes_AssocRsp_t {
    /** Capability Info */
	t_u16 cap_info;
    /** Status Code */
	t_u16 status_code;
    /** AID */
	t_u16 aid;
} PACK_END IEEEtypes_AssocRsp_t;

/** IEEEtypes_ReAssocRqst_t */
typedef PACK_START struct _IEEEtypes_ReAssocRqst_t {
    /** Capability Info */
	t_u16 cap_info;
    /** Listen Interval */
	t_u16 listen_interval;
    /** Current AP Address */
	t_u8 current_ap_addr[ETH_ALEN];
    /** IE Buffer */
	t_u8 ie_buffer[0];
} PACK_END IEEEtypes_ReAssocRqst_t;

/** TLV buffer : Channel Config */
typedef PACK_START struct _tlvbuf_channel_config {
    /** Type */
	t_u16 type;
    /** Length */
	t_u16 len;
    /** Band Configuration
      *
      * [7-6] Channel Selection Mode; 00 manual, 01 ACS
      * [5-4] 00 - no secondary channel,
      *       01 -- secondary channel is above
      *       03 -- secondary channel is below
      * [3-2] Channel Width; 00 20 MHz
      * [1-0] Band Info; 00 2.4 GHz
      */
	t_u8 band_config_type;
    /** Channel number */
	t_u8 chan_number;
} PACK_END tlvbuf_channel_config;

/** MrvlIEtypes_WapiInfoSet_t */
typedef PACK_START struct _MrvlIEtypes_WapiInfoSet_t {
    /** Type */
	t_u16 type;
    /** Length */
	t_u16 len;
    /** Multicast PN */
	t_u8 multicast_PN[16];
} PACK_END MrvlIEtypes_WapiInfoSet_t;

/** MrvlIETypes_MgmtFrameSet_t */
typedef PACK_START struct _MrvlIETypes_MgmtFrameSet_t {
    /** Type */
	t_u16 type;
    /** Length */
	t_u16 len;
    /** Frame Control */
	IEEEtypes_FrameCtl_t frame_control;
    /** Frame Contents */
	t_u8 frame_contents[0];
} PACK_END MrvlIETypes_MgmtFrameSet_t;

/** Debug Type : Event */
#define DEBUG_TYPE_EVENT    0
/** Debug Type : Info */
#define DEBUG_TYPE_INFO     1

/** Major debug id: Authenticator */
#define DEBUG_ID_MAJ_AUTHENTICATOR      1
/** Minor debug id: PWK1 */
#define DEBUG_MAJ_AUTH_MIN_PWK1         0
/** Minor debug id: PWK2 */
#define DEBUG_MAJ_AUTH_MIN_PWK2         1
/** Minor debug id: PWK3 */
#define DEBUG_MAJ_AUTH_MIN_PWK3         2
/** Minor debug id: PWK4 */
#define DEBUG_MAJ_AUTH_MIN_PWK4         3
/** Minor debug id: GWK1 */
#define DEBUG_MAJ_AUTH_MIN_GWK1         4
/** Minor debug id: GWK2 */
#define DEBUG_MAJ_AUTH_MIN_GWK2         5
/** Minor debug id: station reject */
#define DEBUG_MAJ_AUTH_MIN_STA_REJ      6
/** Minor debug id: EAPOL_TR */
#define DEBUG_MAJ_AUTH_MIN_EAPOL_TR     7

/** Major debug id: Assoicate agent */
#define DEBUG_ID_MAJ_ASSOC_AGENT        2
/** Minor debug id: WPA IE*/
#define DEBUG_ID_MAJ_ASSOC_MIN_WPA_IE   0
/** Minor debug id: station reject */
#define DEBUG_ID_MAJ_ASSOC_MIN_STA_REJ  1

/** ether_hdr */
typedef PACK_START struct {
    /** Dest address */
	t_u8 da[ETH_ALEN];
    /** Src address */
	t_u8 sa[ETH_ALEN];
    /** Header type */
	t_u16 type;
} PACK_END ether_hdr_t;

/** 8021x header */
typedef PACK_START struct {
    /** Protocol version*/
	t_u8 protocol_ver;
    /** Packet type*/
	t_u8 pckt_type;
    /** Packet len */
	t_u8 pckt_body_len;
} PACK_END hdr_8021x_t;

/** Nonce size */
#define NONCE_SIZE      32
/** Max WPA IE len */
#define MAX_WPA_IE_LEN  64
/** EAPOL mic size */
#define EAPOL_MIC_SIZE  16

/** EAPOL key message */
typedef PACK_START struct {
    /** Ether header */
	ether_hdr_t ether_hdr;
    /** 8021x header */
	hdr_8021x_t hdr_8021x;
    /** desc_type */
	t_u8 desc_type;
    /** Key info */
	t_u16 k;
    /** Key length */
	t_u16 key_length;
    /** Replay count */
	t_u32 replay_cnt[2];
    /** Key nonce */
	t_u8 key_nonce[NONCE_SIZE];
    /** Key IV */
	t_u8 eapol_key_iv[16];
    /** Key RSC */
	t_u8 key_rsc[8];
    /** Key ID */
	t_u8 key_id[8];
    /** Key MIC */
	t_u8 key_mic[EAPOL_MIC_SIZE];
    /** Key len */
	t_u16 key_material_len;
    /** Key data */
	t_u8 key_data[MAX_WPA_IE_LEN];
} PACK_END eapol_keymsg_debug_t;

/** Failure after receive EAPOL MSG2 PMK */
#define REJECT_STATE_FAIL_EAPOL_2       1
/** Failure after receive EAPOL MSG4 PMK*/
#define REJECT_STATE_FAIL_EAPOL_4       2
/** Failure after receive EAPOL Group MSG2 GWK */
#define REJECT_STATE_FAIL_EAPOL_GROUP_2 3

/** Fail reason: Invalid ie */
#define IEEEtypes_REASON_INVALID_IE     13
/** Fail reason: Mic failure */
#define IEEEtypes_REASON_MIC_FAILURE    14

/** Station reject */
typedef PACK_START struct {
    /** Reject state */
	t_u8 reject_state;
    /** Reject reason */
	t_u16 reject_reason;
    /** Station mac address */
	t_u8 sta_mac_addr[ETH_ALEN];
} PACK_END sta_reject_t;

/** wpa_ie */
typedef PACK_START struct {
    /** Station mac address */
	t_u8 sta_mac_addr[ETH_ALEN];
    /** WPA IE */
	t_u8 wpa_ie[MAX_WPA_IE_LEN];
} PACK_END wpaie_t;

/** Initial state of the state machine */
#define EAPOL_START         1
/** Sent eapol msg1, wait for msg2 from the client */
#define EAPOL_WAIT_PWK2     2
/** Sent eapol msg3, wait for msg4 from the client */
#define EAPOL_WAIT_PWK4     3
/** Sent eapol group key msg1, wait for group msg2 from the client */
#define EAPOL_WAIT_GTK2     4
/** Eapol handshake complete */
#define EAPOL_END           5

/** Eapol state */
typedef PACK_START struct {
    /** Eapol state*/
	t_u8 eapol_state;
    /** Station address*/
	t_u8 sta_mac_addr[ETH_ALEN];
} PACK_END eapol_state_t;

/** Debug Info */
typedef PACK_START union {
    /** Eapol key message */
	eapol_keymsg_debug_t eapol_pwkmsg;
    /** Station reject*/
	sta_reject_t sta_reject;
    /** WPA IE */
	wpaie_t wpaie;
    /** Eapol state */
	eapol_state_t eapol_state;
} PACK_END d_info;

/** Event body : Debug */
typedef PACK_START struct _eventbuf_debug {
    /** Debug type */
	t_u8 debug_type;
    /** Major debug id */
	t_u32 debug_id_major;
    /** Minor debug id */
	t_u32 debug_id_minor;
    /** Debug Info */
	d_info info;
} PACK_END eventbuf_debug;

int ishexstring(void *hex);
inline int ISDIGIT(char *x);
unsigned int a2hex(char *s);
#endif /* _MLAN_EVENT_H */
