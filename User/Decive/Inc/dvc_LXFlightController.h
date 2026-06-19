//
// Created by minec on 2026/6/19.
//

#ifndef ANO_H743_DVC_LXFLIGHTCONTROLLER_H
#define ANO_H743_DVC_LXFLIGHTCONTROLLER_H

// 匿名通信协议 - 基本信息类帧结构体定义
// 所有结构体采用小端模式，与协议一致

#include <cstdint>

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

class LXFlightController {
    public:
    void Init();
    private:

};

#endif //ANO_H743_DVC_LXFLIGHTCONTROLLER_H