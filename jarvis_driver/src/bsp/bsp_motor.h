/*
 * @Descripttion: The driver of the motors
 * @version: v1.0.0
 * @Author: Kevin
 * @Email: kkcoding@qq.com
 * @Date: 2018-01-03 14:39:08
 * @LastEditors: Kevin
 * @LastEditTime: 2019-07-10 16:44:49
 * @Copyright (C): 2016-2020, weikk
 */

#ifndef __BSP_MOTOR_H__
#define __BSP_MOTOR_H__

#include "stm32f10x.h"




void MotorGpioInit(void);
void MotorPwmInit(u16 arr, u16 psc, u16 arr2, u16 psc2);
void LeftMotorForward(void);
void LeftMotorBack(void);
void LeftMotorStop(void);
void RightMotorForward(void);
void RightMotorBack(void);
void RightMotorStop(void);
void LeftMotorPWM(int pw);
void RightMotorPWM(int pw);
void BodyChangeSpeed(int up_down);

#endif
