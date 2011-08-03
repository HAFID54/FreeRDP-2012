/**
 * FreeRDP: A Remote Desktop Protocol Client
 * RDP Core
 *
 * Copyright 2011 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RDP_H
#define __RDP_H

typedef struct rdp_rdp rdpRdp;

#include "mcs.h"
#include "tpkt.h"
#include "tpdu.h"
#include "nego.h"
#include "input.h"
#include "update.h"
#include "license.h"
#include "security.h"
#include "registry.h"
#include "transport.h"
#include "connection.h"
#include "capabilities.h"
#include "vchan.h"

#include <freerdp/freerdp.h>
#include <freerdp/settings.h>
#include <freerdp/utils/stream.h>

/* Security Header Flags */
#define SEC_EXCHANGE_PKT		0x0001
#define SEC_ENCRYPT			0x0008
#define SEC_RESET_SEQNO			0x0010
#define	SEC_IGNORE_SEQNO		0x0020
#define	SEC_INFO_PKT			0x0040
#define	SEC_LICENSE_PKT			0x0080
#define SEC_LICENSE_ENCRYPT_CS		0x0200
#define SEC_LICENSE_ENCRYPT_SC		0x0200
#define SEC_REDIRECTION_PKT		0x0400
#define SEC_SECURE_CHECKSUM		0x0800
#define SEC_FLAGSHI_VALID		0x8000

#define SEC_PKT_CS_MASK			(SEC_EXCHANGE_PKT | SEC_INFO_PKT)
#define SEC_PKT_SC_MASK			(SEC_LICENSE_PKT | SEC_REDIRECTION_PKT)
#define SEC_PKT_MASK			(SEC_PKT_CS_MASK | SEC_PKT_SC_MASK)

#define RDP_SECURITY_HEADER_LENGTH	4
#define RDP_SHARE_CONTROL_HEADER_LENGTH	6
#define RDP_SHARE_DATA_HEADER_LENGTH	12
#define RDP_PACKET_HEADER_LENGTH	(TPDU_DATA_LENGTH + MCS_SEND_DATA_HEADER_LENGTH)

#define PDU_TYPE_DEMAND_ACTIVE		0x1
#define PDU_TYPE_CONFIRM_ACTIVE		0x3
#define PDU_TYPE_DEACTIVATE_ALL		0x6
#define PDU_TYPE_DATA			0x7
#define PDU_TYPE_SERVER_REDIRECTION	0xA

/* Data PDU Types */
#define DATA_PDU_TYPE_UPDATE					0x02
#define DATA_PDU_TYPE_CONTROL					0x14
#define DATA_PDU_TYPE_POINTER					0x1B
#define DATA_PDU_TYPE_INPUT					0x1C
#define DATA_PDU_TYPE_SYNCHRONIZE				0x1F
#define DATA_PDU_TYPE_REFRESH_RECT				0x21
#define DATA_PDU_TYPE_PLAY_SOUND				0x22
#define DATA_PDU_TYPE_SUPPRESS_OUTPUT				0x23
#define DATA_PDU_TYPE_SHUTDOWN_REQUEST				0x24
#define DATA_PDU_TYPE_SHUTDOWN_DENIED				0x25
#define DATA_PDU_TYPE_SAVE_SESSION_INFO				0x26
#define DATA_PDU_TYPE_FONT_LIST					0x27
#define DATA_PDU_TYPE_FONT_MAP					0x28
#define DATA_PDU_TYPE_SET_KEYBOARD_INDICATORS			0x29
#define DATA_PDU_TYPE_BITMAP_CACHE_PERSISTENT_LIST		0x2B
#define DATA_PDU_TYPE_BITMAP_CACHE_ERROR			0x2C
#define DATA_PDU_TYPE_SET_KEYBOARD_IME_STATUS			0x2D
#define DATA_PDU_TYPE_OFFSCREEN_CACHE_ERROR			0x2E
#define DATA_PDU_TYPE_SET_ERROR_INFO				0x2F
#define DATA_PDU_TYPE_DRAW_NINEGRID_ERROR			0x30
#define DATA_PDU_TYPE_DRAW_GDIPLUS_ERROR			0x31
#define DATA_PDU_TYPE_ARC_STATUS				0x32
#define DATA_PDU_TYPE_STATUS_INFO				0x36
#define DATA_PDU_TYPE_MONITOR_LAYOUT				0x37

/* Compression Types */
#define PACKET_COMPRESSED		0x20
#define PACKET_AT_FRONT			0x40
#define PACKET_FLUSHED			0x80
#define PACKET_COMPR_TYPE_8K		0x00
#define PACKET_COMPR_TYPE_64K		0x01
#define PACKET_COMPR_TYPE_RDP6		0x02
#define PACKET_COMPR_TYPE_RDP61		0x03
#define CompressionTypeMask		0x0F

/* Stream Identifiers */
#define STREAM_UNDEFINED		0x00
#define STREAM_LOW			0x01
#define STREAM_MED			0x02
#define STREAM_HI			0x04

/* Error Info Codes */
#define ERRINFO_RPC_INITIATED_DISCONNECT			0x00000001
#define ERRINFO_RPC_INITIATED_LOGOFF				0x00000002
#define ERRINFO_IDLE_TIMEOUT					0x00000003
#define ERRINFO_LOGON_TIMEOUT					0x00000004
#define ERRINFO_DISCONNECTED_BY_OTHER_CONNECTION		0x00000005
#define ERRINFO_OUT_OF_MEMORY					0x00000006
#define ERRINFO_SERVER_DENIED_CONNECTION			0x00000007
#define ERRINFO_SERVER_INSUFFICIENT_PRIVILEGES			0x00000008
#define ERRINFO_SERVER_FRESH_CREDENTIALS_REQUIRED		0x0000000A
#define ERRINFO_RPC_INITIATED_DISCONNECT_BY_USER		0x0000000B
#define	ERRINFO_LICENSE_INTERNAL				0x00000100
#define ERRINFO_LICENSE_NO_LICENSE_SERVER			0x00000101
#define ERRINFO_LICENSE_NO_LICENSE				0x00000102
#define ERRINFO_LICENSE_BAD_CLIENT_MSG				0x00000103
#define ERRINFO_LICENSE_HWID_DOESNT_MATCH_LICENSE		0x00000104
#define ERRINFO_LICENSE_BAD_CLIENT_LICENSE			0x00000105
#define ERRINFO_LICENSE_CANT_FINISH_PROTOCOL			0x00000106
#define ERRINFO_LICENSE_CLIENT_ENDED_PROTOCOL			0x00000107
#define ERRINFO_LICENSE_BAD_CLIENT_ENCRYPTION			0x00000108
#define ERRINFO_LICENSE_CANT_UPGRADE_LICENSE			0x00000109
#define ERRINFO_LICENSE_NO_REMOTE_CONNECTIONS			0x0000010A
#define ERRINFO_UNKNOWN_DATA_PDU_TYPE				0x000010C9
#define ERRINFO_UNKNOWN_PDU_TYPE				0x000010CA
#define ERRINFO_DATA_PDU_SEQUENCE				0x000010CB
#define ERRINFO_CONTROL_PDU_SEQUENCE				0x000010CD
#define ERRINFO_INVALID_CONTROL_PDU_ACTION			0x000010CE
#define ERRINFO_INVALID_INPUT_PDU_TYPE				0x000010CF
#define ERRINFO_INVALID_INPUT_PDU_MOUSE				0x000010D0
#define ERRINFO_INVALID_REFRESH_RECT_PDU			0x000010D1
#define ERRINFO_CREATE_USER_DATA_FAILED				0x000010D2
#define ERRINFO_CONNECT_FAILED					0x000010D3
#define ERRINFO_CONFIRM_ACTIVE_HAS_WRONG_SHAREID		0x000010D4
#define ERRINFO_CONFIRM_ACTIVE_HAS_WRONG_ORIGINATOR		0x000010D5
#define ERRINFO_PERSISTENT_KEY_PDU_BAD_LENGTH			0x000010DA
#define ERRINFO_PERSISTENT_KEY_PDU_ILLEGAL_FIRST		0x000010DB
#define ERRINFO_PERSISTENT_KEY_PDU_TOO_MANY_TOTAL_KEYS		0x000010DC
#define ERRINFO_PERSISTENT_KEY_PDU_TOO_MANY_CACHE_KEYS		0x000010DD
#define ERRINFO_INPUT_PDU_BAD_LENGTH				0x000010DE
#define ERRINFO_BITMAP_CACHE_ERROR_PDU_BAD_LENGTH		0x000010DF
#define ERRINFO_SECURITY_DATA_TOO_SHORT				0x000010E0
#define ERRINFO_VCHANNEL_DATA_TOO_SHORT				0x000010E1
#define ERRINFO_SHARE_DATA_TOO_SHORT				0x000010E2
#define ERRINFO_BAD_SUPPRESS_OUTPUT_PDU				0x000010E3
#define ERRINFO_CONFIRM_ACTIVE_PDU_TOO_SHORT			0x000010E5
#define ERRINFO_CAPABILITY_SET_TOO_SMALL			0x000010E7
#define ERRINFO_CAPABILITY_SET_TOO_LARGE			0x000010E8
#define ERRINFO_NO_CURSOR_CACHE					0x000010E9
#define ERRINFO_BAD_CAPABILITIES				0x000010EA
#define ERRINFO_VIRTUAL_CHANNEL_DECOMPRESSION			0x000010EC
#define ERRINFO_INVALID_VC_COMPRESSION_TYPE			0x000010ED
#define ERRINFO_INVALID_CHANNEL_ID				0x000010EF
#define ERRINFO_VCHANNELS_TOO_MANY				0x000010F0
#define ERRINFO_REMOTEAPP_NOT_ENABLED				0x000010F3
#define ERRINFO_CACHE_CAP_NOT_SET				0x000010F4
#define ERRINFO_BITMAP_CACHE_ERROR_PDU_BAD_LENGTH2 		0x000010F5
#define ERRINFO_OFFSCREEN_CACHE_ERROR_PDU_BAD_LENGTH		0x000010F6
#define ERRINFO_DRAWNINEGRID_CACHE_ERROR_PDU_BAD_LENGTH		0x000010F7
#define ERRINFO_GDI_PLUS_PDU_BAD_LENGTH				0x000010F8
#define ERRINFO_SECURITY_DATA_TOO_SHORT2			0x00001111
#define ERRINFO_SECURITY_DATA_TOO_SHORT3			0x00001112
#define ERRINFO_SECURITY_DATA_TOO_SHORT4			0x00001113
#define ERRINFO_SECURITY_DATA_TOO_SHORT5			0x00001114
#define ERRINFO_SECURITY_DATA_TOO_SHORT6			0x00001115
#define ERRINFO_SECURITY_DATA_TOO_SHORT7			0x00001116
#define ERRINFO_SECURITY_DATA_TOO_SHORT8			0x00001117
#define ERRINFO_SECURITY_DATA_TOO_SHORT9			0x00001118
#define ERRINFO_SECURITY_DATA_TOO_SHORT10			0x00001119
#define ERRINFO_SECURITY_DATA_TOO_SHORT11			0x0000111A
#define ERRINFO_SECURITY_DATA_TOO_SHORT12			0x0000111B
#define ERRINFO_SECURITY_DATA_TOO_SHORT13			0x0000111C
#define ERRINFO_SECURITY_DATA_TOO_SHORT14			0x0000111D
#define ERRINFO_SECURITY_DATA_TOO_SHORT15			0x0000111E
#define ERRINFO_SECURITY_DATA_TOO_SHORT16			0x0000111F
#define ERRINFO_SECURITY_DATA_TOO_SHORT17			0x00001120
#define ERRINFO_SECURITY_DATA_TOO_SHORT18			0x00001121
#define ERRINFO_SECURITY_DATA_TOO_SHORT19			0x00001122
#define ERRINFO_SECURITY_DATA_TOO_SHORT20			0x00001123
#define ERRINFO_SECURITY_DATA_TOO_SHORT21			0x00001124
#define ERRINFO_SECURITY_DATA_TOO_SHORT22			0x00001125
#define ERRINFO_SECURITY_DATA_TOO_SHORT23			0x00001126
#define ERRINFO_BAD_MONITOR_DATA				0x00001129
#define ERRINFO_VC_DECOMPRESSED_REASSEMBLE_FAILED		0x0000112A
#define ERRINFO_VC_DATA_TOO_LONG				0x0000112B
#define ERRINFO_GRAPHICS_MODE_NOT_SUPPORTED			0x0000112D
#define ERRINFO_GRAPHICS_SUBSYSTEM_RESET_FAILED			0x0000112E
#define ERRINFO_UPDATE_SESSION_KEY_FAILED			0x00001191
#define ERRINFO_DECRYPT_FAILED					0x00001192
#define ERRINFO_ENCRYPT_FAILED					0x00001193
#define ERRINFO_ENCRYPTION_PACKAGE_MISMATCH			0x00001194
#define ERRINFO_DECRYPT_FAILED2					0x00001195

struct rdp_rdp
{
	boolean licensed;
	boolean activated;
	struct rdp_mcs* mcs;
	struct rdp_nego* nego;
	struct rdp_input* input;
	struct rdp_update* update;
	struct rdp_license* license;
	struct rdp_settings* settings;
	struct rdp_registry* registry;
	struct rdp_transport* transport;
	struct rdp_vchan* vchan;
};

void rdp_read_security_header(STREAM* s, uint16* flags);
void rdp_write_security_header(STREAM* s, uint16 flags);

void rdp_read_share_control_header(STREAM* s, uint16* length, uint16* type, uint16* channel_id);
void rdp_write_share_control_header(STREAM* s, uint16 length, uint16 type, uint16 channel_id);

void rdp_read_share_data_header(STREAM* s, uint16* length, uint8* type, uint32* share_id);
void rdp_write_share_data_header(STREAM* s, uint16 length, uint8 type, uint32 share_id);

STREAM* rdp_send_stream_init(rdpRdp* rdp);
void rdp_write_header(rdpRdp* rdp, STREAM* s, int length, uint16 channel_id);

STREAM* rdp_pdu_init(rdpRdp* rdp);
void rdp_send_pdu(rdpRdp* rdp, STREAM* s, uint16 type, uint16 channel_id);

STREAM* rdp_data_pdu_init(rdpRdp* rdp);
void rdp_send_data_pdu(rdpRdp* rdp, STREAM* s, uint16 type, uint16 channel_id);

void rdp_send(rdpRdp* rdp, STREAM* s, uint16 channel_id);
void rdp_recv(rdpRdp* rdp);

int rdp_send_channel_data(rdpRdp* rdp, int channel_id, uint8* data, int size);

void rdp_set_blocking_mode(rdpRdp* rdp, boolean blocking);
int rdp_check_fds(rdpRdp* rdp);

rdpRdp* rdp_new(freerdp* instance);
void rdp_free(rdpRdp* rdp);

#endif /* __RDP_H */
