//
// Created by minec on 2026/6/19.
//
/**
 *@author La_Pluma
 */

#include "../Inc/dvc_LXFlightController.h"

#include "../../Config/config.h"

Class_LXFlightController::Class_LXFlightController() {
    memset(&LXFC_Rx_Data_IMU, 0, sizeof(LXFC_Rx_Data_IMU));
    memset(&LXFC_Rx_Data_MagBaroTemp, 0, sizeof(LXFC_Rx_Data_MagBaroTemp));
    memset(&LXFC_Rx_Data_AttitudeEuler, 0, sizeof(LXFC_Rx_Data_AttitudeEuler));
    memset(&LXFC_Rx_Data_AttitudeQuaternion, 0, sizeof(LXFC_Rx_Data_AttitudeQuaternion));
    memset(&LXFC_Rx_Data_Height, 0, sizeof(LXFC_Rx_Data_Height));
    memset(&LXFC_Rx_Data_FlightMode, 0, sizeof(LXFC_Rx_Data_FlightMode));
    memset(&LXFC_Rx_Data_Speed, 0, sizeof(LXFC_Rx_Data_Speed));
    memset(&LXFC_Rx_Data_PositionOffset, 0, sizeof(LXFC_Rx_Data_PositionOffset));
    memset(&LXFC_Rx_Data_WindEstimate, 0, sizeof(LXFC_Rx_Data_WindEstimate));
    memset(&LXFC_Rx_Data_TargetAttitude, 0, sizeof(LXFC_Rx_Data_TargetAttitude));
    memset(&LXFC_Rx_Data_TargetSpeed, 0, sizeof(LXFC_Rx_Data_TargetSpeed));
    memset(&LXFC_Rx_Data_ReturnHome, 0, sizeof(LXFC_Rx_Data_ReturnHome));
    memset(&LXFC_Rx_Data_VoltageCurrent, 0, sizeof(LXFC_Rx_Data_VoltageCurrent));
    memset(&LXFC_Rx_Data_ExternalModule, 0, sizeof(LXFC_Rx_Data_ExternalModule));
    memset(&LXFC_Rx_Data_RGBBrightness, 0, sizeof(LXFC_Rx_Data_RGBBrightness));
    memset(&LXFC_Rx_Data_LogString, 0, sizeof(LXFC_Rx_Data_LogString));
    memset(&LXFC_Rx_Data_LogStringValue, 0, sizeof(LXFC_Rx_Data_LogStringValue));
    memset(&LXFC_Rx_Data_PWMOutput, 0, sizeof(LXFC_Rx_Data_PWMOutput));
    memset(&LXFC_Rx_Data_AttitudeControl, 0, sizeof(LXFC_Rx_Data_AttitudeControl));
    memset(&LXFC_Rx_Data_GPSInfo, 0, sizeof(LXFC_Rx_Data_GPSInfo));
    memset(&LXFC_Rx_Data_PositionSensor, 0, sizeof(LXFC_Rx_Data_PositionSensor));
    memset(&LXFC_Rx_Data_SpeedSensor, 0, sizeof(LXFC_Rx_Data_SpeedSensor));
    memset(&LXFC_Rx_Data_RangeSensor, 0, sizeof(LXFC_Rx_Data_RangeSensor));
    memset(&LXFC_Rx_Data_RC, 0, sizeof(LXFC_Rx_Data_RC));
    memset(&LXFC_Rx_Data_OpticalFlow_Mode0, 0, sizeof(LXFC_Rx_Data_OpticalFlow_Mode0));
    memset(&LXFC_Rx_Data_OpticalFlow_Mode1, 0, sizeof(LXFC_Rx_Data_OpticalFlow_Mode1));
    memset(&LXFC_Rx_Data_OpticalFlow_Mode2, 0, sizeof(LXFC_Rx_Data_OpticalFlow_Mode2));
    memset(&LXFC_Rx_Data_WaypointRead, 0, sizeof(LXFC_Rx_Data_WaypointRead));
    memset(&LXFC_Rx_Data_WaypointWrite, 0, sizeof(LXFC_Rx_Data_WaypointWrite));
}

void Class_LXFlightController::Init(const UART_HandleTypeDef *huart, uint8_t __Frame_Header) {
    if (huart->Instance == USART1) {
        UART_Manage_Object = &UART1_Manage_Object;
    } else if (huart->Instance == USART2) {
        UART_Manage_Object = &UART2_Manage_Object;
    } else if (huart->Instance == USART3) {
        UART_Manage_Object = &UART3_Manage_Object;
    } else if (huart->Instance == UART4) {
        UART_Manage_Object = &UART4_Manage_Object;
    } else if (huart->Instance == UART5) {
        UART_Manage_Object = &UART5_Manage_Object;
    } else if (huart->Instance == USART6) {
        UART_Manage_Object = &UART6_Manage_Object;
    }
    Frame_Header = __Frame_Header;
}

void Class_LXFlightController::LXFC_UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length) {
    Flag += 1;
    Data_Process(Length);
}

void Class_LXFlightController::Data_Process(uint16_t Length) {
    Struct_LXFC_UART_Data *tmp_buffer;
    for (int i = 0; i < Length;) {
        tmp_buffer = (Struct_LXFC_UART_Data *) &UART_Manage_Object->Rx_Buffer[i];

        if (!(tmp_buffer->frame_head == FRAME_HEAD && tmp_buffer->data_addr == DATA_ADDR)) {
            //如果不满足帧头校验与地址校验就跳过
            i++;
            continue;
        }

        //此时已经满足帧头校验以及地址校验，开始进行和校验以及附加校验
        uint8_t sum_check = 0;
        uint8_t add_check = 0;
        for (int j = 0; j < tmp_buffer->data_length + 4; j++) { //这里data_length+4是因为要包含前面的HEAD,D_ADDR,ID,LEN4位
            sum_check += UART_Manage_Object->Rx_Buffer[j];
            add_check += sum_check;
        }

        if (sum_check != UART_Manage_Object->Rx_Buffer[tmp_buffer->data_length + 4] || add_check != UART_Manage_Object->Rx_Buffer[tmp_buffer->data_length + 5]) {
            i += 6 + tmp_buffer->data_length;
            /*
             * 若通过帧头与地址校验，但没通过校验和以及附加校验，则认为该包完整的接收到，但是传输过程出错，直接跳过该包
             * 这里+6是因为HEAD,D_ADDR,ID,LEN,SUM_CHECK,ADD_CHECK共6位
             */
        }

        //通过所有校验，开始解包
        switch ((Enum_LXFC_Function_ID) tmp_buffer->function_ID) {
            case LXFC_Function_ID_IMU: {
                memcpy(&LXFC_Rx_Data_IMU, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_IMU));
                break;
            }
            case LXFC_Function_ID_MagBaroTemp: {
                memcpy(&LXFC_Rx_Data_MagBaroTemp, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_MagBaroTemp));
                break;
            }
            case LXFC_Function_ID_AttitudeEuler: {
                memcpy(&LXFC_Rx_Data_AttitudeEuler, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_AttitudeEuler));
                break;
            }
            case LXFC_Function_ID_AttitudeQuaternion: {
                memcpy(&LXFC_Rx_Data_AttitudeQuaternion, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_AttitudeQuaternion));
                break;
            }
            case LXFC_Function_ID_Height: {
                memcpy(&LXFC_Rx_Data_Height, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_Height));
                break;
            }
            case LXFC_Function_ID_FlightMode: {
                memcpy(&LXFC_Rx_Data_FlightMode, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_FlightMode));
                break;
            }
            case LXFC_Function_ID_Speed: {
                memcpy(&LXFC_Rx_Data_Speed, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_Speed));
                break;
            }
            case LXFC_Function_ID_PositionOffset: {
                memcpy(&LXFC_Rx_Data_PositionOffset, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_PositionOffset));
                break;
            }
            case LXFC_Function_ID_WindEstimate: {
                memcpy(&LXFC_Rx_Data_WindEstimate, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_WindEstimate));
                break;
            }
            case LXFC_Function_ID_TargetAttitude: {
                memcpy(&LXFC_Rx_Data_TargetAttitude, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_TargetAttitude));
                break;
            }
            case LXFC_Function_ID_TargetSpeed: {
                memcpy(&LXFC_Rx_Data_TargetSpeed, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_TargetSpeed));
                break;
            }
            case LXFC_Function_ID_ReturnHome: {
                memcpy(&LXFC_Rx_Data_ReturnHome, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_ReturnHome));
                break;
            }
            case LXFC_Function_ID_VoltageCurrent: {
                memcpy(&LXFC_Rx_Data_VoltageCurrent, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_VoltageCurrent));
                break;
            }
            case LXFC_Function_ID_ExternalModule: {
                memcpy(&LXFC_Rx_Data_ExternalModule, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_ExternalModule));
                break;
            }
            case LXFC_Function_ID_RGBBrightness: {
                memcpy(&LXFC_Rx_Data_RGBBrightness, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_RGBBrightness));
                break;
            }
            case LXFC_Function_ID_PWMOutput: {
                memcpy(&LXFC_Rx_Data_PWMOutput, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_PWMOutput));
                break;
            }
            case LXFC_Function_ID_AttitudeControl: {
                memcpy(&LXFC_Rx_Data_AttitudeControl, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_AttitudeControl));
                break;
            }
            case LXFC_Function_ID_GPSInfo: {
                memcpy(&LXFC_Rx_Data_GPSInfo, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_GPSInfo));
                break;
            }
            // 0x31 为保留帧，无对应结构体，忽略
            case LXFC_Function_ID_PositionSensor: {
                memcpy(&LXFC_Rx_Data_PositionSensor, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_PositionSensor));
                break;
            }
            case LXFC_Function_ID_SpeedSensor: {
                memcpy(&LXFC_Rx_Data_SpeedSensor, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_SpeedSensor));
                break;
            }
            case LXFC_Function_ID_RangeSensor: {
                memcpy(&LXFC_Rx_Data_RangeSensor, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_RangeSensor));
                break;
            }
            case LXFC_Function_ID_RC: {
                memcpy(&LXFC_Rx_Data_RC, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_RC));
                break;
            }
            case LXFC_Function_ID_OpticalFlow: {
                // 光流数据需根据数据区第一个字节（mode）选择对应结构体
                uint8_t mode = ((uint8_t *) tmp_buffer->Data)[0];
                if (mode == 0) {
                    memcpy(&LXFC_Rx_Data_OpticalFlow_Mode0, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_OpticalFlow_Mode0));
                } else if (mode == 1) {
                    memcpy(&LXFC_Rx_Data_OpticalFlow_Mode1, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_OpticalFlow_Mode1));
                } else if (mode == 2) {
                    memcpy(&LXFC_Rx_Data_OpticalFlow_Mode2, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_OpticalFlow_Mode2));
                }
                break;
            }
            case LXFC_Function_ID_WaypointRead: {
                memcpy(&LXFC_Rx_Data_WaypointRead, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_WaypointRead));
                break;
            }
            case LXFC_Function_ID_WaypointWrite: {
                memcpy(&LXFC_Rx_Data_WaypointWrite, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_WaypointWrite));
                break;
            }
            case LXFC_Function_ID_LogString: {
                memcpy(&LXFC_Rx_Data_LogString, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_LogString));
                break;
            }
            case LXFC_Function_ID_LogStringValue: {
                memcpy(&LXFC_Rx_Data_LogStringValue, tmp_buffer, sizeof(Struct_LXFC_Rx_Data_LogStringValue));
                break;
            }
            default: {
                // 未知功能码，可忽略或记录错误
                break;
            }
        }
    }
}
