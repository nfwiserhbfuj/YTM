/*
 * Copyright 2020-2026 Yuntu Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * @file linflexd_uart_config.c
 * @brief 
 * 
 */



#include "linflexd_uart_config.h"


/*linflexd_uart_config0*/
linflexd_uart_state_t linflexd_uart_config0_State;
const linflexd_uart_user_config_t linflexd_uart_config0 = {
    .baudRate=115200U,
    .parityCheck=false,
    .parityType=LINFlexD_UART_PARITY_EVEN,
    .stopBitsCount=LINFlexD_UART_ONE_STOP_BIT,
    .wordLength=LINFlexD_UART_8_BITS,
    .txTransferType=LINFlexD_UART_USING_INTERRUPTS,
    .rxTransferType=LINFlexD_UART_USING_INTERRUPTS,
    .txDMAChannel=0,
    .rxDMAChannel=0,
};

