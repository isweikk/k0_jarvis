/*
 * @Descripttion: The motion controller
 * @version: v1.0.0
 * @Author: Kevin
 * @Email: kkcoding@qq.com
 * @Date: 2018-01-03 14:39:08
 * @LastEditors: Kevin
 * @LastEditTime: 2019-07-10 16:50:13
 * @Copyright (C): 2016-2020, weikk
 */

#include "app_motor.h"

#include "pub_sys.h"
#include "bsp_motor.h"

static int kMotorPulseWideMax = 0;  //脉宽最大值
static int kLeftSpeed = 0, kRightSpeed = 0;

#define MOTOR_SPEED2PW(speed) (kMotorPulseWideMax / kEmSpeedMax * speed)

void MotorInit(void)
{
	MotorGpioInit();  				/*电机GPIO初始化*/
	MotorPwmInit(7200, 0, 7200, 0);	/*不分频。PWM频率 72000000/7200=10khz	  */
    kMotorPulseWideMax = 7200;
}



/**
* @brief         小车前进
* @param[in]     speed  （0~7200） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
*/

void BodyMoveForward(int speed)
{
	LeftMotorForward();
	RightMotorForward();
	LeftMotorPWM(MOTOR_SPEED2PW(speed));		  
	RightMotorPWM(MOTOR_SPEED2PW(speed));
    kLeftSpeed = kRightSpeed = speed;	
}

/**
* @brief         小车后退
* @param[in]     speed  （0~7200） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
*/

void BodyMoveBack(int speed)
{
	LeftMotorBack();
	RightMotorBack();

	LeftMotorPWM(MOTOR_SPEED2PW(speed));		  
	RightMotorPWM(MOTOR_SPEED2PW(speed));
    kLeftSpeed = kRightSpeed = speed;
}

/**
* @brief         小车左转
* @param[in]     speed  （0~7200） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
*/

void BodyMoveLeft(int speed)
{
	LeftMotorStop();
	RightMotorForward();

	LeftMotorPWM(0);		  
	RightMotorPWM(MOTOR_SPEED2PW(speed));
    kLeftSpeed = 0;
    kRightSpeed = speed;
}

/**
* @brief         小车右转
* @param[in]     speed  （0~7200） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
*/

void BodyMoveRight(int speed)
{
	LeftMotorForward();
	RightMotorStop();

	LeftMotorPWM(MOTOR_SPEED2PW(speed));		  
	RightMotorPWM(0);
    kLeftSpeed = speed;
    kRightSpeed = 0;	
}

/**
* @brief         小车刹车
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/

void BodyMoveStop(void)
{
	LeftMotorStop();
	RightMotorStop();

	LeftMotorPWM(0);		  
	RightMotorPWM(0);
    kLeftSpeed = kRightSpeed = 0;
}

/**
* @brief         小车左旋
* @param[in]     left_speed：左电机速度  right_speed：右电机速度 取值范围：（0~7200）
* @param[out]    void
* @retval        void
* @par History   无
*/

void BodySpinLeft(int left_speed, int right_speed)
{
	LeftMotorBack();
	RightMotorForward();

	LeftMotorPWM(MOTOR_SPEED2PW(left_speed));		  
	RightMotorPWM(MOTOR_SPEED2PW(right_speed));
    kLeftSpeed = left_speed;
    kRightSpeed = right_speed;
}

/**
* @brief         小车右旋
* @param[in]     left_speed：左电机速度  right_speed：右电机速度 取值范围：（0~7200）
* @param[out]    void
* @retval        void
* @par History   无
*/

void BodySpinRight(int left_speed, int right_speed)
{
	LeftMotorForward();
	RightMotorBack();

	LeftMotorPWM(MOTOR_SPEED2PW(left_speed));		  
	RightMotorPWM(MOTOR_SPEED2PW(right_speed));
    kLeftSpeed = left_speed;
    kRightSpeed = right_speed;
}

void BodyChangeSpeed(int up_down)
{
    if (up_down == 1) {
        if (kLeftSpeed < kEmSpeedMax && kRightSpeed < kEmSpeedMax) {
            kLeftSpeed++;
            kRightSpeed++;
            LeftMotorPWM(MOTOR_SPEED2PW(kLeftSpeed));		  
            RightMotorPWM(MOTOR_SPEED2PW(kRightSpeed));
        }
    } else if (up_down == 2) {
        if (kLeftSpeed > kEmSpeed1 && kRightSpeed > kEmSpeed1) {
            kLeftSpeed--;
            kRightSpeed--;
            LeftMotorPWM(MOTOR_SPEED2PW(kLeftSpeed));		  
            RightMotorPWM(MOTOR_SPEED2PW(kRightSpeed));
        }
    } 
}
