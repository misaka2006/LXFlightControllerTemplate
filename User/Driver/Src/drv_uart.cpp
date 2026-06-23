
/* Includes ------------------------------------------------------------------*/

#include "../Inc/drv_uart.h"
#include "usart.h"
#include <cstring>
/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Struct_UART_Manage_Object UART1_Manage_Object = {0};
Struct_UART_Manage_Object UART2_Manage_Object = {0};
Struct_UART_Manage_Object UART3_Manage_Object = {0};
Struct_UART_Manage_Object UART4_Manage_Object = {0};
Struct_UART_Manage_Object UART5_Manage_Object = {0};
Struct_UART_Manage_Object UART6_Manage_Object = {0};
Struct_UART_Manage_Object UART7_Manage_Object = {0};
Struct_UART_Manage_Object UART8_Manage_Object = {0};

/* FIFO 缓冲区 — 每个 UART 一个独立的循环缓冲区 */
static uint8_t UART1_FIFO_Buffer[UART_FIFO_SIZE];
static uint8_t UART2_FIFO_Buffer[UART_FIFO_SIZE];
static uint8_t UART3_FIFO_Buffer[UART_FIFO_SIZE];
static uint8_t UART4_FIFO_Buffer[UART_FIFO_SIZE];
static uint8_t UART5_FIFO_Buffer[UART_FIFO_SIZE];
static uint8_t UART6_FIFO_Buffer[UART_FIFO_SIZE];
static uint8_t UART7_FIFO_Buffer[UART_FIFO_SIZE];
static uint8_t UART8_FIFO_Buffer[UART_FIFO_SIZE];

/* Private function declarations ---------------------------------------------*/

/* function prototypes -------------------------------------------------------*/

// ===== FIFO 实现 =====

/**
 * @brief 初始化循环缓冲区
 *
 * @param fifo FIFO 结构体指针
 * @param buffer 数据缓冲区（由调用方分配）
 * @param size 缓冲区容量（字节数）
 */
void UART_FIFO_Init(Struct_UART_FIFO *fifo, uint8_t *buffer, uint16_t size) {
    fifo->buffer = buffer;
    fifo->size = size;
    fifo->head = 0;
    fifo->tail = 0;
    fifo->overflow = false;
}

/**
 * @brief 向 FIFO 写入数据（可在 ISR 中调用）
 *
 * 当 FIFO 剩余空间不足时，写入尽可能多的字节并设置 overflow 标志。
 * 不会阻塞 ISR。
 *
 * @param fifo FIFO 结构体指针
 * @param data 源数据指针
 * @param len 期望写入的长度
 * @return uint16_t 实际写入的字节数
 */
uint16_t UART_FIFO_Push(Struct_UART_FIFO *fifo, const uint8_t *data, uint16_t len) {
    uint16_t pushed = 0;
    uint16_t head = fifo->head;
    const uint16_t tail = fifo->tail;
    const uint16_t size = fifo->size;

    for (uint16_t i = 0; i < len; i++) {
        uint16_t next_head = (head + 1);
        if (next_head >= size) next_head = 0;  // 模 size

        if (next_head == tail) {
            // FIFO 已满，无法继续写入
            fifo->overflow = true;
            break;
        }

        fifo->buffer[head] = data[i];
        head = next_head;
        pushed++;
    }

    fifo->head = head;
    return pushed;
}

/**
 * @brief 从 FIFO 读取数据（仅由应用线程调用）
 *
 * @param fifo FIFO 结构体指针
 * @param data 目标缓冲区指针
 * @param len 期望读取的长度
 * @return uint16_t 实际读取的字节数
 */
uint16_t UART_FIFO_Pop(Struct_UART_FIFO *fifo, uint8_t *data, uint16_t len) {
    uint16_t popped = 0;
    uint16_t tail = fifo->tail;
    const uint16_t head = fifo->head;
    const uint16_t size = fifo->size;

    for (uint16_t i = 0; i < len; i++) {
        if (tail == head) {
            // FIFO 已空
            break;
        }

        data[i] = fifo->buffer[tail];
        tail++;
        if (tail >= size) tail = 0;  // 模 size
        popped++;
    }

    fifo->tail = tail;
    return popped;
}

/**
 * @brief 获取 FIFO 中当前可读取的字节数
 *
 * @param fifo FIFO 结构体指针
 * @return uint16_t 可读取的字节数
 */
uint16_t UART_FIFO_Available(Struct_UART_FIFO *fifo) {
    if (fifo->head >= fifo->tail) {
        return fifo->head - fifo->tail;
    } else {
        return fifo->size - fifo->tail + fifo->head;
    }
}

/**
 * @brief 重置 FIFO（丢弃所有数据）
 *
 * @param fifo FIFO 结构体指针
 */
void UART_FIFO_Reset(Struct_UART_FIFO *fifo) {
    fifo->head = 0;
    fifo->tail = 0;
    fifo->overflow = false;
}

/**
 * @brief 检查 FIFO 溢出标志
 *
 * @param fifo FIFO 结构体指针
 * @return true 发生过溢出
 * @return false 未发生溢出
 */
bool UART_FIFO_IsOverflow(Struct_UART_FIFO *fifo) {
    return fifo->overflow;
}

/**
 * @brief 清除 FIFO 溢出标志
 *
 * @param fifo FIFO 结构体指针
 */
void UART_FIFO_ClearOverflow(Struct_UART_FIFO *fifo) {
    fifo->overflow = false;
}

/**
 * @brief 初始化UART
 *
 * @param huart UART编号
 * @param Callback_Function 处理回调函数
 */
void UART_Init(UART_HandleTypeDef *huart, UART_Call_Back Callback_Function, uint16_t Rx_Buffer_Length) {
    if (huart->Instance == USART1) {
        UART1_Manage_Object.UART_Handler = huart;
        UART1_Manage_Object.Callback_Function = Callback_Function;
        UART1_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        UART_FIFO_Init(&UART1_Manage_Object.Rx_FIFO, UART1_FIFO_Buffer, UART_FIFO_SIZE);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART1_Manage_Object.Rx_Buffer, UART1_Manage_Object.Rx_Buffer_Length);
        //__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
    } else if (huart->Instance == USART2) {
        UART2_Manage_Object.UART_Handler = huart;
        UART2_Manage_Object.Callback_Function = Callback_Function;
        UART2_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        UART_FIFO_Init(&UART2_Manage_Object.Rx_FIFO, UART2_FIFO_Buffer, UART_FIFO_SIZE);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART2_Manage_Object.Rx_Buffer, UART2_Manage_Object.Rx_Buffer_Length);
        //__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
    } else if (huart->Instance == USART3) {
        UART3_Manage_Object.UART_Handler = huart;
        UART3_Manage_Object.Callback_Function = Callback_Function;
        UART3_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        UART_FIFO_Init(&UART3_Manage_Object.Rx_FIFO, UART3_FIFO_Buffer, UART_FIFO_SIZE);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART3_Manage_Object.Rx_Buffer, UART3_Manage_Object.Rx_Buffer_Length);
        __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
    } else if (huart->Instance == UART4) {
        UART4_Manage_Object.UART_Handler = huart;
        UART4_Manage_Object.Callback_Function = Callback_Function;
        UART4_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        UART_FIFO_Init(&UART4_Manage_Object.Rx_FIFO, UART4_FIFO_Buffer, UART_FIFO_SIZE);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART4_Manage_Object.Rx_Buffer, UART4_Manage_Object.Rx_Buffer_Length);
        __HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
    } else if (huart->Instance == UART5) {
        UART5_Manage_Object.UART_Handler = huart;
        UART5_Manage_Object.Callback_Function = Callback_Function;
        UART5_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        UART_FIFO_Init(&UART5_Manage_Object.Rx_FIFO, UART5_FIFO_Buffer, UART_FIFO_SIZE);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART5_Manage_Object.Rx_Buffer, UART5_Manage_Object.Rx_Buffer_Length);
        //			__HAL_DMA_DISABLE_IT(&hdma_usart5_rx, DMA_IT_HT);
    } else if (huart->Instance == USART6) {
        UART6_Manage_Object.UART_Handler = huart;
        UART6_Manage_Object.Callback_Function = Callback_Function;
        UART6_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        UART_FIFO_Init(&UART6_Manage_Object.Rx_FIFO, UART6_FIFO_Buffer, UART_FIFO_SIZE);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART6_Manage_Object.Rx_Buffer, UART6_Manage_Object.Rx_Buffer_Length);
        //__HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT);
        memset(UART6_Manage_Object.Rx_Buffer, 0, UART6_Manage_Object.Rx_Buffer_Length);
    }
}

/**
 * @brief 发送数据帧
 *
 * @param huart UART编号
 * @param Data 被发送的数据指针
 * @param Length 长度
 * @return uint8_t 执行状态
 */
uint8_t UART_Send_Data(UART_HandleTypeDef *huart, uint8_t *Data, uint16_t Length) {
    return (HAL_UART_Transmit_DMA(huart, Data, Length));
}

/**
 * @brief UART的TIM定时器中断发送回调函数
 *
 */
void TIM_UART_PeriodElapsedCallback() {
    // UART1超电通讯
    UART_Send_Data(&huart1, UART1_Manage_Object.Tx_Buffer, 10);
}

/**
 * @brief HAL库UART接收DMA空闲中断
 *
 * @param huart UART编号
 * @param Size 长度
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    //停止DMA接收 保护处理过程
    //    HAL_UART_DMAStop(huart);
    //选择回调函数
    if (huart->Instance == USART1) {
        UART1_Manage_Object.Rx_Length = Size;
        UART_FIFO_Push(&UART1_Manage_Object.Rx_FIFO, UART1_Manage_Object.Rx_Buffer, Size);
        UART1_Manage_Object.Callback_Function(UART1_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART1_Manage_Object.Rx_Buffer, UART1_Manage_Object.Rx_Buffer_Length);
        //__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
    } else if (huart->Instance == USART2) {
        UART2_Manage_Object.Rx_Length = Size;
        UART_FIFO_Push(&UART2_Manage_Object.Rx_FIFO, UART2_Manage_Object.Rx_Buffer, Size);
        UART2_Manage_Object.Callback_Function(UART2_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART2_Manage_Object.Rx_Buffer, UART2_Manage_Object.Rx_Buffer_Length);
        //				__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
    } else if (huart->Instance == USART3) {
        UART3_Manage_Object.Rx_Length = Size;
        UART_FIFO_Push(&UART3_Manage_Object.Rx_FIFO, UART3_Manage_Object.Rx_Buffer, Size);
        UART3_Manage_Object.Callback_Function(UART3_Manage_Object.Rx_Buffer, Size);
        memset(UART3_Manage_Object.Rx_Buffer, 0, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART3_Manage_Object.Rx_Buffer, UART3_Manage_Object.Rx_Buffer_Length);
        __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
    } else if (huart->Instance == UART4) {
        UART4_Manage_Object.Rx_Length = Size;
        UART_FIFO_Push(&UART4_Manage_Object.Rx_FIFO, UART4_Manage_Object.Rx_Buffer, Size);
        UART4_Manage_Object.Callback_Function(UART4_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART4_Manage_Object.Rx_Buffer, UART4_Manage_Object.Rx_Buffer_Length);
        //				__HAL_DMA_DISABLE_IT(&hdma_usart4_rx, DMA_IT_HT);
    } else if (huart->Instance == UART5) {
        UART5_Manage_Object.Rx_Length = Size;
        UART_FIFO_Push(&UART5_Manage_Object.Rx_FIFO, UART5_Manage_Object.Rx_Buffer, Size);
        UART5_Manage_Object.Callback_Function(UART5_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART5_Manage_Object.Rx_Buffer, UART5_Manage_Object.Rx_Buffer_Length);
        //				__HAL_DMA_DISABLE_IT(&hdma_usart5_rx, DMA_IT_HT);
    } else if (huart->Instance == USART6) {
        UART6_Manage_Object.Rx_Length = Size;
        UART_FIFO_Push(&UART6_Manage_Object.Rx_FIFO, UART6_Manage_Object.Rx_Buffer, Size);
        UART6_Manage_Object.Callback_Function(UART6_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART6_Manage_Object.Rx_Buffer, UART6_Manage_Object.Rx_Buffer_Length);
        //__HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT);
    }
}

//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
//{
//	if (huart->Instance == USART3)
//    {
//        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART3_Manage_Object.Rx_Buffer, UART3_Manage_Object.Rx_Buffer_Length);
//				__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
//		}
//}
/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
