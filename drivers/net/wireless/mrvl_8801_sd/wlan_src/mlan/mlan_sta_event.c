/** @file mlan_sta_event.c
 *
 *  @brief This file contains MLAN event handling.
 *
 *  (C) Copyright 2008-2016 Marvell International Ltd. All Rights Reserved
 *
 *  MARVELL CONFIDENTIAL
 *  The source code contained or described herein and all documents related to
 *  the source code ("Material") are owned by Marvell International Ltd or its
 *  suppliers or licensors. Title to the Material remains with Marvell
 *  International Ltd or its suppliers and licensors. The Material contains
 *  trade secrets and proprietary and confidential information of Marvell or its
 *  suppliers and licensors. The Material is protected by worldwide copyright
 *  and trade secret laws and treaty provisions. No part of the Material may be
 *  used, copied, reproduced, modified, published, uploaded, posted,
 *  transmitted, distributed, or disclosed in any way without Marvell's prior
 *  express written permission.
 *
 *  No license under any patent, copyright, trade secret or other intellectual
 *  property right is granted to or conferred upon you by disclosure or delivery
 *  of the Materials, either expressly, by implication, inducement, estoppel or
 *  otherwise. Any license under such intellectual property rights must be
 *  express and approved by Marvell in writing.
 *
 */

/********************************************************
Change log:
    10/13/2008: initial version
********************************************************/

#include "mlan.h"
#include "mlan_join.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_wmm.h"
#include "mlan_11n.h"

/********************************************************
			Global Variables
********************************************************/

/********************************************************
			Local Functions
********************************************************/

/**
 *  @brief This function handles link lost, deauth and
 *          disassoc events.
 *
 *  @param pmpriv   A pointer to mlan_private structure
 *  @return         N/A
 */
static t_void
wlan_handle_disconnect_event(pmlan_private pmpriv)
{
	ENTER();

	if (pmpriv->media_connected == MTRUE)
		wlan_reset_connect_state(pmpriv, MTRUE);

	LEAVE();
}

/********************************************************
			Global Functions
********************************************************/
/**
 *  @brief This function handles disconnect event, reports disconnect
 *          to upper layer, cleans tx/rx packets,
 *          resets link state etc.
 *
 *  @param priv             A pointer to mlan_private structure
 *  @param drv_disconnect   Flag indicating the driver should disconnect
 *                          and flush pending packets.
 *
 *  @return                 N/A
 */
t_void
wlan_reset_connect_state(pmlan_private priv, t_u8 drv_disconnect)
{
	mlan_adapter *pmadapter = priv->adapter;
	mlan_status ret = MLAN_STATUS_SUCCESS;
	state_11d_t enable;

	ENTER();

	PRINTM(MINFO, "Handles disconnect event.\n");

	if (drv_disconnect) {
		priv->media_connected = MFALSE;
	}

	if (priv->port_ctrl_mode == MTRUE) {
		/* Close the port on Disconnect */
		PRINTM(MINFO, "DISC: port_status = CLOSED\n");
		priv->port_open = MFALSE;
	}
	priv->tx_pause = MFALSE;
	pmadapter->scan_block = MFALSE;

	/* Reset SNR/NF/RSSI values */
	priv->data_rssi_last = 0;
	priv->data_nf_last = 0;
	priv->data_rssi_avg = 0;
	priv->data_nf_avg = 0;
	priv->bcn_rssi_last = 0;
	priv->bcn_nf_last = 0;
	priv->bcn_rssi_avg = 0;
	priv->bcn_nf_avg = 0;
	priv->rxpd_rate = 0;
	priv->rxpd_htinfo = 0;
	priv->max_amsdu = 0;

	priv->sec_info.ewpa_enabled = MFALSE;
	priv->sec_info.wpa_enabled = MFALSE;
	priv->sec_info.wpa2_enabled = MFALSE;
	priv->wpa_ie_len = 0;

	priv->sec_info.wapi_enabled = MFALSE;
	priv->wapi_ie_len = 0;
	priv->sec_info.wapi_key_on = MFALSE;

	priv->wps.session_enable = MFALSE;
	memset(priv->adapter, (t_u8 *)&priv->wps.wps_ie, 0x00,
	       sizeof(priv->wps.wps_ie));

	priv->sec_info.encryption_mode = MLAN_ENCRYPTION_MODE_NONE;

	/* Enable auto data rate */
	priv->is_data_rate_auto = MTRUE;
	priv->data_rate = 0;

	if (priv->bss_mode == MLAN_BSS_MODE_IBSS) {
		priv->adhoc_state = ADHOC_IDLE;
		priv->adhoc_is_link_sensed = MFALSE;
	}

	if (drv_disconnect) {
		/* Free Tx and Rx packets, report disconnect to upper layer */
		wlan_clean_txrx(priv);

		/* Need to erase the current SSID and BSSID info */
		memset(pmadapter,
		       &priv->curr_bss_params, 0x00,
		       sizeof(priv->curr_bss_params));
		priv->assoc_rsp_size = 0;
	}
	pmadapter->tx_lock_flag = MFALSE;
	pmadapter->pps_uapsd_mode = MFALSE;
	pmadapter->delay_null_pkt = MFALSE;
	if (priv->bss_type == MLAN_BSS_TYPE_STA)
		pmadapter->hs_wake_interval = 0;

	if ((wlan_11d_is_enabled(priv)) &&
	    (priv->state_11d.user_enable_11d == DISABLE_11D)) {

		priv->state_11d.enable_11d = DISABLE_11D;
		enable = DISABLE_11D;

		/* Send cmd to FW to enable/disable 11D function */
		ret = wlan_prepare_cmd(priv,
				       HostCmd_CMD_802_11_SNMP_MIB,
				       HostCmd_ACT_GEN_SET,
				       Dot11D_i, MNULL, &enable);
		if (ret)
			PRINTM(MERROR, "11D: Failed to enable 11D\n");
	}
	if (pmadapter->num_cmd_timeout && pmadapter->curr_cmd &&
	    (pmadapter->cmd_timer_is_set == MFALSE)) {
		LEAVE();
		return;
	}

	wlan_recv_event(priv, MLAN_EVENT_ID_FW_DISCONNECTED, MNULL);

	LEAVE();
}

/**
 *  @brief This function sends the OBSS scan parameters to the application
 *
 *  @param pmpriv     A pointer to mlan_private structure
 *
 *  @return           N/A
 */
t_void
wlan_2040_coex_event(pmlan_private pmpriv)
{
	t_u8 event_buf[100];
	mlan_event *pevent = (mlan_event *)event_buf;
	t_u8 ele_len;

	ENTER();

	if (pmpriv->curr_bss_params.bss_descriptor.poverlap_bss_scan_param &&
	    pmpriv->curr_bss_params.bss_descriptor.poverlap_bss_scan_param->
	    ieee_hdr.element_id == OVERLAPBSSSCANPARAM) {
		ele_len =
			pmpriv->curr_bss_params.bss_descriptor.
			poverlap_bss_scan_param->ieee_hdr.len;
		pevent->bss_index = pmpriv->bss_index;
		pevent->event_id = MLAN_EVENT_ID_DRV_OBSS_SCAN_PARAM;
		/* Copy OBSS scan parameters */
		memcpy(pmpriv->adapter, (t_u8 *)pevent->event_buf,
		       (t_u8 *)&pmpriv->curr_bss_params.bss_descriptor.
		       poverlap_bss_scan_param->obss_scan_param, ele_len);
		pevent->event_len = ele_len;
		wlan_recv_event(pmpriv, MLAN_EVENT_ID_DRV_OBSS_SCAN_PARAM,
				pevent);
	}

	LEAVE();
}

/**
 *  @brief This function will process tx pause event
 *
 *  @param priv    A pointer to mlan_private
 *  @param pevent  A pointer to event buf
 *
 *  @return        N/A
 */
static void
wlan_process_sta_tx_pause_event(pmlan_private priv, pmlan_buffer pevent)
{
	t_u16 tlv_type, tlv_len;
	int tlv_buf_left = pevent->data_len - sizeof(t_u32);
	MrvlIEtypesHeader_t *tlv =
		(MrvlIEtypesHeader_t *)(pevent->pbuf + pevent->data_offset +
					sizeof(t_u32));
	MrvlIEtypes_tx_pause_t *tx_pause_tlv;
	t_u8 *bssid = MNULL;
	ENTER();
	if (priv->media_connected)
		bssid = priv->curr_bss_params.bss_descriptor.mac_address;
	while (tlv_buf_left >= (int)sizeof(MrvlIEtypesHeader_t)) {
		tlv_type = wlan_le16_to_cpu(tlv->type);
		tlv_len = wlan_le16_to_cpu(tlv->len);
		if ((sizeof(MrvlIEtypesHeader_t) + tlv_len) >
		    (unsigned int)tlv_buf_left) {
			PRINTM(MERROR, "wrong tlv: tlvLen=%d, tlvBufLeft=%d\n",
			       tlv_len, tlv_buf_left);
			break;
		}
		if (tlv_type == TLV_TYPE_TX_PAUSE) {
			tx_pause_tlv = (MrvlIEtypes_tx_pause_t *)tlv;
			PRINTM(MCMND, "TxPause: " MACSTR " pause=%d, pkts=%d\n",
			       MAC2STR(tx_pause_tlv->peermac),
			       tx_pause_tlv->tx_pause, tx_pause_tlv->pkt_cnt);

			if (bssid &&
			    !memcmp(priv->adapter, bssid, tx_pause_tlv->peermac,
				    MLAN_MAC_ADDR_LENGTH)) {
				if (tx_pause_tlv->tx_pause)
					priv->tx_pause = MTRUE;
				else
					priv->tx_pause = MFALSE;
			}

		}
		tlv_buf_left -= (sizeof(MrvlIEtypesHeader_t) + tlv_len);
		tlv = (MrvlIEtypesHeader_t *)((t_u8 *)tlv + tlv_len +
					      sizeof(MrvlIEtypesHeader_t));
	}

	LEAVE();
	return;
}

/**
 *  @brief This function handles events generated by firmware
 *
 *  @param priv A pointer to mlan_private structure
 *
 *  @return     MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_ops_sta_process_event(IN t_void *priv)
{
	pmlan_private pmpriv = (pmlan_private)priv;
	pmlan_adapter pmadapter = pmpriv->adapter;
	mlan_status ret = MLAN_STATUS_SUCCESS;
	t_u32 eventcause = pmadapter->event_cause;
	t_u8 event_buf[100];
	t_u8 *evt_buf = MNULL;
	pmlan_buffer pmbuf = pmadapter->pmlan_buffer_event;
	t_u16 reason_code;
	t_u8 sta_addr[MLAN_MAC_ADDR_LENGTH];
	sta_node *sta_ptr = MNULL;
	t_u8 i = 0;
	pmlan_callbacks pcb = &pmadapter->callbacks;
	mlan_event *pevent = (mlan_event *)event_buf;

	ENTER();

	if (!pmbuf) {
		LEAVE();
		return MLAN_STATUS_FAILURE;
	}

	/* Event length check */
	if ((pmbuf->data_len - sizeof(eventcause)) > MAX_EVENT_SIZE) {
		pmbuf->status_code = MLAN_ERROR_PKT_SIZE_INVALID;
		LEAVE();
		return MLAN_STATUS_FAILURE;
	}

	if (eventcause != EVENT_PS_SLEEP && eventcause != EVENT_PS_AWAKE &&
	    pmbuf->data_len > sizeof(eventcause))
		DBG_HEXDUMP(MEVT_D, "EVENT", pmbuf->pbuf + pmbuf->data_offset,
			    pmbuf->data_len);

	switch (eventcause) {
	case EVENT_DUMMY_HOST_WAKEUP_SIGNAL:
		PRINTM(MERROR,
		       "Invalid EVENT: DUMMY_HOST_WAKEUP_SIGNAL, ignoring it\n");
		break;
	case EVENT_LINK_SENSED:
		PRINTM(MEVENT, "EVENT: LINK_SENSED\n");
		pmpriv->adhoc_is_link_sensed = MTRUE;
		wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_ADHOC_LINK_SENSED,
				MNULL);
		break;

	case EVENT_DEAUTHENTICATED:
		if (pmpriv->wps.session_enable) {
			PRINTM(MMSG,
			       "wlan: Recevie deauth event in wps session\n");
			break;
		}
		reason_code =
			wlan_le16_to_cpu(*(t_u16 *)
					 (pmbuf->pbuf + pmbuf->data_offset +
					  sizeof(eventcause)));
		PRINTM(MMSG, "wlan: EVENT: Deauthenticated (reason 0x%x)\n",
		       reason_code);
		pmadapter->dbg.num_event_deauth++;
		wlan_handle_disconnect_event(pmpriv);

		break;

	case EVENT_DISASSOCIATED:
		if (pmpriv->wps.session_enable) {
			PRINTM(MMSG,
			       "wlan: Recevie disassociate event in wps session\n");
			break;
		}
		reason_code =
			wlan_le16_to_cpu(*(t_u16 *)
					 (pmbuf->pbuf + pmbuf->data_offset +
					  sizeof(eventcause)));
		PRINTM(MMSG, "wlan: EVENT: Disassociated (reason 0x%x)\n",
		       reason_code);
		pmadapter->dbg.num_event_disassoc++;
		wlan_handle_disconnect_event(pmpriv);
		break;

	case EVENT_LINK_LOST:
		reason_code =
			wlan_le16_to_cpu(*(t_u16 *)
					 (pmbuf->pbuf + pmbuf->data_offset +
					  sizeof(eventcause)));
		PRINTM(MMSG, "wlan: EVENT: Link lost (reason 0x%x)\n",
		       reason_code);
		pmadapter->dbg.num_event_link_lost++;
		wlan_handle_disconnect_event(pmpriv);
		break;

	case EVENT_PS_SLEEP:
		PRINTM(MINFO, "EVENT: SLEEP\n");
		PRINTM(MEVENT, "_");

		/* Handle unexpected PS SLEEP event */
		if (pmadapter->ps_state == PS_STATE_SLEEP_CFM)
			break;
		pmadapter->ps_state = PS_STATE_PRE_SLEEP;

		wlan_check_ps_cond(pmadapter);
		break;

	case EVENT_PS_AWAKE:
		PRINTM(MINFO, "EVENT: AWAKE\n");
		PRINTM(MEVENT, "|");
		if (!pmadapter->pps_uapsd_mode &&
		    pmpriv->media_connected &&
		    (pmpriv->port_open || !pmpriv->port_ctrl_mode) &&
		    pmadapter->sleep_period.period) {
			pmadapter->pps_uapsd_mode = MTRUE;
			PRINTM(MEVENT, "PPS/UAPSD mode activated\n");
		}
		/* Handle unexpected PS AWAKE event */
		if (pmadapter->ps_state == PS_STATE_SLEEP_CFM)
			break;
		pmadapter->tx_lock_flag = MFALSE;
		if (pmadapter->pps_uapsd_mode && pmadapter->gen_null_pkt) {
			if (MTRUE == wlan_check_last_packet_indication(pmpriv)) {
				if (!pmadapter->data_sent) {
					if (wlan_send_null_packet(pmpriv,
								  MRVDRV_TxPD_POWER_MGMT_NULL_PACKET
								  |
								  MRVDRV_TxPD_POWER_MGMT_LAST_PACKET)
					    == MLAN_STATUS_SUCCESS) {
						LEAVE();
						return MLAN_STATUS_SUCCESS;
					}
				}
			}
		}
		pmadapter->ps_state = PS_STATE_AWAKE;
		pmadapter->pm_wakeup_card_req = MFALSE;
		pmadapter->pm_wakeup_fw_try = MFALSE;
		break;

	case EVENT_HS_ACT_REQ:
		PRINTM(MEVENT, "EVENT: HS_ACT_REQ\n");
		ret = wlan_prepare_cmd(priv,
				       HostCmd_CMD_802_11_HS_CFG_ENH,
				       0, 0, MNULL, MNULL);
		break;

	case EVENT_MIC_ERR_UNICAST:
		PRINTM(MEVENT, "EVENT: UNICAST MIC ERROR\n");
		wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_MIC_ERR_UNI, MNULL);
		break;

	case EVENT_MIC_ERR_MULTICAST:
		PRINTM(MEVENT, "EVENT: MULTICAST MIC ERROR\n");
		wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_MIC_ERR_MUL, MNULL);
		break;
	case EVENT_MIB_CHANGED:
	case EVENT_INIT_DONE:
		break;

	case EVENT_ADHOC_BCN_LOST:
		PRINTM(MEVENT, "EVENT: ADHOC_BCN_LOST\n");
		pmpriv->adhoc_is_link_sensed = MFALSE;
		wlan_clean_txrx(pmpriv);
		wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_ADHOC_LINK_LOST,
				MNULL);
		break;

	case EVENT_FW_DEBUG_INFO:
		/* Allocate memory for event buffer */
		ret = pcb->moal_malloc(pmadapter->pmoal_handle,
				       MAX_EVENT_SIZE, MLAN_MEM_DEF, &evt_buf);
		if ((ret == MLAN_STATUS_SUCCESS) && evt_buf) {
			pevent = (pmlan_event)evt_buf;
			pevent->bss_index = pmpriv->bss_index;
			PRINTM(MEVENT, "EVENT: FW Debug Info\n");
			pevent->event_id = MLAN_EVENT_ID_FW_DEBUG_INFO;
			pevent->event_len =
				pmbuf->data_len - sizeof(eventcause);
			memcpy(pmadapter, (t_u8 *)pevent->event_buf,
			       pmbuf->pbuf + pmbuf->data_offset +
			       sizeof(eventcause), pevent->event_len);
			wlan_recv_event(pmpriv, pevent->event_id, pevent);
			pcb->moal_mfree(pmadapter->pmoal_handle, evt_buf);
		}
		break;

	case EVENT_BG_SCAN_REPORT:
		PRINTM(MEVENT, "EVENT: BGS_REPORT\n");
		pmadapter->bgscan_reported = MTRUE;
		wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_BG_SCAN, MNULL);
		break;
	case EVENT_BG_SCAN_STOPPED:
		PRINTM(MEVENT, "EVENT: BGS_STOPPED\n");
		wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_BG_SCAN_STOPPED,
				MNULL);
		break;

	case EVENT_PORT_RELEASE:
		PRINTM(MEVENT, "EVENT: PORT RELEASE\n");
		/* Open the port for e-supp mode */
		if (pmpriv->port_ctrl_mode == MTRUE) {
			PRINTM(MINFO, "PORT_REL: port_status = OPEN\n");
			pmpriv->port_open = MTRUE;
		}
		pmadapter->scan_block = MFALSE;
		wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_PORT_RELEASE, MNULL);
		break;

	case EVENT_EXT_SCAN_REPORT:
		PRINTM(MEVENT, "EVENT: EXT_SCAN Report (%d)\n",
		       pmbuf->data_len);
		if (pmadapter->pscan_ioctl_req && pmadapter->ext_scan)
			ret = wlan_handle_event_ext_scan_report(priv, pmbuf);
		break;
	case EVENT_WMM_STATUS_CHANGE:
		if (pmbuf && pmbuf->data_len
		    > sizeof(eventcause) + sizeof(MrvlIEtypesHeader_t)) {
			PRINTM(MEVENT, "EVENT: WMM status changed: %d\n",
			       pmbuf->data_len);

			evt_buf = (pmbuf->pbuf
				   + pmbuf->data_offset + sizeof(eventcause));

			wlan_ret_wmm_get_status(pmpriv,
						evt_buf,
						pmbuf->data_len -
						sizeof(eventcause));
		} else {
			PRINTM(MEVENT, "EVENT: WMM status changed\n");
			ret = wlan_cmd_wmm_status_change(pmpriv);
		}
		break;

	case EVENT_IBSS_COALESCED:
		PRINTM(MEVENT, "EVENT: IBSS_COALESCED\n");
		ret = wlan_prepare_cmd(pmpriv,
				       HostCmd_CMD_802_11_IBSS_COALESCING_STATUS,
				       HostCmd_ACT_GEN_GET, 0, MNULL, MNULL);
		break;
	case EVENT_ADDBA:
		PRINTM(MEVENT, "EVENT: ADDBA Request\n");
		if (pmpriv->media_connected == MTRUE)
			ret = wlan_prepare_cmd(pmpriv,
					       HostCmd_CMD_11N_ADDBA_RSP,
					       HostCmd_ACT_GEN_SET, 0, MNULL,
					       pmadapter->event_body);
		else
			PRINTM(MERROR,
			       "Ignore ADDBA Request event in disconnected state\n");
		break;
	case EVENT_DELBA:
		PRINTM(MEVENT, "EVENT: DELBA Request\n");
		if (pmpriv->media_connected == MTRUE)
			wlan_11n_delete_bastream(pmpriv, pmadapter->event_body);
		else
			PRINTM(MERROR,
			       "Ignore DELBA Request event in disconnected state\n");
		break;
	case EVENT_BA_STREAM_TIMEOUT:
		PRINTM(MEVENT, "EVENT:  BA Stream timeout\n");
		if (pmpriv->media_connected == MTRUE)
			wlan_11n_ba_stream_timeout(pmpriv,
						   (HostCmd_DS_11N_BATIMEOUT *)
						   pmadapter->event_body);
		else
			PRINTM(MERROR,
			       "Ignore BA Stream timeout event in disconnected state\n");
		break;
	case EVENT_RXBA_SYNC:
		PRINTM(MEVENT, "EVENT:  RXBA_SYNC\n");
		wlan_11n_rxba_sync_event(pmpriv, pmadapter->event_body,
					 pmbuf->data_len - sizeof(eventcause));
		break;
	case EVENT_AMSDU_AGGR_CTRL:
		PRINTM(MEVENT, "EVENT:  AMSDU_AGGR_CTRL %d\n",
		       *(t_u16 *)pmadapter->event_body);
		pmadapter->tx_buf_size =
			MIN(pmadapter->curr_tx_buf_size,
			    wlan_le16_to_cpu(*(t_u16 *)pmadapter->event_body));
		PRINTM(MEVENT, "tx_buf_size %d\n", pmadapter->tx_buf_size);
		break;

	case EVENT_WEP_ICV_ERR:
		PRINTM(MEVENT, "EVENT: WEP ICV error\n");
		pevent->bss_index = pmpriv->bss_index;
		pevent->event_id = MLAN_EVENT_ID_FW_WEP_ICV_ERR;
		pevent->event_len = sizeof(Event_WEP_ICV_ERR);
		memcpy(pmadapter, (t_u8 *)pevent->event_buf,
		       pmadapter->event_body, pevent->event_len);
		wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_WEP_ICV_ERR, pevent);
		break;

	case EVENT_BW_CHANGE:
		PRINTM(MEVENT, "EVENT: BW Change\n");
		pevent->bss_index = pmpriv->bss_index;
		pevent->event_id = MLAN_EVENT_ID_FW_BW_CHANGED;
		pevent->event_len = sizeof(t_u8);
		/* Copy event body from the event buffer */
		memcpy(pmadapter, (t_u8 *)pevent->event_buf,
		       pmadapter->event_body, pevent->event_len);
		wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_BW_CHANGED, pevent);
		break;

	case EVENT_TX_DATA_PAUSE:
		PRINTM(MEVENT, "EVENT: TX_DATA_PAUSE\n");
		wlan_process_sta_tx_pause_event(priv, pmbuf);
		break;

	case EVENT_IBSS_STATION_CONNECT:
		memcpy(pmadapter, sta_addr, pmadapter->event_body + 2,
		       MLAN_MAC_ADDR_LENGTH);
		sta_ptr = wlan_add_station_entry(pmpriv, sta_addr);
		if (sta_ptr) {
			PRINTM(MMSG,
			       "wlan: EVENT: IBSS_STA_CONNECT " MACSTR "\n",
			       MAC2STR(sta_addr));
			if (pmpriv->adapter->adhoc_11n_enabled) {
				wlan_check_sta_capability(pmpriv, pmbuf,
							  sta_ptr);
				for (i = 0; i < MAX_NUM_TID; i++) {
					if (sta_ptr->is_11n_enabled)
						sta_ptr->ampdu_sta[i] =
							pmpriv->ibss_ampdu[i];
					else
						sta_ptr->ampdu_sta[i] =
							BA_STREAM_NOT_ALLOWED;
				}
				memset(pmadapter, sta_ptr->rx_seq, 0xff,
				       sizeof(sta_ptr->rx_seq));
			}
		}
		break;
	case EVENT_IBSS_STATION_DISCONNECT:
		memcpy(pmadapter, sta_addr, pmadapter->event_body + 2,
		       MLAN_MAC_ADDR_LENGTH);
		PRINTM(MMSG, "wlan: EVENT: IBSS_STA_DISCONNECT " MACSTR "\n",
		       MAC2STR(sta_addr));

		if (pmpriv->adapter->adhoc_11n_enabled) {
			wlan_cleanup_reorder_tbl(pmpriv, sta_addr);
			pmadapter->callbacks.moal_spin_lock(pmadapter->
							    pmoal_handle,
							    pmpriv->wmm.
							    ra_list_spinlock);
			wlan_11n_cleanup_txbastream_tbl(pmpriv, sta_addr);
			pmadapter->callbacks.moal_spin_unlock(pmadapter->
							      pmoal_handle,
							      pmpriv->wmm.
							      ra_list_spinlock);
		}

		wlan_wmm_delete_peer_ralist(pmpriv, sta_addr);
		wlan_delete_station_entry(pmpriv, sta_addr);
		break;
	case EVENT_SAD_REPORT:
		{
#ifdef DEBUG_LEVEL1
			t_u8 *pevt_dat =
				pmbuf->pbuf + pmbuf->data_offset +
				sizeof(t_u32);
#endif
			PRINTM(MEVENT,
			       "EVENT: Antenna Diversity %d  (%d, %d, %d, %d)\n",
			       eventcause, pevt_dat[0] + 1, pevt_dat[1] + 1,
			       pevt_dat[2], pevt_dat[3]);
		}
		break;

	case EVENT_TX_STATUS_REPORT:
		PRINTM(MINFO, "EVENT: TX_STATUS\n");
		pevent->bss_index = pmpriv->bss_index;
		pevent->event_id = MLAN_EVENT_ID_FW_TX_STATUS;
		pevent->event_len = pmbuf->data_len;
		memcpy(pmadapter, (t_u8 *)pevent->event_buf,
		       pmbuf->pbuf + pmbuf->data_offset, MIN(pevent->event_len,
							     sizeof
							     (event_buf)));
		wlan_recv_event(pmpriv, pevent->event_id, pevent);
		break;
	case EVENT_BT_COEX_WLAN_PARA_CHANGE:
		PRINTM(MEVENT, "EVENT: BT coex wlan param update\n");
		wlan_bt_coex_wlan_param_update_event(pmpriv, pmbuf);
		break;

	default:
		PRINTM(MEVENT, "EVENT: unknown event id: %#x\n", eventcause);
		wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_UNKNOWN, MNULL);
		break;
	}

	LEAVE();
	return ret;
}
