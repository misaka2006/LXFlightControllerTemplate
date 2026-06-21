//
// Created by minec on 2026/6/21.
//

#include "main.h"
#include "usart.h"
#include "../../Config/config.h"
#include "../../Driver/Inc/drv_uart.h"
#include "../../Interaction/Inc/ita_drone.h"

Class_Drone Drone;
/**
 * @brief UART4 LX飞控通信回调函数
 *
 */
void LXFC_UART4_Callback(uint8_t *Buffer, uint16_t Length) {
    Drone.LXFlightController.LXFC_UART_RxCpltCallback(Buffer, Length);
}

extern "C" void Init_Task() {

    UART_Init(&UART_LXFC, LXFC_UART4_Callback,128);

}