/*
 * Copyright 2020-2026 Yuntu Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * @file pin_mux.c
 * @brief 
 * 
 */



#include "pin_mux.h"

/*! @brief User configuration cfg InitConfigArr0 array */
const pin_settings_config_t g_pin_mux_InitConfigArr0[NUM_OF_CONFIGURED_PINS0] = {
    /* PTA_9-143-LINFlexD2_TX- */
    {
        .base = PCTRLA,
        .pinPortIdx = 9U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .passiveFilter = false,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .mux = PCTRL_MUX_ALT2,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
            .width = 0U,
        },
        .gpioBase = GPIOA,
        .direction = GPIO_INPUT_DIRECTION,
        .initValue = 0,
    },
    /* PTA_8-144-LINFlexD2_RX- */
    {
        .base = PCTRLA,
        .pinPortIdx = 8U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .passiveFilter = false,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .mux = PCTRL_MUX_ALT2,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
            .width = 0U,
        },
        .gpioBase = GPIOA,
        .direction = GPIO_INPUT_DIRECTION,
        .initValue = 0,
    },
    /* PTB_4-41-GPIO-LED5 */
    {
        .base = PCTRLB,
        .pinPortIdx = 4U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .passiveFilter = false,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .mux = PCTRL_MUX_AS_GPIO,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
            .width = 0U,
        },
        .gpioBase = GPIOB,
        .direction = GPIO_OUTPUT_DIRECTION,
        .initValue = 0,
    },
    /* PTD_2-102-CAN5_RX- */
    {
        .base = PCTRLD,
        .pinPortIdx = 2U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .passiveFilter = false,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .mux = PCTRL_MUX_ALT5,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
            .width = 0U,
        },
        .gpioBase = GPIOD,
        .direction = GPIO_INPUT_DIRECTION,
        .initValue = 0,
    },
    /* PTD_3-101-CAN5_TX- */
    {
        .base = PCTRLD,
        .pinPortIdx = 3U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .passiveFilter = false,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .mux = PCTRL_MUX_ALT5,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
            .width = 0U,
        },
        .gpioBase = GPIOD,
        .direction = GPIO_INPUT_DIRECTION,
        .initValue = 0,
    },
    /* PTD_4-100-GPIO- */
    {
        .base = PCTRLD,
        .pinPortIdx = 4U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .passiveFilter = false,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .mux = PCTRL_MUX_AS_GPIO,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
            .width = 0U,
        },
        .gpioBase = GPIOD,
        .direction = GPIO_OUTPUT_DIRECTION,
        .initValue = 0,
    },
    /* PTE_5-8-CAN0_TX- */
    {
        .base = PCTRLE,
        .pinPortIdx = 5U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .passiveFilter = false,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .mux = PCTRL_MUX_ALT5,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
            .width = 0U,
        },
        .gpioBase = GPIOE,
        .direction = GPIO_INPUT_DIRECTION,
        .initValue = 0,
    },
    /* PTE_4-9-CAN0_RX- */
    {
        .base = PCTRLE,
        .pinPortIdx = 4U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .passiveFilter = false,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .mux = PCTRL_MUX_ALT5,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
            .width = 0U,
        },
        .gpioBase = GPIOE,
        .direction = GPIO_INPUT_DIRECTION,
        .initValue = 0,
    },
    /* PTD_5-46-GPIO- */
    {
        .base = PCTRLD,
        .pinPortIdx = 5U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .passiveFilter = false,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .mux = PCTRL_MUX_AS_GPIO,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
            .width = 0U,
        },
        .gpioBase = GPIOD,
        .direction = GPIO_OUTPUT_DIRECTION,
        .initValue = 0,
    },
    /* PTE_8-39-GPIO- */
    {
        .base = PCTRLE,
        .pinPortIdx = 8U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .passiveFilter = false,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .mux = PCTRL_MUX_AS_GPIO,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
            .width = 0U,
        },
        .gpioBase = GPIOE,
        .direction = GPIO_OUTPUT_DIRECTION,
        .initValue = 0,
    },
};



/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

