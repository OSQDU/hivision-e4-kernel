/** @file  mlanevent.c
 *
 *  @brief Program to receive events from the driver/firmware of the uAP
 *         driver.
 *
 *  Usage: mlanevent.exe [-option]
 *
 *  (C) Copyright 2008-2016 Marvell International Ltd. All Rights Reserved
 *
 *  MARVELL CONFIDENTIAL
 *  The source code contained or described herein and all documents related to
 *  the source code ("Material") are owned by Marvell International Ltd or its
 *  suppliers or licensors. Title to the Material remains with Marvell International Ltd
 *  or its suppliers and licensors. The Material contains trade secrets and
 *  proprietary and confidential information of Marvell or its suppliers and
 *  licensors. The Material is protected by worldwide copyright and trade secret
 *  laws and treaty provisions. No part of the Material may be used, copied,
 *  reproduced, modified, published, uploaded, posted, transmitted, distributed,
 *  or disclosed in any way without Marvell's prior express written permission.
 *
 *  No license under any patent, copyright, trade secret or other intellectual
 *  property right is granted to or conferred upon you by disclosure or delivery
 *  of the Materials, either expressly, by implication, inducement, estoppel or
 *  otherwise. Any license under such intellectual property rights must be
 *  express and approved by Marvell in writing.
 *
 */
/****************************************************************************
Change log:
    03/18/08: Initial creation
****************************************************************************/

/****************************************************************************
        Header files
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <getopt.h>

#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/if.h>
#include "mlanevent.h"

/****************************************************************************
        Definitions
****************************************************************************/
/** Enable or disable debug outputs */
#define DEBUG   0

/****************************************************************************
        Global variables
****************************************************************************/
/** Termination flag */
int terminate_flag = 0;

/****************************************************************************
        Local functions
****************************************************************************/
/**
 *  @brief Signal handler
 *
 *  @param sig      Received signal number
 *  @return         N/A
 */
void
sig_handler(int sig)
{
	printf("Stopping application.\n");
#if DEBUG
	printf("Process ID of process killed = %d\n", getpid());
#endif
	terminate_flag = 1;
}

/**
 *  @brief Dump hex data
 *
 *  @param p        A pointer to data buffer
 *  @param len      The len of data buffer
 *  @param delim    Deliminator character
 *  @return         Hex integer
 */
static void
hexdump(void *p, t_s32 len, char delim)
{
	t_s32 i;
	t_u8 *s = p;
	for (i = 0; i < len; i++) {
		if (i != len - 1)
			printf("%02x%c", *s++, delim);
		else
			printf("%02x\n", *s);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
}

/**
 *    @brief isdigit for String.
 *
 *    @param x            Char string
 *    @return             MLAN_EVENT_FAILURE for non-digit.
 *                        0 for digit
 */
inline int
ISDIGIT(char *x)
{
	unsigned int i;
	for (i = 0; i < strlen(x); i++)
		if (isdigit(x[i]) == 0)
			return MLAN_EVENT_FAILURE;
	return 0;
}

/**
 *  @brief      Hex to number
 *
 *  @param c    Hex value
 *  @return     Integer value or -1
 */
int
hex2num(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return -1;
}

/**
 *  @brief      Check hex string
 *
 *  @param hex      A pointer to hex string
 *  @return         0 or MLAN_EVENT_FAILURE
 */
int
ishexstring(void *hex)
{
	int i, a;
	char *p = hex;
	int len = strlen(p);
	if (!strncasecmp("0x", p, 2)) {
		p += 2;
		len -= 2;
	}
	for (i = 0; i < len; i++) {
		a = hex2num(*p);
		if (a < 0)
			return MLAN_EVENT_FAILURE;
		p++;
	}
	return 0;
}

/**
 *    @brief Convert char to hex integer
 *
 *    @param chr          Char
 *    @return             Hex integer
 */
unsigned char
hexc2bin(char chr)
{
	if (chr >= '0' && chr <= '9')
		chr -= '0';
	else if (chr >= 'A' && chr <= 'F')
		chr -= ('A' - 10);
	else if (chr >= 'a' && chr <= 'f')
		chr -= ('a' - 10);

	return chr;
}

/**
 *    @brief Convert string to hex integer
 *
 *    @param s            A pointer string buffer
 *    @return             Hex integer
 */
unsigned int
a2hex(char *s)
{
	unsigned int val = 0;
	if (!strncasecmp("0x", s, 2)) {
		s += 2;
	}
	while (*s && isxdigit(*s)) {
		val = (val << 4) + hexc2bin(*s++);
	}
	return val;
}

/**
 *  @brief Prints a MAC address in colon separated form from raw data
 *
 *  @param raw      A pointer to the hex data buffer
 *  @return         N/A
 */
void
print_mac(t_u8 *raw)
{
	printf("%02x:%02x:%02x:%02x:%02x:%02x", (unsigned int)raw[0],
	       (unsigned int)raw[1], (unsigned int)raw[2], (unsigned int)raw[3],
	       (unsigned int)raw[4], (unsigned int)raw[5]);
	return;
}

/**
 *  @brief Print usage information
 *
 *  @return         N/A
 */
void
print_usage(void)
{
	printf("\n");
	printf("Usage : mlanevent.exe [-v] [-h]\n");
	printf("    -v               : Print version information\n");
	printf("    -h               : Print help information\n");
	printf("    -i               : Specify device number from 0 to %d\n",
	       MAX_NO_OF_DEVICES - 1);
	printf("                       0xff for all devices\n");
	printf("\n");
}

/**
 *  @brief Parse and print STA deauthentication event data
 *
 *  @param buffer   Pointer to received event buffer
 *  @param size     Length of the received event data
 *  @return         N/A
 */
void
print_event_sta_deauth(t_u8 *buffer, t_u16 size)
{
	eventbuf_sta_deauth *event_body = NULL;

	if (size < sizeof(eventbuf_sta_deauth)) {
		printf("ERR:Event buffer too small!\n");
		return;
	}
	event_body = (eventbuf_sta_deauth *)buffer;
	event_body->reason_code = uap_le16_to_cpu(event_body->reason_code);
	printf("EVENT: STA_DEAUTH\n");
	printf("Deauthenticated STA MAC: ");
	print_mac(event_body->sta_mac_address);
	printf("\nReason: ");
	switch (event_body->reason_code) {
	case 1:
		printf("Client station leaving the network\n");
		break;
	case 2:
		printf("Client station aged out\n");
		break;
	case 3:
		printf("Client station deauthenticated by user's request\n");
		break;
	case 4:
		printf("Client station authentication failure\n");
		break;
	case 5:
		printf("Client station association failure\n");
		break;
	case 6:
		printf("Client mac address is blocked by ACL filter\n");
		break;
	case 7:
		printf("Client station table is full\n");
		break;
	case 8:
		printf("Client 4-way handshake timeout\n");
		break;
	case 9:
		printf("Client group key handshake timeout\n");
		break;
	default:
		printf("Unspecified\n");
		break;
	}
	return;
}

/**
 *  @brief Parse and print WEP ICV error event data
 *
 *  @param buffer   Pointer to received event buffer
 *  @param size     Length of the received event data
 *  @return         N/A
 */
void
print_event_wep_icv_error(t_u8 *buffer, t_u16 size)
{
	int i = 0;
	eventbuf_wep_icv_error *event_body = NULL;

	if (size < sizeof(eventbuf_wep_icv_error)) {
		printf("ERR:Event buffer too small!\n");
		return;
	}
	event_body = (eventbuf_wep_icv_error *)buffer;
	event_body->reason_code = uap_le16_to_cpu(event_body->reason_code);
	printf("EVENT: WEP_ICV_ERROR\n");
	printf("Deauthenticated STA MAC: ");
	print_mac(event_body->sta_mac_address);
	printf("WEP key index = %d\n", event_body->wep_key_index);
	printf("WEP key length = %d\n", event_body->wep_key_length);
	printf("WEP key : \n");
	for (i = 0; i < event_body->wep_key_length; i++) {
		printf("%02x ", event_body->wep_key[i]);
	}
	printf("\n");
	return;
}

/**
 *  @brief Prints mgmt frame
 *
 *  @param mgmt_tlv A pointer to mgmt_tlv
 *  @param tlv_len  Length of tlv payload
 *  @return         N/A
 */
void
print_mgmt_frame(MrvlIETypes_MgmtFrameSet_t *mgmt_tlv, int tlv_len)
{
	IEEEtypes_AssocRqst_t *assoc_req = NULL;
	IEEEtypes_ReAssocRqst_t *reassoc_req = NULL;
	IEEEtypes_AssocRsp_t *assoc_resp = NULL;
	t_u16 frm_ctl = 0;
	printf("\nMgmt Frame:\n");
	memcpy(&frm_ctl, &mgmt_tlv->frame_control, sizeof(t_u16));
	printf("FrameControl: 0x%x\n", frm_ctl);
	if (mgmt_tlv->frame_control.type != 0) {
		printf("Frame type=%d subtype=%d:\n",
		       mgmt_tlv->frame_control.type,
		       mgmt_tlv->frame_control.sub_type);
		hexdump(mgmt_tlv->frame_contents, tlv_len - sizeof(t_u16), ' ');
		return;
	}
	switch (mgmt_tlv->frame_control.sub_type) {
	case SUBTYPE_ASSOC_REQUEST:
		printf("Assoc Request:\n");
		assoc_req = (IEEEtypes_AssocRqst_t *)mgmt_tlv->frame_contents;
		printf("CapInfo: 0x%x  ListenInterval: 0x%x \n",
		       uap_le16_to_cpu(assoc_req->cap_info),
		       uap_le16_to_cpu(assoc_req->listen_interval));
		printf("AssocReqIE:\n");
		hexdump(assoc_req->ie_buffer,
			tlv_len - sizeof(IEEEtypes_AssocRqst_t)
			- sizeof(IEEEtypes_FrameCtl_t), ' ');
		break;
	case SUBTYPE_REASSOC_REQUEST:
		printf("ReAssoc Request:\n");
		reassoc_req =
			(IEEEtypes_ReAssocRqst_t *)mgmt_tlv->frame_contents;
		printf("CapInfo: 0x%x  ListenInterval: 0x%x \n",
		       uap_le16_to_cpu(reassoc_req->cap_info),
		       uap_le16_to_cpu(reassoc_req->listen_interval));
		printf("Current AP address: ");
		print_mac(reassoc_req->current_ap_addr);
		printf("\nReAssocReqIE:\n");
		hexdump(reassoc_req->ie_buffer,
			tlv_len - sizeof(IEEEtypes_ReAssocRqst_t)
			- sizeof(IEEEtypes_FrameCtl_t), ' ');
		break;
	case SUBTYPE_ASSOC_RESPONSE:
	case SUBTYPE_REASSOC_RESPONSE:
		if (mgmt_tlv->frame_control.sub_type == SUBTYPE_ASSOC_RESPONSE)
			printf("Assoc Response:\n");
		else
			printf("ReAssoc Response:\n");
		assoc_resp = (IEEEtypes_AssocRsp_t *)mgmt_tlv->frame_contents;
		printf("CapInfo: 0x%x  StatusCode: %d  AID: 0x%x \n",
		       uap_le16_to_cpu(assoc_resp->cap_info),
		       (int)(uap_le16_to_cpu(assoc_resp->status_code)),
		       uap_le16_to_cpu(assoc_resp->aid) & 0x3fff);
		break;
	default:
		printf("Frame subtype = %d:\n",
		       mgmt_tlv->frame_control.sub_type);
		hexdump(mgmt_tlv->frame_contents, tlv_len - sizeof(t_u16), ' ');
		break;
	}
	return;
}

/**
 *  @brief Parse and print RSN connect event data
 *
 *  @param buffer   Pointer to received buffer
 *  @param size     Length of the received event data
 *  @return         N/A
 */
void
print_event_rsn_connect(t_u8 *buffer, t_u16 size)
{
	int tlv_buf_left = size;
	t_u16 tlv_type, tlv_len;
	tlvbuf_header *tlv = NULL;
	eventbuf_rsn_connect *event_body = NULL;
	if (size < sizeof(eventbuf_rsn_connect)) {
		printf("ERR:Event buffer too small!\n");
		return;
	}
	event_body = (eventbuf_rsn_connect *)buffer;
	printf("EVENT: RSN_CONNECT\n");
	printf("Station MAC: ");
	print_mac(event_body->sta_mac_address);
	printf("\n");
	tlv_buf_left = size - sizeof(eventbuf_rsn_connect);
	if (tlv_buf_left < (int)sizeof(tlvbuf_header))
		return;
	tlv = (tlvbuf_header *)(buffer + sizeof(eventbuf_rsn_connect));

	while (tlv_buf_left >= (int)sizeof(tlvbuf_header)) {
		tlv_type = uap_le16_to_cpu(tlv->type);
		tlv_len = uap_le16_to_cpu(tlv->len);
		if ((sizeof(tlvbuf_header) + tlv_len) >
		    (unsigned int)tlv_buf_left) {
			printf("wrong tlv: tlvLen=%d, tlvBufLeft=%d\n", tlv_len,
			       tlv_buf_left);
			break;
		}
		switch (tlv_type) {
		case IEEE_WPA_IE:
			printf("WPA IE:\n");
			hexdump((t_u8 *)tlv + sizeof(tlvbuf_header), tlv_len,
				' ');
			break;
		case IEEE_RSN_IE:
			printf("RSN IE:\n");
			hexdump((t_u8 *)tlv + sizeof(tlvbuf_header), tlv_len,
				' ');
			break;
		default:
			printf("unknown tlv: %d\n", tlv_type);
			break;
		}
		tlv_buf_left -= (sizeof(tlvbuf_header) + tlv_len);
		tlv = (tlvbuf_header *)((t_u8 *)tlv + tlv_len +
					sizeof(tlvbuf_header));
	}
	return;
}

/**
 *  @brief Parse and print STA associate event data
 *
 *  @param buffer   Pointer to received buffer
 *  @param size     Length of the received event data
 *  @return         N/A
 */
void
print_event_sta_assoc(t_u8 *buffer, t_u16 size)
{
	int tlv_buf_left = size;
	t_u16 tlv_type, tlv_len;
	tlvbuf_header *tlv = NULL;
	MrvlIEtypes_WapiInfoSet_t *wapi_tlv = NULL;
	MrvlIETypes_MgmtFrameSet_t *mgmt_tlv = NULL;
	eventbuf_sta_assoc *event_body = NULL;
	if (size < sizeof(eventbuf_sta_assoc)) {
		printf("ERR:Event buffer too small!\n");
		return;
	}
	event_body = (eventbuf_sta_assoc *)buffer;
	printf("EVENT: STA_ASSOCIATE\n");
	printf("Associated STA MAC: ");
	print_mac(event_body->sta_mac_address);
	printf("\n");
	tlv_buf_left = size - sizeof(eventbuf_sta_assoc);
	if (tlv_buf_left < (int)sizeof(tlvbuf_header))
		return;
	tlv = (tlvbuf_header *)(buffer + sizeof(eventbuf_sta_assoc));

	while (tlv_buf_left >= (int)sizeof(tlvbuf_header)) {
		tlv_type = uap_le16_to_cpu(tlv->type);
		tlv_len = uap_le16_to_cpu(tlv->len);
		if ((sizeof(tlvbuf_header) + tlv_len) >
		    (unsigned int)tlv_buf_left) {
			printf("wrong tlv: tlvLen=%d, tlvBufLeft=%d\n", tlv_len,
			       tlv_buf_left);
			break;
		}
		switch (tlv_type) {
		case MRVL_WAPI_INFO_TLV_ID:
			wapi_tlv = (MrvlIEtypes_WapiInfoSet_t *)tlv;
			printf("WAPI Multicast PN:\n");
			hexdump(wapi_tlv->multicast_PN, tlv_len, ' ');
			break;
		case MRVL_MGMT_FRAME_TLV_ID:
			mgmt_tlv = (MrvlIETypes_MgmtFrameSet_t *)tlv;
			print_mgmt_frame(mgmt_tlv, tlv_len);
			break;
		default:
			printf("unknown tlv: %d\n", tlv_type);
			break;
		}
		tlv_buf_left -= (sizeof(tlvbuf_header) + tlv_len);
		tlv = (tlvbuf_header *)((t_u8 *)tlv + tlv_len +
					sizeof(tlvbuf_header));
	}
	return;
}

/**
 *  @brief Parse and print BSS start event data
 *
 *  @param buffer   Pointer to received buffer
 *  @param size     Length of the received event data
 *  @return         N/A
 */
void
print_event_bss_start(t_u8 *buffer, t_u16 size)
{
	eventbuf_bss_start *event_body = NULL;
	int tlv_buf_left = size;
	t_u16 tlv_type, tlv_len;
	tlvbuf_header *tlv = NULL;
	tlvbuf_channel_config *channel_tlv = NULL;

	if (size < sizeof(eventbuf_bss_start)) {
		printf("ERR:Event buffer too small!\n");
		return;
	}
	event_body = (eventbuf_bss_start *)buffer;
	printf("EVENT: BSS_START ");
	printf("BSS MAC: ");
	print_mac(event_body->ap_mac_address);
	printf("\n");
	tlv_buf_left = size - sizeof(eventbuf_bss_start);
	if (tlv_buf_left < (int)sizeof(tlvbuf_header))
		return;
	tlv = (tlvbuf_header *)(buffer + sizeof(eventbuf_bss_start));

	while (tlv_buf_left >= (int)sizeof(tlvbuf_header)) {
		tlv_type = uap_le16_to_cpu(tlv->type);
		tlv_len = uap_le16_to_cpu(tlv->len);
		if ((sizeof(tlvbuf_header) + tlv_len) >
		    (unsigned int)tlv_buf_left) {
			printf("wrong tlv: tlvLen=%d, tlvBufLeft=%d\n", tlv_len,
			       tlv_buf_left);
			break;
		}
		switch (tlv_type) {
		case MRVL_CHANNELCONFIG_TLV_ID:
			channel_tlv = (tlvbuf_channel_config *)tlv;
			printf("Channel = %d\n", channel_tlv->chan_number);
			printf("Channel Select Mode = %s\n",
			       (channel_tlv->
				band_config_type & BAND_CONFIG_ACS_MODE) ? "ACS"
			       : "Manual");
			channel_tlv->band_config_type &= 0x30;
			if (channel_tlv->band_config_type == 0)
				printf("no secondary channel\n");
			else if (channel_tlv->band_config_type ==
				 SECOND_CHANNEL_ABOVE)
				printf("secondary channel is above primary channel\n");
			else if (channel_tlv->band_config_type ==
				 SECOND_CHANNEL_BELOW)
				printf("secondary channel is below primary channel\n");
			break;
		default:
#if DEBUG
			printf("unknown tlv: %d\n", tlv_type);
#endif
			break;
		}
		tlv_buf_left -= (sizeof(tlvbuf_header) + tlv_len);
		tlv = (tlvbuf_header *)((t_u8 *)tlv + tlv_len +
					sizeof(tlvbuf_header));
	}

	return;
}

/**
 *  @brief Prints station reject state
 *
 *  @param state    Fail state
 *  @return         N/A
 */
void
print_reject_state(t_u8 state)
{
	switch (state) {
	case REJECT_STATE_FAIL_EAPOL_2:
		printf("Reject state: FAIL_EAPOL_2\n");
		break;
	case REJECT_STATE_FAIL_EAPOL_4:
		printf("Reject state: FAIL_EAPOL_4:\n");
		break;
	case REJECT_STATE_FAIL_EAPOL_GROUP_2:
		printf("Reject state: FAIL_EAPOL_GROUP_2\n");
		break;
	default:
		printf("ERR: unknown reject state %d\n", state);
		break;
	}
	return;
}

/**
 *  @brief Prints station reject reason
 *
 *  @param reason   Reason code
 *  @return         N/A
 */
void
print_reject_reason(t_u16 reason)
{
	switch (reason) {
	case IEEEtypes_REASON_INVALID_IE:
		printf("Reject reason: Invalid IE\n");
		break;
	case IEEEtypes_REASON_MIC_FAILURE:
		printf("Reject reason: Mic Failure\n");
		break;
	default:
		printf("Reject reason: %d\n", reason);
		break;
	}
	return;
}

/**
 *  @brief Prints EAPOL state
 *
 *  @param state    Eapol state
 *  @return         N/A
 */
void
print_eapol_state(t_u8 state)
{
	switch (state) {
	case EAPOL_START:
		printf("Eapol state: EAPOL_START\n");
		break;
	case EAPOL_WAIT_PWK2:
		printf("Eapol state: EAPOL_WAIT_PWK2\n");
		break;
	case EAPOL_WAIT_PWK4:
		printf("Eapol state: EAPOL_WAIT_PWK4\n");
		break;
	case EAPOL_WAIT_GTK2:
		printf("Eapol state: EAPOL_WAIT_GTK2\n");
		break;
	case EAPOL_END:
		printf("Eapol state: EAPOL_END\n");
		break;
	default:
		printf("ERR: unknow eapol state%d\n", state);
		break;
	}
	return;
}

/**
 *  @brief Parse and print debug event data
 *
 *  @param buffer   Pointer to received buffer
 *  @param size     Length of the received event data
 *  @return         N/A
 */
void
print_event_debug(t_u8 *buffer, t_u16 size)
{
	eventbuf_debug *event_body = NULL;
	if (size < sizeof(eventbuf_debug)) {
		printf("ERR:Event buffer too small!\n");
		return;
	}
	event_body = (eventbuf_debug *)buffer;
	printf("Debug Event Type: %s\n",
	       (event_body->debug_type == 0) ? "EVENT" : "INFO");
	printf("%s log:\n",
	       (uap_le32_to_cpu(event_body->debug_id_major) ==
		DEBUG_ID_MAJ_AUTHENTICATOR) ? "Authenticator" : "Assoc_agent");
	if (uap_le32_to_cpu(event_body->debug_id_major) ==
	    DEBUG_ID_MAJ_AUTHENTICATOR) {
		switch (uap_le32_to_cpu(event_body->debug_id_minor)) {
		case DEBUG_MAJ_AUTH_MIN_PWK1:
			printf("EAPOL Key message 1 (PWK):\n");
			hexdump((t_u8 *)&event_body->info.eapol_pwkmsg,
				sizeof(eapol_keymsg_debug_t), ' ');
			break;
		case DEBUG_MAJ_AUTH_MIN_PWK2:
			printf("EAPOL Key message 2 (PWK):\n");
			hexdump((t_u8 *)&event_body->info.eapol_pwkmsg,
				sizeof(eapol_keymsg_debug_t), ' ');
			break;
		case DEBUG_MAJ_AUTH_MIN_PWK3:
			printf("EAPOL Key message 3 (PWK):\n");
			hexdump((t_u8 *)&event_body->info.eapol_pwkmsg,
				sizeof(eapol_keymsg_debug_t), ' ');
			break;
		case DEBUG_MAJ_AUTH_MIN_PWK4:
			printf("EAPOL Key message 4: (PWK)\n");
			hexdump((t_u8 *)&event_body->info.eapol_pwkmsg,
				sizeof(eapol_keymsg_debug_t), ' ');
			break;
		case DEBUG_MAJ_AUTH_MIN_GWK1:
			printf("EAPOL Key message 1: (GWK)\n");
			hexdump((t_u8 *)&event_body->info.eapol_pwkmsg,
				sizeof(eapol_keymsg_debug_t), ' ');
			break;
		case DEBUG_MAJ_AUTH_MIN_GWK2:
			printf("EAPOL Key message 2: (GWK)\n");
			hexdump((t_u8 *)&event_body->info.eapol_pwkmsg,
				sizeof(eapol_keymsg_debug_t), ' ');
			break;
		case DEBUG_MAJ_AUTH_MIN_STA_REJ:
			printf("Reject STA MAC: ");
			print_mac(event_body->info.sta_reject.sta_mac_addr);
			printf("\n");
			print_reject_state(event_body->info.sta_reject.
					   reject_state);
			print_reject_reason(uap_le16_to_cpu
					    (event_body->info.sta_reject.
					     reject_reason));
			break;
		case DEBUG_MAJ_AUTH_MIN_EAPOL_TR:
			printf("STA MAC: ");
			print_mac(event_body->info.eapol_state.sta_mac_addr);
			printf("\n");
			print_eapol_state(event_body->info.eapol_state.
					  eapol_state);
			break;
		default:
			printf("ERR: unknow debug_id_minor: %d\n",
			       (int)uap_le32_to_cpu(event_body->
						    debug_id_minor));
			hexdump(buffer, size, ' ');
			return;
		}
	} else if (uap_le32_to_cpu(event_body->debug_id_major) ==
		   DEBUG_ID_MAJ_ASSOC_AGENT) {
		switch (uap_le32_to_cpu(event_body->debug_id_minor)) {
		case DEBUG_ID_MAJ_ASSOC_MIN_WPA_IE:
			printf("STA MAC: ");
			print_mac(event_body->info.wpaie.sta_mac_addr);
			printf("\n");
			printf("wpa ie:\n");
			hexdump(event_body->info.wpaie.wpa_ie, MAX_WPA_IE_LEN,
				' ');
			break;
		case DEBUG_ID_MAJ_ASSOC_MIN_STA_REJ:
			printf("Reject STA MAC: ");
			print_mac(event_body->info.sta_reject.sta_mac_addr);
			printf("\n");
			print_reject_state(event_body->info.sta_reject.
					   reject_state);
			print_reject_reason(uap_le16_to_cpu
					    (event_body->info.sta_reject.
					     reject_reason));
			break;
		default:
			printf("ERR: unknow debug_id_minor: %d\n",
			       (int)uap_le32_to_cpu(event_body->
						    debug_id_minor));
			hexdump(buffer, size, ' ');
			return;
		}
	}
	return;
}

/**
 *  @brief Parse and print received event information
 *
 *  @param event    Pointer to received event
 *  @param size     Length of the received event
 *  @return         N/A
 */
void
print_event(event_header *event, t_u16 size)
{
	t_u32 event_id = event->event_id;
	switch (event_id) {
	case MICRO_AP_EV_ID_STA_DEAUTH:
		print_event_sta_deauth(event->event_data, size - EVENT_ID_LEN);
		break;
	case MICRO_AP_EV_ID_STA_ASSOC:
		print_event_sta_assoc(event->event_data, size - EVENT_ID_LEN);
		break;
	case MICRO_AP_EV_ID_BSS_START:
		print_event_bss_start(event->event_data, size - EVENT_ID_LEN);
		break;
	case MICRO_AP_EV_ID_DEBUG:
		print_event_debug(event->event_data, size - EVENT_ID_LEN);
		break;
	case MICRO_AP_EV_BSS_IDLE:
		printf("EVENT: BSS_IDLE\n");
		break;
	case MICRO_AP_EV_BSS_ACTIVE:
		printf("EVENT: BSS_ACTIVE\n");
		break;
	case MICRO_AP_EV_RSN_CONNECT:
		print_event_rsn_connect(event->event_data, size - EVENT_ID_LEN);
		break;

	case UAP_EVENT_ID_DRV_HS_ACTIVATED:
		printf("EVENT: uAP HS_ACTIVATED\n");
		break;
	case UAP_EVENT_ID_DRV_HS_DEACTIVATED:
		printf("EVENT: uAP HS_DEACTIVATED\n");
		break;
	case UAP_EVENT_ID_HS_WAKEUP:
		printf("EVENT: uAP HS_WAKEUP\n");
		break;
	case UAP_EVENT_HOST_SLEEP_AWAKE:
		break;
	case UAP_EVENT_ID_DRV_MGMT_FRAME:
		printf("EVENT: Mgmt frame from FW\n");
		hexdump((void *)event, size, ' ');
		break;
	case MICRO_AP_EV_WMM_STATUS_CHANGE:
		printf("EVENT: WMM_STATUS_CHANGE\n");
		break;
	case EVENT_WEP_ICV_ERROR:
		print_event_wep_icv_error(event->event_data,
					  size - EVENT_ID_LEN);
		break;
	case EVENT_ID_DRV_SCAN_REPORT:
		printf("Scan request completed.\n");
		break;

	default:
		/* Handle string based events */
#define CUS_EVT_PORT_RELEASE           "EVENT=PORT_RELEASE"
		if (!strncmp
		    ((char *)event, CUS_EVT_PORT_RELEASE,
		     strlen(CUS_EVT_PORT_RELEASE))) {
			printf("EVENT: PORT_RELEASE.\n");
			break;
		}
#define CUS_EVT_TDLS_CONNECTED           "EVENT=TDLS_CONNECTED"
		if (!strncmp
		    ((char *)event, CUS_EVT_TDLS_CONNECTED,
		     strlen(CUS_EVT_TDLS_CONNECTED))) {
			printf("EVENT: TDLS_CONNECTED\n");
			print_mac((t_u8 *)event +
				  strlen(CUS_EVT_TDLS_CONNECTED));
			printf("\n");
			break;
		}
#define CUS_EVT_TDLS_TEARDOWN            "EVENT=TDLS_TEARDOWN"
		if (!strncmp
		    ((char *)event, CUS_EVT_TDLS_TEARDOWN,
		     strlen(CUS_EVT_TDLS_TEARDOWN))) {
			printf("EVENT: TDLS_TEARDOWN\n");
			print_mac((t_u8 *)event +
				  strlen(CUS_EVT_TDLS_TEARDOWN));
			printf("\n");
			break;
		}
#define CUS_EVT_STA_CONNECTED           "EVENT=STA_CONNECTED"
		if (!strncmp
		    ((char *)event, CUS_EVT_STA_CONNECTED,
		     strlen(CUS_EVT_STA_CONNECTED))) {
			printf("EVENT: STA_CONNECTED\n");
			print_mac((t_u8 *)event +
				  strlen(CUS_EVT_STA_CONNECTED) + 1);
			printf("\n");
			break;
		}
#define CUS_EVT_STA_DISCONNECTED           "EVENT=STA_DISCONNECTED"
		if (!strncmp
		    ((char *)event, CUS_EVT_STA_DISCONNECTED,
		     strlen(CUS_EVT_STA_DISCONNECTED))) {
			printf("EVENT: STA_DISCONNECTED\n");
			break;
		}
#define CUS_EVT_AP_CONNECTED           "EVENT=AP_CONNECTED"
		if (!strncmp
		    ((char *)event, CUS_EVT_AP_CONNECTED,
		     strlen(CUS_EVT_AP_CONNECTED))) {
			printf("EVENT: AP_CONNECTED\n");
			print_mac((t_u8 *)event + strlen(CUS_EVT_AP_CONNECTED));
			printf("\n");
			break;
		}
#define CUS_EVT_ADHOC_LINK_SENSED   "EVENT=ADHOC_LINK_SENSED"
		if (!strncmp
		    ((char *)event, CUS_EVT_ADHOC_LINK_SENSED,
		     strlen(CUS_EVT_ADHOC_LINK_SENSED))) {
			printf("EVENT: ADHOC_LINK_SENSED\n");
			break;
		}
#define CUS_EVT_ADHOC_LINK_LOST     "EVENT=ADHOC_LINK_LOST"
		if (!strncmp
		    ((char *)event, CUS_EVT_ADHOC_LINK_LOST,
		     strlen(CUS_EVT_ADHOC_LINK_LOST))) {
			printf("EVENT: ADHOC_LINK_LOST\n");
			break;
		}
#define CUS_EVT_OBSS_SCAN_PARAM     "EVENT=OBSS_SCAN_PARAM"
		if (!strncmp
		    ((char *)event, CUS_EVT_OBSS_SCAN_PARAM,
		     strlen(CUS_EVT_OBSS_SCAN_PARAM))) {
			printf("EVENT: OBSS_SCAN_PARAM\n");
			break;
		}
#define CUS_EVT_BW_CHANGED      "EVENT=BW_CHANGED"
		if (!strncmp
		    ((char *)event, CUS_EVT_BW_CHANGED,
		     strlen(CUS_EVT_BW_CHANGED))) {
			printf("EVENT: BW_CHANGED\n");
			break;
		}
#define CUS_EVT_MLME_MIC_ERR_UNI    "MLME-MICHAELMICFAILURE.indication unicast"
		if (!strncmp
		    ((char *)event, CUS_EVT_MLME_MIC_ERR_UNI,
		     strlen(CUS_EVT_MLME_MIC_ERR_UNI))) {
			printf("EVENT: MLME-MICHAELMICFAILURE.indication unicast\n");
			break;
		}
#define CUS_EVT_MLME_MIC_ERR_MUL    "MLME-MICHAELMICFAILURE.indication multicast"
		if (!strncmp
		    ((char *)event, CUS_EVT_MLME_MIC_ERR_MUL,
		     strlen(CUS_EVT_MLME_MIC_ERR_MUL))) {
			printf("EVENT: MLME-MICHAELMICFAILURE.indication multicast\n");
			break;
		}
#define CUS_EVT_BEACON_RSSI_LOW     "EVENT=BEACON_RSSI_LOW"
		if (!strncmp
		    ((char *)event, CUS_EVT_BEACON_RSSI_LOW,
		     strlen(CUS_EVT_BEACON_RSSI_LOW))) {
			printf("EVENT: BEACON_RSSI_LOW\n");
			break;
		}
#define CUS_EVT_BEACON_RSSI_HIGH    "EVENT=BEACON_RSSI_HIGH"
		if (!strncmp
		    ((char *)event, CUS_EVT_BEACON_RSSI_HIGH,
		     strlen(CUS_EVT_BEACON_RSSI_HIGH))) {
			printf("EVENT: BEACON_RSSI_HIGH\n");
			break;
		}
#define CUS_EVT_BEACON_SNR_LOW      "EVENT=BEACON_SNR_LOW"
		if (!strncmp
		    ((char *)event, CUS_EVT_BEACON_SNR_LOW,
		     strlen(CUS_EVT_BEACON_SNR_LOW))) {
			printf("EVENT: BEACON_SNR_LOW\n");
			break;
		}
#define CUS_EVT_BEACON_SNR_HIGH     "EVENT=BEACON_SNR_HIGH"
		if (!strncmp
		    ((char *)event, CUS_EVT_BEACON_SNR_HIGH,
		     strlen(CUS_EVT_BEACON_SNR_HIGH))) {
			printf("EVENT: BEACON_SNR_HIGH\n");
			break;
		}
#define CUS_EVT_MAX_FAIL        "EVENT=MAX_FAIL"
		if (!strncmp
		    ((char *)event, CUS_EVT_MAX_FAIL,
		     strlen(CUS_EVT_MAX_FAIL))) {
			printf("EVENT: MAX_FAIL\n");
			break;
		}
#define CUS_EVT_DATA_RSSI_LOW       "EVENT=DATA_RSSI_LOW"
		if (!strncmp
		    ((char *)event, CUS_EVT_DATA_RSSI_LOW,
		     strlen(CUS_EVT_DATA_RSSI_LOW))) {
			printf("EVENT: DATA_RSSI_LOW\n");
			break;
		}
#define CUS_EVT_DATA_SNR_LOW        "EVENT=DATA_SNR_LOW"
		if (!strncmp
		    ((char *)event, CUS_EVT_DATA_SNR_LOW,
		     strlen(CUS_EVT_DATA_SNR_LOW))) {
			printf("EVENT: DATA_SNR_LOW\n");
			break;
		}
#define CUS_EVT_DATA_RSSI_HIGH      "EVENT=DATA_RSSI_HIGH"
		if (!strncmp
		    ((char *)event, CUS_EVT_DATA_RSSI_HIGH,
		     strlen(CUS_EVT_DATA_RSSI_HIGH))) {
			printf("EVENT: DATA_RSSI_HIGH\n");
			break;
		}
#define CUS_EVT_DATA_SNR_HIGH       "EVENT=DATA_SNR_HIGH"
		if (!strncmp
		    ((char *)event, CUS_EVT_DATA_SNR_HIGH,
		     strlen(CUS_EVT_DATA_SNR_HIGH))) {
			printf("EVENT: DATA_SNR_HIGH\n");
			break;
		}
#define CUS_EVT_LINK_QUALITY        "EVENT=LINK_QUALITY"
		if (!strncmp
		    ((char *)event, CUS_EVT_LINK_QUALITY,
		     strlen(CUS_EVT_LINK_QUALITY))) {
			printf("EVENT: LINK_QUALITY\n");
			break;
		}
#define CUS_EVT_WEP_ICV_ERR     "EVENT=WEP_ICV_ERR"
		if (!strncmp
		    ((char *)event, CUS_EVT_WEP_ICV_ERR,
		     strlen(CUS_EVT_WEP_ICV_ERR))) {
			printf("EVENT: WEP_ICV_ERR\n");
			break;
		}
#define CUS_EVT_CHANNEL_SWITCH_ANN  "EVENT=CHANNEL_SWITCH_ANN"
		if (!strncmp
		    ((char *)event, CUS_EVT_CHANNEL_SWITCH_ANN,
		     strlen(CUS_EVT_CHANNEL_SWITCH_ANN))) {
			printf("EVENT: CHANNEL_SWITCH_ANN\n");
			break;
		}
#define CUS_EVT_HS_WAKEUP       "HS_WAKEUP"
		if (!strncmp
		    ((char *)event, CUS_EVT_HS_WAKEUP,
		     strlen(CUS_EVT_HS_WAKEUP))) {
			printf("EVENT: HS_WAKEUP\n");
			break;
		}
#define CUS_EVT_HS_ACTIVATED        "HS_ACTIVATED"
		if (!strncmp
		    ((char *)event, CUS_EVT_HS_ACTIVATED,
		     strlen(CUS_EVT_HS_ACTIVATED))) {
			printf("EVENT: HS_ACTIVATED\n");
			break;
		}
#define CUS_EVT_HS_DEACTIVATED      "HS_DEACTIVATED"
		if (!strncmp
		    ((char *)event, CUS_EVT_HS_DEACTIVATED,
		     strlen(CUS_EVT_HS_DEACTIVATED))) {
			printf("EVENT: HS_DEACTIVATED\n");
			break;
		}
/** Custom indication message sent to the application layer for WMM changes */
#define WMM_CONFIG_CHANGE_INDICATION  "WMM_CONFIG_CHANGE.indication"
		if (!strncmp
		    ((char *)event, WMM_CONFIG_CHANGE_INDICATION,
		     strlen(WMM_CONFIG_CHANGE_INDICATION))) {
			printf("EVENT: STA_DISCONNECTED\n");
			break;
		}
#define CUS_EVT_DRIVER_HANG	            "EVENT=DRIVER_HANG"
		if (!strncmp
		    ((char *)event, CUS_EVT_DRIVER_HANG,
		     strlen(CUS_EVT_DRIVER_HANG))) {
			printf("EVENT: DRIVER_HANG\n");
			break;
		}
		printf("ERR:Undefined event type (0x%X). Dumping event buffer:\n", (unsigned int)event_id);
		hexdump((void *)event, size, ' ');
		break;
	}
	return;
}

/**
 *  @brief Read event data from netlink socket
 *
 *  @param sk_fd    Netlink socket handler
 *  @param buffer   Pointer to the data buffer
 *  @param nlh      Pointer to netlink message header
 *  @param msg      Pointer to message header
 *  @return         Number of bytes read or MLAN_EVENT_FAILURE
 */
int
read_event_netlink_socket(int sk_fd, unsigned char *buffer,
			  struct nlmsghdr *nlh, struct msghdr *msg)
{
	int count = -1;
	count = recvmsg(sk_fd, msg, 0);
#if DEBUG
	printf("DBG:Waiting for message from NETLINK.\n");
#endif
	if (count < 0) {
		printf("ERR:NETLINK read failed!\n");
		terminate_flag++;
		return MLAN_EVENT_FAILURE;
	}
#if DEBUG
	printf("DBG:Received message payload (%d)\n", count);
#endif
	if (count > NLMSG_SPACE(NL_MAX_PAYLOAD)) {
		printf("ERR:Buffer overflow!\n");
		return MLAN_EVENT_FAILURE;
	}
	memset(buffer, 0, NL_MAX_PAYLOAD);
	memcpy(buffer, NLMSG_DATA(nlh), count - NLMSG_HDRLEN);
#if DEBUG
	hexdump(buffer, count - NLMSG_HDRLEN, ' ');
#endif
	return count - NLMSG_HDRLEN;
}

/**
 *  @brief Configure and read event data from netlink socket
 *
 *  @param sk_fd    Array of netlink sockets
 *  @param no_of_sk Number of netlink sockets opened
 *  @param recv_buf Pointer to the array of evt_buf structures
 *  @param timeout  Socket listen timeout value
 *  @param nlh      Pointer to netlink message header
 *  @param msg      Pointer to message header
 *  @return         Number of bytes read or MLAN_EVENT_FAILURE
 */
int
read_event(int *sk_fd, int no_of_sk, evt_buf *recv_buf, int timeout,
	   struct nlmsghdr *nlh, struct msghdr *msg)
{
	struct timeval tv;
	fd_set rfds;
	int i = 0, max_sk_fd = sk_fd[0];
	int ret = MLAN_EVENT_FAILURE;

	/* Setup read fds */
	FD_ZERO(&rfds);
	for (i = 0; i < no_of_sk; i++) {
		if (sk_fd[i] > max_sk_fd)
			max_sk_fd = sk_fd[i];

		if (sk_fd[i] > 0)
			FD_SET(sk_fd[i], &rfds);
	}

	/* Initialize timeout value */
	if (timeout != 0)
		tv.tv_sec = timeout;
	else
		tv.tv_sec = UAP_RECV_WAIT_DEFAULT;
	tv.tv_usec = 0;

	/* Wait for reply */
	ret = select(max_sk_fd + 1, &rfds, NULL, NULL, &tv);
	if (ret == -1) {
		/* Error */
		terminate_flag++;
		return MLAN_EVENT_FAILURE;
	} else if (!ret) {
		/* Timeout. Try again */
		return MLAN_EVENT_FAILURE;
	}
	for (i = 0; i < no_of_sk; i++) {
		if (sk_fd[i] > 0) {
			if (FD_ISSET(sk_fd[i], &rfds)) {
				/* Success */
				recv_buf[i].flag = 1;
				recv_buf[i].length =
					read_event_netlink_socket(sk_fd[i],
								  recv_buf[i].
								  buffer, nlh,
								  msg);
				ret += recv_buf[i].length;
			}
		}
	}
	return ret;
}

/* Command line options */
static const struct option long_opts[] = {
	{"help", no_argument, NULL, 'h'},
	{"version", no_argument, NULL, 'v'},
	{NULL, 0, NULL, 0}
};

/**
 *  @brief Determine the netlink number
 *
 *  @param i socket number
 *
 *  @return         Netlink number to use
 */
static int
get_netlink_num(int i)
{
	FILE *fp;
	int netlink_num = NETLINK_MARVELL;
	char str[64];
	char *srch = "netlink_num";
	char filename[64];

	if (i == 0) {
		strcpy(filename, "/proc/mwlan/config");
	} else if (i > 0) {
		sprintf(filename, "/proc/mwlan/config%d", i);
	}
	/* Try to open /proc/mwlan/config$ */
	fp = fopen(filename, "r");
	if (fp) {
		while (!feof(fp)) {
			fgets(str, sizeof(str), fp);
			if (strncmp(str, srch, strlen(srch)) == 0) {
				netlink_num = atoi(str + strlen(srch) + 1);
				break;
			}
		}
		fclose(fp);
	} else {
		return -1;
	}
	printf("Netlink number = %d\n", netlink_num);
	return netlink_num;
}

/****************************************************************************
        Global functions
****************************************************************************/
/**
 *  @brief The main function
 *
 *  @param argc     Number of arguments
 *  @param argv     Pointer to the arguments
 *  @return         0 or 1
 */
int
main(int argc, char *argv[])
{
	int opt;
	int nl_sk[MAX_NO_OF_DEVICES];
	struct nlmsghdr *nlh = NULL;
	struct sockaddr_nl src_addr, dest_addr;
	struct msghdr msg;
	struct iovec iov;
	evt_buf evt_recv_buf[MAX_NO_OF_DEVICES];
	struct timeval current_time;
	struct tm *timeinfo;
	int num_events = 0;
	event_header *event = NULL;
	int ret = MLAN_EVENT_FAILURE;
	int netlink_num[MAX_NO_OF_DEVICES];
	char if_name[IFNAMSIZ + 1];
	t_u32 event_id = 0;
	int i = 0, no_of_sk = 0, dev_index = -1;

	/* Check command line options */
	while ((opt = getopt_long(argc, argv, "hvti:", long_opts, NULL)) > 0) {
		switch (opt) {
		case 'h':
			print_usage();
			return 0;
		case 'v':
			printf("mlanevent version : %s\n", MLAN_EVENT_VERSION);
			return 0;
			break;
		case 'i':
			if ((IS_HEX_OR_DIGIT(argv[2]) == MLAN_EVENT_FAILURE) ||
			    (A2HEXDECIMAL(argv[2]) < 0)
			    || ((A2HEXDECIMAL(argv[2]) >= MAX_NO_OF_DEVICES) &&
				(A2HEXDECIMAL(argv[2]) != 0xff))) {
				print_usage();
				return 1;
			} else {
				dev_index = A2HEXDECIMAL(argv[2]);
				argc -= optind;
				argv += optind;
			}
			break;
		default:
			print_usage();
			return 1;
		}
	}

	if (optind < argc) {
		fputs("Too many arguments.\n", stderr);
		print_usage();
		return 1;
	}
	if ((dev_index >= 0) && (dev_index < MAX_NO_OF_DEVICES)) {
		no_of_sk = 1;
	} else {
		/* Currently, we support maximum 4 devices */
		/* TODO: determine no_of_sk at run time */
		no_of_sk = MAX_NO_OF_DEVICES;
	}

	for (i = 0; i < no_of_sk; i++) {
		/* Initialise */
		nl_sk[i] = -1;
		if (no_of_sk == 1) {
			netlink_num[i] = get_netlink_num(dev_index);
			if (netlink_num[i] < 0) {
				printf("ERR:Could not get netlink socket. Invalid device number.\n");
				ret = MLAN_EVENT_FAILURE;
				goto done;
			}
		} else {
			netlink_num[i] = get_netlink_num(i);
		}
		if (netlink_num[i] >= 0) {
			/* Open netlink socket */
			nl_sk[i] = socket(PF_NETLINK, SOCK_RAW, netlink_num[i]);
			if (nl_sk[i] < 0) {
				printf("ERR:Could not open netlink socket.\n");
				ret = MLAN_EVENT_FAILURE;
				goto done;
			}

			/* Set source address */
			memset(&src_addr, 0, sizeof(src_addr));
			src_addr.nl_family = AF_NETLINK;
			src_addr.nl_pid = getpid();	/* Our PID */
			src_addr.nl_groups = NL_MULTICAST_GROUP;

			/* Bind socket with source address */
			if (bind
			    (nl_sk[i], (struct sockaddr *)&src_addr,
			     sizeof(src_addr)) < 0) {
				printf("ERR:Could not bind socket!\n");
				ret = MLAN_EVENT_FAILURE;
				goto done;
			}

			/* Set destination address */
			memset(&dest_addr, 0, sizeof(dest_addr));
			dest_addr.nl_family = AF_NETLINK;
			dest_addr.nl_pid = 0;	/* Kernel */
			dest_addr.nl_groups = NL_MULTICAST_GROUP;

			/* Initialize netlink header */
			nlh = (struct nlmsghdr *)
				malloc(NLMSG_SPACE(NL_MAX_PAYLOAD));
			if (!nlh) {
				printf("ERR: Could not alloc buffer\n");
				ret = MLAN_EVENT_FAILURE;
				goto done;
			}
			memset(nlh, 0, NLMSG_SPACE(NL_MAX_PAYLOAD));

			/* Initialize I/O vector */
			iov.iov_base = (void *)nlh;
			iov.iov_len = NLMSG_SPACE(NL_MAX_PAYLOAD);

			/* Initialize message header */
			memset(&msg, 0, sizeof(struct msghdr));
			msg.msg_name = (void *)&dest_addr;
			msg.msg_namelen = sizeof(dest_addr);
			msg.msg_iov = &iov;
			msg.msg_iovlen = 1;

			memset(&evt_recv_buf[i], 0, sizeof(evt_buf));
		}
	}
	gettimeofday(&current_time, NULL);

	printf("\n");
	printf("**********************************************\n");
	if ((timeinfo = localtime(&(current_time.tv_sec))))
		printf("mlanevent start time : %s", asctime(timeinfo));
	printf("                      %u usecs\n",
	       (unsigned int)current_time.tv_usec);
	printf("**********************************************\n");

	signal(SIGTERM, sig_handler);
	signal(SIGINT, sig_handler);
	signal(SIGALRM, sig_handler);
	while (1) {
		if (terminate_flag) {
			printf("Stopping!\n");
			break;
		}
		ret = read_event(nl_sk, no_of_sk, evt_recv_buf, 0, nlh, &msg);

		/* No result. Loop again */
		if (ret == MLAN_EVENT_FAILURE) {
			continue;
		}
		if (ret == 0) {
			/* Zero bytes received */
			printf("ERR:Received zero bytes!\n");
			continue;
		}
		for (i = 0; i < no_of_sk; i++) {
			if (evt_recv_buf[i].flag == 1) {
				num_events++;
				gettimeofday(&current_time, NULL);
				printf("\n");
				printf("============================================\n");
				printf("Received event");
				if ((timeinfo =
				     localtime(&(current_time.tv_sec))))
					printf(": %s", asctime(timeinfo));
				printf("                     %u usecs\n",
				       (unsigned int)current_time.tv_usec);
				printf("============================================\n");

				memcpy(&event_id, evt_recv_buf[i].buffer,
				       sizeof(event_id));
				if (((event_id & 0xFF000000) == 0x80000000) ||
				    ((event_id & 0xFF000000) == 0)) {
					event = (event_header
						 *)(evt_recv_buf[i].buffer);
				} else {
					memset(if_name, 0, IFNAMSIZ + 1);
					memcpy(if_name, evt_recv_buf[i].buffer,
					       IFNAMSIZ);
					printf("EVENT for interface %s\n",
					       if_name);
					event = (event_header
						 *)((t_u8 *)(evt_recv_buf[i].
							     buffer) +
						    IFNAMSIZ);
					ret -= IFNAMSIZ;
					evt_recv_buf[i].length -= IFNAMSIZ;
				}
#if DEBUG
				printf("DBG:Received buffer =\n");
				hexdump(evt_recv_buf[i].buffer,
					evt_recv_buf[i].length + IFNAMSIZ, ' ');
#endif
				print_event(event, evt_recv_buf[i].length);
				/* Reset event flag after reading */
				evt_recv_buf[i].flag = 0;
			}
		}
		fflush(stdout);
	}
	gettimeofday(&current_time, NULL);
	printf("\n");
	printf("*********************************************\n");
	if ((timeinfo = localtime(&(current_time.tv_sec))))
		printf("mlanevent end time  : %s", asctime(timeinfo));
	printf("                     %u usecs\n",
	       (unsigned int)current_time.tv_usec);
	printf("Total events       : %u\n", num_events);
	printf("*********************************************\n");
done:
	for (i = 0; i < no_of_sk; i++) {
		if (nl_sk[i] > 0)
			close(nl_sk[i]);
	}
	if (nlh)
		free(nlh);
	return 0;
}
