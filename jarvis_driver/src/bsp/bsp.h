

#ifndef __BSP_H__
#define __BSP_H__

#include "bsp_gpio.h"
#include "bsp_motor.h"
#include "bsp_servo.h"
#include "bsp_timer.h"



/*所有外设宏定义开关，根据需要可以开关所有外设驱动*/

/*定义需要初始化的舵机宏定义开关*/
#define USE_SERVO_J1
//#define USE_SERVO_J2
//#define USE_SERVO_J3
//#define USE_SERVO_J4
//#define USE_SERVO_J5
//#define USE_SERVO_J6

/*巡线开关*/
#define USE_LINE_L1
#define USE_LINE_L2
#define USE_LINE_R1
#define USE_LINE_R2

/*红外避障开关*/
#define USE_IRAVOID_L1
#define USE_IRAVOID_R1

/*七彩探照灯*/
#define USE_COLOR_RED
#define USE_COLOR_GREEN
#define USE_COLOR_BLUE

/*寻光传感器*/
#define USE_LIGHTSEEKING_L1
#define USE_LIGHTSEEKING_R1

/*超声波传感器*/
#define USE_ULTRASONIC

/*蜂鸣器*/
#define USE_BUZZER

/*灭火*/
#define USE_FIRE

/*灰度传感器*/
#define USE_GS


#endif
