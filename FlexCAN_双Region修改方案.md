# FlexCAN FD 双 Region 不同负载修改方案

---

# 第一部分：代码使用介绍

## 运行环境

本演示代码运行在 **YTM32B1ME0** 芯片的单个 CAN 实例（CAN0）上，配置了 39 个邮箱，结合 64 字节与 8 字节 payload 的 CAN FD 数据帧收发。

## 邮箱总览


| MB | 方向 | 负载 | ID 类型 |    CAN ID    |  位于哪个 Region  |
| :-: | :--: | :--: | :-----: | :----------: | :---------------: |
| 0 |  RX  | 64B |   STD   |   `0x100`   | Region 0 (MBDSR0) |
| 1 |  RX  | 64B |   STD   |   `0x101`   | Region 0 (MBDSR0) |
| 2 |  RX  | 64B |   EXT   | `0x18DA00F1` | Region 0 (MBDSR0) |
| 3 |  TX  | 64B |   STD   |   `0x200`   | Region 0 (MBDSR0) |
| 4 |  TX  | 64B |   STD   |   `0x201`   | Region 0 (MBDSR0) |
| 5 |  TX  | 64B |   EXT   | `0x1AABBCC` | Region 0 (MBDSR0) |
| 6 |  TX  | 64B |   EXT   | `0x1AABBCD` | Region 0 (MBDSR0) |
| 7 |  RX  |  8B  |   STD   |   `0x102`   | Region 1 (MBDSR1) |
| 8 |  RX  |  8B  |   STD   |   `0x103`   | Region 1 (MBDSR1) |
| 9 |  RX  |  8B  |   STD   |   `0x104`   | Region 1 (MBDSR1) |
| 10 |  RX  |  8B  |   STD   |   `0x105`   | Region 1 (MBDSR1) |
| 11 |  RX  |  8B  |   STD   |   `0x106`   | Region 1 (MBDSR1) |
| 12 |  RX  |  8B  |   STD   |   `0x107`   | Region 1 (MBDSR1) |
| 13 |  RX  |  8B  |   STD   |   `0x108`   | Region 1 (MBDSR1) |
| 14 |  RX  |  8B  |   STD   |   `0x109`   | Region 1 (MBDSR1) |
| 15 |  RX  |  8B  |   EXT   | `0x18DA00F2` | Region 1 (MBDSR1) |
| 16 |  RX  |  8B  |   EXT   | `0x18DA00F3` | Region 1 (MBDSR1) |
| 17 |  RX  |  8B  |   EXT   | `0x18DA00F4` | Region 1 (MBDSR1) |
| 18 |  RX  |  8B  |   EXT   | `0x18DA00F5` | Region 1 (MBDSR1) |
| 19 |  RX  |  8B  |   EXT   | `0x18DA00F6` | Region 1 (MBDSR1) |
| 20 |  RX  |  8B  |   EXT   | `0x18DA00F7` | Region 1 (MBDSR1) |
| 21 |  RX  |  8B  |   EXT   | `0x18DA00F8` | Region 1 (MBDSR1) |
| 22 |  RX  |  8B  |   EXT   | `0x18DA00F9` | Region 1 (MBDSR1) |
| 23 |  TX  |  8B  |   STD   |   `0x202`   | Region 1 (MBDSR1) |
| 24 |  TX  |  8B  |   STD   |   `0x203`   | Region 1 (MBDSR1) |
| 25 |  TX  |  8B  |   STD   |   `0x204`   | Region 1 (MBDSR1) |
| 26 |  TX  |  8B  |   STD   |   `0x205`   | Region 1 (MBDSR1) |
| 27 |  TX  |  8B  |   STD   |   `0x206`   | Region 1 (MBDSR1) |
| 28 |  TX  |  8B  |   STD   |   `0x207`   | Region 1 (MBDSR1) |
| 29 |  TX  |  8B  |   STD   |   `0x208`   | Region 1 (MBDSR1) |
| 30 |  TX  |  8B  |   STD   |   `0x209`   | Region 1 (MBDSR1) |
| 31 |  TX  |  8B  |   EXT   | `0x1AABBCE` | Region 1 (MBDSR1) |
| 32 |  TX  |  8B  |   EXT   | `0x1AABBCF` | Region 1 (MBDSR1) |
| 33 |  TX  |  8B  |   EXT   | `0x1AABBD0` | Region 1 (MBDSR1) |
| 34 |  TX  |  8B  |   EXT   | `0x1AABBD1` | Region 1 (MBDSR1) |
| 35 |  TX  |  8B  |   EXT   | `0x1AABBD2` | Region 1 (MBDSR1) |
| 36 |  TX  |  8B  |   EXT   | `0x1AABBD3` | Region 1 (MBDSR1) |
| 37 |  TX  |  8B  |   EXT   | `0x1AABBD4` | Region 1 (MBDSR1) |
| 38 |  TX  |  8B  |   EXT   | `0x1AABBD5` | Region 1 (MBDSR1) |

### ID 公式

```c
#define RX_MSG_ID1 (0x100U)        // 标准帧 RX 基准
#define TX_MSG_ID1 (0x200U)        // 标准帧 TX 基准
#define RX_MSG_ID2 (0x18DA00F1)    // 扩展帧 RX 基准
#define TX_MSG_ID2 (0x1AABBCC)     // 扩展帧 TX 基准
```

- 64B RX MB 0~2：`RX_MSG_ID1`, `RX_MSG_ID1+1`, `RX_MSG_ID2`
- 64B TX MB 3~6：`TX_MSG_ID1`, `TX_MSG_ID1+1`, `TX_MSG_ID2`, `TX_MSG_ID2+1`
- 8B RX STD MB 7~14：`RX_MSG_ID1 + i + 2`，i=0-7
- 8B RX EXT MB 15~22：`RX_MSG_ID2 + i + 2`，i=0-7
- 8B TX STD MB 23~30：`TX_MSG_ID1 + i + 2`，i=0-7
- 8B TX EXT MB 31~38：`TX_MSG_ID2 + i + 2`，i=0-7

## 邮箱分组

### 第一组：64B 负载 (MB 0~6, Region 0)


| MB | 功能 | 说明                                        |
| :-: | :--- | :------------------------------------------ |
| 0~2 | RX   | 接收 64B CAN FD 帧                          |
| 3~6 | TX   | 发送 64B CAN FD 帧，数据 = offset1 + i 递增 |

### 第二组：8B RX (MB 7~22, Region 1)


| MB 范围 | 帧类型 |           ID 范围           |
| :-----: | :----: | :-------------------------: |
|  7~14  | 标准帧 |      `0x102` ~ `0x109`      |
|  15~22  | 扩展帧 | `0x18DA00F2` ~ `0x18DA00F9` |

### 第三组：8B TX (MB 23~38, Region 1)


| MB 范围 | 帧类型 |          ID 范围          |
| :-----: | :----: | :-----------------------: |
|  23~30  | 标准帧 |     `0x202` ~ `0x209`     |
|  31~38  | 扩展帧 | `0x1AABBCE` ~ `0x1AABBD5` |

## 程序流程

```
1. Board_Init() → 时钟/引脚/CAN0(Region0=64B, Region1=8B, 39MB)
2. 注册回调 → RX COMPLETE 时 can0_rx_flag++, 自动 re-arm
3. 配置 39 个邮箱 (RxMb/TxMb)
4. 启动接收 (Receive on MB 0~2, 7~22)
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
CAN0 MB3 发送 64B → 对端 CAN5 MB0 接收:
  ID=0x200, DLC=15, Data=[0x01..0x40], BRS=1, IDE=0

CAN0 MB23 发送 8B → 对端 CAN5 MB7 接收:
  ID=0x202, DLC=8, Data=txMsg0前8字节, BRS=1, IDE=0
```

---

# 第二部分：修改说明与原理讲解

原始代码（`C:\Users\ytm\Desktop\test\ME0\Flexcan_Canfd_Demo`）中，**Region 0 和 Region 1 使用相同的负载大小**（由 `flexcanInitConfig.payload` 统一配置，例如 `FLEXCAN_PAYLOAD_SIZE_64`）。所有邮箱无论位于哪个 Region，都有相同大小的数据区。

本修改方案的目标是：**让 Region 0（MBDSR0）和 Region 1（MBDSR1）使用不同负载大小**，具体为 **Region 0 = 64 bytes，Region 1 = 8 bytes**。

> **核心原理**：YTM32B1ME0 的 FlexCAN 模块有 1024 bytes（256 words）的 Message Buffer RAM，分为两个 512 bytes 块。通过 FDCTRL 寄存器中的 MBDSR0 和 MBDSR1 位域，可以分别为两块 RAM 设置不同的 payload 大小。

---

## 修改涉及的 4 个文件


| 文件                  | 路径                                               | 说明                             |
| :-------------------- | :------------------------------------------------- | :------------------------------- |
| `flexcan_hw_access.h` | `platform/drivers/src/flexcan/flexcan_hw_access.h` | 函数声明                         |
| `flexcan_hw_access.c` | `platform/drivers/src/flexcan/flexcan_hw_access.c` | 底层硬件访问函数（**5 处修改**） |
| `can_config.c`        | `board/can_config.c`                               | 驱动初始化参数                   |
| `main.c`              | `app/main.c`                                       | 应用层邮箱分配和收发逻辑         |

---

## 修改 1：函数声明 `flexcan_hw_access.h`

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.h 第 590 行
```

### 修改前（原始代码）

```c
uint8_t FLEXCAN_GetPayloadSize(const CAN_Type * base);
```

### 修改后

```c
uint8_t FLEXCAN_GetPayloadSize(const CAN_Type * base, uint32_t msgBuffIdx);
```

### 说明

原始函数只有一个参数（CAN 基地址），因为所有邮箱负载相同，只需要读 MBDSR0 即可。修改后增加 `msgBuffIdx` 参数，根据邮箱索引号判断该邮箱属于哪个 Region，返回对应的 payload 大小。

---

## 修改 2：`FLEXCAN_GetPayloadSize` 函数体

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 1996 行附近 → 修改后第 2024 行附近
```

### 修改前（原始代码）

```c
uint8_t FLEXCAN_GetPayloadSize(const CAN_Type * base)
{
    uint32_t payloadSize;

    /* The standard payload size is 8 bytes */
    if (!FLEXCAN_IsFDEnabled(base))
    {
        payloadSize = 8U;
    }
    else
    {
        // 所有 MB 统一读 MBDSR0
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

    /* The standard payload size is 8 bytes */
    if (!FLEXCAN_IsFDEnabled(base))
    {
        payloadSize = 8U;
    }
    /* Region0 (MBDSR0) = MB 0~6 (64B), Region1 (MBDSR1) = MB 7+ (8B) */
    else if (msgBuffIdx < 7)
    {
        // MB 0~6 → 读 MBDSR0
        payloadSize = 1UL << (((base->FDCTRL & CAN_FDCTRL_MBDSR0_MASK)
                               >> CAN_FDCTRL_MBDSR0_SHIFT) + 3U);
    }
    else
    {
        // MB 7+ → 读 MBDSR1
        payloadSize = 1UL << (((base->FDCTRL & CAN_FDCTRL_MBDSR1_MASK)
                               >> CAN_FDCTRL_MBDSR1_SHIFT) + 3U);
    }

    return (uint8_t)payloadSize;
}
```

### 说明

- `msgBuffIdx < 7`：邮箱 0~6 属于 Region 0，读取 MBDSR0 寄存器
- `msgBuffIdx >= 7`：邮箱 7+ 属于 Region 1，读取 MBDSR1 寄存器
- 边界值 `7` 是根据 **7 个 64B 邮箱 = 504B** 刚好放入第一块 512B RAM 来确定的

---

## 修改 3：`FLEXCAN_GetMsgBuffRegion` 函数体

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 370 行附近 → 修改后第 375 行附近
```

### 修改前（原始代码）

```c
volatile uint32_t* FLEXCAN_GetMsgBuffRegion(
        CAN_Type * base,
        uint32_t msgBuffIdx)
{
#if FEATURE_CAN_HAS_FD
    uint8_t payload_size = FLEXCAN_GetPayloadSize(base);  // ← 无 msgBuffIdx
#else
    uint8_t payload_size = 8U;
#endif

    uint8_t arbitration_field_size = 8U;
    uint32_t ramBlockSize = 512U;
    uint32_t ramBlockOffset;

    uint8_t mb_size = (uint8_t)(payload_size + arbitration_field_size);
    uint8_t maxMbNum = (uint8_t)(ramBlockSize / mb_size);

    ramBlockOffset = 128U * (msgBuffIdx / (uint32_t)maxMbNum);

    /* Multiply the MB index by the MB size (in words) */
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
    uint8_t payload_size = FLEXCAN_GetPayloadSize(base, msgBuffIdx);  // ← 加上 msgBuffIdx
#else
    uint8_t payload_size = 8U;
#endif

    uint8_t arbitration_field_size = 8U;
    uint32_t mb_index;

    if (msgBuffIdx < 7)
    {
        /* Region0 (64B payload): MB 0~6, each 72B (18 words), all in block 0 */
        uint8_t mb_size = (uint8_t)(payload_size + arbitration_field_size);
        mb_index = msgBuffIdx * ((uint32_t)mb_size >> 2U);
    }
    else
    {
        /* Region1 (8B payload): MB 7+, each 16B (4 words),
           start from block 1 (RAM[128]) */
        uint8_t mb_size = (uint8_t)(payload_size + arbitration_field_size);
        uint32_t region1_idx = msgBuffIdx - 7U;
        mb_index = 128U + (region1_idx * ((uint32_t)mb_size >> 2U));
    }

    return &(base->RAM[mb_index]);
}
```

### 说明

这是**最关键的一处修改**。原始代码假设所有 MB 大小相同，用统一的公式 `ramBlockOffset + offset` 计算物理地址。修改后分为两个 Region 独立计算：


| Region | MB 范围 | Payload |    MB 大小    | 起始 RAM 偏移 |          布局方式          |
| :----: | :-----: | :-----: | :------------: | :-----------: | :-------------------------: |
|   R0   |   0~6   |   64B   | 72B (18 words) |    RAM[0]    |     `msgBuffIdx × 18`     |
|   R1   |  7~38  |   8B   | 16B (4 words) |   RAM[128]   | `128 + (msgBuffIdx-7) × 4` |

**物理布局验证**：

- Region 0：7 × 18 = 126 words → RAM[0] ~ RAM[125] ✅
- 间隙：RAM[126~127]（2 words，因 512 无法被 72 整除）
- Region 1：32 × 4 = 128 words → RAM[128] ~ RAM[255] ✅
- **总计**：504 + 512 = **1016 bytes ≤ 1024 bytes** ✅

---

## 修改 4：`FLEXCAN_SetPayloadSize` 函数体

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 1971 行附近 → 修改后第 1993 行附近
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
        tmp |= ((uint32_t)payloadSize) << CAN_FDCTRL_MBDSR0_SHIFT;   // ← 相同值
#if FEATURE_CAN_HAS_MBDSR1
        tmp &= ~(CAN_FDCTRL_MBDSR1_MASK);
        tmp |= ((uint32_t)payloadSize) << CAN_FDCTRL_MBDSR1_SHIFT;   // ← 相同值
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
    flexcan_fd_payload_size_t payloadSize)
{
    uint32_t tmp;

    DEV_ASSERT(FLEXCAN_IsFDEnabled(base) || (payloadSize == FLEXCAN_PAYLOAD_SIZE_8));

    if (FLEXCAN_IsFDEnabled(base))
    {
        tmp = base->FDCTRL;
        tmp &= ~(CAN_FDCTRL_MBDSR0_MASK);
        tmp |= ((uint32_t)FLEXCAN_PAYLOAD_SIZE_64) << CAN_FDCTRL_MBDSR0_SHIFT;  // ← 硬编码 64
#if FEATURE_CAN_HAS_MBDSR1
        tmp &= ~(CAN_FDCTRL_MBDSR1_MASK);
        tmp |= ((uint32_t)FLEXCAN_PAYLOAD_SIZE_8) << CAN_FDCTRL_MBDSR1_SHIFT;   // ← 硬编码 8
#endif
        base->FDCTRL = tmp;
    }
}
```

### 说明

原始代码将同一个 `payloadSize` 写入所有 MBDSR 寄存器。修改后硬编码 **MBDSR0 = 64B**、**MBDSR1 = 8B**，参数 `payloadSize` 仅用于不存在的 MBDSR2/MBDSR3。

> ⚠️ 此处的硬编码意味着 `flexcanInitConfig.payload` 的值被忽略。如果需要更灵活的配置，可以将此函数改为接受两个 payload 参数，或通过其他方式传入两个不同的值。

---

## 修改 5：`FLEXCAN_SetTxMsgBuff` 中的 assert

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 597 行附近 → 修改后第 602 行附近
```

### 修改前（原始代码）

```c
/* Check if the Payload Size is smaller than the payload configured */
DEV_ASSERT((uint8_t)cs->dataLen <= FLEXCAN_GetPayloadSize(base));
```

### 修改后

```c
/* Check if the Payload Size is smaller than the payload configured */
DEV_ASSERT((uint8_t)cs->dataLen <= FLEXCAN_GetPayloadSize(base, msgBuffIdx));
```

### 说明

`FLEXCAN_GetPayloadSize` 函数签名从 1 个参数变成了 2 个参数，必须同步修改所有调用处。此处补充 `msgBuffIdx` 参数，确保 assert 检查的是当前 MB 所在 Region 的正确 payload 大小。

> 同理，`FLEXCAN_GetMsgBuff` 函数中调用 `FLEXCAN_GetPayloadSize` 的地方也需要加上 `msgBuffIdx` 参数。

---

## 修改 6：`FLEXCAN_GetMsgBuff` 中的调用

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 860 行附近 → 修改后第 872 行附近
```

### 修改前（原始代码）

```c
#if FEATURE_CAN_HAS_FD
    if (payload_size > FLEXCAN_GetPayloadSize(base))
    {
        payload_size = FLEXCAN_GetPayloadSize(base);
    }
#endif
```

### 修改后

```c
#if FEATURE_CAN_HAS_FD
    if (payload_size > FLEXCAN_GetPayloadSize(base, msgBuffIdx))
    {
        payload_size = FLEXCAN_GetPayloadSize(base, msgBuffIdx);
    }
#endif
```

### 说明

同上，补上 `msgBuffIdx` 参数。

---

## 修改 7：`FLEXCAN_SetMaxMsgBuffNum`——端地址检查

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 1076 行附近 → 修改后第 1088 行附近
```

### 修改前（原始代码）

```c
#if FEATURE_CAN_HAS_FD
    uint8_t can_real_payload = FLEXCAN_GetPayloadSize(base);
#else
    uint8_t can_real_payload = 8U;
#endif
    status_t status = STATUS_SUCCESS;

#if FEATURE_CAN_HAS_FD
    uint8_t arbitration_field_size = 8U;
    volatile uint32_t *valEndMbPointer = FLEXCAN_GetMsgBuffRegion(base, (maxMsgBuffNum - 1U));
    uint32_t valEndMb = (uint32_t)valEndMbPointer + can_real_payload + arbitration_field_size;
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
    status_t status = STATUS_SUCCESS;

#if FEATURE_CAN_HAS_FD
    uint8_t arbitration_field_size = 8U;
    volatile uint32_t *valEndMbPointer = FLEXCAN_GetMsgBuffRegion(base, (maxMsgBuffNum - 1U));
    uint32_t valEndMb = (uint32_t)valEndMbPointer + can_real_payload + arbitration_field_size;
    if ((valEndMb > (uint32_t)&base->RAM[FEATURE_CAN_RAM_COUNT])
        || (maxMsgBuffNum > FLEXCAN_GetMaxMbNum(base)))
```

### 说明

关键区别：

```c
// 原始：从全局获取 payload（所有 MB 相同）
uint8_t can_real_payload = FLEXCAN_GetPayloadSize(base);

// 修改后：从最后一个 MB 获取 payload（不同 Region 可能不同）
msgBuffIdx = maxMsgBuffNum - 1U;
uint8_t can_real_payload = FLEXCAN_GetPayloadSize(base, msgBuffIdx);
```

**为什么必须修改？** 原始代码假设所有 MB 大小相同，从 MB 0 的 Region 读取 payload 值就可以代表所有 MB。但双 Region 布局下，最后一个 MB（如 MB 38）很可能在 Region 1（8B payload），如果仍然从 MB 0 取 payload（64B），会错误地算出最后一个 MB 占 64 字节 → 超界 → Init 失败。

---

## 修改 8：`FLEXCAN_SetMaxMsgBuffNum`——初始化循环

### 位置

```
platform/drivers/src/flexcan/flexcan_hw_access.c
原第 1130 行附近 → 修改后第 1127 行附近
```

### 修改前（原始代码）

```c
/* Initialize all message buffers as inactive */
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
/* Initialize all message buffers as inactive */
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


| 修改点   | 原始                                                  | 修改后                                                           |
| :------- | :---------------------------------------------------- | :--------------------------------------------------------------- |
| 清零长度 | `can_real_payload`（固定值）                          | `FLEXCAN_GetPayloadSize(base, msgBuffIdx)`（每个 MB 按真实大小） |
| 效果     | Region 0 MB 清 64B，Region 1 MB**也清 64B（越界！）** | Region 0 MB 清 64B，Region 1 MB 清 8B ✅                         |

原始代码中用 `can_real_payload`（一个固定值）去清零所有 MB 的数据区。在双 Region 布局中，Region 1 的 MB 只有 8B 数据区，却尝试写 64B → 写到下一个 MB 的仲裁区/数据区 → 数据破坏。

---

## 修改 9：`can_config.c`——邮箱数量

### 位置

```
board/can_config.c 第 22 行
```

### 修改前（原始代码）

```c
const flexcan_user_config_t flexcanInitConfig0 = {
    .max_num_mb = 14UL,       // 14 个 64B 邮箱（2 × 512B 块）
    .payload = FLEXCAN_PAYLOAD_SIZE_64,
    ...
};
```

### 修改后

```c
const flexcan_user_config_t flexcanInitConfig0 = {
    .max_num_mb = 39UL,       // 7 个 64B + 32 个 8B = 39 个邮箱
    .payload = FLEXCAN_PAYLOAD_SIZE_8,
    ...
};
```

### 说明

- 原始：全部 64B 负载，1024 ÷ 72 = 14.2 → 最多 14 个邮箱
- 修改后：Region0 有 7 个 64B（504B）+ Region1 有 32 个 8B（512B）= 39 个邮箱，总计 1016B

> ⚠️ `payload = FLEXCAN_PAYLOAD_SIZE_8` 写什么值不重要，因为 `FLEXCAN_SetPayloadSize` 中硬编码了 64 和 8。

---

## 修改 10：`main.c`——应用层邮箱分配

### 说明

原始 `main.c` 使用两个 CAN 实例（CAN0 和 CAN5），每个只用 2 个邮箱（1 收 1 发），全部 64B 负载。

修改后 `main.c` 只用一个 CAN0 实例，使用 39 个邮箱。详细分配见[第一部分：邮箱总览](#邮箱总览)。

关键点：

- 64B 负载的 data_info 使用 `rxMbStdInfo0/rxMbExtInfo0/txMbStdInfo0/txMbExtInfo0`（`data_length = 64`）
- 8B 负载的 data_info 使用 `rxMbStdInfo1/rxMbExtInfo1/txMbStdInfo1/txMbExtInfo1`（`data_length = 8`）
- `data_length` **必须 ≤ 所在 Region 的 payload**
- 收发操作中的 MB 索引必须与配置时一致

---

## 修改总结

|:--:|:---:|:---:|:------:|:------:|:------------:|
| 0 | RX | 64B | STD | `0x100` | Region 0 (MBDSR0) |
| 1 | RX | 64B | STD | `0x101` | Region 0 (MBDSR0) |
| 2 | RX | 64B | EXT | `0x18DA00F1` | Region 0 (MBDSR0) |
| 3 | TX | 64B | STD | `0x200` | Region 0 (MBDSR0) |
| 4 | TX | 64B | STD | `0x201` | Region 0 (MBDSR0) |
| 5 | TX | 64B | EXT | `0x1AABBCC` | Region 0 (MBDSR0) |
| 6 | TX | 64B | EXT | `0x1AABBCD` | Region 0 (MBDSR0) |
| 7 | RX | 8B | STD | `0x102` | Region 1 (MBDSR1) |
| 8 | RX | 8B | STD | `0x103` | Region 1 (MBDSR1) |
| 9 | RX | 8B | STD | `0x104` | Region 1 (MBDSR1) |
| 10 | RX | 8B | STD | `0x105` | Region 1 (MBDSR1) |
| 11 | RX | 8B | STD | `0x106` | Region 1 (MBDSR1) |
| 12 | RX | 8B | STD | `0x107` | Region 1 (MBDSR1) |
| 13 | RX | 8B | STD | `0x108` | Region 1 (MBDSR1) |
| 14 | RX | 8B | STD | `0x109` | Region 1 (MBDSR1) |
| 15 | RX | 8B | EXT | `0x18DA00F2` | Region 1 (MBDSR1) |
| 16 | RX | 8B | EXT | `0x18DA00F3` | Region 1 (MBDSR1) |
| 17 | RX | 8B | EXT | `0x18DA00F4` | Region 1 (MBDSR1) |
| 18 | RX | 8B | EXT | `0x18DA00F5` | Region 1 (MBDSR1) |
| 19 | RX | 8B | EXT | `0x18DA00F6` | Region 1 (MBDSR1) |
| 20 | RX | 8B | EXT | `0x18DA00F7` | Region 1 (MBDSR1) |
| 21 | RX | 8B | EXT | `0x18DA00F8` | Region 1 (MBDSR1) |
| 22 | RX | 8B | EXT | `0x18DA00F9` | Region 1 (MBDSR1) |
| 23 | TX | 8B | STD | `0x202` | Region 1 (MBDSR1) |
| 24 | TX | 8B | STD | `0x203` | Region 1 (MBDSR1) |
| 25 | TX | 8B | STD | `0x204` | Region 1 (MBDSR1) |
| 26 | TX | 8B | STD | `0x205` | Region 1 (MBDSR1) |
| 27 | TX | 8B | STD | `0x206` | Region 1 (MBDSR1) |
| 28 | TX | 8B | STD | `0x207` | Region 1 (MBDSR1) |
| 29 | TX | 8B | STD | `0x208` | Region 1 (MBDSR1) |
| 30 | TX | 8B | STD | `0x209` | Region 1 (MBDSR1) |
| 31 | TX | 8B | EXT | `0x1AABBCE` | Region 1 (MBDSR1) |
| 32 | TX | 8B | EXT | `0x1AABBCF` | Region 1 (MBDSR1) |
| 33 | TX | 8B | EXT | `0x1AABBD0` | Region 1 (MBDSR1) |
| 34 | TX | 8B | EXT | `0x1AABBD1` | Region 1 (MBDSR1) |
| 35 | TX | 8B | EXT | `0x1AABBD2` | Region 1 (MBDSR1) |
| 36 | TX | 8B | EXT | `0x1AABBD3` | Region 1 (MBDSR1) |
| 37 | TX | 8B | EXT | `0x1AABBD4` | Region 1 (MBDSR1) |
| 38 | TX | 8B | EXT | `0x1AABBD5` | Region 1 (MBDSR1) |

### ID 计算公式

代码中通过以下宏定义和循环公式生成每个 MB 的 CAN ID：

```c
#define RX_MSG_ID1 (0x100U)        // 标准帧 RX 基准 ID
#define TX_MSG_ID1 (0x200U)        // 标准帧 TX 基准 ID
#define RX_MSG_ID2 (0x18DA00F1)    // 扩展帧 RX 基准 ID
#define TX_MSG_ID2 (0x1AABBCC)     // 扩展帧 TX 基准 ID
```

- 64B RX MB 0~2：直接使用 `RX_MSG_ID1`, `RX_MSG_ID1+1`, `RX_MSG_ID2`
- 64B TX MB 3~6：直接使用 `TX_MSG_ID1`, `TX_MSG_ID1+1`, `TX_MSG_ID2`, `TX_MSG_ID2+1`
- 8B RX STD MB 7~14：`RX_MSG_ID1 + 2 + i`，i=0~7
- 8B RX EXT MB 15~22：`RX_MSG_ID2 + 1 + i`，i=0~7
- 8B TX STD MB 23~30：`TX_MSG_ID1 + i + 2`，i=0~7
- 8B TX EXT MB 31~38：`TX_MSG_ID2 + i + 2`，i=0~7

### 收发对应关系


| 发送 MB |   发送 ID   | 数据长度 | 期望接收 MB |   接收 ID   | 接收数据长度 |
| :-----: | :---------: | :------: | :---------: | :----------: | :----------: |
|    3    |   `0x200`   |   64B   |      0      |   `0x100`   |     64B     |
|    4    |   `0x201`   |   64B   |      1      |   `0x101`   |     64B     |
|    5    | `0x1AABBCC` |   64B   |      2      | `0x18DA00F1` |     64B     |
|    6    | `0x1AABBCD` |   64B   |     —     |      —      |      —      |
|   23   |   `0x202`   |    8B    |      7      |   `0x102`   |      8B      |
|   24   |   `0x203`   |    8B    |      8      |   `0x103`   |      8B      |
|   25   |   `0x204`   |    8B    |      9      |   `0x104`   |      8B      |
|   26   |   `0x205`   |    8B    |     10     |   `0x105`   |      8B      |
|   27   |   `0x206`   |    8B    |     11     |   `0x106`   |      8B      |
|   28   |   `0x207`   |    8B    |     12     |   `0x107`   |      8B      |
|   29   |   `0x208`   |    8B    |     13     |   `0x108`   |      8B      |
|   30   |   `0x209`   |    8B    |     14     |   `0x109`   |      8B      |
|   31   | `0x1AABBCE` |    8B    |     15     | `0x18DA00F2` |      8B      |
|   32   | `0x1AABBCF` |    8B    |     16     | `0x18DA00F3` |      8B      |
|   33   | `0x1AABBD0` |    8B    |     17     | `0x18DA00F4` |      8B      |
|   34   | `0x1AABBD1` |    8B    |     18     | `0x18DA00F5` |      8B      |
|   35   | `0x1AABBD2` |    8B    |     19     | `0x18DA00F6` |      8B      |
|   36   | `0x1AABBD3` |    8B    |     20     | `0x18DA00F7` |      8B      |
|   37   | `0x1AABBD4` |    8B    |     21     | `0x18DA00F8` |      8B      |
|   38   | `0x1AABBD5` |    8B    |     22     | `0x18DA00F9` |      8B      |

> ⚠️ 注意：当前代码中**发送和接收使用的 ID 不同**（例如 TX 发 `0x200`，RX 收 `0x100`），这适用于两个 CAN 节点通过外部接线互联的场景。做单节点自回环测试时，需要确保 TX ID 和 RX ID 匹配，或者使用另一个 CAN 节点作为对端来收发对应 ID。

---

## 代码使用介绍

### 运行环境说明

本演示代码运行在 **YTM32B1ME0** 芯片的单个 CAN 实例（CAN0）上，配置了 39 个邮箱，涵盖了标准的 CAN FD 数据帧收发操作。

### 硬件连接方式

**双节点互联测试（推荐）：**

将两块 EVB 板的 CAN_H 和 CAN_L 对接：

```
EVB-A CAN0               EVB-B CAN5
CAN_H ──────────────────── CAN_H
CAN_L ──────────────────── CAN_L
```

- EVB-A 烧录本工程代码，CAN0 发送 `0x200` 等 ID，CAN5 接收 `0x100` 等 ID
- EVB-B 烧录配套代码，CAN5 接收 `0x100` 等 ID，发送 `0x200` 等 ID

**单板自回环测试：**

将同一块 EVB 板的 CAN0 与 CAN5 短接：

```
CAN0 CAN_H ──── CAN5 CAN_H
CAN0 CAN_L ──── CAN5 CAN_L
```

### 邮箱分组说明

代码中 39 个邮箱分为 **3 大组**，每组功能如下：

#### 第一组：64 字节负载邮箱（MB 0~6，位于 Region 0）


| MB | 功能 | 说明                                          |
| :-: | :--- | :-------------------------------------------- |
| 0 | RX   | 接收 64 字节 CAN FD 标准帧（ID=`0x100`）      |
| 1 | RX   | 接收 64 字节 CAN FD 标准帧（ID=`0x101`）      |
| 2 | RX   | 接收 64 字节 CAN FD 扩展帧（ID=`0x18DA00F1`） |
| 3 | TX   | 发送 64 字节 CAN FD 标准帧（ID=`0x200`）      |
| 4 | TX   | 发送 64 字节 CAN FD 标准帧（ID=`0x201`）      |
| 5 | TX   | 发送 64 字节 CAN FD 扩展帧（ID=`0x1AABBCC`）  |
| 6 | TX   | 发送 64 字节 CAN FD 扩展帧（ID=`0x1AABBCD`）  |

**数据内容**：64 字节依次递增，例如发送时 `data[0] = offset1, data[1] = offset1+1, ...`

#### 第二组：8 字节 RX 邮箱（MB 7~22，位于 Region 1）


| MB 范围 | 帧类型 |           ID 范围           | 说明               |
| :-----: | :----: | :-------------------------: | :----------------- |
|  7~14  | 标准帧 |      `0x102` ~ `0x109`      | 8 字节 CAN FD 接收 |
|  15~22  | 扩展帧 | `0x18DA00F2` ~ `0x18DA00F9` | 8 字节 CAN FD 接收 |

#### 第三组：8 字节 TX 邮箱（MB 23~38，位于 Region 1）


| MB 范围 | 帧类型 |          ID 范围          | 说明               |
| :-----: | :----: | :-----------------------: | :----------------- |
|  23~30  | 标准帧 |     `0x202` ~ `0x209`     | 8 字节 CAN FD 发送 |
|  31~38  | 扩展帧 | `0x1AABBCE` ~ `0x1AABBD5` | 8 字节 CAN FD 发送 |

**数据内容**：8 字节，与 64B 邮箱共用 `txMsg0` 的前 8 字节数据。

### 程序运行流程

```
1. Board_Init()
   ├─ 初始化系统时钟
   ├─ 初始化引脚复用
   └─ FLEXCAN_DRV_Init(CAN0) → Region0=64B, Region1=8B, 39个MB

2. 注册中断回调 FLEXCAN_DRV_InstallEventCallback()
   └─ 收到帧后自动调用 Can0Tp_Hal_EventCallback
      └─ RX COMPLETE → can0_rx_flag++ → 自动重新 arm RX

3. 配置邮箱
   ├─ MB 0~2:  ConfigRxMb (64B)
   ├─ MB 3~6:  ConfigTxMb (64B)
   ├─ MB 7~14: ConfigRxMb (8B)
   ├─ MB 15~22:ConfigRxMb (8B)
   ├─ MB 23~30:ConfigTxMb (8B)
   └─ MB 31~38:ConfigTxMb (8B)

4. 启动接收
   ├─ MB 0~2:  FLEXCAN_DRV_Receive
   └─ MB 7~22: FLEXCAN_DRV_Receive

5. while(1) 主循环
   ├─ 检查 MB 3~6  状态 → 空闲则 Send 64B 数据
   ├─ 检查 MB 23~30 状态 → 空闲则 Send 8B 数据
   ├─ 检查 MB 31~38 状态 → 空闲则 Send 8B 数据
   └─ OSIF_TimeDelay(10) → 延时 10ms，避免总线堵塞
```

### 接收回调说明

代码中注册了 CAN FD 事件回调函数 `Can0Tp_Hal_EventCallback`：

```c
FLEXCAN_DRV_InstallEventCallback(0, Can0Tp_Hal_EventCallback, NULL);
```

- **RX COMPLETE** 事件触发时：`can0_rx_flag++`，然后自动调用 `FLEXCAN_DRV_Receive` 重新 arm 该 MB 接收下一帧
- **TX COMPLETE** 事件触发时：不做额外处理，由 while 循环中的 `GetTransferStatus` 检测状态变更
- 回调中的 `mbIdx` 参数可用于区分是哪个 MB 完成了收发

### CAN FD 关键参数


| 参数            |          值          | 说明                                                    |
| :-------------- | :------------------: | :------------------------------------------------------ |
| 仲裁段波特率    |   由`bitrate` 配置   | `.propSeg=4, .phaseSeg1=7, .phaseSeg2=1, .preDivider=2` |
| 数据段波特率    | 由`bitrate_cbt` 配置 | `.propSeg=6, .phaseSeg1=2, .phaseSeg2=1, .preDivider=0` |
| FD 模式         |         开启         | `.fd_enable = true`                                     |
| BRS（速率切换） |         开启         | `.enable_brs = true`，数据段使用更高速率                |
| 发送填充值      |          0          | `.fd_padding = 0`                                       |

### 数据流示例

假设正常运行，CAN0 发送一帧 64B 标准帧到对端 CAN5：

```
CAN0 MB3 发送:
   ID    = 0x200
   DLC   = 15 (对应 64 字节)
   Data  = [0x01, 0x02, ..., 0x40]  (offset1=1 时)
   BRS   = 1 (数据段切换到高速率)
   IDE   = 0 (标准帧)

CAN5 MB0 接收:
   ID    = 0x200
   DLC   = 15
   Data  = [0x01, 0x02, ..., 0x40]
   接收成功后：rx_flag_can5++，自动 re-arm 等待下一帧
```

---

## 修改总结


| # | 文件                  | 函数/位置                             | 修改类型 | 关键变更                                               |
| :-: | :-------------------- | :------------------------------------ | :------- | :----------------------------------------------------- |
| 1 | `flexcan_hw_access.h` | `FLEXCAN_GetPayloadSize` 声明         | 签名变更 | 增加`msgBuffIdx` 参数                                  |
| 2 | `flexcan_hw_access.c` | `FLEXCAN_GetPayloadSize` 定义         | 逻辑变更 | 根据`msgBuffIdx` 选择读 MBDSR0 还是 MBDSR1             |
| 3 | `flexcan_hw_access.c` | `FLEXCAN_GetMsgBuffRegion`            | 逻辑重写 | 两段式物理地址映射，替代统一公式                       |
| 4 | `flexcan_hw_access.c` | `FLEXCAN_SetPayloadSize`              | 逻辑变更 | MBDSR0 硬编码 64、MBDSR1 硬编码 8                      |
| 5 | `flexcan_hw_access.c` | `FLEXCAN_SetTxMsgBuff` assert         | 参数补齐 | `FLEXCAN_GetPayloadSize(base)` → `(base, msgBuffIdx)` |
| 6 | `flexcan_hw_access.c` | `FLEXCAN_GetMsgBuff`                  | 参数补齐 | 同上                                                   |
| 7 | `flexcan_hw_access.c` | `FLEXCAN_SetMaxMsgBuffNum` 端地址检查 | 逻辑变更 | 从最后一个 MB 获取 payload                             |
| 8 | `flexcan_hw_access.c` | `FLEXCAN_SetMaxMsgBuffNum` 初始化循环 | 逻辑变更 | 每个 MB 按真实 payload 大小清零                        |
| 9 | `can_config.c`        | `flexcanInitConfig0`                  | 参数变更 | `max_num_mb = 39`                                      |
| 10 | `main.c`              | 邮箱配置和收发循环                    | 逻辑重写 | 64B MB 在 0~6，8B MB 在 7~38                           |

---

## 物理 RAM 布局验证

```
FlexCAN RAM 总大小：1024 bytes (256 words)

┌──────────────────────────────────────────┐
│  Region 0: 7 个 64B 邮箱（504B）          │
│  MB 0: RAM[0~17]    (72B)               │
│  MB 1: RAM[18~35]   (72B)               │
│  MB 2: RAM[36~53]   (72B)               │
│  MB 3: RAM[54~71]   (72B)               │
│  MB 4: RAM[72~89]   (72B)               │
│  MB 5: RAM[90~107]  (72B)               │
│  MB 6: RAM[108~125] (72B)               │
├──────────────────────────────────────────┤
│  间隙: RAM[126~127] (8B)      ← 浪费     │
├══════════════════════════════════════════┤
│  Region 1: 32 个 8B 邮箱（512B）         │
│  MB 7:  RAM[128~131] (16B)              │
│  MB 8:  RAM[132~135] (16B)              │
│  ...                                     │
│  MB 38: RAM[252~255] (16B)              │
├══════════════════════════════════════════┤
│  总计: 504 + 512 = 1016B                 │
│  剩余: 8B（RAM[126~127] 间隙）            │
└──────────────────────────────────────────┘
```

---

## 注意事项

1. **API 签名变更**：`FLEXCAN_GetPayloadSize` 从 1 参数变为 2 参数，所有调用处必须同步修改，否则编译失败
2. **data_length 约束**：每个 MB 的 `flexcan_data_info_t.data_length` 不能超过其所在 Region 的 payload 大小（底层有 `DEV_ASSERT` 检查）
3. **payload 硬编码**：当前 MBDSR0/MBDSR1 在 `FLEXCAN_SetPayloadSize` 中写死为 64/8，`flexcanInitConfig.payload` 的值被忽略
4. **边界常量**：如果调整 64B 邮箱数量，需要同步修改 3 处边界条件：
   - `FLEXCAN_GetPayloadSize` 中的 `msgBuffIdx < 7`
   - `FLEXCAN_GetMsgBuffRegion` 中的 `msgBuffIdx < 7` 和 Region1 起始偏移 `128U`
   - `can_config.c` 中的 `max_num_mb`
5. **编译验证**：修改后务必确保 0 个编译诊断错误
