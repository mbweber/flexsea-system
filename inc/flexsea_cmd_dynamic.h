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
	[This file] flexsea_cmd_dynamic: Dynamic R/W Commands
*****************************************************************************/

#ifndef INC_FLEXSEA_CMD_DYNAMIC_H
#define INC_FLEXSEA_CMD_DYNAMIC_H

#ifdef __cplusplus
extern "C" {
#endif

void init_flexsea_payload_ptr_dynamic(void);

void tx_cmd_dynamic_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, uint16_t *len, \
						int slave, uint8_t numFields, uint8_t* fields);

void tx_cmd_dynamic_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, uint16_t *len, \
						int slave, uint8_t numFields, uint8_t* fields, int32_t* values);

void rx_cmd_dynamic_r(uint8_t *buf, uint8_t *info);
void rx_cmd_dynamic_rw(uint8_t *buf, uint8_t *info);

void rx_cmd_dynamic_rr(uint8_t *buf, uint8_t *info);

#define CMD_DYNAMIC_MAX_NUMFIELDS 4

#define CMD_DYNAMIC_ACCEL_X 	0x00
#define CMD_DYNAMIC_ACCEL_Y		0x01
#define CMD_DYNAMIC_ACCEL_Z		0x02
#define CMD_DYNAMIC_GYRO_X		0x03
#define CMD_DYNAMIC_GYRO_Y		0x04
#define CMD_DYNAMIC_GYRO_Z		0x05

#ifdef __cplusplus
}
#endif


#endif	//INC_FLEXSEA_CMD_DYNAMIC_H
