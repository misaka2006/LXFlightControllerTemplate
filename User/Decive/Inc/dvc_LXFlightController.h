//
// Created by minec on 2026/6/19.
//

#ifndef ANO_H743_DVC_LXFLIGHTCONTROLLER_H
#define ANO_H743_DVC_LXFLIGHTCONTROLLER_H

// 匿名通信协议 - 基本信息类帧结构体定义
// 所有结构体采用小端模式，与协议一致

#include <cstdint>
#include <cstring>
#include "../../Driver/Inc/drv_uart.h"

// ==================== 1. 飞控相关信息类 ====================

// ID: 0x01 惯性传感器数据
struct Struct_LXFC_Rx_Data_IMU {
    uint8_t frame_head;         // 0xAA
    uint8_t data_addr;          // 目标地址
    uint8_t function_ID;        // 0x01
    uint8_t data_length;        // 13
    int16_t acc_x;              // 加速度 X
    int16_t acc_y;              // 加速度 Y
    int16_t acc_z;              // 加速度 Z
    int16_t gyro_x;             // 陀螺仪 X
    int16_t gyro_y;             // 陀螺仪 Y
    int16_t gyro_z;             // 陀螺仪 Z
    uint8_t shock_status;       // 震动状态
    uint8_t check_sum;          // 和校验
    uint8_t add_sum;            // 附加校验
} __attribute__((packed));

// ID: 0x02 罗盘、气压、温度传感器数据
struct Struct_LXFC_Rx_Data_MagBaroTemp {
    uint8_t frame_head;         // 0xAA
    uint8_t data_addr;
    uint8_t function_ID;        // 0x02
    uint8_t data_length;        // 14
    int16_t mag_x;              // 磁罗盘 X
    int16_t mag_y;              // 磁罗盘 Y
    int16_t mag_z;              // 磁罗盘 Z
    int32_t alt_bar;            // 气压计高度，单位 cm
    int16_t tmp;                // 温度，放大10倍，0.1℃
    uint8_t bar_status;         // 气压状态
    uint8_t mag_status;         // 罗盘状态
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x03 飞控姿态：欧拉角格式
struct Struct_LXFC_Rx_Data_AttitudeEuler {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x03
    uint8_t data_length;        // 7
    int16_t roll;               // 横滚角 *100，单位 0.01°
    int16_t pitch;              // 俯仰角 *100
    int16_t yaw;                // 航向角 *100
    uint8_t fusion_status;      // 融合状态
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x04 飞控姿态：四元数格式
struct Struct_LXFC_Rx_Data_AttitudeQuaternion {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x04
    uint8_t data_length;        // 9
    int16_t v0;                 // 四元数 V0 *10000
    int16_t v1;                 // 四元数 V1 *10000
    int16_t v2;                 // 四元数 V2 *10000
    int16_t v3;                 // 四元数 V3 *10000
    uint8_t fusion_status;      // 融合状态
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x05 高度数据
struct Struct_LXFC_Rx_Data_Height {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x05
    uint8_t data_length;        // 9
    int32_t alt_fu;             // 融合对地高度，cm
    int32_t alt_add;            // 附加测距高度，cm（如超声波）
    uint8_t alt_status;         // 测距状态
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x06 飞控运行模式
struct Struct_LXFC_Rx_Data_FlightMode {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x06
    uint8_t data_length;        // 5
    uint8_t mode;               // 飞控模式
    uint8_t locked;             // 锁定标志，1解锁，0锁定
    uint8_t cid;                // 当前执行指令的 CID
    uint8_t cmd0;               // 当前执行指令的 CMD0
    uint8_t cmd1;               // 当前执行指令的 CMD1
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x07 飞行速度数据
struct Struct_LXFC_Rx_Data_Speed {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x07
    uint8_t data_length;        // 6
    int16_t speed_x;            // X 方向速度，cm/s
    int16_t speed_y;            // Y 方向速度，cm/s
    int16_t speed_z;            // Z 方向速度，cm/s
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x08 位置偏移数据
struct Struct_LXFC_Rx_Data_PositionOffset {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x08
    uint8_t data_length;        // 8
    int32_t pos_x;              // 相对起飞点 X 偏移，cm
    int32_t pos_y;              // 相对起飞点 Y 偏移，cm
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x09 风速估计
struct Struct_LXFC_Rx_Data_WindEstimate {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x09
    uint8_t data_length;        // 4
    int16_t wind_x;             // 风速 X（单位未明确，推测为 cm/s 或其他）
    int16_t wind_y;             // 风速 Y
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x0A 目标姿态数据
struct Struct_LXFC_Rx_Data_TargetAttitude {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x0A
    uint8_t data_length;        // 6
    int16_t tar_roll;           // 目标横滚角 *100
    int16_t tar_pitch;          // 目标俯仰角 *100
    int16_t tar_yaw;            // 目标航向角 *100
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x0B 目标速度数据
struct Struct_LXFC_Rx_Data_TargetSpeed {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x0B
    uint8_t data_length;        // 6
    int16_t tar_speed_x;        // 目标 X 速度，cm/s
    int16_t tar_speed_y;        // 目标 Y 速度，cm/s
    int16_t tar_speed_z;        // 目标 Z 速度，cm/s
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x0C 回航信息
struct Struct_LXFC_Rx_Data_ReturnHome {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x0C
    uint8_t data_length;        // 4
    int16_t r_a;                // 回航角度 *10，单位 0.1°
    uint16_t r_d;               // 回航距离，m
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x0D 电压电流数据
struct Struct_LXFC_Rx_Data_VoltageCurrent {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x0D
    uint8_t data_length;        // 4
    uint16_t voltage;           // 电压 *100，单位 0.01V
    uint16_t current;           // 电流 *100，单位 0.01A
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x0E 外接模块工作状态
struct Struct_LXFC_Rx_Data_ExternalModule {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x0E
    uint8_t data_length;        // 4
    uint8_t sta_g_vel;          // 通用速度传感器状态
    uint8_t sta_g_pos;          // 通用位置传感器状态
    uint8_t sta_gps;            // GPS 状态
    uint8_t sta_alt_add;        // 附加测距状态
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x0F RGB 亮度信息输出
struct Struct_LXFC_Rx_Data_RGBBrightness {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x0F
    uint8_t data_length;        // 4
    uint8_t bri_r;              // 红色亮度 0~20
    uint8_t bri_g;              // 绿色亮度 0~20
    uint8_t bri_b;              // 蓝色亮度 0~20
    uint8_t bri_a;              // 单独 LED 亮度 0~20
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0xA0 LOG 信息输出 -- 字符串（可变长度，此处定义最大 32 字节）
#define MAX_LOG_STR_LEN 32
struct Struct_LXFC_Rx_Data_LogString {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0xA0
    uint8_t data_length;        // 实际长度 = 1 + strlen(STR)
    uint8_t color;              // 颜色：0黑，1红，2绿
    uint8_t str[MAX_LOG_STR_LEN]; // 字符串内容（ASCII），实际有效长度由 data_length-1 决定
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0xA1 LOG 信息输出 -- 字符串 + 数字
struct Struct_LXFC_Rx_Data_LogStringValue {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0xA1
    uint8_t data_length;        // 实际长度 = 4 + strlen(STR)
    int32_t val;                // 数值
    uint8_t str[MAX_LOG_STR_LEN]; // 字符串内容（ASCII）
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ==================== 2. 飞控控制量输出类 ====================

// ID: 0x20 PWM 控制量（最大 8 通道）
struct Struct_LXFC_Rx_Data_PWMOutput {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x20
    uint8_t data_length;        // 8~16，实际通道数*2
    uint16_t pwm1;              // 通道1，0~10000，单位0.01%油门
    uint16_t pwm2;
    uint16_t pwm3;
    uint16_t pwm4;
    uint16_t pwm5;              // 6/8轴模式使用
    uint16_t pwm6;
    uint16_t pwm7;
    uint16_t pwm8;
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x21 姿态控制量
struct Struct_LXFC_Rx_Data_AttitudeControl {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x21
    uint8_t data_length;        // 8
    int16_t ctrl_roll;          // 横滚控制量 ±5000
    int16_t ctrl_pitch;         // 俯仰控制量 ±5000
    int16_t ctrl_thr;           // 油门控制量 0~10000
    int16_t ctrl_yaw;           // 航向控制量 ±5000
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ==================== 3. 飞控接收信息类 ====================

// ID: 0x30 GPS 传感器信息1
struct Struct_LXFC_Rx_Data_GPSInfo {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x30
    uint8_t data_length;        // 23
    uint8_t fix_sta;            // 定位状态 (UBX)
    uint8_t s_num;              // 卫星数量
    int32_t lng;                // 经度 *1e7
    int32_t lat;                // 纬度 *1e7
    int32_t alt_gps;            // GPS 高度，单位 cm
    int16_t n_spec;             // NED 北向速度，cm/s
    int16_t e_spec;             // NED 东向速度，cm/s
    int16_t d_spec;             // NED 地向速度，cm/s
    uint8_t pdop;               // 定位精度，实际值/100，范围0~200
    uint8_t sacc;               // 速度精度，实际值/100，mm
    uint8_t vacc;               // 高度精度，实际值/100，mm
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x32 通用位置型传感器数据（非捷联载体测量型）
struct Struct_LXFC_Rx_Data_PositionSensor {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x32
    uint8_t data_length;        // 12
    int32_t pos_x;              // X 位置，cm，0x80000000 表示无效
    int32_t pos_y;              // Y 位置，cm
    int32_t pos_z;              // Z 位置，cm
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x33 通用速度型传感器数据（捷联载体测量型）
struct Struct_LXFC_Rx_Data_SpeedSensor {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x33
    uint8_t data_length;        // 6
    int16_t speed_x;            // X 速度，cm/s，0x8000 无效
    int16_t speed_y;            // Y 速度，cm/s
    int16_t speed_z;            // Z 速度，cm/s
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x34 通用测距传感器数据（捷联载体测量型）
struct Struct_LXFC_Rx_Data_RangeSensor {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x34
    uint8_t data_length;        // 7
    uint8_t direction;          // 安装方向：0水平，1垂直
    uint16_t angle;             // 角度 0~359
    uint32_t dist;              // 距离，cm，0xFFFFFFFF 无效
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x40 遥控器数据（10 通道）
struct Struct_LXFC_Rx_Data_RC {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x40
    uint8_t data_length;        // 20
    int16_t ch1;                // 通道1
    int16_t ch2;
    int16_t ch3;
    int16_t ch4;
    int16_t ch5;
    int16_t ch6;
    int16_t ch7;
    int16_t ch8;
    int16_t ch9;
    int16_t ch10;
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ==================== 4. 光流信息类 ====================

// ID: 0x51 匿名光流数据 - MODE 0 (原始光流)
struct Struct_LXFC_Rx_Data_OpticalFlow_Mode0 {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x51
    uint8_t data_length;        // 5 (根据文档)
    uint8_t mode;               // 0
    uint8_t state;              // 0无效，1有效
    int8_t dx_0;                // X 光流，像素/20ms
    int8_t dy_0;                // Y 光流，像素/20ms
    uint8_t quality;            // 质量 0~255
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x51 MODE 1 (融合光流)
struct Struct_LXFC_Rx_Data_OpticalFlow_Mode1 {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x51
    uint8_t data_length;        // 7
    uint8_t mode;               // 1
    uint8_t state;              // 0无效，1有效
    int16_t dx_1;               // X 地面速度，cm/s
    int16_t dy_1;               // Y 地面速度，cm/s
    uint8_t quality;            // 质量 0~255
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x51 MODE 2 (惯导融合光流)
struct Struct_LXFC_Rx_Data_OpticalFlow_Mode2 {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x51
    uint8_t data_length;        // 15
    uint8_t mode;               // 2
    uint8_t state;              // 0无效，1有效
    int16_t dx_2;               // X 融合速度，cm/s
    int16_t dy_2;               // Y 融合速度，cm/s
    int16_t dx_fix;             // X 修正速度，适用于积分
    int16_t dy_fix;             // Y 修正速度
    int16_t integ_x;            // X 速度积分，cm，-32768~32767循环
    int16_t integ_y;            // Y 速度积分
    uint8_t quality;            // 质量 0~255
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ==================== 5. GPS 航点读写帧 ====================

// ID: 0x60 航点读取
struct Struct_LXFC_Rx_Data_WaypointRead {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x60
    uint8_t data_length;        // 1
    uint8_t num;                // 0xFF 读取数量，n 读取第 n 个航点
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

// ID: 0x61 航点写入 / 航点读取返回
struct Struct_LXFC_Rx_Data_WaypointWrite {
    uint8_t frame_head;
    uint8_t data_addr;
    uint8_t function_ID;        // 0x61
    uint8_t data_length;        // 22
    uint8_t num;                // 航点编号（0~）
    int32_t lng;                // 经度 *1e7
    int32_t lat;                // 纬度 *1e7
    int32_t alt;                // 高度，cm
    uint16_t spd;               // 飞行速度，cm/s
    uint16_t yaw;               // 机头朝向（0~359 或 400 指向目标）
    uint8_t fun;                // 航点功能
    uint8_t cmd1;               // 功能参数1
    uint8_t cmd2;               // 功能参数2
    uint8_t cmd3;               // 功能参数3
    uint8_t cmd4;               // 功能参数4
    uint8_t check_sum;
    uint8_t add_sum;
} __attribute__((packed));

class Class_LXFlightController {
public:
    // 初始化所有成员变量为零
    Class_LXFlightController();
    void Init(const UART_HandleTypeDef *huart, uint8_t __Frame_Header);

    // ==================== 0x01 IMU ====================
    inline int16_t Get_IMU_Acc_X() const { return LXFC_Rx_Data_IMU.acc_x; }
    inline int16_t Get_IMU_Acc_Y() const { return LXFC_Rx_Data_IMU.acc_y; }
    inline int16_t Get_IMU_Acc_Z() const { return LXFC_Rx_Data_IMU.acc_z; }
    inline int16_t Get_IMU_Gyro_X() const { return LXFC_Rx_Data_IMU.gyro_x; }
    inline int16_t Get_IMU_Gyro_Y() const { return LXFC_Rx_Data_IMU.gyro_y; }
    inline int16_t Get_IMU_Gyro_Z() const { return LXFC_Rx_Data_IMU.gyro_z; }
    inline uint8_t Get_IMU_Shock_Status() const { return LXFC_Rx_Data_IMU.shock_status; }

    // ==================== 0x02 罗盘、气压、温度 ====================
    inline int16_t Get_Mag_X() const { return LXFC_Rx_Data_MagBaroTemp.mag_x; }
    inline int16_t Get_Mag_Y() const { return LXFC_Rx_Data_MagBaroTemp.mag_y; }
    inline int16_t Get_Mag_Z() const { return LXFC_Rx_Data_MagBaroTemp.mag_z; }
    inline int32_t Get_Bar_Alt() const { return LXFC_Rx_Data_MagBaroTemp.alt_bar; }
    inline int16_t Get_Temp() const { return LXFC_Rx_Data_MagBaroTemp.tmp; }         // 放大10倍
    inline uint8_t Get_Bar_Status() const { return LXFC_Rx_Data_MagBaroTemp.bar_status; }
    inline uint8_t Get_Mag_Status() const { return LXFC_Rx_Data_MagBaroTemp.mag_status; }

    // ==================== 0x03 欧拉角 ====================
    inline int16_t Get_Attitude_Roll() const { return LXFC_Rx_Data_AttitudeEuler.roll; }
    inline int16_t Get_Attitude_Pitch() const { return LXFC_Rx_Data_AttitudeEuler.pitch; }
    inline int16_t Get_Attitude_Yaw() const { return LXFC_Rx_Data_AttitudeEuler.yaw; }
    inline uint8_t Get_Attitude_Fusion_Status() const { return LXFC_Rx_Data_AttitudeEuler.fusion_status; }

    // ==================== 0x04 四元数 ====================
    inline int16_t Get_Quaternion_V0() const { return LXFC_Rx_Data_AttitudeQuaternion.v0; }
    inline int16_t Get_Quaternion_V1() const { return LXFC_Rx_Data_AttitudeQuaternion.v1; }
    inline int16_t Get_Quaternion_V2() const { return LXFC_Rx_Data_AttitudeQuaternion.v2; }
    inline int16_t Get_Quaternion_V3() const { return LXFC_Rx_Data_AttitudeQuaternion.v3; }
    inline uint8_t Get_Quaternion_Fusion_Status() const { return LXFC_Rx_Data_AttitudeQuaternion.fusion_status; }

    // ==================== 0x05 高度 ====================
    inline int32_t Get_Alt_Fu() const { return LXFC_Rx_Data_Height.alt_fu; }
    inline int32_t Get_Alt_Add() const { return LXFC_Rx_Data_Height.alt_add; }
    inline uint8_t Get_Alt_Status() const { return LXFC_Rx_Data_Height.alt_status; }

    // ==================== 0x06 飞行模式 ====================
    inline uint8_t Get_Mode() const { return LXFC_Rx_Data_FlightMode.mode; }
    inline uint8_t Get_Locked() const { return LXFC_Rx_Data_FlightMode.locked; }
    inline uint8_t Get_CID() const { return LXFC_Rx_Data_FlightMode.cid; }
    inline uint8_t Get_CMD0() const { return LXFC_Rx_Data_FlightMode.cmd0; }
    inline uint8_t Get_CMD1() const { return LXFC_Rx_Data_FlightMode.cmd1; }

    // ==================== 0x07 速度 ====================
    inline int16_t Get_Speed_X() const { return LXFC_Rx_Data_Speed.speed_x; }
    inline int16_t Get_Speed_Y() const { return LXFC_Rx_Data_Speed.speed_y; }
    inline int16_t Get_Speed_Z() const { return LXFC_Rx_Data_Speed.speed_z; }

    // ==================== 0x08 位置偏移 ====================
    inline int32_t Get_Pos_Offset_X() const { return LXFC_Rx_Data_PositionOffset.pos_x; }
    inline int32_t Get_Pos_Offset_Y() const { return LXFC_Rx_Data_PositionOffset.pos_y; }

    // ==================== 0x09 风速估计 ====================
    inline int16_t Get_Wind_X() const { return LXFC_Rx_Data_WindEstimate.wind_x; }
    inline int16_t Get_Wind_Y() const { return LXFC_Rx_Data_WindEstimate.wind_y; }

    // ==================== 0x0A 目标姿态 ====================
    inline int16_t Get_Target_Roll() const { return LXFC_Rx_Data_TargetAttitude.tar_roll; }
    inline int16_t Get_Target_Pitch() const { return LXFC_Rx_Data_TargetAttitude.tar_pitch; }
    inline int16_t Get_Target_Yaw() const { return LXFC_Rx_Data_TargetAttitude.tar_yaw; }

    // ==================== 0x0B 目标速度 ====================
    inline int16_t Get_Target_Speed_X() const { return LXFC_Rx_Data_TargetSpeed.tar_speed_x; }
    inline int16_t Get_Target_Speed_Y() const { return LXFC_Rx_Data_TargetSpeed.tar_speed_y; }
    inline int16_t Get_Target_Speed_Z() const { return LXFC_Rx_Data_TargetSpeed.tar_speed_z; }

    // ==================== 0x0C 回航信息 ====================
    inline int16_t Get_Return_Angle() const { return LXFC_Rx_Data_ReturnHome.r_a; }     // *10 deg
    inline uint16_t Get_Return_Distance() const { return LXFC_Rx_Data_ReturnHome.r_d; } // m

    // ==================== 0x0D 电压电流 ====================
    inline uint16_t Get_Voltage() const { return LXFC_Rx_Data_VoltageCurrent.voltage; } // *100
    inline uint16_t Get_Current() const { return LXFC_Rx_Data_VoltageCurrent.current; } // *100

    // ==================== 0x0E 外接模块状态 ====================
    inline uint8_t Get_Ext_G_Vel_Status() const { return LXFC_Rx_Data_ExternalModule.sta_g_vel; }
    inline uint8_t Get_Ext_G_Pos_Status() const { return LXFC_Rx_Data_ExternalModule.sta_g_pos; }
    inline uint8_t Get_Ext_GPS_Status() const { return LXFC_Rx_Data_ExternalModule.sta_gps; }
    inline uint8_t Get_Ext_Alt_Add_Status() const { return LXFC_Rx_Data_ExternalModule.sta_alt_add; }

    // ==================== 0x0F RGB亮度 ====================
    inline uint8_t Get_RGB_R() const { return LXFC_Rx_Data_RGBBrightness.bri_r; }
    inline uint8_t Get_RGB_G() const { return LXFC_Rx_Data_RGBBrightness.bri_g; }
    inline uint8_t Get_RGB_B() const { return LXFC_Rx_Data_RGBBrightness.bri_b; }
    inline uint8_t Get_RGB_A() const { return LXFC_Rx_Data_RGBBrightness.bri_a; }

    // ==================== 0xA0 LOG字符串 ====================
    inline uint8_t Get_Log_Color() const { return LXFC_Rx_Data_LogString.color; }
    inline const uint8_t* Get_Log_Str() const { return LXFC_Rx_Data_LogString.str; }
    // 字符串有效长度 = data_length - 1，如需可单独获取 data_length，但未提供，可自行扩展

    // ==================== 0xA1 LOG字符串+数值 ====================
    inline int32_t Get_Log_Value() const { return LXFC_Rx_Data_LogStringValue.val; }
    inline const uint8_t* Get_Log_Value_Str() const { return LXFC_Rx_Data_LogStringValue.str; }

    // ==================== 0x20 PWM控制量 ====================
    inline uint16_t Get_PWM_1() const { return LXFC_Rx_Data_PWMOutput.pwm1; }
    inline uint16_t Get_PWM_2() const { return LXFC_Rx_Data_PWMOutput.pwm2; }
    inline uint16_t Get_PWM_3() const { return LXFC_Rx_Data_PWMOutput.pwm3; }
    inline uint16_t Get_PWM_4() const { return LXFC_Rx_Data_PWMOutput.pwm4; }
    inline uint16_t Get_PWM_5() const { return LXFC_Rx_Data_PWMOutput.pwm5; }
    inline uint16_t Get_PWM_6() const { return LXFC_Rx_Data_PWMOutput.pwm6; }
    inline uint16_t Get_PWM_7() const { return LXFC_Rx_Data_PWMOutput.pwm7; }
    inline uint16_t Get_PWM_8() const { return LXFC_Rx_Data_PWMOutput.pwm8; }

    // ==================== 0x21 姿态控制量 ====================
    inline int16_t Get_Ctrl_Roll() const { return LXFC_Rx_Data_AttitudeControl.ctrl_roll; }
    inline int16_t Get_Ctrl_Pitch() const { return LXFC_Rx_Data_AttitudeControl.ctrl_pitch; }
    inline int16_t Get_Ctrl_Thr() const { return LXFC_Rx_Data_AttitudeControl.ctrl_thr; }
    inline int16_t Get_Ctrl_Yaw() const { return LXFC_Rx_Data_AttitudeControl.ctrl_yaw; }

    // ==================== 0x30 GPS信息 ====================
    inline uint8_t Get_GPS_Fix() const { return LXFC_Rx_Data_GPSInfo.fix_sta; }
    inline uint8_t Get_GPS_SatNum() const { return LXFC_Rx_Data_GPSInfo.s_num; }
    inline int32_t Get_GPS_Lng() const { return LXFC_Rx_Data_GPSInfo.lng; }
    inline int32_t Get_GPS_Lat() const { return LXFC_Rx_Data_GPSInfo.lat; }
    inline int32_t Get_GPS_Alt() const { return LXFC_Rx_Data_GPSInfo.alt_gps; }
    inline int16_t Get_GPS_N_Speed() const { return LXFC_Rx_Data_GPSInfo.n_spec; }
    inline int16_t Get_GPS_E_Speed() const { return LXFC_Rx_Data_GPSInfo.e_spec; }
    inline int16_t Get_GPS_D_Speed() const { return LXFC_Rx_Data_GPSInfo.d_spec; }
    inline uint8_t Get_GPS_PDOP() const { return LXFC_Rx_Data_GPSInfo.pdop; }   // 实际值/100
    inline uint8_t Get_GPS_SACC() const { return LXFC_Rx_Data_GPSInfo.sacc; }   // mm/100
    inline uint8_t Get_GPS_VACC() const { return LXFC_Rx_Data_GPSInfo.vacc; }   // mm/100

    // ==================== 0x32 通用位置传感器 ====================
    inline int32_t Get_PosSensor_X() const { return LXFC_Rx_Data_PositionSensor.pos_x; }
    inline int32_t Get_PosSensor_Y() const { return LXFC_Rx_Data_PositionSensor.pos_y; }
    inline int32_t Get_PosSensor_Z() const { return LXFC_Rx_Data_PositionSensor.pos_z; }

    // ==================== 0x33 通用速度传感器 ====================
    inline int16_t Get_SpeedSensor_X() const { return LXFC_Rx_Data_SpeedSensor.speed_x; }
    inline int16_t Get_SpeedSensor_Y() const { return LXFC_Rx_Data_SpeedSensor.speed_y; }
    inline int16_t Get_SpeedSensor_Z() const { return LXFC_Rx_Data_SpeedSensor.speed_z; }

    // ==================== 0x34 通用测距传感器 ====================
    inline uint8_t Get_Range_Direction() const { return LXFC_Rx_Data_RangeSensor.direction; }
    inline uint16_t Get_Range_Angle() const { return LXFC_Rx_Data_RangeSensor.angle; }
    inline uint32_t Get_Range_Dist() const { return LXFC_Rx_Data_RangeSensor.dist; }

    // ==================== 0x40 遥控器数据 ====================
    inline int16_t Get_RC_Ch1() const { return LXFC_Rx_Data_RC.ch1; }
    inline int16_t Get_RC_Ch2() const { return LXFC_Rx_Data_RC.ch2; }
    inline int16_t Get_RC_Ch3() const { return LXFC_Rx_Data_RC.ch3; }
    inline int16_t Get_RC_Ch4() const { return LXFC_Rx_Data_RC.ch4; }
    inline int16_t Get_RC_Ch5() const { return LXFC_Rx_Data_RC.ch5; }
    inline int16_t Get_RC_Ch6() const { return LXFC_Rx_Data_RC.ch6; }
    inline int16_t Get_RC_Ch7() const { return LXFC_Rx_Data_RC.ch7; }
    inline int16_t Get_RC_Ch8() const { return LXFC_Rx_Data_RC.ch8; }
    inline int16_t Get_RC_Ch9() const { return LXFC_Rx_Data_RC.ch9; }
    inline int16_t Get_RC_Ch10() const { return LXFC_Rx_Data_RC.ch10; }

    // ==================== 0x51 光流 Mode0 ====================
    inline uint8_t Get_OpticalFlow_Mode0_Mode() const { return LXFC_Rx_Data_OpticalFlow_Mode0.mode; }
    inline uint8_t Get_OpticalFlow_Mode0_State() const { return LXFC_Rx_Data_OpticalFlow_Mode0.state; }
    inline int8_t Get_OpticalFlow_Mode0_DX() const { return LXFC_Rx_Data_OpticalFlow_Mode0.dx_0; }
    inline int8_t Get_OpticalFlow_Mode0_DY() const { return LXFC_Rx_Data_OpticalFlow_Mode0.dy_0; }
    inline uint8_t Get_OpticalFlow_Mode0_Quality() const { return LXFC_Rx_Data_OpticalFlow_Mode0.quality; }

    // ==================== 0x51 光流 Mode1 ====================
    inline uint8_t Get_OpticalFlow_Mode1_Mode() const { return LXFC_Rx_Data_OpticalFlow_Mode1.mode; }
    inline uint8_t Get_OpticalFlow_Mode1_State() const { return LXFC_Rx_Data_OpticalFlow_Mode1.state; }
    inline int16_t Get_OpticalFlow_Mode1_DX() const { return LXFC_Rx_Data_OpticalFlow_Mode1.dx_1; }
    inline int16_t Get_OpticalFlow_Mode1_DY() const { return LXFC_Rx_Data_OpticalFlow_Mode1.dy_1; }
    inline uint8_t Get_OpticalFlow_Mode1_Quality() const { return LXFC_Rx_Data_OpticalFlow_Mode1.quality; }

    // ==================== 0x51 光流 Mode2 ====================
    inline uint8_t Get_OpticalFlow_Mode2_Mode() const { return LXFC_Rx_Data_OpticalFlow_Mode2.mode; }
    inline uint8_t Get_OpticalFlow_Mode2_State() const { return LXFC_Rx_Data_OpticalFlow_Mode2.state; }
    inline int16_t Get_OpticalFlow_Mode2_DX() const { return LXFC_Rx_Data_OpticalFlow_Mode2.dx_2; }
    inline int16_t Get_OpticalFlow_Mode2_DY() const { return LXFC_Rx_Data_OpticalFlow_Mode2.dy_2; }
    inline int16_t Get_OpticalFlow_Mode2_DX_Fix() const { return LXFC_Rx_Data_OpticalFlow_Mode2.dx_fix; }
    inline int16_t Get_OpticalFlow_Mode2_DY_Fix() const { return LXFC_Rx_Data_OpticalFlow_Mode2.dy_fix; }
    inline int16_t Get_OpticalFlow_Mode2_IntegX() const { return LXFC_Rx_Data_OpticalFlow_Mode2.integ_x; }
    inline int16_t Get_OpticalFlow_Mode2_IntegY() const { return LXFC_Rx_Data_OpticalFlow_Mode2.integ_y; }
    inline uint8_t Get_OpticalFlow_Mode2_Quality() const { return LXFC_Rx_Data_OpticalFlow_Mode2.quality; }

    // ==================== 0x60 航点读取 ====================
    inline uint8_t Get_Waypoint_Read_Num() const { return LXFC_Rx_Data_WaypointRead.num; }

    // ==================== 0x61 航点写入/读取返回 ====================
    inline uint8_t Get_Waypoint_Num() const { return LXFC_Rx_Data_WaypointWrite.num; }
    inline int32_t Get_Waypoint_Lng() const { return LXFC_Rx_Data_WaypointWrite.lng; }
    inline int32_t Get_Waypoint_Lat() const { return LXFC_Rx_Data_WaypointWrite.lat; }
    inline int32_t Get_Waypoint_Alt() const { return LXFC_Rx_Data_WaypointWrite.alt; }
    inline uint16_t Get_Waypoint_Spd() const { return LXFC_Rx_Data_WaypointWrite.spd; }
    inline uint16_t Get_Waypoint_Yaw() const { return LXFC_Rx_Data_WaypointWrite.yaw; }
    inline uint8_t Get_Waypoint_Fun() const { return LXFC_Rx_Data_WaypointWrite.fun; }
    inline uint8_t Get_Waypoint_Cmd1() const { return LXFC_Rx_Data_WaypointWrite.cmd1; }
    inline uint8_t Get_Waypoint_Cmd2() const { return LXFC_Rx_Data_WaypointWrite.cmd2; }
    inline uint8_t Get_Waypoint_Cmd3() const { return LXFC_Rx_Data_WaypointWrite.cmd3; }
    inline uint8_t Get_Waypoint_Cmd4() const { return LXFC_Rx_Data_WaypointWrite.cmd4; }

protected:

    Struct_UART_Manage_Object *UART_Manage_Object;
    uint8_t Frame_Header;

    struct Struct_LXFC_Rx_Data_IMU LXFC_Rx_Data_IMU;
    struct Struct_LXFC_Rx_Data_MagBaroTemp LXFC_Rx_Data_MagBaroTemp;
    struct Struct_LXFC_Rx_Data_AttitudeEuler LXFC_Rx_Data_AttitudeEuler;
    struct Struct_LXFC_Rx_Data_AttitudeQuaternion LXFC_Rx_Data_AttitudeQuaternion;
    struct Struct_LXFC_Rx_Data_Height LXFC_Rx_Data_Height;
    struct Struct_LXFC_Rx_Data_FlightMode LXFC_Rx_Data_FlightMode;
    struct Struct_LXFC_Rx_Data_Speed LXFC_Rx_Data_Speed;
    struct Struct_LXFC_Rx_Data_PositionOffset LXFC_Rx_Data_PositionOffset;
    struct Struct_LXFC_Rx_Data_WindEstimate LXFC_Rx_Data_WindEstimate;
    struct Struct_LXFC_Rx_Data_TargetAttitude LXFC_Rx_Data_TargetAttitude;
    struct Struct_LXFC_Rx_Data_TargetSpeed LXFC_Rx_Data_TargetSpeed;
    struct Struct_LXFC_Rx_Data_ReturnHome LXFC_Rx_Data_ReturnHome;
    struct Struct_LXFC_Rx_Data_VoltageCurrent LXFC_Rx_Data_VoltageCurrent;
    struct Struct_LXFC_Rx_Data_ExternalModule LXFC_Rx_Data_ExternalModule;
    struct Struct_LXFC_Rx_Data_RGBBrightness LXFC_Rx_Data_RGBBrightness;
    struct Struct_LXFC_Rx_Data_LogString LXFC_Rx_Data_LogString;
    struct Struct_LXFC_Rx_Data_LogStringValue LXFC_Rx_Data_LogStringValue;
    struct Struct_LXFC_Rx_Data_PWMOutput LXFC_Rx_Data_PWMOutput;
    struct Struct_LXFC_Rx_Data_AttitudeControl LXFC_Rx_Data_AttitudeControl;
    struct Struct_LXFC_Rx_Data_GPSInfo LXFC_Rx_Data_GPSInfo;
    struct Struct_LXFC_Rx_Data_PositionSensor LXFC_Rx_Data_PositionSensor;
    struct Struct_LXFC_Rx_Data_SpeedSensor LXFC_Rx_Data_SpeedSensor;
    struct Struct_LXFC_Rx_Data_RangeSensor LXFC_Rx_Data_RangeSensor;
    struct Struct_LXFC_Rx_Data_RC LXFC_Rx_Data_RC;
    struct Struct_LXFC_Rx_Data_OpticalFlow_Mode0 LXFC_Rx_Data_OpticalFlow_Mode0;
    struct Struct_LXFC_Rx_Data_OpticalFlow_Mode1 LXFC_Rx_Data_OpticalFlow_Mode1;
    struct Struct_LXFC_Rx_Data_OpticalFlow_Mode2 LXFC_Rx_Data_OpticalFlow_Mode2;
    struct Struct_LXFC_Rx_Data_WaypointRead LXFC_Rx_Data_WaypointRead;
    struct Struct_LXFC_Rx_Data_WaypointWrite LXFC_Rx_Data_WaypointWrite;
};

#endif //ANO_H743_DVC_LXFLIGHTCONTROLLER_H