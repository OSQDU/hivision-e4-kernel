/** @file moal_uap_cfg80211.c
  *
  * @brief This file contains the functions for uAP CFG80211.
  *
  * Copyright (C) 2011-2016, Marvell International Ltd.
  *
  * This software file (the "File") is distributed by Marvell International
  * Ltd. under the terms of the GNU General Public License Version 2, June 1991
  * (the "License").  You may use, redistribute and/or modify this File in
  * accordance with the terms and conditions of the License, a copy of which
  * is available by writing to the Free Software Foundation, Inc.,
  * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or on the
  * worldwide web at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
  *
  * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE
  * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about
  * this warranty disclaimer.
  *
  */

#include "moal_cfg80211.h"
#include "moal_uap_cfg80211.h"
/** secondary channel is below */
#define SECOND_CHANNEL_BELOW    0x30
/** secondary channel is above */
#define SECOND_CHANNEL_ABOVE    0x10
/** no secondary channel */
#define SECONDARY_CHANNEL_NONE     0x00
/** deauth reason code */
#define  REASON_CODE_DEAUTH_LEAVING 3
/********************************************************
				Local Variables
********************************************************/

/********************************************************
				Global Variables
********************************************************/

/********************************************************
				Local Functions
********************************************************/

/********************************************************
				Global Functions
********************************************************/

/**
 * @brief send deauth to station
 *
 * @param                 A pointer to moal_private
 * @param mac			  A pointer to station mac address
 * @param reason_code     ieee deauth reason code
 * @return                0 -- success, otherwise fail
 */
static int
woal_deauth_station(moal_private *priv, u8 *mac_addr, u16 reason_code)
{
	mlan_ioctl_req *ioctl_req = NULL;
	mlan_ds_bss *bss = NULL;
	int ret = 0;
	mlan_status status = MLAN_STATUS_SUCCESS;

	ENTER();

	ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
	if (ioctl_req == NULL) {
		ret = -ENOMEM;
		goto done;
	}
	bss = (mlan_ds_bss *)ioctl_req->pbuf;
	bss->sub_command = MLAN_OID_UAP_DEAUTH_STA;
	ioctl_req->req_id = MLAN_IOCTL_BSS;
	ioctl_req->action = MLAN_ACT_SET;

	memcpy(bss->param.deauth_param.mac_addr, mac_addr,
	       MLAN_MAC_ADDR_LENGTH);
	bss->param.deauth_param.reason_code = reason_code;
	status = woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT);
	if (status != MLAN_STATUS_SUCCESS) {
		ret = -EFAULT;
		goto done;
	}

done:
	if (status != MLAN_STATUS_PENDING)
		kfree(ioctl_req);
	LEAVE();
	return ret;
}

/**
 * @brief send deauth to all station
 *
 * @param                 A pointer to moal_private
 * @param mac			  A pointer to station mac address
 *
 * @return                0 -- success, otherwise fail
 */
static int
woal_deauth_all_station(moal_private *priv)
{
	int ret = -EFAULT;
	int i = 0;
	mlan_ds_get_info *info = NULL;
	mlan_ioctl_req *ioctl_req = NULL;
	mlan_status status = MLAN_STATUS_SUCCESS;

	ENTER();
	if (priv->media_connected == MFALSE) {
		PRINTM(MINFO, "cfg80211: Media not connected!\n");
		LEAVE();
		return 0;
	}
	PRINTM(MIOCTL, "del all station\n");
	/* Allocate an IOCTL request buffer */
	ioctl_req =
		(mlan_ioctl_req *)
		woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
	if (ioctl_req == NULL) {
		ret = -ENOMEM;
		goto done;
	}

	info = (mlan_ds_get_info *)ioctl_req->pbuf;
	info->sub_command = MLAN_OID_UAP_STA_LIST;
	ioctl_req->req_id = MLAN_IOCTL_GET_INFO;
	ioctl_req->action = MLAN_ACT_GET;

	status = woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT);
	if (status != MLAN_STATUS_SUCCESS)
		goto done;
	if (!info->param.sta_list.sta_count)
		goto done;
	for (i = 0; i < info->param.sta_list.sta_count; i++) {
		PRINTM(MIOCTL, "deauth station " MACSTR "\n",
		       MAC2STR(info->param.sta_list.info[i].mac_address));
		ret = woal_deauth_station(priv,
					  info->param.sta_list.info[i].
					  mac_address,
					  REASON_CODE_DEAUTH_LEAVING);
	}
	woal_sched_timeout(200);
done:
	if (status != MLAN_STATUS_PENDING)
		kfree(ioctl_req);
	return ret;
}

/**
 * @brief Verify RSN IE
 *
 * @param rsn_ie          Pointer RSN IE
 * @param sys_config      Pointer to mlan_uap_bss_param structure
 *
 * @return                MTRUE/MFALSE
 */
static t_u8
woal_check_rsn_ie(IEEEtypes_Rsn_t *rsn_ie, mlan_uap_bss_param *sys_config)
{
	int left = 0;
	int count = 0;
	int i = 0;
	wpa_suite_auth_key_mgmt_t *key_mgmt = NULL;
	left = rsn_ie->len + 2;
	if (left < sizeof(IEEEtypes_Rsn_t))
		return MFALSE;
	sys_config->wpa_cfg.group_cipher = 0;
	sys_config->wpa_cfg.pairwise_cipher_wpa2 = 0;
	sys_config->key_mgmt = 0;
	/* check the group cipher */
	switch (rsn_ie->group_cipher.type) {
	case WPA_CIPHER_TKIP:
		sys_config->wpa_cfg.group_cipher = CIPHER_TKIP;
		break;
	case WPA_CIPHER_AES_CCM:
		sys_config->wpa_cfg.group_cipher = CIPHER_AES_CCMP;
		break;
	default:
		break;
	}
	count = le16_to_cpu(rsn_ie->pairwise_cipher.count);
	for (i = 0; i < count; i++) {
		switch (rsn_ie->pairwise_cipher.list[i].type) {
		case WPA_CIPHER_TKIP:
			sys_config->wpa_cfg.pairwise_cipher_wpa2 |= CIPHER_TKIP;
			break;
		case WPA_CIPHER_AES_CCM:
			sys_config->wpa_cfg.pairwise_cipher_wpa2 |=
				CIPHER_AES_CCMP;
			break;
		default:
			break;
		}
	}
	left -= sizeof(IEEEtypes_Rsn_t) + (count - 1) * sizeof(wpa_suite);
	if (left < sizeof(wpa_suite_auth_key_mgmt_t))
		return MFALSE;
	key_mgmt =
		(wpa_suite_auth_key_mgmt_t *)((u8 *)rsn_ie +
					      sizeof(IEEEtypes_Rsn_t) + (count -
									 1) *
					      sizeof(wpa_suite));
	count = le16_to_cpu(key_mgmt->count);
	if (left <
	    (sizeof(wpa_suite_auth_key_mgmt_t) +
	     (count - 1) * sizeof(wpa_suite)))
		return MFALSE;
	for (i = 0; i < count; i++) {
		switch (key_mgmt->list[i].type) {
		case RSN_AKM_8021X:
			sys_config->key_mgmt |= KEY_MGMT_EAP;
			break;
		case RSN_AKM_PSK:
			sys_config->key_mgmt |= KEY_MGMT_PSK;
			break;
		case RSN_AKM_PSK_SHA256:
			sys_config->key_mgmt |= KEY_MGMT_PSK_SHA256;
			break;
		}
	}
	return MTRUE;
}

/**
 * @brief Verify WPA IE
 *
 * @param wpa_ie          Pointer WPA IE
 * @param sys_config      Pointer to mlan_uap_bss_param structure
 *
 * @return                MTRUE/MFALSE
 */
static t_u8
woal_check_wpa_ie(IEEEtypes_Wpa_t *wpa_ie, mlan_uap_bss_param *sys_config)
{
	int left = 0;
	int count = 0;
	int i = 0;
	wpa_suite_auth_key_mgmt_t *key_mgmt = NULL;
	left = wpa_ie->len + 2;
	if (left < sizeof(IEEEtypes_Wpa_t))
		return MFALSE;
	sys_config->wpa_cfg.group_cipher = 0;
	sys_config->wpa_cfg.pairwise_cipher_wpa = 0;
	switch (wpa_ie->group_cipher.type) {
	case WPA_CIPHER_TKIP:
		sys_config->wpa_cfg.group_cipher = CIPHER_TKIP;
		break;
	case WPA_CIPHER_AES_CCM:
		sys_config->wpa_cfg.group_cipher = CIPHER_AES_CCMP;
		break;
	default:
		break;
	}
	count = le16_to_cpu(wpa_ie->pairwise_cipher.count);
	for (i = 0; i < count; i++) {
		switch (wpa_ie->pairwise_cipher.list[i].type) {
		case WPA_CIPHER_TKIP:
			sys_config->wpa_cfg.pairwise_cipher_wpa |= CIPHER_TKIP;
			break;
		case WPA_CIPHER_AES_CCM:
			sys_config->wpa_cfg.pairwise_cipher_wpa |=
				CIPHER_AES_CCMP;
			break;
		default:
			break;
		}
	}
	left -= sizeof(IEEEtypes_Wpa_t) + (count - 1) * sizeof(wpa_suite);
	if (left < sizeof(wpa_suite_auth_key_mgmt_t))
		return MFALSE;
	key_mgmt =
		(wpa_suite_auth_key_mgmt_t *)((u8 *)wpa_ie +
					      sizeof(IEEEtypes_Wpa_t) + (count -
									 1) *
					      sizeof(wpa_suite));
	count = le16_to_cpu(key_mgmt->count);
	if (left <
	    (sizeof(wpa_suite_auth_key_mgmt_t) +
	     (count - 1) * sizeof(wpa_suite)))
		return MFALSE;
	for (i = 0; i < count; i++) {
		switch (key_mgmt->list[i].type) {
		case RSN_AKM_8021X:
			sys_config->key_mgmt = KEY_MGMT_EAP;
			break;
		case RSN_AKM_PSK:
			sys_config->key_mgmt = KEY_MGMT_PSK;
			break;
		}
	}
	return MTRUE;
}

/**
 * @brief Find RSN/WPA IES
 *
 * @param ie              Pointer IE buffer
 * @param sys_config      Pointer to mlan_uap_bss_param structure
 *
 * @return                MTRUE/MFALSE
 */
static t_u8
woal_find_wpa_ies(const t_u8 *ie, int len, mlan_uap_bss_param *sys_config)
{
	int bytes_left = len;
	const t_u8 *pcurrent_ptr = ie;
	t_u16 total_ie_len;
	t_u8 element_len;
	t_u8 wpa2 = 0;
	t_u8 wpa = 0;
	t_u8 ret = MFALSE;
	IEEEtypes_ElementId_e element_id;
	IEEEtypes_VendorSpecific_t *pvendor_ie;
	const t_u8 wpa_oui[4] = { 0x00, 0x50, 0xf2, 0x01 };

	while (bytes_left >= 2) {
		element_id = (IEEEtypes_ElementId_e)(*((t_u8 *)pcurrent_ptr));
		element_len = *((t_u8 *)pcurrent_ptr + 1);
		total_ie_len = element_len + sizeof(IEEEtypes_Header_t);
		if (bytes_left < total_ie_len) {
			PRINTM(MERROR,
			       "InterpretIE: Error in processing IE, bytes left < IE length\n");
			bytes_left = 0;
			continue;
		}
		switch (element_id) {
		case RSN_IE:
			wpa2 = woal_check_rsn_ie((IEEEtypes_Rsn_t *)
						 pcurrent_ptr, sys_config);
			break;
		case VENDOR_SPECIFIC_221:
			pvendor_ie = (IEEEtypes_VendorSpecific_t *)pcurrent_ptr;
			if (!memcmp
			    (pvendor_ie->vend_hdr.oui, wpa_oui,
			     sizeof(pvendor_ie->vend_hdr.oui)) &&
			    (pvendor_ie->vend_hdr.oui_type == wpa_oui[3])) {
				wpa = woal_check_wpa_ie((IEEEtypes_Wpa_t *)
							pcurrent_ptr,
							sys_config);
			}
			break;
		default:
			break;
		}
		pcurrent_ptr += element_len + 2;
		/* Need to account for IE ID and IE Len */
		bytes_left -= (element_len + 2);
	}
	if (wpa && wpa2) {
		sys_config->protocol = PROTOCOL_WPA | PROTOCOL_WPA2;
		ret = MTRUE;
	} else if (wpa2) {
		sys_config->protocol = PROTOCOL_WPA2;
		ret = MTRUE;
	} else if (wpa) {
		sys_config->protocol = PROTOCOL_WPA;
		ret = MTRUE;
	}
	return ret;
}

/**
 * @brief Find and set WMM IES
 *
 * @param ie              Pointer IE buffer
 * @param sys_config      Pointer to mlan_uap_bss_param structure
 *
 * @return                N/A
 */
static t_void
woal_set_wmm_ies(const t_u8 *ie, int len, mlan_uap_bss_param *sys_config)
{
	int bytes_left = len;
	const t_u8 *pcurrent_ptr = ie;
	t_u16 total_ie_len;
	t_u8 element_len;
	IEEEtypes_VendorSpecific_t *pvendor_ie;
	IEEEtypes_ElementId_e element_id;
	const t_u8 wmm_oui[4] = { 0x00, 0x50, 0xf2, 0x02 };

	while (bytes_left >= 2) {
		element_id = (IEEEtypes_ElementId_e)(*((t_u8 *)pcurrent_ptr));
		element_len = *((t_u8 *)pcurrent_ptr + 1);
		total_ie_len = element_len + sizeof(IEEEtypes_Header_t);
		if (bytes_left < total_ie_len) {
			PRINTM(MERROR,
			       "InterpretIE: Error in processing IE, bytes left < IE length\n");
			bytes_left = 0;
			continue;
		}
		switch (element_id) {
		case VENDOR_SPECIFIC_221:
			pvendor_ie = (IEEEtypes_VendorSpecific_t *)pcurrent_ptr;
			if (!memcmp
			    (pvendor_ie->vend_hdr.oui, wmm_oui,
			     sizeof(wmm_oui))) {
				if (total_ie_len ==
				    sizeof(IEEEtypes_WmmParameter_t)) {
					/*
					 * Only accept and copy the WMM IE if
					 * it matches the size expected for the
					 * WMM Parameter IE.
					 */
					memcpy(&sys_config->wmm_para,
					       pcurrent_ptr +
					       sizeof(IEEEtypes_Header_t),
					       element_len);
				}
			}

			break;
		default:
			break;
		}
		pcurrent_ptr += element_len + 2;
		/* Need to account for IE ID and IE Len */
		bytes_left -= (element_len + 2);
	}

}

/**
 * @brief get ht_cap from beacon ie
 *
 * @param ie              Pointer to IEs
 * @param len             Total length of ie
 *
 * @return                ht_cap
 */
static t_u16
woal_get_htcap_from_beacon_ies(const t_u8 *ie, int len)
{
	int left_len = len;
	t_u8 *pos = (t_u8 *)ie;
	int length;
	t_u8 id = 0;
	IEEEtypes_HTCap_t *phtcap_ie = NULL;

	/* HT_CAPABILITY will be fileter out */
	while (left_len >= 2) {
		length = *(pos + 1);
		id = *pos;
		if ((length + 2) > left_len)
			break;
		if (HT_CAPABILITY == id) {
			phtcap_ie = (IEEEtypes_HTCap_t *)pos;
			PRINTM(MINFO, "Get ht_cap from beacon ies: 0x%x\n",
			       phtcap_ie->ht_cap.ht_cap_info);
			return phtcap_ie->ht_cap.ht_cap_info;
		}
		pos += (length + 2);
		left_len -= (length + 2);
	}
	return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
/**
 * @brief initialize AP or GO bss config
 *
 * @param priv            A pointer to moal private structure
 * @param params          A pointer to cfg80211_ap_settings structure
 * @return                0 -- success, otherwise fail
 */
static int
woal_cfg80211_beacon_config(moal_private *priv,
			    struct cfg80211_ap_settings *params)
#else
/**
 * @brief initialize AP or GO bss config
 *
 * @param priv            A pointer to moal private structure
 * @param params          A pointer to beacon_parameters structure
 * @return                0 -- success, otherwise fail
 */
static int
woal_cfg80211_beacon_config(moal_private *priv,
			    struct beacon_parameters *params)
#endif
{
	struct wiphy *wiphy = NULL;
	int ret = 0;
	mlan_uap_bss_param sys_config;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0) || defined(COMPAT_WIRELESS)
	int i = 0;
#else
	t_u8 wpa_ies;
	const t_u8 *ssid_ie = NULL;
	struct ieee80211_mgmt *head = NULL;
	t_u16 capab_info = 0;
#endif
	t_u8 rates_bg[13] = {
		0x82, 0x84, 0x8b, 0x96,
		0x0c, 0x12, 0x18, 0x24,
		0x30, 0x48, 0x60, 0x6c,
		0x00
	};
	t_u8 chan2Offset = 0;
	t_u8 enable_11n = MTRUE;
	t_u16 ht_cap = 0;
	ENTER();

	if (params == NULL) {
		ret = -EFAULT;
		goto done;
	}

	wiphy = priv->phandle->wiphy;
	if (priv->bss_type != MLAN_BSS_TYPE_UAP) {
		ret = -EFAULT;
		goto done;
	}

	/* Initialize the uap bss values which are uploaded from firmware */
	if (MLAN_STATUS_SUCCESS != woal_set_get_sys_config(priv,
							   MLAN_ACT_GET,
							   MOAL_IOCTL_WAIT,
							   &sys_config)) {
		PRINTM(MERROR, "Error getting AP confiruration\n");
		ret = -EFAULT;
		goto done;
	}

	/* Setting the default values */
	sys_config.channel = 6;
	sys_config.preamble_type = 0;
	sys_config.mgmt_ie_passthru_mask = priv->mgmt_subtype_mask;
	memcpy(sys_config.mac_addr, priv->current_addr, ETH_ALEN);

	/* Set frag_threshold, rts_threshold, and retry limit */
	sys_config.frag_threshold = wiphy->frag_threshold;
	sys_config.rts_threshold = wiphy->rts_threshold;
	sys_config.retry_limit = wiphy->retry_long;
	if (sys_config.frag_threshold == MLAN_FRAG_RTS_DISABLED) {
		sys_config.frag_threshold = MLAN_FRAG_MAX_VALUE;
	}
	if (sys_config.rts_threshold == MLAN_FRAG_RTS_DISABLED) {
		sys_config.rts_threshold = MLAN_RTS_MAX_VALUE;
	}

	if (priv->bss_type == MLAN_BSS_TYPE_UAP) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
		if (params->beacon_interval)
			sys_config.beacon_period = params->beacon_interval;
#else
		if (params->interval)
			sys_config.beacon_period = params->interval;
#endif
		if (params->dtim_period)
			sys_config.dtim_period = params->dtim_period;
	}
	if (priv->channel) {
		memset(sys_config.rates, 0, sizeof(sys_config.rates));
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
		switch (params->chandef.width) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
		case NL80211_CHAN_WIDTH_5:
		case NL80211_CHAN_WIDTH_10:
#endif
		case NL80211_CHAN_WIDTH_20_NOHT:
			enable_11n = MFALSE;
			break;
		case NL80211_CHAN_WIDTH_20:
			break;
		case NL80211_CHAN_WIDTH_40:
			if (params->chandef.center_freq1 <
			    params->chandef.chan->center_freq)
				chan2Offset = SECOND_CHANNEL_BELOW;
			else
				chan2Offset = SECOND_CHANNEL_ABOVE;
			break;
		default:
			PRINTM(MWARN, "Unknown channel width: %d\n",
			       params->chandef.width);
			break;
		}
#else
		switch (params->channel_type) {
		case NL80211_CHAN_NO_HT:
			enable_11n = MFALSE;
			break;
		case NL80211_CHAN_HT20:
			break;
		case NL80211_CHAN_HT40PLUS:
			chan2Offset = SECOND_CHANNEL_ABOVE;
			break;
		case NL80211_CHAN_HT40MINUS:
			chan2Offset = SECOND_CHANNEL_BELOW;
			break;
		default:
			PRINTM(MWARN, "Unknown channel type: %d\n",
			       params->channel_type);
			break;
		}
#endif
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0) */
		sys_config.channel = priv->channel;
		if (priv->channel <= MAX_BG_CHANNEL) {
			sys_config.band_cfg = BAND_CONFIG_2G;
			memcpy(sys_config.rates, rates_bg, sizeof(rates_bg));
		}
		/* Disable GreenField by default */
		sys_config.ht_cap_info = 0x10c;
		if (enable_11n)
			sys_config.ht_cap_info |= 0x20;
		if (chan2Offset) {
			sys_config.band_cfg |= chan2Offset;
			sys_config.ht_cap_info |= 0x1042;
			sys_config.ampdu_param = 3;
		}
		if (priv->bss_type == MLAN_BSS_TYPE_UAP) {
			ht_cap = woal_get_htcap_from_beacon_ies(
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
								       params->
								       beacon.
								       tail,
								       params->
								       beacon.
								       tail_len
#else
								       params->
								       tail,
								       params->
								       tail_len
#endif
				);
			if (ht_cap)
				sys_config.ht_cap_info =
					ht_cap & (wiphy->
						  bands[IEEE80211_BAND_2GHZ]->
						  ht_cap.cap & 0x10f3);
		}
		PRINTM(MCMND,
		       "11n=%d, ht_cap=0x%x, channel=%d, band_cfg=0x%x\n",
		       enable_11n, sys_config.ht_cap_info, priv->channel,
		       sys_config.band_cfg);
	}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0) || defined(COMPAT_WIRELESS)
	if (!params->ssid || !params->ssid_len) {
		ret = -EINVAL;
		goto done;
	}
	memcpy(sys_config.ssid.ssid, params->ssid,
	       MIN(MLAN_MAX_SSID_LENGTH, params->ssid_len));
	sys_config.ssid.ssid_len = MIN(MLAN_MAX_SSID_LENGTH, params->ssid_len);
	if (params->hidden_ssid)
		sys_config.bcast_ssid_ctl = 0;
	else
		sys_config.bcast_ssid_ctl = 1;
	if (params->auth_type == NL80211_AUTHTYPE_SHARED_KEY)
		sys_config.auth_mode = MLAN_AUTH_MODE_SHARED;
	else
		sys_config.auth_mode = MLAN_AUTH_MODE_OPEN;
	if (params->crypto.n_akm_suites) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
		woal_find_wpa_ies(params->beacon.tail,
				  (int)params->beacon.tail_len, &sys_config);
#else
		woal_find_wpa_ies(params->tail, params->tail_len, &sys_config);
#endif
	}
	for (i = 0; i < params->crypto.n_akm_suites; i++) {
		switch (params->crypto.akm_suites[i]) {
		case WLAN_AKM_SUITE_8021X:
			sys_config.key_mgmt |= KEY_MGMT_EAP;
			if ((params->crypto.
			     wpa_versions & NL80211_WPA_VERSION_1) &&
			    (params->crypto.
			     wpa_versions & NL80211_WPA_VERSION_2))
				sys_config.protocol =
					PROTOCOL_WPA | PROTOCOL_WPA2;
			else if (params->crypto.
				 wpa_versions & NL80211_WPA_VERSION_2)
				sys_config.protocol = PROTOCOL_WPA2;
			else if (params->crypto.
				 wpa_versions & NL80211_WPA_VERSION_1)
				sys_config.protocol = PROTOCOL_WPA;
			break;
		case WLAN_AKM_SUITE_PSK:
			sys_config.key_mgmt |= KEY_MGMT_PSK;
			if ((params->crypto.
			     wpa_versions & NL80211_WPA_VERSION_1) &&
			    (params->crypto.
			     wpa_versions & NL80211_WPA_VERSION_2))
				sys_config.protocol =
					PROTOCOL_WPA | PROTOCOL_WPA2;
			else if (params->crypto.
				 wpa_versions & NL80211_WPA_VERSION_2)
				sys_config.protocol = PROTOCOL_WPA2;
			else if (params->crypto.
				 wpa_versions & NL80211_WPA_VERSION_1)
				sys_config.protocol = PROTOCOL_WPA;
			break;
		}
	}
	sys_config.wpa_cfg.pairwise_cipher_wpa = 0;
	sys_config.wpa_cfg.pairwise_cipher_wpa2 = 0;
	for (i = 0; i < params->crypto.n_ciphers_pairwise; i++) {
		switch (params->crypto.ciphers_pairwise[i]) {
		case WLAN_CIPHER_SUITE_WEP40:
		case WLAN_CIPHER_SUITE_WEP104:
			break;
		case WLAN_CIPHER_SUITE_TKIP:
			if (params->crypto.wpa_versions & NL80211_WPA_VERSION_1)
				sys_config.wpa_cfg.pairwise_cipher_wpa |=
					CIPHER_TKIP;
			if (params->crypto.wpa_versions & NL80211_WPA_VERSION_2)
				sys_config.wpa_cfg.pairwise_cipher_wpa2 |=
					CIPHER_TKIP;
			break;
		case WLAN_CIPHER_SUITE_CCMP:
			if (params->crypto.wpa_versions & NL80211_WPA_VERSION_1)
				sys_config.wpa_cfg.pairwise_cipher_wpa |=
					CIPHER_AES_CCMP;
			if (params->crypto.wpa_versions & NL80211_WPA_VERSION_2)
				sys_config.wpa_cfg.pairwise_cipher_wpa2 |=
					CIPHER_AES_CCMP;
			break;
		}
	}
	switch (params->crypto.cipher_group) {
	case WLAN_CIPHER_SUITE_WEP40:
	case WLAN_CIPHER_SUITE_WEP104:
		if ((priv->cipher == WLAN_CIPHER_SUITE_WEP40) ||
		    (priv->cipher == WLAN_CIPHER_SUITE_WEP104)) {
			sys_config.protocol = PROTOCOL_STATIC_WEP;
			sys_config.key_mgmt = KEY_MGMT_NONE;
			sys_config.wpa_cfg.length = 0;
			memcpy(&sys_config.wep_cfg.key0, &priv->uap_wep_key[0],
			       sizeof(wep_key));
			memcpy(&sys_config.wep_cfg.key1, &priv->uap_wep_key[1],
			       sizeof(wep_key));
			memcpy(&sys_config.wep_cfg.key2, &priv->uap_wep_key[2],
			       sizeof(wep_key));
			memcpy(&sys_config.wep_cfg.key3, &priv->uap_wep_key[3],
			       sizeof(wep_key));
		}
		break;
	case WLAN_CIPHER_SUITE_TKIP:
		sys_config.wpa_cfg.group_cipher = CIPHER_TKIP;
		break;
	case WLAN_CIPHER_SUITE_CCMP:
		sys_config.wpa_cfg.group_cipher = CIPHER_AES_CCMP;
		break;
	}
#else
	/* Since in Android ICS 4.0.1's wpa_supplicant, there is no way to set
	   ssid when GO (AP) starts up, so get it from beacon head parameter
	   TODO: right now use hard code 24 -- ieee80211 header lenth, 12 --
	   fixed element length for beacon */
#define BEACON_IE_OFFSET	36
	/* Find SSID in head SSID IE id: 0, right now use hard code */
	ssid_ie = woal_parse_ie_tlv(params->head + BEACON_IE_OFFSET,
				    params->head_len - BEACON_IE_OFFSET, 0);

	if (!ssid_ie) {
		PRINTM(MERROR, "No ssid IE found.\n");
		ret = -EFAULT;
		goto done;
	}
	if (*(ssid_ie + 1) > 32) {
		PRINTM(MERROR, "ssid len error: %d\n", *(ssid_ie + 1));
		ret = -EFAULT;
		goto done;
	}
	memcpy(sys_config.ssid.ssid, ssid_ie + 2, *(ssid_ie + 1));
	sys_config.ssid.ssid_len = *(ssid_ie + 1);
	head = (struct ieee80211_mgmt *)params->head;

	capab_info = le16_to_cpu(head->u.beacon.capab_info);
	PRINTM(MIOCTL, "capab_info=0x%x\n", head->u.beacon.capab_info);
	sys_config.auth_mode = MLAN_AUTH_MODE_OPEN;
	/** For ICS, we don't support OPEN mode */
	if ((priv->cipher == WLAN_CIPHER_SUITE_WEP40) ||
	    (priv->cipher == WLAN_CIPHER_SUITE_WEP104)) {
		sys_config.protocol = PROTOCOL_STATIC_WEP;
		sys_config.key_mgmt = KEY_MGMT_NONE;
		sys_config.wpa_cfg.length = 0;
		memcpy(&sys_config.wep_cfg.key0, &priv->uap_wep_key[0],
		       sizeof(wep_key));
		memcpy(&sys_config.wep_cfg.key1, &priv->uap_wep_key[1],
		       sizeof(wep_key));
		memcpy(&sys_config.wep_cfg.key2, &priv->uap_wep_key[2],
		       sizeof(wep_key));
		memcpy(&sys_config.wep_cfg.key3, &priv->uap_wep_key[3],
		       sizeof(wep_key));
	} else {
		/** Get cipher and key_mgmt from RSN/WPA IE */
		if (capab_info & WLAN_CAPABILITY_PRIVACY) {
			wpa_ies =
				woal_find_wpa_ies(params->tail,
						  params->tail_len,
						  &sys_config);
			if (wpa_ies == MFALSE) {
				/* hard code setting to wpa2-psk */
				sys_config.protocol = PROTOCOL_WPA2;
				sys_config.key_mgmt = KEY_MGMT_PSK;
				sys_config.wpa_cfg.pairwise_cipher_wpa2 =
					CIPHER_AES_CCMP;
				sys_config.wpa_cfg.group_cipher =
					CIPHER_AES_CCMP;
			}
		}
	}
#endif /* COMPAT_WIRELESS */

	if (priv->bss_type == MLAN_BSS_TYPE_UAP) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
		/* find and set wmm ie */
		woal_set_wmm_ies(params->beacon.tail,
				 (int)params->beacon.tail_len, &sys_config);
#else
		woal_set_wmm_ies(params->tail, params->tail_len, &sys_config);
#endif
	}
	/* If the security mode is configured as WEP or WPA-PSK, it will
	   disable 11n automatically, and if configured as open(off) or
	   wpa2-psk, it will automatically enable 11n */
	if ((sys_config.protocol == PROTOCOL_STATIC_WEP) ||
	    (sys_config.protocol == PROTOCOL_WPA))
		enable_11n = MFALSE;
	if (!enable_11n) {
		woal_set_uap_ht_tx_cfg(priv, sys_config.band_cfg, MFALSE);
		woal_uap_set_11n_status(&sys_config, MLAN_ACT_DISABLE);
	} else {
		woal_set_uap_ht_tx_cfg(priv, sys_config.band_cfg, MTRUE);
		woal_uap_set_11n_status(&sys_config, MLAN_ACT_ENABLE);
		woal_set_get_tx_bf_cap(priv, MLAN_ACT_GET,
				       &sys_config.tx_bf_cap);
	}
	if (MLAN_STATUS_SUCCESS != woal_set_get_sys_config(priv,
							   MLAN_ACT_SET,
							   MOAL_IOCTL_WAIT,
							   &sys_config)) {
		ret = -EFAULT;
		goto done;
	}
done:
	LEAVE();
	return ret;
}

/**
 *  @brief This function check if uap interface is ready
 *
 * @param wiphy           A pointer to wiphy structure
 * @param name            Virtual interface name
 *
 *  @return        MTRUE/MFALSE;
 */
static t_u8
woal_uap_interface_ready(struct wiphy *wiphy, char *name,
			 struct net_device **new_dev)
{
	moal_handle *handle = (moal_handle *)woal_get_wiphy_priv(wiphy);
	moal_private *priv = NULL;
	int i;

	for (i = 0; i < handle->priv_num; i++) {
		priv = handle->priv[i];
		if (priv && (priv->bss_type == MLAN_BSS_TYPE_UAP) &&
		    !strcmp(priv->netdev->name, name)) {
			priv->wdev->iftype = NL80211_IFTYPE_AP;
			*new_dev = priv->netdev;
			break;
		}
	}
	if (priv)
		return MTRUE;
	else
		return MFALSE;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 37) || defined(COMPAT_WIRELESS)
/**
 * @brief Request the driver to add a virtual interface
 *
 * @param wiphy           A pointer to wiphy structure
 * @param name            Virtual interface name
 * @param type            Virtual interface type
 * @param flags           Flags for the virtual interface
 * @param params          A pointer to vif_params structure
 *
 * @return                A pointer to net_device -- success, otherwise null
 */
struct net_device *
woal_cfg80211_add_virtual_intf(struct wiphy *wiphy,
			       char *name, enum nl80211_iftype type, u32 *flags,
			       struct vif_params *params)
#else
/**
 * @brief Request the driver to add a virtual interface
 *
 * @param wiphy           A pointer to wiphy structure
 * @param name            Virtual interface name
 * @param type            Virtual interface type
 * @param flags           Flags for the virtual interface
 * @param params          A pointer to vif_params structure
 *
 * @return                0 -- success, otherwise fail
 */
int
woal_cfg80211_add_virtual_intf(struct wiphy *wiphy,
			       char *name, enum nl80211_iftype type, u32 *flags,
			       struct vif_params *params)
#endif
#else
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 1, 0)
/**
 * @brief Request the driver to add a virtual interface
 *
 * @param wiphy           A pointer to wiphy structure
 * @param name            Virtual interface name
 * @param type            Virtual interface type
 * @param flags           Flags for the virtual interface
 * @param params          A pointer to vif_params structure
 *
 * @return                A pointer to wireless_dev -- success, otherwise null
 */
struct wireless_dev *
woal_cfg80211_add_virtual_intf(struct wiphy *wiphy,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0)
			       const
#endif
			       char *name, enum nl80211_iftype type, u32 *flags,
			       struct vif_params *params)
#else
/**
 * @brief Request the driver to add a virtual interface
 *
 * @param wiphy             A pointer to wiphy structure
 * @param name              Virtual interface name
 * @param name_assign_type  Interface name assignment type
 * @param type              Virtual interface type
 * @param flags             Flags for the virtual interface
 * @param params            A pointer to vif_params structure
 *
 * @return                  A pointer to wireless_dev -- success, otherwise null
 */
struct wireless_dev *
woal_cfg80211_add_virtual_intf(struct wiphy *wiphy,
			       const char *name,
			       unsigned char name_assign_type,
			       enum nl80211_iftype type, u32 *flags,
			       struct vif_params *params)
#endif
#endif
{
	struct net_device *ndev = NULL;
	int ret = 0;

	ENTER();
	PRINTM(MIOCTL, "add virtual intf: %d name: %s\n", type, name);
	switch (type) {
	case NL80211_IFTYPE_AP:
		if (!woal_uap_interface_ready(wiphy, (char *)name, &ndev)) {
			PRINTM(MMSG,
			       "Not support dynamically create %s UAP interface\n",
			       name);
			ret = -EFAULT;
		}
		break;
	default:
		PRINTM(MWARN, "Not supported if type: %d\n", type);
		ret = -EFAULT;
		break;
	}
	LEAVE();
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 37) || defined(COMPAT_WIRELESS)
	if (ret)
		return ERR_PTR(ret);
	else
		return ndev;
#else
	return ret;
#endif
#else
	if (ret)
		return ERR_PTR(ret);
	else
		return ndev->ieee80211_ptr;
#endif
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)
/**
 * @brief Request the driver to del a virtual interface
 *
 * @param wiphy           A pointer to wiphy structure
 * @param dev             The pointer to net_device
 *
 * @return               0 -- success, otherwise fail
 */
int
woal_cfg80211_del_virtual_intf(struct wiphy *wiphy, struct net_device *dev)
#else
/**
 * @brief Request the driver to del a virtual interface
 *
 * @param wiphy           A pointer to wiphy structure
 * @param wdev            The pointer to wireless_dev
 *
 * @return               0 -- success, otherwise fail
 */
int
woal_cfg80211_del_virtual_intf(struct wiphy *wiphy, struct wireless_dev *wdev)
#endif
{
	int ret = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
	struct net_device *dev = wdev->netdev;
#endif
	ENTER();

	PRINTM(MIOCTL, "del virtual intf %s\n", dev->name);
	ASSERT_RTNL();
	LEAVE();
	return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
/**
 * @brief initialize AP or GO parameters

 *
 * @param wiphy           A pointer to wiphy structure
 * @param dev             A pointer to net_device structure
 * @param params          A pointer to cfg80211_ap_settings structure
 * @return                0 -- success, otherwise fail
 */
int
woal_cfg80211_add_beacon(struct wiphy *wiphy,
			 struct net_device *dev,
			 struct cfg80211_ap_settings *params)
#else
/**
 * @brief initialize AP or GO parameters

 *
 * @param wiphy           A pointer to wiphy structure
 * @param dev             A pointer to net_device structure
 * @param params          A pointer to beacon_parameters structure
 * @return                0 -- success, otherwise fail
 */
int
woal_cfg80211_add_beacon(struct wiphy *wiphy,
			 struct net_device *dev,
			 struct beacon_parameters *params)
#endif
{
	moal_private *priv = (moal_private *)woal_get_netdev_priv(dev);
	int ret = 0;

	ENTER();

	PRINTM(MMSG, "wlan: Starting AP\n");
#ifdef STA_CFG80211
	/*** cancel pending scan */
	woal_cancel_scan(priv, MOAL_IOCTL_WAIT);
#endif
	if (!params) {
		LEAVE();
		return -EFAULT;
	}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
	priv->channel =
		ieee80211_frequency_to_channel(params->chandef.chan->
					       center_freq);
#else
	priv->channel =
		ieee80211_frequency_to_channel(params->channel->center_freq);
#endif
#endif
	/* bss config */
	if (MLAN_STATUS_SUCCESS != woal_cfg80211_beacon_config(priv, params)) {
		ret = -EFAULT;
		goto done;
	}

	/* set mgmt frame ies */
	ret = woal_cfg80211_mgmt_frame_ie(priv,
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 2, 0) && !defined(COMPAT_WIRELESS)
					  params->tail, params->tail_len, NULL,
					  0, NULL, 0, NULL, 0, MGMT_MASK_BEACON
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
					  params->beacon.tail,
					  params->beacon.tail_len,
					  params->beacon.proberesp_ies,
					  params->beacon.proberesp_ies_len,
					  params->beacon.assocresp_ies,
					  params->beacon.assocresp_ies_len,
#else
					  params->tail, params->tail_len,
					  params->proberesp_ies,
					  params->proberesp_ies_len,
					  params->assocresp_ies,
					  params->assocresp_ies_len,
#endif
					  NULL, 0,
					  MGMT_MASK_BEACON |
					  MGMT_MASK_PROBE_RESP |
					  MGMT_MASK_ASSOC_RESP
#endif
					  , MOAL_IOCTL_WAIT);
	if (ret)
		goto done;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0) || defined(COMPAT_WIRELESS)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
	if (params->beacon.beacon_ies && params->beacon.beacon_ies_len) {
		ret = woal_cfg80211_mgmt_frame_ie(priv,
						  params->beacon.beacon_ies,
						  params->beacon.beacon_ies_len,
						  NULL, 0, NULL, 0, NULL, 0,
						  MGMT_MASK_BEACON_WPS_P2P,
						  MOAL_IOCTL_WAIT);
		if (ret) {
			PRINTM(MERROR, "Failed to set beacon wps/p2p ie\n");
			goto done;
		}
	}
#else
	if (params->beacon_ies && params->beacon_ies_len) {
		ret = woal_cfg80211_mgmt_frame_ie(priv,
						  params->beacon_ies,
						  params->beacon_ies_len, NULL,
						  0, NULL, 0, NULL, 0,
						  MGMT_MASK_BEACON_WPS_P2P,
						  MOAL_IOCTL_WAIT);
		if (ret) {
			PRINTM(MERROR, "Failed to set beacon wps/p2p ie\n");
			goto done;
		}
	}
#endif
#endif
	priv->uap_host_based = MTRUE;
	/* if the bss is stopped, then start it */
	if (priv->bss_started == MFALSE) {
		if (MLAN_STATUS_SUCCESS !=
		    woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_START)) {
			priv->uap_host_based = MFALSE;
			ret = -EFAULT;
			goto done;
		}
	}
	PRINTM(MMSG, "wlan: AP started\n");
done:
	LEAVE();
	return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
/**
 * @brief set AP or GO parameter
 *
 * @param wiphy           A pointer to wiphy structure
 * @param dev             A pointer to net_device structure
 * @param params          A pointer to cfg80211_beacon_data structure
 * @return                0 -- success, otherwise fail
 */
int
woal_cfg80211_set_beacon(struct wiphy *wiphy,
			 struct net_device *dev,
			 struct cfg80211_beacon_data *params)
#else
/**
 * @brief set AP or GO parameter
 *
 * @param wiphy           A pointer to wiphy structure
 * @param dev             A pointer to net_device structure
 * @param params          A pointer to beacon_parameters structure
 * @return                0 -- success, otherwise fail
 */
int
woal_cfg80211_set_beacon(struct wiphy *wiphy,
			 struct net_device *dev,
			 struct beacon_parameters *params)
#endif
{
	moal_private *priv = (moal_private *)woal_get_netdev_priv(dev);
	int ret = 0;

	ENTER();

	PRINTM(MIOCTL, "set beacon\n");
	if (params != NULL) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 2, 0) && !defined(COMPAT_WIRELESS)
		if (params->tail && params->tail_len) {
			ret = woal_cfg80211_mgmt_frame_ie(priv,
							  params->tail,
							  params->tail_len,
							  NULL, 0, NULL, 0,
							  NULL, 0,
							  MGMT_MASK_BEACON,
							  MOAL_IOCTL_WAIT);
			if (ret)
				goto done;
		}
#else
		t_u16 mask = 0;
		if (params->tail && params->tail_len)
			mask |= MGMT_MASK_BEACON;
		if (params->proberesp_ies && params->proberesp_ies_len)
			mask |= MGMT_MASK_PROBE_RESP;
		if (params->assocresp_ies && params->assocresp_ies_len)
			mask |= MGMT_MASK_ASSOC_RESP;
		PRINTM(MIOCTL, "Set beacon: mask=0x%x\n", mask);
		if (mask) {
			ret = woal_cfg80211_mgmt_frame_ie(priv, params->tail,
							  params->tail_len,
							  params->proberesp_ies,
							  params->
							  proberesp_ies_len,
							  params->assocresp_ies,
							  params->
							  assocresp_ies_len,
							  NULL, 0, mask,
							  MOAL_IOCTL_WAIT);
			if (ret)
				goto done;
		}
		if (params->beacon_ies && params->beacon_ies_len) {
			ret = woal_cfg80211_mgmt_frame_ie(priv,
							  params->beacon_ies,
							  params->
							  beacon_ies_len, NULL,
							  0, NULL, 0, NULL, 0,
							  MGMT_MASK_BEACON_WPS_P2P,
							  MOAL_IOCTL_WAIT);
			if (ret) {
				PRINTM(MERROR,
				       "Failed to set beacon wps/p2p ie\n");
				goto done;
			}
		}
#endif
	}

done:
	LEAVE();
	return ret;
}

/**
 * @brief reset AP or GO parameters
 *
 * @param wiphy           A pointer to wiphy structure
 * @param dev             A pointer to net_device structure
 *
 * @return                0 -- success, otherwise fail
 */
int
woal_cfg80211_del_beacon(struct wiphy *wiphy, struct net_device *dev)
{
	moal_private *priv = (moal_private *)woal_get_netdev_priv(dev);
	int ret = 0;
#ifdef STA_SUPPORT
	moal_private *pmpriv = NULL;
#endif

	ENTER();

	priv->phandle->driver_state = woal_check_driver_status(priv->phandle);
	if (priv->phandle->driver_state) {
		PRINTM(MERROR,
		       "Block  woal_cfg80211_del_beacon in abnormal driver state\n");
		LEAVE();
		return 0;
	}
	if (priv->bss_started != MTRUE) {
		PRINTM(MMSG, "wlan: AP already stopped!");
		LEAVE();
		return 0;
	}
	priv->uap_host_based = MFALSE;
	PRINTM(MMSG, "wlan: Stoping AP\n");
#ifdef STA_SUPPORT
	woal_cancel_scan(priv, MOAL_IOCTL_WAIT);
#endif
	memset(priv->dscp_map, 0xFF, sizeof(priv->dscp_map));
	woal_deauth_all_station(priv);
	/* if the bss is still running, then stop it */
	if (priv->bss_started == MTRUE) {
		if (MLAN_STATUS_SUCCESS !=
		    woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_STOP)) {
			ret = -EFAULT;
			goto done;
		}
		if (MLAN_STATUS_SUCCESS !=
		    woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_RESET)) {
			ret = -EFAULT;
			goto done;
		}
		/* Set WLAN MAC addresses */
		if (MLAN_STATUS_SUCCESS != woal_request_set_mac_address(priv)) {
			PRINTM(MERROR, "Set MAC address failed\n");
			ret = -EFAULT;
			goto done;
		}
	}
	woal_clear_all_mgmt_ies(priv, MOAL_IOCTL_WAIT);
#ifdef STA_SUPPORT
	if (!woal_is_any_interface_active(priv->phandle)) {
		pmpriv = woal_get_priv((moal_handle *)priv->phandle,
				       MLAN_BSS_ROLE_STA);
		if (pmpriv)
			woal_set_scan_time(pmpriv, ACTIVE_SCAN_CHAN_TIME,
					   PASSIVE_SCAN_CHAN_TIME,
					   SPECIFIC_SCAN_CHAN_TIME);
	}
#endif

	priv->cipher = 0;
	memset(priv->uap_wep_key, 0, sizeof(priv->uap_wep_key));
	priv->channel = 0;
	PRINTM(MMSG, "wlan: AP stopped\n");
done:
	LEAVE();
	return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
/**
 * @brief change BSS
 *
 * @param wiphy           A pointer to wiphy structure
 * @param dev             A pointer to net_device structure
 * @param params          A pointer to bss_parameters structure
 *
 * @return                0 -- success, otherwise fail
 */
int
woal_cfg80211_change_bss(struct wiphy *wiphy, struct net_device *dev,
			 struct bss_parameters *params)
{
	int ret = 0;
	t_u8 change = MFALSE;
	mlan_uap_bss_param sys_config;
	u8 bss_started = MFALSE;
	t_u8 pkt_forward_ctl;
	moal_private *priv = (moal_private *)woal_get_netdev_priv(dev);

	ENTER();
	PRINTM(MIOCTL, "isolate=%d\n", params->ap_isolate);

	if (MLAN_STATUS_SUCCESS != woal_set_get_sys_config(priv,
							   MLAN_ACT_GET,
							   MOAL_IOCTL_WAIT,
							   &sys_config)) {
		PRINTM(MERROR, "Error getting AP confiruration\n");
		ret = -EFAULT;
		goto done;
	}

	pkt_forward_ctl = sys_config.pkt_forward_ctl;
	if (params->ap_isolate) {
	/** disable packet forwarding */
		sys_config.pkt_forward_ctl |= PKT_FWD_INTRA_BCAST;
		sys_config.pkt_forward_ctl |= PKT_FWD_INTRA_UCAST;
	} else {
		sys_config.pkt_forward_ctl &= ~PKT_FWD_INTRA_BCAST;
		sys_config.pkt_forward_ctl &= ~PKT_FWD_INTRA_UCAST;
	}
	if (pkt_forward_ctl != sys_config.pkt_forward_ctl) {
		change = MTRUE;
		PRINTM(MIOCTL, "ap_isolate=%xd\n", params->ap_isolate);
	}
	if (change) {
		if (priv->bss_started == MTRUE) {
			bss_started = MTRUE;
			woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_STOP);
		}
		if (MLAN_STATUS_SUCCESS == woal_set_get_sys_config(priv,
								   MLAN_ACT_SET,
								   MOAL_IOCTL_WAIT,
								   &sys_config))
			ret = 0;
		if (bss_started)
			woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_START);
	}
done:
	LEAVE();
	return ret;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0)
/**
 * @brief del station
 *
 * @param wiphy           A pointer to wiphy structure
 * @param dev             A pointer to net_device structure
 * @param param           A pointer tostation_del_parameters structure
 *
 * @return                0 -- success, otherwise fail
 */
#else
/**
 * @brief del station
 *
 * @param wiphy           A pointer to wiphy structure
 * @param dev             A pointer to net_device structure
 * @param mac_addr		  A pointer to station mac address
 *
 * @return                0 -- success, otherwise fail
 */
#endif
int
woal_cfg80211_del_station(struct wiphy *wiphy, struct net_device *dev,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0)
			  struct station_del_parameters *param)
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0)
			  const u8 *mac_addr)
#else
			  u8 *mac_addr)
#endif
#endif
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0)
	const u8 *mac_addr = NULL;
#endif
	u16 reason_code = REASON_CODE_DEAUTH_LEAVING;
	moal_private *priv = (moal_private *)woal_get_netdev_priv(dev);
	ENTER();
	if (priv->media_connected == MFALSE) {
		PRINTM(MINFO, "cfg80211: Media not connected!\n");
		LEAVE();
		return 0;
	}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0)
	if (param) {
		mac_addr = param->mac;
		reason_code = param->reason_code;
	}
#endif
    /** we will not send deauth to p2p interface, it might cause WPS failure */
	if (mac_addr) {
		PRINTM(MMSG, "wlan: deauth station " MACSTR "\n",
		       MAC2STR(mac_addr));
		woal_deauth_station(priv, (u8 *)mac_addr, reason_code);
	} else {
		PRINTM(MIOCTL, "del all station\n");
	}
	LEAVE();
	return 0;

}

/**
 * @brief Get station info
 *
 * @param wiphy           A pointer to wiphy structure
 * @param dev             A pointer to net_device structure
 * @param mac			  A pointer to station mac address
 * @param stainfo		  A pointer to station_info structure
 *
 * @return                0 -- success, otherwise fail
 */
int
woal_uap_cfg80211_get_station(struct wiphy *wiphy, struct net_device *dev,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0)
			      const u8 *mac,
#else
			      u8 *mac,
#endif
			      struct station_info *stainfo)
{
	moal_private *priv = (moal_private *)woal_get_netdev_priv(dev);
	int ret = -EFAULT;
	int i = 0;
	mlan_ds_get_info *info = NULL;
	mlan_ioctl_req *ioctl_req = NULL;
	mlan_status status = MLAN_STATUS_SUCCESS;

	ENTER();
	if (priv->media_connected == MFALSE) {
		PRINTM(MINFO, "cfg80211: Media not connected!\n");
		LEAVE();
		return -ENOENT;
	}

	/* Allocate an IOCTL request buffer */
	ioctl_req =
		(mlan_ioctl_req *)
		woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
	if (ioctl_req == NULL) {
		ret = -ENOMEM;
		goto done;
	}

	info = (mlan_ds_get_info *)ioctl_req->pbuf;
	info->sub_command = MLAN_OID_UAP_STA_LIST;
	ioctl_req->req_id = MLAN_IOCTL_GET_INFO;
	ioctl_req->action = MLAN_ACT_GET;

	status = woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT);
	if (status != MLAN_STATUS_SUCCESS)
		goto done;
	for (i = 0; i < info->param.sta_list.sta_count; i++) {
		if (!memcmp
		    (info->param.sta_list.info[i].mac_address, mac, ETH_ALEN)) {
			PRINTM(MIOCTL, "Get station: " MACSTR " RSSI=%d\n",
			       MAC2STR(mac),
			       (int)info->param.sta_list.info[i].rssi);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0)
			stainfo->filled = BIT(NL80211_STA_INFO_INACTIVE_TIME) |
				BIT(NL80211_STA_INFO_SIGNAL);
#else
			stainfo->filled =
				STATION_INFO_INACTIVE_TIME |
				STATION_INFO_SIGNAL;
#endif
			stainfo->inactive_time = 0;
			stainfo->signal = info->param.sta_list.info[i].rssi;
			ret = 0;
			break;
		}
	}
done:
	if (status != MLAN_STATUS_PENDING)
		kfree(ioctl_req);
	LEAVE();
	return ret;
}

/**
 * @brief Request the driver to dump the station information
 *
 * @param wiphy           A pointer to wiphy structure
 * @param dev             A pointer to net_device structure
 * @param idx             Station index
 * @param mac             MAC address of the station
 * @param sinfo           A pointer to station_info structure
 *
 * @return                0 -- success, otherwise fail
 */
int
woal_uap_cfg80211_dump_station(struct wiphy *wiphy,
			       struct net_device *dev, int idx,
			       t_u8 *mac, struct station_info *sinfo)
{
	moal_private *priv = (moal_private *)woal_get_netdev_priv(dev);
	int ret = -EFAULT;
	mlan_ds_get_info *info = NULL;
	mlan_ioctl_req *ioctl_req = NULL;
	mlan_status status = MLAN_STATUS_SUCCESS;

	ENTER();
	if (priv->media_connected == MFALSE) {
		PRINTM(MINFO, "cfg80211: Media not connected!\n");
		LEAVE();
		return -ENOENT;
	}

	/* Allocate an IOCTL request buffer */
	ioctl_req =
		(mlan_ioctl_req *)
		woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
	if (ioctl_req == NULL) {
		ret = -ENOMEM;
		goto done;
	}

	info = (mlan_ds_get_info *)ioctl_req->pbuf;
	info->sub_command = MLAN_OID_UAP_STA_LIST;
	ioctl_req->req_id = MLAN_IOCTL_GET_INFO;
	ioctl_req->action = MLAN_ACT_GET;

	status = woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT);
	if (status != MLAN_STATUS_SUCCESS)
		goto done;
	if (idx >= info->param.sta_list.sta_count) {
		ret = -EFAULT;
		goto done;
	}
	ret = 0;
	memcpy(mac, info->param.sta_list.info[idx].mac_address, ETH_ALEN);
	PRINTM(MIOCTL, "Dump station: " MACSTR " RSSI=%d\n", MAC2STR(mac),
	       (int)info->param.sta_list.info[idx].rssi);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0)
	sinfo->filled =
		BIT(NL80211_STA_INFO_INACTIVE_TIME) |
		BIT(NL80211_STA_INFO_SIGNAL);
#else
	sinfo->filled = STATION_INFO_INACTIVE_TIME | STATION_INFO_SIGNAL;
#endif
	sinfo->inactive_time = 0;
	sinfo->signal = info->param.sta_list.info[idx].rssi;
done:
	if (status != MLAN_STATUS_PENDING)
		kfree(ioctl_req);
	LEAVE();
	return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
/**
 * @brief set mac filter
 *
 * @param wiphy           A pointer to wiphy structure
 * @param dev             A pointer to net_device structure
 * @param params		  A pointer to cfg80211_acl_data structure
 *
 * @return                0 -- success, otherwise fail
 */
int
woal_cfg80211_set_mac_acl(struct wiphy *wiphy, struct net_device *dev,
			  const struct cfg80211_acl_data *params)
{
	int ret = -EFAULT;
	mlan_uap_bss_param sys_config;
	moal_private *priv = (moal_private *)woal_get_netdev_priv(dev);
	u8 bss_started = MFALSE;
	ENTER();

	PRINTM(MIOCTL, "Set mac acl, entries=%d, policy=%d\n",
	       params->n_acl_entries, params->acl_policy);
	/* Initialize the uap bss values which are uploaded from firmware */
	if (MLAN_STATUS_SUCCESS != woal_set_get_sys_config(priv,
							   MLAN_ACT_GET,
							   MOAL_IOCTL_WAIT,
							   &sys_config)) {
		PRINTM(MERROR, "Error getting AP confiruration\n");
		ret = -EFAULT;
		goto done;
	}
	memset(&sys_config.filter, 0, sizeof(mac_filter));
	if (params->n_acl_entries <= MAX_MAC_FILTER_NUM)
		sys_config.filter.mac_count = params->n_acl_entries;
	else
		sys_config.filter.mac_count = MAX_MAC_FILTER_NUM;

	if (params->acl_policy == NL80211_ACL_POLICY_DENY_UNLESS_LISTED)
		sys_config.filter.filter_mode = MAC_FILTER_MODE_ALLOW_MAC;
	else if (params->acl_policy == NL80211_ACL_POLICY_ACCEPT_UNLESS_LISTED)
		sys_config.filter.filter_mode = MAC_FILTER_MODE_BLOCK_MAC;
	memcpy(sys_config.filter.mac_list, params->mac_addrs,
	       sys_config.filter.mac_count * sizeof(mlan_802_11_mac_addr));
	if (priv->bss_started == MTRUE) {
		bss_started = MTRUE;
		woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_STOP);
	}
	if (MLAN_STATUS_SUCCESS == woal_set_get_sys_config(priv,
							   MLAN_ACT_SET,
							   MOAL_IOCTL_WAIT,
							   &sys_config))
		ret = 0;
done:
	if (bss_started)
		woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_START);
	LEAVE();
	return ret;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0)|| defined(COMPAT_WIRELESS)
/**
 * @brief Set txq parameters

 * @param wiphy           A pointer to wiphy structure
 * @param dev             A pointer to net_device structure
 * @param params		  A pointer to ieee80211_txq_params structure
 *
 * @return                0 -- success, otherwise fail
 */
int
woal_cfg80211_set_txq_params(struct wiphy *wiphy, struct net_device *dev,
			     struct ieee80211_txq_params *params)
{
	int ret = 0;
	u8 ac = 0;
	wmm_parameter_t ap_wmm_para;
	moal_private *priv = (moal_private *)woal_get_netdev_priv(dev);

	ENTER();

	/* AC_BE: 0, AC_BK:1, AC_VI: 2, AC_VO:3 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0)
	switch (params->ac) {
	case NL80211_AC_VO:
		ac = 3;
		break;
	case NL80211_AC_VI:
		ac = 2;
		break;
	case NL80211_AC_BK:
		ac = 1;
		break;
	case NL80211_AC_BE:
		ac = 0;
		break;
	default:
		break;
	}
#else
	switch (params->queue) {
	case NL80211_TXQ_Q_VO:
		ac = 3;
		break;
	case NL80211_TXQ_Q_VI:
		ac = 2;
		break;
	case NL80211_TXQ_Q_BK:
		ac = 1;
		break;
	case NL80211_TXQ_Q_BE:
		ac = 0;
		break;
	default:
		break;
	}
#endif

	PRINTM(MMSG, "Set AC=%d, txop=%d cwmin=%d, cwmax=%d aifs=%d\n", ac,
	       params->txop, params->cwmin, params->cwmax, params->aifs);

	memset(&ap_wmm_para, 0, sizeof(wmm_parameter_t));

	if (MLAN_STATUS_SUCCESS !=
	    woal_set_get_ap_wmm_para(priv, MLAN_ACT_GET, &ap_wmm_para)) {
		PRINTM(MERROR, "wlan: We don't support AP WMM parameter\n");
		LEAVE();
		return ret;
	}
	ap_wmm_para.ac_params[ac].aci_aifsn.aifsn = params->aifs;
	ap_wmm_para.ac_params[ac].ecw.ecw_max = ilog2(params->cwmax + 1);
	ap_wmm_para.ac_params[ac].ecw.ecw_min = ilog2(params->cwmin + 1);
	ap_wmm_para.ac_params[ac].tx_op_limit = params->txop;
	if (MLAN_STATUS_SUCCESS !=
	    woal_set_get_ap_wmm_para(priv, MLAN_ACT_SET, &ap_wmm_para)) {
		PRINTM(MERROR, "wlan: Fail to set AP WMM parameter\n");
		ret = -EFAULT;
	}
	LEAVE();
	return ret;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0)
/**
 * @brief Notify cfg80211 uap channel changed
 *
 * @param priv          A pointer moal_private structure
 * @param pchan_info    A pointer to chan_band structure
 *
 * @return          N/A
 */
void
woal_cfg80211_notify_uap_channel(moal_private *priv,
				 chan_band_info * pchan_info)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
	struct cfg80211_chan_def chandef;
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0)
	enum nl80211_channel_type type;
	enum ieee80211_band band;
	int freq = 0;
#endif
#endif
	ENTER();

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
	woal_chandef_create(priv, &chandef, pchan_info);
	cfg80211_ch_switch_notify(priv->netdev, &chandef);
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0)
	if (pchan_info->band_config.chanBand == BAND_2GHZ)
		band = IEEE80211_BAND_2GHZ;
	else if (pchan_info->band_config.chanBand == BAND_5GHZ)
		band = IEEE80211_BAND_5GHZ;
	freq = ieee80211_channel_to_frequency(pchan_info->channel, band);
	switch (pchan_info->band_config.chanWidth) {
	case CHAN_BW_20MHZ:
		if (pchan_info->is_11n_enabled)
			type = NL80211_CHAN_HT20;
		else
			type = NL80211_CHAN_NO_HT;
		break;
	default:
		if (pchan_info->band_config.chan2Offset == SEC_CHAN_ABOVE)
			type = NL80211_CHAN_HT40PLUS;
		else if (pchan_info->band_config.chan2Offset == SEC_CHAN_BELOW)
			type = NL80211_CHAN_HT40MINUS;
		else
			type = NL80211_CHAN_HT20;
		break;
	}
	cfg80211_ch_switch_notify(priv->netdev, freq, type);
#endif
#endif
	LEAVE();
}
#endif

/**
 * @brief Register the device with cfg80211
 *
 * @param dev       A pointer to net_device structure
 * @param bss_type  BSS type
 *
 * @return          MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_register_uap_cfg80211(struct net_device *dev, t_u8 bss_type)
{
	mlan_status ret = MLAN_STATUS_SUCCESS;
	moal_private *priv = (moal_private *)netdev_priv(dev);
	struct wireless_dev *wdev = NULL;

	ENTER();

	wdev = (struct wireless_dev *)&priv->w_dev;
	memset(wdev, 0, sizeof(struct wireless_dev));

	wdev->wiphy = priv->phandle->wiphy;
	if (!wdev->wiphy) {
		LEAVE();
		return MLAN_STATUS_FAILURE;
	}

	if (bss_type == MLAN_BSS_TYPE_UAP)
		wdev->iftype = NL80211_IFTYPE_AP;

	dev_net_set(dev, wiphy_net(wdev->wiphy));
	dev->ieee80211_ptr = wdev;
	SET_NETDEV_DEV(dev, wiphy_dev(wdev->wiphy));
	priv->wdev = wdev;

	LEAVE();
	return ret;
}
