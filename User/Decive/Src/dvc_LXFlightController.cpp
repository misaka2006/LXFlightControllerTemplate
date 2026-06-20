//
// Created by minec on 2026/6/19.
//
/**
 *@author La_Pluma
 */

#include "../Inc/dvc_LXFlightController.h"

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

void Class_LXFlightController::Init(const UART_HandleTypeDef *huart,uint8_t __Frame_Header) {
    if (huart->Instance == USART1)
    {
        UART_Manage_Object = &UART1_Manage_Object;
    }
    else if (huart->Instance == USART2)
    {
        UART_Manage_Object = &UART2_Manage_Object;
    }
    else if (huart->Instance == USART3)
    {
        UART_Manage_Object = &UART3_Manage_Object;
    }
    else if (huart->Instance == UART4)
    {
        UART_Manage_Object = &UART4_Manage_Object;
    }
    else if (huart->Instance == UART5)
    {
        UART_Manage_Object = &UART5_Manage_Object;
    }
    else if (huart->Instance == USART6)
    {
        UART_Manage_Object = &UART6_Manage_Object;
    }
    Frame_Header = __Frame_Header;
}

void Class_LXFlightController::LXFC_UART_RxCpltCallback(uint8_t *Rx_Data,uint16_t Length) {

}