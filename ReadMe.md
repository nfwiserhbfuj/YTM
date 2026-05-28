# YTM — 云途半导体 MCU Demo 项目集

基于 [云途半导体](https://www.yuntu-mcu.com/) YTM32B1 系列 MCU 的示例项目集合。

## 项目列表

| 项目 | 说明 | 芯片型号 | 开发板 |
|------|------|----------|--------|
| [Flexcan_Canfd_Demo](./Flexcan_Canfd_Demo/) | FlexCAN CANFD 收发示例 | YTM32B1ME0 | YTM32B1ME0-EVB-Q144 |

## Flexcan_Canfd_Demo 简介

演示 FlexCAN CANFD 模式下双 Region 不同负载多邮箱收发通信：

- **CANFD 模式**：标称波特率 500Kbps，数据波特率 2000Kbps
- **双 Region 不同负载**：Region 0 (MBDSR0) = 16B，Region 1 (MBDSR1) = 8B，配置 53 个邮箱
- **16B 报文 (Region 0)**：物理 MB 0~2 (RX)、MB 3~6 (TX)
- **8B 报文 (Region 1)**：物理 MB 21~28 (RX STD)、MB 29~36 (RX EXT)、MB 37~44 (TX STD)、MB 45~52 (TX EXT)

> 详细说明参见 [Flexcan_Canfd_Demo/README.md](./Flexcan_Canfd_Demo/README.md)

## 环境要求

| 工具 | 版本 |
|------|------|
| SDK | YTM32B1ME0_1_4_0+ |
| GCC | GNU GCC v10.3+ |
| Keil/MDK | ARM Keil/MDK v5.36+ |
| IAR | IAR v8.40+ |
| VS Code | v1.76+ (CMake v3.25, Ninja, Cortex-Debug) |
| J-LINK | SEGGER JLINK v6.96+ (需安装 YTM32B1ME0 补丁) |

## 目录结构

```
YTM/
├── README.md
└── Flexcan_Canfd_Demo/
    ├── app/                  # 应用层代码 (main.c)
    ├── board/                # 板级配置 (CAN/Clock/Pin/DMA/Interrupt)
    ├── cmake/                # CMake 工具链 & 构建脚本
    ├── CMSIS/                # CMSIS-Core (Cortex-M33)
    ├── middleware/            # 中间件 (printf)
    ├── platform/             # MCU 驱动 & 设备头文件
    │   ├── devices/          # 芯片寄存器定义
    │   └── drivers/          # 外设驱动 (FlexCAN/DMA/Clock/Pins/UART)
    ├── rtos/                 # OS 抽象层 (BareMetal)
    └── CMakeLists.txt        # 顶层构建入口
```

## 许可证

SPDX-License-Identifier: BSD-3-Clause

Copyright 2020-2026 Yuntu Microelectronics Co., Ltd.
