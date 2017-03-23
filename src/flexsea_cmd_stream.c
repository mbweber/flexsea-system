/****************************************************************************
	[Project] FlexSEA: Flexible & Scalable Electronics Architecture
	[Sub-project] 'flexsea-system' System commands & functions
	Copyright (C) 2016 Dephy, Inc. <http://dephy.com/>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************
	[Lead developper] Jean-Francois (JF) Duval, jfduval at dephy dot com.
	[Origin] Based on Jean-Francois Duval's work at the MIT Media Lab
	Biomechatronics research group <http://biomech.media.mit.edu/>
	[Contributors]
*****************************************************************************
	[This file] flexsea_cmd_stream: commands allowing plan to put execute into a streaming mode
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2017-03-13 | dudds4 | Initial GPL-3.0 release 
****************************************************************************/

#include "flexsea_cmd_stream.h"
#include "flexsea_system.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t isStreaming = 0;
int streamCmd = -1;
uint16_t streamPeriod = 1;
uint8_t streamReceiver = 0;
uint8_t streamPortInfo = PORT_USB;

void tx_cmd_stream_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t cmdToStream, uint8_t periodInMS, uint8_t startStop)
{
	uint16_t index = 0;
	(*cmd) = CMD_STREAM;
	(*cmdType) = CMD_WRITE;

	shBuf[index++] = cmdToStream;
	shBuf[index++] = periodInMS;
	shBuf[index++] = startStop;

	*len = index;
}

void tx_cmd_stream_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len)
{
	uint16_t index = 0;
	(*cmd) = CMD_STREAM;
	(*cmdType) = CMD_READ;

	(void)shBuf;
	
	*len = index;
}

uint8_t isLegalStreamCmd(uint8_t cmd)
{
	//We have a couple system commands that are streamable
	//and we allow streaming for all user cmds?
	//These are basically all the commands available in slavecomm in Plan GUI
	
	return 
		cmd == CMD_READ_ALL || cmd == CMD_IN_CONTROL || cmd == CMD_BATT ||
		(cmd > 99 && cmd < 128);
}

void rx_cmd_stream_w(uint8_t *buf, uint8_t *info)
{
	uint16_t index = P_DATA1;
	(void) info;

	uint8_t cmdToStream = buf[index++];
	uint8_t periodInMS = buf[index++];
	uint8_t startStop = buf[index++];

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		//case: turn streaming on
		if(startStop && isLegalStreamCmd(cmdToStream))
		{
			streamCmd = cmdToStream;
			isStreaming = 1;
			streamPeriod = periodInMS;
			streamReceiver = buf[P_XID];
			streamPortInfo = *info;

		}
		//case: turn streaming off
		else 
		{ 
			streamCmd = -1;
			isStreaming = 0;
			streamPeriod = 12345;
			streamPortInfo = 0;
		}
			
	#else

		(void)buf;

	#endif	//BOARD_TY
}

void rx_cmd_stream_r(uint8_t *buf, uint8_t *info)
{
	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
//	tx_cmd_stream_w(TX_N_DEFAULT, current stream mode, whether currently streaming)
// 	packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, SEND_TO_MASTER);
	#endif

	(void) buf;
	(void) info;
}

void rx_cmd_stream_rr(uint8_t *buf, uint8_t *info)
{
	uint16_t index = P_DATA1;
	(void) info;

	uint8_t cmdToStream = buf[index++];
	uint8_t periodInMS = buf[index++];
	uint8_t startStop = buf[index++];

	// do stuff ?
}

void init_flexsea_payload_ptr_stream(void)
{
	flexsea_payload_ptr[CMD_STREAM][RX_PTYPE_READ] = &rx_cmd_stream_r;
	flexsea_payload_ptr[CMD_STREAM][RX_PTYPE_WRITE] = &rx_cmd_stream_w;
	flexsea_payload_ptr[CMD_STREAM][RX_PTYPE_REPLY] = &rx_cmd_stream_rr;
}


#ifdef __cplusplus
}
#endif
