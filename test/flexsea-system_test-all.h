#ifndef TEST_ALL_FX_SYSTEM_H
#define TEST_ALL_FX_SYSTEM_H

#include "main.h"
#include "unity.h"
#include "../inc/flexsea_system.h"

#define TEST_PL_LEN		4
#define FILLER			0

extern uint8_t tmpPayload[PAYLOAD_BUF_LEN];
extern uint8_t transferBuf[COMM_STR_BUF_LEN];
extern uint8_t cmdCode, cmdType;
extern uint16_t len;

void prepTxCmdTest(void);
int flexsea_system_test(void);

//Prototypes for public functions defined in individual test files:
void test_flexsea_system(void);
void test_flexsea_cmd_application(void);
void test_flexsea_cmd_control(void);
void test_flexsea_cmd_data(void);
void test_flexsea_cmd_external(void);
void test_flexsea_cmd_sensors(void);

#endif	//TEST_ALL_FX_SYSTEM_H
