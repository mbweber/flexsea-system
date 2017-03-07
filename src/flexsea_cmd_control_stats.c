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
	[This file] flexsea_cmd_control_stats: Exposing controller stats to the GUI
*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "flexsea_cmd_control_stats.h"
#include "flexsea.h"

#include "flexsea_board.h"

#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
#include "control.h"
#endif
#ifdef BOARD_TYPE_FLEXSEA_PLAN
#include <QDebug>
#endif


void tx_cmd_control_stats_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, uint16_t *len, \
						uint8_t slave)
{
	*cmd = CMD_CONTROL_STATS;
	*cmdType = CMD_READ;

	int index = 0;
	shBuf[index++] = slave;
	*len = index;
}

#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
static void fillResponse(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, uint16_t *len)
{

	int index = 0;
	//this code will simplify once we switch to the generic controller implementation

	//setpoint, actual, effort?
	if(ctrl.active_ctrl == CTRL_POSITION)
	{
		SPLIT_32((uint32_t)ctrl.position.setp, shBuf, &index);
		SPLIT_32((uint32_t)ctrl.position.pos, shBuf, &index);
	}
	else if(ctrl.active_ctrl == CTRL_OPEN) {;}	// skip, since in open control we just set a pwm
	else if(ctrl.active_ctrl == CTRL_CURRENT || ctrl.active_ctrl == CTRL_IMPEDANCE) 
	{
		SPLIT_32((uint32_t)ctrl.current.setpoint_val, shBuf, &index);
		SPLIT_32((uint32_t)exec1.current, shBuf, &index);
	}

	*index = 8; //I don't care if above code is ballsed up, PWM goes at the (zero-indexed) 8th byte
	shBuf[8] = exec1.sine_commut_pwm;
}
#endif //BOARD_TYPE_FLEXSEA_EXECUTE

void rx_cmd_control_stats_r(uint8_t *buf, uint8_t *info)
{
	int index = 0;
	uint8_t slave = buf[index++];

	#if(defined BOARD_TYPE_FLEXSEA_MANAGE)
	
		//pass message on to execute?

	#elif(defined BOARD_TYPE_FLEXSEA_EXECUTE)

		fillResponse(TX_N_DEFAULT);
		packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, SEND_TO_MASTER);
	
	#endif
}

void rx_cmd_control_stats_rr(uint8_t *buf, uint8_t *info)
{
	#if(defined BOARD_TYPE_FLEXSEA_MANAGE)
	
		//pass message on to plan?

	#elif(defined BOARD_TYPE_FLEXSEA_PLAN)

		//do smthng
		qDebug() << "Received control stats."
	
	#endif
}

#ifdef __cplusplus
}
#endif
