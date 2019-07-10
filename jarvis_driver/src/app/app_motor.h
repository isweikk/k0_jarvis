/*
 * @Descripttion: The motion controller
 * @version: v1.0.0
 * @Author: Kevin
 * @Email: kkcoding@qq.com
 * @Date: 2018-01-03 14:39:08
 * @LastEditors: Kevin
 * @LastEditTime: 2019-07-10 16:54:17
 * @Copyright (C): 2016-2020, weikk
 */

#ifndef __APP_MOTOR_H__
#define __APP_MOTOR_H__

enum EmBodySpeed{
    kEmSpeed1 = 1,
    kEmSpeed2,
    kEmSpeed3,
    kEmSpeed4,
    kEmSpeed5,
    kEmSpeed6,
    kEmSpeed7,
    kEmSpeed8,
    kEmSpeed9,
    kEmSpeedMax
};

void MotorInit(void);
void BodyMoveForward(int speed);
void BodyMoveBack(int speed);
void BodyMoveLeft(int speed);
void BodyMoveRight(int speed);
void BodyMoveStop(void);
void BodySpinStop(void);
void BodySpinLeft(int left_speed, int right_speed);
void BodySpinRight(int left_speed, int right_speed);


#endif


