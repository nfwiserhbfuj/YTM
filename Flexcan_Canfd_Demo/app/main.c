/* USER CODE BEGIN Header */
/* you can remove the copyright */

/*
 *  Copyright 2020-2023 Yuntu Microelectronics Co., Ltd.
 *  All rights reserved.
 * 
 *  SPDX-License-Identifier: BSD-3-Clause
 * 
 * @file main.c
 * @brief 
 * 
 */

/* USER CODE END Header */
#include "sdk_project_config.h"
/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CAN0_INST (0U)

#define RX_MSG_ID1 (0x100U)
#define TX_MSG_ID1 (0x200U)

#define RX_MSG_ID2 (0x18DA00F1)
#define TX_MSG_ID2 (0x1AABBCC)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

volatile uint8_t can0_rx_flag = 0;

/* CAN0 configuration */
/* Define receive buffer */
flexcan_msgbuff_t rxMsg1;
/* Prepare message to be sent */
flexcan_msgbuff_t txMsg0 = {
    .cs = 0U,
    .msgId = TX_MSG_ID1,
    .data[0] = 0x00,
    .dataLen = 64U,
};

flexcan_msgbuff_t txMsg1 = {
    .cs = 0U,
    .msgId = TX_MSG_ID1,
    .data[0] = 0x00,
    .dataLen = 64U,
};

/* 64-bit message */
const flexcan_data_info_t rxMbStdInfo0 = {
    .msg_id_type = FLEXCAN_MSG_ID_STD,
    .data_length = 16,
    .fd_enable = true,
    .fd_padding = 0,
    .enable_brs = true,
    .is_remote = false,
};

const flexcan_data_info_t rxMbExtInfo0 = {
    .msg_id_type = FLEXCAN_MSG_ID_EXT,
    .data_length = 16,
    .fd_enable = true,
    .fd_padding = 0,
    .enable_brs = true,
    .is_remote = false,
};

const flexcan_data_info_t txMbStdInfo0 = {
    .msg_id_type = FLEXCAN_MSG_ID_STD,
    .data_length = 16,
    .fd_enable = true,
    .fd_padding = 0,
    .enable_brs = true,
    .is_remote = false,
};

const flexcan_data_info_t txMbExtInfo0 = {
    .msg_id_type = FLEXCAN_MSG_ID_EXT,
    .data_length = 16,
    .fd_enable = true,
    .fd_padding = 0,
    .enable_brs = true,
    .is_remote = false,
};

/* 8-bit message */
const flexcan_data_info_t rxMbStdInfo1 = {
    .msg_id_type = FLEXCAN_MSG_ID_STD,
    .data_length = 8,
    .fd_enable = true,
    .fd_padding = 0,
    .enable_brs = true,
    .is_remote = false,
};

const flexcan_data_info_t rxMbExtInfo1 = {
    .msg_id_type = FLEXCAN_MSG_ID_EXT,
    .data_length = 8,
    .fd_enable = true,
    .fd_padding = 0,
    .enable_brs = true,
    .is_remote = false,
};

const flexcan_data_info_t txMbStdInfo1 = {
    .msg_id_type = FLEXCAN_MSG_ID_STD,
    .data_length = 8,
    .fd_enable = true,
    .fd_padding = 0,
    .enable_brs = true,
    .is_remote = false,
};

const flexcan_data_info_t txMbExtInfo1 = { 
    .msg_id_type = FLEXCAN_MSG_ID_EXT,
    .data_length = 8,
    .fd_enable = true,
    .fd_padding = 0,
    .enable_brs = true,
    .is_remote = false,
};
/* USER CODE END PV */

/* Private function declare --------------------------------------------------*/
/* USER CODE BEGIN PFDC */
/* USER CODE END PFDC */
static void Board_Init(void);

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void Can0Tp_Hal_EventCallback(
    uint8_t inst,
    flexcan_event_type_t eventType,
    uint32_t mbIdx,
    flexcan_state_t *flexCanState
)
{
    switch (eventType)
    {
        case FLEXCAN_EVENT_RX_COMPLETE:
            can0_rx_flag++;
            FLEXCAN_DRV_Receive(CAN0_INST, mbIdx, &rxMsg1);
            break;
        case FLEXCAN_EVENT_TX_COMPLETE:
            break;
        default:
            break;
    }
}

/* USER CODE END 0 */


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */
    status_t status = STATUS_SUCCESS;
    uint8_t offset1 = 0;
    /* USER CODE END 1 */ 
    Board_Init();
    /* USER CODE BEGIN 2 */
    FLEXCAN_DRV_InstallEventCallback(0, Can0Tp_Hal_EventCallback, NULL);

    /* Configure 64B RX buffer: MB 0~2 */
    status |= FLEXCAN_DRV_ConfigRxMb(CAN0_INST, 0, &rxMbStdInfo0, RX_MSG_ID1);
    status |= FLEXCAN_DRV_ConfigRxMb(CAN0_INST, 1, &rxMbStdInfo0, RX_MSG_ID1+1);
    status |= FLEXCAN_DRV_ConfigRxMb(CAN0_INST, 2, &rxMbExtInfo0, RX_MSG_ID2);

    /* Configure 64B TX buffer: MB 3~6 */
    status |= FLEXCAN_DRV_ConfigTxMb(CAN0_INST, 3, &txMbStdInfo0, TX_MSG_ID1);
    status |= FLEXCAN_DRV_ConfigTxMb(CAN0_INST, 4, &txMbStdInfo0, TX_MSG_ID1+1);
    status |= FLEXCAN_DRV_ConfigTxMb(CAN0_INST, 5, &txMbExtInfo0, TX_MSG_ID2);
    status |= FLEXCAN_DRV_ConfigTxMb(CAN0_INST, 6, &txMbExtInfo0, TX_MSG_ID2+1);

    /* Configure 8B RX STD buffer: MB 7~14 */
    for(int i = 0; i < 8; i++)
    {
        status |= FLEXCAN_DRV_ConfigRxMb(CAN0_INST, 21+i, &rxMbStdInfo1, RX_MSG_ID1+2+i);
    }

    /* Configure 8B RX EXT buffer: MB 15~22 */
    for(int i = 0; i < 8; i++)
    {
        status |= FLEXCAN_DRV_ConfigRxMb(CAN0_INST, 29+i, &rxMbExtInfo1, RX_MSG_ID2+1+i);
    }

    /* Configure 8B TX STD buffer: MB 23~30 */
    for(int i = 0; i < 8; i++)
    {
        status |= FLEXCAN_DRV_ConfigTxMb(CAN0_INST, 37+i, &txMbStdInfo1, TX_MSG_ID1+i+2);
    }

    /* Configure 8B TX EXT buffer: MB 31~38 */
    for(int i = 0; i < 8; i++)
    {
        status |= FLEXCAN_DRV_ConfigTxMb(CAN0_INST, 45+i, &txMbExtInfo1, TX_MSG_ID2+i+2);
    }
    
    /* Start receiving data in RX_MAILBOX. */
    for(int i = 0; i < 3; i++)
    {
        status |= FLEXCAN_DRV_Receive(CAN0_INST, i, &rxMsg1);
    }
    for(int i = 0; i < 16; i++)
    {
        status |= FLEXCAN_DRV_Receive(CAN0_INST, i+21, &rxMsg1);
    }

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        if (status != STATUS_SUCCESS)
        {
            break;
        }
         /* 64B TX: MB 3~6 */
        if (FLEXCAN_DRV_GetTransferStatus(CAN0_INST, 3) != STATUS_BUSY)
        {
            for (int i = 0; i < 16; i++)
            {
                txMsg0.data[i] = offset1 + i;
            }
            offset1++;
            status |= FLEXCAN_DRV_Send(CAN0_INST, 3, &txMbStdInfo0, TX_MSG_ID1, txMsg0.data);
        }

        if (FLEXCAN_DRV_GetTransferStatus(CAN0_INST, 4) != STATUS_BUSY)
        {
            status |= FLEXCAN_DRV_Send(CAN0_INST, 4, &txMbStdInfo0, TX_MSG_ID1+1, txMsg0.data);
        }

        if (FLEXCAN_DRV_GetTransferStatus(CAN0_INST, 5) != STATUS_BUSY)
        {
            status |= FLEXCAN_DRV_Send(CAN0_INST, 5, &txMbExtInfo0, TX_MSG_ID2, txMsg0.data);
        }

        if (FLEXCAN_DRV_GetTransferStatus(CAN0_INST, 6) != STATUS_BUSY)
        {
            status |= FLEXCAN_DRV_Send(CAN0_INST, 6, &txMbExtInfo0, TX_MSG_ID2+1, txMsg0.data);
        }
        
        /* 8B TX STD: MB 37~44 */
        for(int i = 0; i < 8; i++)
        {
            if (FLEXCAN_DRV_GetTransferStatus(CAN0_INST, 37+i) != STATUS_BUSY)
            {
                status |= FLEXCAN_DRV_Send(CAN0_INST, 37+i, &txMbStdInfo1, TX_MSG_ID1+i+2, txMsg0.data);
            }
        }

        /* 8B TX EXT: MB 45~52 */
        for(int i = 0; i < 8; i++)
        {
            if (FLEXCAN_DRV_GetTransferStatus(CAN0_INST, 45+i) != STATUS_BUSY)
            {
                status |= FLEXCAN_DRV_Send(CAN0_INST, 45+i, &txMbExtInfo1, TX_MSG_ID2+i+2, txMsg0.data);
            }
        }

        OSIF_TimeDelay(10);
        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

static void Board_Init(void)
{
    CLOCK_SYS_Init(g_clockManConfigsArr,CLOCK_MANAGER_CONFIG_CNT,g_clockManCallbacksArr,CLOCK_MANAGER_CALLBACK_CNT);
    if(STATUS_SUCCESS != CLOCK_SYS_UpdateConfiguration(CLOCK_MANAGER_ACTIVE_INDEX,CLOCK_MANAGER_POLICY_AGREEMENT))
    {
        /* USER CODE BEGIN ERROR_HANDLER 1 */
        SystemSoftwareReset();
        /* USER CODE END ERROR_HANDLER 1 */
    }
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0,g_pin_mux_InitConfigArr0);
    FLEXCAN_DRV_Init(0,&flexcanInitConfig0_State,&flexcanInitConfig0);
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */
