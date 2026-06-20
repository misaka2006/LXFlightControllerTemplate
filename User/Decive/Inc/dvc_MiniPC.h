//
// Created by minec on 2026/6/20.
//

#ifndef ANO_H743_DVC_MINIPC_H
#define ANO_H743_DVC_MINIPC_H

#include "main.h"

/**
 * @brief 迷你主机状态
 *
 */
enum Enum_MiniPC_Status : uint8_t
{
    MiniPC_Status_DISABLE = 0,
    MiniPC_Status_ENABLE,
};

class Class_MiniPC {
    public:

    void Init(UART_HandleTypeDef *huart);

protected:

};

#endif //ANO_H743_DVC_MINIPC_H