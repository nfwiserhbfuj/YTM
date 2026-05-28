# FlexCAN FD 双 Region 不同负载修改方案

---

# 第一部分：代码使用介绍

## 运行环境

本演示代码运行在 **YTM32B1ME0** 芯片的单个 CAN 实例（CAN0）上，配置了 38 个邮箱，Region 0（MBDSR0）使用 16 字节 payload，Region 1（MBDSR1）使用 8 字节 payload 的 CAN FD 数据帧收发。

## 邮箱总览

应用程序中实际使用的邮箱如下：


| 逻辑 MB | 物理 MB 索引 | 方向 | 负载 | ID 类型 |    CAN ID    |  位于哪个 Region  |
| :-----: | :----------: | :--: | :--: | :-----: | :----------: | :---------------: |
|    0    |      0      |  RX  | 16B |   STD   |   `0x100`   | Region 0 (MBDSR0) |
|    1    |      1      |  RX  | 16B |   STD   |   `0x101`   | Region 0 (MBDSR0) |
|    2    |      2      |  RX  | 16B |   EXT   | `0x18DA00F1` | Region 0 (MBDSR0) |
|    3    |      3      |  TX  | 16B |   STD   |   `0x200`   | Region 0 (MBDSR0) |
|    4    |      4      |  TX  | 16B |   STD   |   `0x201`   | Region 0 (MBDSR0) |
|    5    |      5      |  TX  | 16B |   EXT   | `0x1AABBCC` | Region 0 (MBDSR0) |
|    6    |      6      |  TX  | 16B |   EXT   | `0x1AABBCD` | Region 0 (MBDSR0) |
|    7    |      21      |  RX  |  8B  |   STD   |   `0x102`   | Region 1 (MBDSR1) |
|    8    |      22      |  RX  |  8B  |   STD   |   `0x103`   | Region 1 (MBDSR1) |
|    9    |      23      |  RX  |  8B  |   STD   |   `0x104`   | Region 1 (MBDSR1) |
|   10   |      24      |  RX  |  8B  |   STD   |   `0x105`   | Region 1 (MBDSR1) |
|   11   |      25      |  RX  |  8B  |   STD   |   `0x106`   | Region 1 (MBDSR1) |
|   12   |      26      |  RX  |  8B  |   STD   |   `0x107`   | Region 1 (MBDSR1) |
|   13   |      27      |  RX  |  8B  |   STD   |   `0x108`   | Region 1 (MBDSR1) |
|   14   |      28      |  RX  |  8B  |   STD   |   `0x109`   | Region 1 (MBDSR1) |
|   15   |      29      |  RX  |  8B  |   EXT   | `0x18DA00F2` | Region 1 (MBDSR1) |
|   16   |      30      |  RX  |  8B  |   EXT   | `0x18DA00F3` | Region 1 (MBDSR1) |
|   17   |      31      |  RX  |  8B  |   EXT   | `0x18DA00F4` | Region 1 (MBDSR1) |
|   18   |      32      |  RX  |  8B  |   EXT   | `0x18DA00F5` | Region 1 (MBDSR1) |
|   19   |      33      |  RX  |  8B  |   EXT   | `0x18DA00F6` | Region 1 (MBDSR1) |
|   20   |      34      |  RX  |  8B  |   EXT   | `0x18DA00F7` | Region 1 (MBDSR1) |
|   21   |      35      |  RX  |  8B  |   EXT   | `0x18DA00F8` | Region 1 (MBDSR1) |
|   22   |      36      |  RX  |  8B  |   EXT   | `0x18DA00F9` | Region 1 (MBDSR1) |
|   23   |      37      |  TX  |  8B  |   STD   |   `0x202`   | Region 1 (MBDSR1) |
|   24   |      38      |  TX  |  8B  |   STD   |   `0x203`   | Region 1 (MBDSR1) |
|   25   |      39      |  TX  |  8B  |   STD   |   `0x204`   | Region 1 (MBDSR1) |
|   26   |      40      |  TX  |  8B  |   STD   |   `0x205`   | Region 1 (MBDSR1) |
|   27   |      41      |  TX  |  8B  |   STD   |   `0x206`   | Region 1 (MBDSR1) |
|   28   |      42      |  TX  |  8B  |   STD   |   `0x207`   | Region 1 (MBDSR1) |
|   29   |      43      |  TX  |  8B  |   STD   |   `0x208`   | Region 1 (MBDSR1) |
|   30   |      44      |  TX  |  8B  |   STD   |   `0x209`   | Region 1 (MBDSR1) |
|   31   |      45      |  TX  |  8B  |   EXT   | `0x1AABBCE` | Region 1 (MBDSR1) |
|   32   |      46      |  TX  |  8B  |   EXT   | `0x1AABBCF` | Region 1 (MBDSR1) |
|   33   |      47      |  TX  |  8B  |   EXT   | `0x1AABBD0` | Region 1 (MBDSR1) |
|   34   |      48      |  TX  |  8B  |   EXT   | `0x1AABBD1` | Region 1 (MBDSR1) |
|   35   |      49      |  TX  |  8B  |   EXT   | `0x1AABBD2` | Region 1 (MBDSR1) |
|   36   |      50      |  TX  |  8B  |   EXT   | `0x1AABBD3` | Region 1 (MBDSR1) |
|   37   |      51      |  TX  |  8B  |   EXT   | `0x1AABBD4` | Region 1 (MBDSR1) |
|   38   |      52      |  TX  |  8B  |   EXT   | `0x1AABBD5` | Region 1 (MBDSR1) |

> **注意**：上述表格中"逻辑 MB"是应用层的连续编号，"物理 MB 索引"是传递给 `FLEXCAN_DRV_ConfigRxMb` / `FLEXCAN_DRV_ConfigTxMb` 的实际参数值。物理 MB 索引不是连续的：Region 0 占用了物理索引 0~20（共 21 个 MB，应用层仅使用 0~6），Region 1 的邮箱从物理索引 21 开始。

### ID 公式

```c
#define RX_MSG_ID1 (0x100U)        // 标准帧 RX 基准
#define TX_MSG_ID1 (0x200U)        // 标准帧 TX 基准
#define RX_MSG_ID2 (0x18DA00F1)    // 扩展帧 RX 基准
#define TX_MSG_ID2 (0x1AABBCC)     // 扩展帧 TX 基准
```

- 16B RX MB 0~2：`RX_MSG_ID1`, `RX_MSG_ID1+1`, `RX_MSG_ID2`
- 16B TX MB 3~6：`TX_MSG_ID1`, `TX_MSG_ID1+1`, `TX_MSG_ID2`, `TX_MSG_ID2+1`
- 8B RX STD MB 21~29：`RX_MSG_ID1 + i + 2`，i=0-7
- 8B RX EXT MB 29~36：`RX_MSG_ID2 + i + 2`，i=0-7
- 8B TX STD MB 37~44：`TX_MSG_ID1 + i + 2`，i=0-7
- 8B TX EXT MB 45~52：`TX_MSG_ID2 + i + 2`，i=0-7

## 邮箱分组

### 第一组：16B 负载 (物理 MB 0~6, Region 0)


| MB | 功能 | 说明                                        |
| :-: | :--- | :------------------------------------------ |
| 0~2 | RX   | 接收 16B CAN FD 帧                          |
| 3~6 | TX   | 发送 16B CAN FD 帧，数据 = offset1 + i 递增 |

### 第二组：8B RX (物理 MB 21~36, Region 1)


| MB 范围 | 帧类型 |           ID 范围           |
| :-----: | :----: | :-------------------------: |
|  21~28  | 标准帧 |      `0x102` ~ `0x109`      |
|  29~36  | 扩展帧 | `0x18DA00F2` ~ `0x18DA00F9` |

### 第三组：8B TX (物理 MB 37~52, Region 1)


| MB 范围 | 帧类型 |          ID 范围          |
| :-----: | :----: | :-----------------------: |
|  37~44  | 标准帧 |     `0x202` ~ `0x209`     |
|  45~52  | 扩展帧 | `0x1AABBCE` ~ `0x1AABBD5` |

## 程序流程

```
1. Board_Init() → 时钟/引脚/CAN0(Region0=16B, Region1=8B, 53MB)
2. 注册回调 → RX COMPLETE 时 can0_rx_flag++, 自动 re-arm
3. 配置 39 个邮箱 (RxMb/TxMb)
4. 启动接收 (Receive on MB 0~2, 21~36)
5. while(1): 检查状态 → Send 数据 → Delay(10ms)
```

## 回调说明

```c
FLEXCAN_DRV_InstallEventCallback(0, Can0Tp_Hal_EventCallback, NULL);
```

- RX COMPLETE：`can0_rx_flag++` → 自动 `Receive` 重新 arm
- TX COMPLETE：无额外处理，由 while 循环检测

## CAN FD 参数


| 参数    | 说明                                                |
| :------ | :-------------------------------------------------- |
| 仲裁段  | `propSeg=4, phaseSeg1=7, phaseSeg2=1, preDivider=2` |
| 数据段  | `propSeg=6, phaseSeg1=2, phaseSeg2=1, preDivider=0` |
| FD 模式 | `.fd_enable = true`                                 |
| BRS     | `.enable_brs = true`（数据段高速率）                |
| 填充    | `.fd_padding = 0`                                   |

## 数据流示例

```
CAN0 MB3 发送 16B → 对端接收:
  ID=0x200, DLC=4, Data=[0x00..0x0F], BRS=1, IDE=0

CAN0 MB37 (逻辑23) 发送 8B → 对端接收:
  ID=0x202, DLC=8, Data=txMsg0前8字节, BRS=1, IDE=0
```

---

# 第二部分：修改说明与原理讲解

原始代码（`C:\Users\ytm\Desktop\test\ME0\Flexcan_Canfd_Demo`）中，**Region 0 和 Region 1 使用相同的负载大小**（由 `flexcanInitConfig.payload` 统一配置，例如 `FLEXCAN_PAYLOAD_SIZE_64`）。所有邮箱无论位于哪个 Region，都有相同大小的数据区。

本修改方案的目标是：**让 Region 0（MBDSR0）和 Region 1（MBDSR1）使用不同负载大小**，具体为 **Region 0 = 16 bytes，Region 1 = 8 bytes**。

> **核心原理**：YTM32B1ME0 的 FlexCAN 模块有 1024 bytes（256 words）的 Message Buffer RAM，分为两个 512 bytes 块。通过 FDCTRL 寄存器中的 MBDSR0 和 MBDSR1 位域，可以分别为两块 RAM 设置不同的 payload 大小。`FEATURE_CAN_HAS_MBDSR1` 特性宏已使能。

---

## 修改涉及的 6 个文件


| 文件                  | 路径                                               | 说明                               |
| :-------------------- | :------------------------------------------------- | :--------------------------------- |
| `flexcan_driver.h`    | `platform/drivers/inc/flexcan_driver.h`            | 配置结构体定义                     |
| `flexcan_driver.c`    | `platform/drivers/src/flexcan/flexcan_driver.c`    | 驱动层 Init 和 GetDefaultConfig    |
| `flexcan_hw_access.h` | `platform/drivers/src/flexcan/flexcan_hw_access.h` | 硬件访问层函数声明                 |
| `flexcan_hw_access.c` | `platform/drivers/src/flexcan/flexcan_hw_access.c` | 硬件访问层函数实现（**7 处修改**） |
| `can_config.c`        | `board/can_config.c`                               | 驱动初始化参数                     |
| `main.c`              | `app/main.c`                                       | 应用层邮箱分配和收发逻辑           |

---

## 修改 1：`flexcan_user_config_t` 结构体 —— 拆分 payload 字段

### 位置

```
platform/drivers/inc/flexcan_driver.h 第 350 行附近
```

### 修改前（原始代码）

```c
typedef struct
{
    ...
#if FEATURE_CAN_HAS_FD
    flexcan_fd_payload_size_t payload;              /*!< The payload size of the mailboxes specified in bytes. */
    bool fd_enable;                                 /*!< Enable/Disable the Flexible Data Rate feature. */
#endif
    ...
} flexcan_user_config_t;
```

### 修改后

```c
typedef struct
{
    ...
#if FEATURE_CAN_HAS_FD
    flexcan_fd_payload_size_t region0_payload;      /*!< The payload size of Region 0 (MBDSR0) mailboxes specified in bytes. */
    flexcan_fd_payload_size_t region1_payload;      /*!< The payload size of Region 1 (MBDSR1) mailboxes specified in bytes.
                                                         Set to the same value as payload for single-region mode. */
    bool fd_enable;                                 /*!< Enable/Disable the Flexible Data Rate feature. */
#endif
    ...
} flexcan_user_config_t;
```

### 说明

原始结构只有一个 `payload` 字段，两个 Region 使用相同的 payload 大小。修改后将 `payload` 拆分为 `region0_payload` 和 `region1_payload` 两个字段，用户可为每个 Region 独立配置不同的 payload 大小。

---

## 修改 2：`flexcan_driver.c` 驱动层调用

### 位置

`FLEXCAN_DRV_Init()`：`platform/drivers/src/flexcan/flexcan_driver.c` 第 985 行附近

`FLEXCAN_DRV_GetDefaultConfig()`：同文件第 3490 行附近

### 修改前（原始代码）

```c
/* FLEXCAN_DRV_Init 中 */
FLEXCAN_SetPayloadSize(base, data->payload);

/* FLEXCAN_DRV_GetDefaultConfig 中 */
config->payload = FLEXCAN_PAYLOAD_SIZE_8;
```

### 修改后

```c
/* FLEXCAN_DRV_Init 中 */
FLEXCAN_SetPayloadSize(base, data->region0_payload, data->region1_payload);

/* FLEXCAN_DRV_GetDefaultConfig 中 */
config->region0_payload = FLEXCAN_PAYLOAD_SIZE_8;
config->region1_payload = FLEXCAN_PAYLOAD_SIZE_8;
```

### 说明

驱动初始化时传递两个 Region 的 payload 参数。`GetDefaultConfig` 中将两个 Region 都初始化为 8B（安全默认值），用户根据实际需求覆盖。

---

## 修改 3：`flexcan_hw_access.h` 函数声明

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.h 第 575~595 行
```

### 修改前（原始代码）

```c
void FLEXCAN_SetPayloadSize(
    CAN_Type * base,
    flexcan_fd_payload_size_t payloadSize);

uint8_t FLEXCAN_GetPayloadSize(const CAN_Type * base);
```

### 修改后

```c
void FLEXCAN_SetPayloadSize(
    CAN_Type * base,
    flexcan_fd_payload_size_t region0Payload,
    flexcan_fd_payload_size_t region1Payload);

uint8_t FLEXCAN_GetPayloadSize(const CAN_Type * base, uint32_t msgBuffIdx);
```

### 说明

- `FLEXCAN_SetPayloadSize` 从单参数改为双参数，分别指定 Region 0 和 Region 1 的 payload 大小。
- `FLEXCAN_GetPayloadSize` 新增 `msgBuffIdx` 参数，根据邮箱索引判断该邮箱属于哪个 Region，返回对应的 payload 大小。

---

## 修改 4：新增 `FLEXCAN_GetRegion0MaxMbCount` / `FLEXCAN_GetRegion1MaxMbCount` 辅助函数

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
修改后第 375~393 行（两个函数均在 `FLEXCAN_GetMsgBuffRegion` 之前）
```

### 新增代码

```c
/*!
 * @brief Computes how many MBs fit in Region 0's 512-byte block
 *        based on the currently configured MBDSR0 payload size.
 */
static inline uint8_t FLEXCAN_GetRegion0MaxMbCount(const CAN_Type * base)
{
    uint32_t payloadSize = 1UL << (((base->FDCTRL & CAN_FDCTRL_MBDSR0_MASK)
                                    >> CAN_FDCTRL_MBDSR0_SHIFT) + 3U);
    return (uint8_t)(512U / (uint8_t)(payloadSize + 8U));
}

/*!
 * @brief Computes how many MBs fit in Region 1's 512-byte block
 *        based on the currently configured MBDSR1 payload size.
 */
static inline uint8_t FLEXCAN_GetRegion1MaxMbCount(const CAN_Type * base)
{
    uint32_t payloadSize = 1UL << (((base->FDCTRL & CAN_FDCTRL_MBDSR1_MASK)
                                    >> CAN_FDCTRL_MBDSR1_SHIFT) + 3U);
    return (uint8_t)(512U / (uint8_t)(payloadSize + 8U));
}
```

### 说明

这两个函数是**本次修改的核心基础设施**。它们根据 FDCTRL 寄存器中的 MBDSR0/MBDSR1 当前配置值，动态计算每个 512B RAM 块中能容纳的最大邮箱数量。

- MB size = payload_size + 8 (仲裁字段固定 8 字节)
- Max MB count = 512 / (payload_size + 8)，向下取整

对于当前配置（Region0=16B, Region1=8B）：

- Region 0: 512 / (16 + 8) = 512 / 24 = 21 MBs
- Region 1: 512 / (8 + 8) = 512 / 16 = 32 MBs
- **总计**：21 + 32 = **53 MBs**

---

## 修改 5：`FLEXCAN_GetPayloadSize` 函数体

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 2014 行附近 → 修改后第 2054 行附近
```

### 修改前（原始代码）

```c
uint8_t FLEXCAN_GetPayloadSize(const CAN_Type * base)
{
    uint32_t payloadSize;

    if (!FLEXCAN_IsFDEnabled(base))
    {
        payloadSize = 8U;
    }
    else
    {
        payloadSize = 1UL << (((base->FDCTRL & CAN_FDCTRL_MBDSR0_MASK)
                               >> CAN_FDCTRL_MBDSR0_SHIFT) + 3U);
    }

    return (uint8_t)payloadSize;
}
```

### 修改后

```c
uint8_t FLEXCAN_GetPayloadSize(const CAN_Type * base, uint32_t msgBuffIdx)
{
    uint32_t payloadSize;

    if (!FLEXCAN_IsFDEnabled(base))
    {
        payloadSize = 8U;
    }
    else if (msgBuffIdx < FLEXCAN_GetRegion0MaxMbCount(base))
    {
        payloadSize = 1UL << (((base->FDCTRL & CAN_FDCTRL_MBDSR0_MASK)
                               >> CAN_FDCTRL_MBDSR0_SHIFT) + 3U);
    }
    else
    {
        payloadSize = 1UL << (((base->FDCTRL & CAN_FDCTRL_MBDSR1_MASK)
                               >> CAN_FDCTRL_MBDSR1_SHIFT) + 3U);
    }

    return (uint8_t)payloadSize;
}
```

### 说明

- 原始函数始终读取 MBDSR0，对双 Region 来说不准确。
- 修改后通过 `msgBuffIdx < FLEXCAN_GetRegion0MaxMbCount(base)` 判断邮箱属于哪个 Region：
  - 物理索引 0~20（`region0MaxMb - 1`） → 读取 MBDSR0 → 16B
  - 物理索引 21+ → 读取 MBDSR1 → 8B
- 边界值由 `FLEXCAN_GetRegion0MaxMbCount` 动态计算，与 payload 配置自动适配。

---

## 修改 6：`FLEXCAN_GetMsgBuffRegion` 函数体 —— 双 Region 地址计算

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 376 行附近 → 修改后第 399 行附近
```

### 修改前（原始代码）

```c
volatile uint32_t* FLEXCAN_GetMsgBuffRegion(
        CAN_Type * base,
        uint32_t msgBuffIdx)
{
#if FEATURE_CAN_HAS_FD
    uint8_t payload_size = FLEXCAN_GetPayloadSize(base);
#else
    uint8_t payload_size = 8U;
#endif

    uint8_t arbitration_field_size = 8U;
    uint32_t ramBlockSize = 512U;
    uint32_t ramBlockOffset;

    uint8_t mb_size = (uint8_t)(payload_size + arbitration_field_size);
    uint8_t maxMbNum = (uint8_t)(ramBlockSize / mb_size);

    ramBlockOffset = 128U * (msgBuffIdx / (uint32_t)maxMbNum);

    uint32_t mb_index = ramBlockOffset
        + ((msgBuffIdx % (uint32_t)maxMbNum) * ((uint32_t)mb_size >> 2U));

    return &(base->RAM[mb_index]);
}
```

### 修改后

```c
volatile uint32_t* FLEXCAN_GetMsgBuffRegion(
        CAN_Type * base,
        uint32_t msgBuffIdx)
{
#if FEATURE_CAN_HAS_FD
    uint8_t payload_size = FLEXCAN_GetPayloadSize(base, msgBuffIdx);
#else
    uint8_t payload_size = 8U;
#endif

    uint8_t arbitration_field_size = 8U;
    uint32_t mb_index;

#if FEATURE_CAN_HAS_FD
    uint8_t region0MaxMb = FLEXCAN_GetRegion0MaxMbCount(base);

    if (msgBuffIdx < region0MaxMb)
    {
        /* Region 0: contiguous in block 0 */
        uint8_t mb_size = (uint8_t)(payload_size + arbitration_field_size);
        mb_index = msgBuffIdx * ((uint32_t)mb_size >> 2U);
    }
    else
    {
        /* Region 1: start from block 1 (RAM[128]) */
        uint8_t mb_size = (uint8_t)(payload_size + arbitration_field_size);
        uint32_t region1_idx = msgBuffIdx - (uint32_t)region0MaxMb;
        mb_index = 128U + (region1_idx * ((uint32_t)mb_size >> 2U));
    }
#else
    uint8_t mb_size = (uint8_t)(payload_size + arbitration_field_size);
    mb_index = msgBuffIdx * ((uint32_t)mb_size >> 2U);
#endif

    return &(base->RAM[mb_index]);
}
```

### 说明

这是**最关键的一处修改**。原始代码假设所有 MB 大小相同，用统一的公式计算物理地址，Region 分界由 `maxMbNum = 512 / mb_size` 决定。

修改后的计算方式：


| Region | 物理 MB 范围 | Payload |    MB 大小    | 起始 RAM 偏移 |                 布局公式                 |
| :----: | :----------: | :-----: | :-----------: | :-----------: | :--------------------------------------: |
|   R0   |     0~20     |   16B   | 24B (6 words) |    RAM[0]    |            `msgBuffIdx × 6`            |
|   R1   |    21~52    |   8B   | 16B (4 words) |   RAM[128]   | `128 + (msgBuffIdx - region0MaxMb) × 4` |

**物理布局验证（16B+8B 配置）**：

- Region 0 RAM 占用：21 × 6 = 126 words → RAM[0] ~ RAM[125] ✅
- 间隙：RAM[126~127]（2 words，因 512 无法被 24 整除，但不影响使用）
- Region 1 RAM 占用：32 × 4 = 128 words → RAM[128] ~ RAM[255] ✅
- **总计**：504 + 512 = **1016 bytes ≤ 1024 bytes** ✅

> **注意**：`FLEXCAN_GetMsgBuffRegion` 先调用 `FLEXCAN_GetPayloadSize(base, msgBuffIdx)` 获取当前 MB 的 payload（Region 0 返回 16B，Region 1 返回 8B），然后根据 `region0MaxMb` 边界分别计算两个 Region 的物理地址。

---

## 修改 7：`FLEXCAN_SetPayloadSize` 函数体

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 1978 行附近 → 修改后第 2017 行附近
```

### 修改前（原始代码）

```c
void FLEXCAN_SetPayloadSize(
    CAN_Type * base,
    flexcan_fd_payload_size_t payloadSize)
{
    uint32_t tmp;

    DEV_ASSERT(FLEXCAN_IsFDEnabled(base) || (payloadSize == FLEXCAN_PAYLOAD_SIZE_8));

    if (FLEXCAN_IsFDEnabled(base))
    {
        tmp = base->FDCTRL;
        tmp &= ~(CAN_FDCTRL_MBDSR0_MASK);
        tmp |= ((uint32_t)payloadSize) << CAN_FDCTRL_MBDSR0_SHIFT;
#if FEATURE_CAN_HAS_MBDSR1
        tmp &= ~(CAN_FDCTRL_MBDSR1_MASK);
        tmp |= ((uint32_t)payloadSize) << CAN_FDCTRL_MBDSR1_SHIFT;
#endif
#if FEATURE_CAN_HAS_MBDSR2
        tmp &= ~(CAN_FDCTRL_MBDSR2_MASK);
        tmp |= ((uint32_t)payloadSize) << CAN_FDCTRL_MBDSR2_SHIFT;
#endif
        base->FDCTRL = tmp;
    }
}
```

### 修改后

```c
void FLEXCAN_SetPayloadSize(
    CAN_Type * base,
    flexcan_fd_payload_size_t region0Payload,
    flexcan_fd_payload_size_t region1Payload)
{
    uint32_t tmp;

    DEV_ASSERT(FLEXCAN_IsFDEnabled(base) || (region0Payload == FLEXCAN_PAYLOAD_SIZE_8));

    if (FLEXCAN_IsFDEnabled(base))
    {
        tmp = base->FDCTRL;
        tmp &= ~(CAN_FDCTRL_MBDSR0_MASK);
        tmp |= ((uint32_t)region0Payload) << CAN_FDCTRL_MBDSR0_SHIFT;
#if FEATURE_CAN_HAS_MBDSR1
        tmp &= ~(CAN_FDCTRL_MBDSR1_MASK);
        tmp |= ((uint32_t)region1Payload) << CAN_FDCTRL_MBDSR1_SHIFT;
#endif
#if FEATURE_CAN_HAS_MBDSR2
        tmp &= ~(CAN_FDCTRL_MBDSR2_MASK);
        tmp |= ((uint32_t)region0Payload) << CAN_FDCTRL_MBDSR2_SHIFT;
#endif
#if defined (FEATURE_CAN_HAS_MBDSR3)&&(FEATURE_CAN_HAS_MBDSR3 == 1)
        tmp &= ~(CAN_FDCTRL_MBDSR3_MASK);
        tmp |= ((uint32_t)region0Payload) << CAN_FDCTRL_MBDSR3_SHIFT;
#endif
        base->FDCTRL = tmp;
    }
}
```

### 说明

原始代码将同一个 `payloadSize` 写入所有 MBDSR 寄存器。修改后：

- 函数接受两个独立的 payload 参数：`region0Payload` 和 `region1Payload`
- MBDSR0 写入 `region0Payload`（当前为 `FLEXCAN_PAYLOAD_SIZE_16`）
- MBDSR1 写入 `region1Payload`（当前为 `FLEXCAN_PAYLOAD_SIZE_8`）
- MBDSR2/MBDSR3（若存在）仍使用 `region0Payload`

---

## 修改 8：`FLEXCAN_SetTxMsgBuff` 中的 assert

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 596 行附近 → 修改后第 629 行附近
```

### 修改前（原始代码）

```c
DEV_ASSERT((uint8_t)cs->dataLen <= FLEXCAN_GetPayloadSize(base));
```

### 修改后

```c
DEV_ASSERT((uint8_t)cs->dataLen <= FLEXCAN_GetPayloadSize(base, msgBuffIdx));
```

### 说明

`FLEXCAN_GetPayloadSize` 签名从 1 参数变为 2 参数。补充 `msgBuffIdx` 以确保检查当前 MB 所在 Region 的正确 payload 大小。

---

## 修改 9：`FLEXCAN_GetMsgBuff` 中的调用

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 867~869 行附近 → 修改后第 900~902 行附近
```

### 修改前（原始代码）

```c
if (payload_size > FLEXCAN_GetPayloadSize(base))
{
    payload_size = FLEXCAN_GetPayloadSize(base);
}
```

### 修改后

```c
if (payload_size > FLEXCAN_GetPayloadSize(base, msgBuffIdx))
{
    payload_size = FLEXCAN_GetPayloadSize(base, msgBuffIdx);
}
```

### 说明

同上，补上 `msgBuffIdx` 参数。

---

## 修改 10：`FLEXCAN_SetMaxMsgBuffNum` —— 端地址检查与总量上限

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 1083 行附近 → 修改后第 1117 行附近
```

### 修改前（原始代码）

```c
#if FEATURE_CAN_HAS_FD
    uint8_t can_real_payload = FLEXCAN_GetPayloadSize(base);
#else
    uint8_t can_real_payload = 8U;
#endif
    ...
    if ((valEndMb > (uint32_t)&base->RAM[FEATURE_CAN_RAM_COUNT])
        || (maxMsgBuffNum > FLEXCAN_GetMaxMbNum(base)))
```

### 修改后

```c
#if FEATURE_CAN_HAS_FD
    msgBuffIdx = maxMsgBuffNum - 1U;
    uint8_t can_real_payload = FLEXCAN_GetPayloadSize(base, msgBuffIdx);
#else
    uint8_t can_real_payload = 8U;
#endif
    ...
    uint32_t totalMaxMb = (uint32_t)FLEXCAN_GetRegion0MaxMbCount(base)
                        + (uint32_t)FLEXCAN_GetRegion1MaxMbCount(base);
    if ((valEndMb > (uint32_t)&base->RAM[FEATURE_CAN_RAM_COUNT])
        || (maxMsgBuffNum > FLEXCAN_GetMaxMbNum(base))
        || (maxMsgBuffNum > totalMaxMb))
```

### 说明

两处关键改动：

1. **从最后一个 MB 获取 payload**：

   ```c
   // 原始：从 MB 0 的 Region 读取 payload（所有 MB 相同）
   uint8_t can_real_payload = FLEXCAN_GetPayloadSize(base);

   // 修改后：从最后一个 MB 获取其所属 Region 的 payload
   msgBuffIdx = maxMsgBuffNum - 1U;
   uint8_t can_real_payload = FLEXCAN_GetPayloadSize(base, msgBuffIdx);
   ```

   原始代码假设所有 MB 大小相同，`can_real_payload` 从 MB 0（Region 0）取值。但双 Region 布局下，最后一个 MB 在 Region 1（8B payload），如果仍取 16B（Region 0 的值），会错误算出超界。
2. **新增总量上限检查**：

   ```c
   || (maxMsgBuffNum > totalMaxMb)
   ```

   `totalMaxMb` = Region 0 最大 MB 数 + Region 1 最大 MB 数（当前为 21 + 32 = 53）。确保用户配置的 `max_num_mb` 不超过两个 512B RAM 块能容纳的总量。

---

## 修改 11：`FLEXCAN_SetMaxMsgBuffNum` —— 初始化循环

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 1125 行附近 → 修改后第 1161 行附近
```

### 修改前（原始代码）

```c
for (msgBuffIdx = 0; msgBuffIdx < maxMsgBuffNum; msgBuffIdx++)
{
    volatile uint32_t *flexcan_mb = FLEXCAN_GetMsgBuffRegion(base, msgBuffIdx);
    volatile uint32_t *flexcan_mb_id = &flexcan_mb[1];
    volatile uint8_t *flexcan_mb_data = (volatile uint8_t *)(&flexcan_mb[2]);

    *flexcan_mb = 0x0;
    *flexcan_mb_id = 0x0;
    for (databyte = 0; databyte < can_real_payload; databyte++)
    {
        flexcan_mb_data[databyte] = 0x0;
    }
}
```

### 修改后

```c
for (msgBuffIdx = 0; msgBuffIdx < maxMsgBuffNum; msgBuffIdx++)
{
    volatile uint32_t *flexcan_mb = FLEXCAN_GetMsgBuffRegion(base, msgBuffIdx);
    volatile uint32_t *flexcan_mb_id = &flexcan_mb[1];
    volatile uint8_t *flexcan_mb_data = (volatile uint8_t *)(&flexcan_mb[2]);
    uint8_t mb_payload = FLEXCAN_GetPayloadSize(base, msgBuffIdx);

    *flexcan_mb = 0x0;
    *flexcan_mb_id = 0x0;
    for (databyte = 0; databyte < mb_payload; databyte++)
    {
        flexcan_mb_data[databyte] = 0x0;
    }
}
```

### 说明


| 修改点   | 原始                                                                     | 修改后                                                                        |
| :------- | :----------------------------------------------------------------------- | :---------------------------------------------------------------------------- |
| 清零长度 | `can_real_payload`（固定值，从 MB 0 的 Region 取值）                     | `mb_payload = FLEXCAN_GetPayloadSize(base, msgBuffIdx)`（每个 MB 按真实大小） |
| 效果     | Region 0 MB 清零正确，Region 1 MB 也清零同样长度 →**越界写破坏相邻 MB** | Region 0 MB 清零 16B，Region 1 MB 清零 8B ✅                                  |

原始代码循环中用同一个 `can_real_payload` 清零所有 MB 的数据区。双 Region 布局中 Region 1 的 MB 只有 8B 数据区，却写满 16B → 跨越 MB 边界 → 数据破坏。

---

## 修改 12：`can_config.c` —— 配置参数

### 位置

```
board/can_config.c 第 19~53 行
```

### 修改前（原始代码）

```c
const flexcan_user_config_t flexcanInitConfig0 = {
    .max_num_mb = 14UL,
    .payload = FLEXCAN_PAYLOAD_SIZE_64,
    .fd_enable = true,
    ...
};

flexcan_state_t flexcanInitConfig1_State;
const flexcan_user_config_t flexcanInitConfig1 = {
    .max_num_mb = 7UL,
    .payload = FLEXCAN_PAYLOAD_SIZE_64,
    .fd_enable = true,
    ...
};
```

### 修改后

```c
const flexcan_user_config_t flexcanInitConfig0 = {
    .max_num_mb = 53UL,
    .region0_payload = FLEXCAN_PAYLOAD_SIZE_16,
    .region1_payload = FLEXCAN_PAYLOAD_SIZE_8,
    .fd_enable = true,
    ...
};
/* flexcanInitConfig1 已删除 */
```

### 说明


| 参数              | 原始                 | 修改后                    | 含义                                         |
| :---------------- | :------------------- | :------------------------ | :------------------------------------------- |
| `max_num_mb`      | 14 (CAN0) + 7 (CAN1) | 53 (CAN0 only)            | Region 0 最多 21 + Region 1 最多 32 = 53 MBs |
| `payload`         | 64B（统一值）        | —                        | 已废弃                                       |
| `region0_payload` | —                   | `FLEXCAN_PAYLOAD_SIZE_16` | Region 0 每个 MB 的 payload                  |
| `region1_payload` | —                   | `FLEXCAN_PAYLOAD_SIZE_8`  | Region 1 每个 MB 的 payload                  |

- 原始代码配置了两个 CAN 实例（CAN0=实例0，CAN1=实例5），均使用 64B payload。
- 修改后只使用 CAN0 单个实例，配置为 Region 0 = 16B、Region 1 = 8B 的双 Region 布局。

---

## 修改 13：`main.c` —— 应用层

### 位置

```
app/main.c
```

### 修改概述

原始 `main.c` 是一个双 CAN 实例（CAN0 + CAN5）互相收发 64B FD 帧的环形测试。修改后的 `main.c` 围绕单个 CAN0 实例，使用 39 个邮箱（逻辑 0~38）分布在两个 Region 中：

- **16B 负载组**：MB 0~6（物理），包含 3 个 RX + 4 个 TX
- **8B 负载组**：MB 21~52（物理），包含 16 个 RX + 16 个 TX

核心代码变更：

```c
// 16B payload 配置
const flexcan_data_info_t rxMbStdInfo0 = {
    .data_length = 16,     // 原始为 64
    .fd_enable = true,
    .enable_brs = true,
    ...
};

// 8B payload 配置
const flexcan_data_info_t rxMbStdInfo1 = {
    .data_length = 8,
    .fd_enable = true,
    .enable_brs = true,
    ...
};

// Region 1 邮箱从物理索引 21 开始
for(int i = 0; i < 8; i++)
{
    status |= FLEXCAN_DRV_ConfigRxMb(CAN0_INST, 21+i, &rxMbStdInfo1, RX_MSG_ID1+2+i);
}
```

> ⚠️ **关键点**：应用层给 Region 1 的邮箱分配的是**物理 MB 索引 21~52**，而不是紧凑地从 7 开始。这是因为 Region 0 的 `FLEXCAN_PAYLOAD_SIZE_16` 配置使得硬件分界在 MB 21（0~20 为 Region 0，21~52 为 Region 1）。MB 7~20 虽然也在 Region 0，但应用层不使用它们。

---

## 修改完成检查清单

1. **`flexcan_driver.h`**：`flexcan_user_config_t` 中 `payload` 字段拆分为 `region0_payload` + `region1_payload`
2. **`flexcan_driver.c`**：`FLEXCAN_DRV_Init` 和 `FLEXCAN_DRV_GetDefaultConfig` 使用新的双字段
3. **`flexcan_hw_access.h`**：`FLEXCAN_SetPayloadSize` 和 `FLEXCAN_GetPayloadSize` 签名修改
4. **`flexcan_hw_access.c`**：
   - 新增 `FLEXCAN_GetRegion0MaxMbCount` / `FLEXCAN_GetRegion1MaxMbCount` 辅助函数
   - `FLEXCAN_SetPayloadSize` 接受两个独立 payload 参数
   - `FLEXCAN_GetPayloadSize` 按 MB 索引返回对应 Region 的 payload
   - `FLEXCAN_GetMsgBuffRegion` 使用动态边界分 Region 计算物理地址
   - `FLEXCAN_SetTxMsgBuff` assert 使用 `msgBuffIdx`
   - `FLEXCAN_GetMsgBuff` 两处 `GetPayloadSize` 调用使用 `msgBuffIdx`
   - `FLEXCAN_SetMaxMsgBuffNum` 端地址检查 + totalMaxMb 上限 + 初始化循环按 MB 独立获取 payload
5. **`can_config.c`**：使用新字段，更新 max_num_mb 和 payload 配置
6. **`main.c`**：应用层适配双 Region 物理布局
