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
#include "flexsea_cmd_dynamic.h"
#include "flexsea.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t fieldIds[CMD_DYNAMIC_MAX_NUMFIELDS];

// maps field id to pointer to data
uint8_t* fieldPointers[CMD_DYNAMIC_MAX_NUMFIELDS];

// maps field id to length of data in bytes
uint8_t fieldLengths[CMD_DYNAMIC_MAX_NUMFIELDS];


void init_flexsea_payload_ptr_dynamic(void)
{
	int i;
	for(i = 0; i < CMD_DYNAMIC_MAX_NUMFIELDS; i++) {	fieldIds[i]=0; }
}

//Execute

void tx_cmd_dynamic_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, uint16_t *len, \
						uint8_t slave, uint8_t numFields, uint8_t* fields, int32_t* values)
{
	if(numFields > CMD_DYNAMIC_MAX_NUMFIELDS) return;

	(*cmd) = CMD_DYNAMIC;
	(*cmdType) = CMD_DYNAMIC_RW;

	int index = 0;
	shBuf[index++] = slave;
	shBuf[index++] = numFields;

	int i;
	for(i = 0; i < numFields; i++)
	{
		shBuf[index++] = fields[i];
	}

	for(i = 0; i < numFields; i++)
	{
		SPLIT_32((uint32_t)values[i], shBuf, &index);
	}

	*len = index;
}

void parseAndPackReply(uint8_t* buf)
{
	int index = P_DATA1;
	uint8_t slave = buf[index++];
	uint8_t numFields = buf[index++];
	int i;
	if(numFields > CMD_DYNAMIC_MAX_NUMFIELDS) return;

	uint8_t fieldIdResponse[CMD_DYNAMIC_MAX_NUMFIELDS];
	uint8_t fieldResponse[4*CMD_DYNAMIC_MAX_NUMFIELDS]; //4 bytes for each field

	uint8_t fieldId;
	uint8_t* dataPointer;
	uint8_t fieldLength;
	for(i = 0; i < numFields; i++)
	{
		fieldId = buf[index+i];
		if(fieldId >= 0 && fieldId < FIELD_ID_MAX)
		{
			fieldLength = fieldLengths[fieldId];
			dataPointer = fieldPointers[fieldId];
			
			fieldIdResponse[i] = fieldId;
			if(dataPointer != NULL && fieldLength <= 4)
			{
				int j;
				for(j = 0; j < fieldLength; j++)
				{
					fieldResponse[i*4 + j] = dataPointer[j];
				}
			}
			else
			{
				buf[i] = -1;
			}	
		}
		else { fieldIdResponse[i] = -1; }
	}

	tx_cmd_dynamic_w(TX_N_DEFAULT, slave, numFields, fieldIdResponse, fieldResponse);
}

void parseAndSave(uint8_t* buf)
{
	int index = P_DATA1;
	uint8_t slave = buf[index++];
	uint8_t numFields = buf[index++];
	int i;
	if(numFields > CMD_DYNAMIC_MAX_NUMFIELDS) return;

	uint8_t ids[CMD_DYNAMIC_MAX_NUMFIELDS];
	uint8_t* dataPointer;
	uint8_t fieldLength;
	
	for(i = 0; i < numFields; i++)
	{
		ids[i] = buf[index++];
	}

	for(i = 0; i < numFields; i++)
	{
		if(fieldId >= 0 && fieldId < FIELD_ID_MAX)
		{
			dataPointer = fieldPointers[ids[i]];
			fieldLength = fieldLengths[ids[i]];

			if(dataPointer != NULL && fieldLength <= 4)
			{
				int j;
				for(j = 0; j < fieldLength; j++)
				{
					dataPointer[j] = buf[index + i*4 + j];
				}
			}
		}
	}	
}

void rx_cmd_dynamic_r(uint8_t *buf, uint8_t *info)
{
	#if(defined BOARD_TYPE_FLEXSEA_EXECUTE)
		parseAndPackReply(buf);
		packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, SEND_TO_MASTER);

		(void) info;
	#elif(defined BOARD_TYPE_FLEXSEA_MANAGE)
		int slave = buf[P_DATA1];
		if(slave == 0) 
		{ 
			parseAndPackReply(buf);
		}
		else 
		{
			//	still need to figure this out
		}
	#else
		(void) info;
		(void) buf;
	#endif
}

void rx_cmd_dynamic_w(uint8_t *buf, uint8_t *info)
{
	#if(defined BOARD_TYPE_FLEXSEA_EXECUTE)
		parseAndSave(buf);
		(void) info;
	#elif(defined BOARD_TYPE_FLEXSEA_MANAGE)
		int slave = buf[P_DATA1];
		if(slave == 0) 
		{ 
			parseAndSave(buf);
		}
		else 
		{
			//	still need to figure this out
		}
	#else
		(void) info;
		(void) buf;
	#endif
}

#ifdef __cplusplus
}
#endif
