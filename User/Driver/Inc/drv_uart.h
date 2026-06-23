

#ifndef DRV_UART_H
#define DRV_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32h7xx_hal.h"
#include <stdbool.h>

/* Exported macros -----------------------------------------------------------*/

// 缓冲区字节长度
#define UART_BUFFER_SIZE 128

// FIFO 缓冲区容量（必须是 2 的幂，以支持高效的模运算）
#define UART_FIFO_SIZE 256

/* Exported types ------------------------------------------------------------*/

/**
 * @brief UART通信接收回调函数数据类型
 *
 */
typedef void (*UART_Call_Back)(uint8_t *Buffer, uint16_t Length);

/**
 * @brief UART循环缓冲区（FIFO）结构体
 *
 * 基于 head/tail 指针的环形缓冲区：
 *  - head == tail → 空
 *  - (head + 1) % size == tail → 满（保留一个槽位区分空/满）
 *
 *  ISR 只写入 head，应用线程只写入 tail，Cortex-M7 上 uint16_t 对齐读写是原子的，
 *  因此 Push（ISR）和 Pop（线程）之间无需临界区保护。
 */
struct Struct_UART_FIFO
{
    uint8_t *buffer;            // FIFO 存储区指针
    uint16_t size;              // FIFO 容量
    volatile uint16_t head;     // 写指针（仅在 ISR 中修改）
    volatile uint16_t tail;     // 读指针（仅在线程中修改）
    volatile bool overflow;     // 溢出标志（FIFO 满时有过新数据被丢弃）
};

/**
 * @brief UART通信处理结构体
 */
struct Struct_UART_Manage_Object
{
    UART_HandleTypeDef *UART_Handler;
    uint8_t Tx_Buffer[UART_BUFFER_SIZE];
    uint8_t Rx_Buffer[UART_BUFFER_SIZE];
    uint16_t Rx_Buffer_Length;
    uint16_t Tx_Buffer_Length;
    uint16_t Rx_Length;
    uint16_t Tx_Length;
    UART_Call_Back Callback_Function;
    Struct_UART_FIFO Rx_FIFO;   // 接收循环缓冲区
};

/* Exported variables --------------------------------------------------------*/



extern UART_HandleTypeDef huart1;
//extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
//extern UART_HandleTypeDef huart7;

extern struct Struct_UART_Manage_Object UART1_Manage_Object;
extern struct Struct_UART_Manage_Object UART2_Manage_Object;
extern struct Struct_UART_Manage_Object UART3_Manage_Object;
extern struct Struct_UART_Manage_Object UART4_Manage_Object;
extern struct Struct_UART_Manage_Object UART5_Manage_Object;
extern struct Struct_UART_Manage_Object UART6_Manage_Object;
extern struct Struct_UART_Manage_Object UART7_Manage_Object;
extern struct Struct_UART_Manage_Object UART8_Manage_Object;

/* Exported function declarations --------------------------------------------*/

void UART_Init(UART_HandleTypeDef *huart, UART_Call_Back Callback_Function, uint16_t Rx_Buffer_Length);

uint8_t UART_Send_Data(UART_HandleTypeDef *huart, uint8_t *Data, uint16_t Length);

void TIM_UART_PeriodElapsedCallback();

// ===== FIFO API（线程安全：Push 由 ISR 调用，Pop/Available 由应用线程调用） =====

void UART_FIFO_Init(Struct_UART_FIFO *fifo, uint8_t *buffer, uint16_t size);
uint16_t UART_FIFO_Push(Struct_UART_FIFO *fifo, const uint8_t *data, uint16_t len);
uint16_t UART_FIFO_Pop(Struct_UART_FIFO *fifo, uint8_t *data, uint16_t len);
uint16_t UART_FIFO_Available(Struct_UART_FIFO *fifo);
void UART_FIFO_Reset(Struct_UART_FIFO *fifo);
bool UART_FIFO_IsOverflow(Struct_UART_FIFO *fifo);
void UART_FIFO_ClearOverflow(Struct_UART_FIFO *fifo);

#ifdef __cplusplus
}
#endif

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
