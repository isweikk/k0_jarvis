/*
 * @Descripttion: The main function
 * @version: v1.0.0
 * @Author: Kevin
 * @Email: kkcoding@qq.com
 * @Date: 2018-01-03 14:39:08
 * @LastEditors: Kevin
 * @LastEditTime: 2019-07-10 16:57:28
 * @Copyright (C): 2016-2020, weikk
 */

 
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f10x.h"

#include "typedef.h"
#include "pub_sys.h"
#include "pub_serial.h"
#include "bsp.h"
#include "protocol.h"
#include "app_motor.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
enum {
    kEmRotoModeCtrl = 11,
    kEmRotoModeOther = 21,
}EmRobotMode;

void BspInit(void);

static int kRobotMode = kEmRotoModeCtrl;
static int kRobotMoveSpeed = kEmSpeed3;

void RobotChangeSpeed(int up_down)
{
    if (up_down == 1) {
        if (kRobotMoveSpeed < kEmSpeedMax) {
            kRobotMoveSpeed++;
        }
    } else if (up_down == 2) {
        if (kRobotMoveSpeed > kEmSpeed1) {
            kRobotMoveSpeed--;
        }
    } 
}


int U2CmdHandleTask(void){
    u8 buf[128];
    u16 cmd_len;
    StCmdParam prm = {0};
    
    cmd_len = U2GetCmdData(buf);
    if (cmd_len > 0) {
        ParseString((const char*)buf, &prm);
        PUB_LOG("MODE=%d, spin=%d, steer=%d, ptz=%d", prm.mode, prm.spin, prm.steer, prm.ptz);
        if (prm.type == 2) {
            if (prm.mode > 0 && prm.mode != kRobotMode) {
                BodyMoveStop();
                switch (prm.mode) {
                    case kEmRotoModeCtrl:
                        kRobotMode = kEmRotoModeCtrl;
                        //ModeBEEP(0);
                        break;
                    default: break;
                }
                return EVN_OK;
            }
        } else if (prm.type == 1) {
            if (prm.gearbox > 0) {
                RobotChangeSpeed(prm.gearbox);
                BodyChangeSpeed(prm.gearbox);
            } else if (prm.spin == 1) {
                BodySpinLeft(kRobotMoveSpeed, kRobotMoveSpeed);
            } else if (prm.spin == 2) {
                BodySpinRight(kRobotMoveSpeed, kRobotMoveSpeed);
            } else if (prm.steer == 1) {
                BodyMoveForward(kRobotMoveSpeed);
            } else if (prm.steer == 2) {
                BodyMoveBack(kRobotMoveSpeed);
            } else if (prm.steer == 3) {
                BodyMoveLeft(kRobotMoveSpeed);
            } else if (prm.steer == 4) {
                BodyMoveRight(kRobotMoveSpeed);
            } else {
                BodyMoveStop();
            }
        }
        
    }
    return EVN_OK;
}


int main(void) {    
	SysConfig();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                     RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                     RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF, ENABLE);
	DelayInit();
    Uart1Init(9600);
    Uart2Init(9600);

    BspInit();
    MotorInit();
    PUB_LOG("Init ok!\r\n");
    
	while (1) {
        U2CmdHandleTask();
        Uart1ConsoleTask();
        //MsDelay(5);
	}
}

/**
* @brief         硬件设备初始化
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/
void BspInit(void)
{
	ServoInit();				    /*舵机接口GPIO初始化*/	
	Timer1Iint(9, 72);				/*100Khz的计数频率，计数到10为10us  */ 
    
//	Colorful_GPIO_Init();				/*七彩探照灯*/
//	Colorful_PWM_Init(1000,0, 1000,0, 1000,0); /*RGB PWM*/
//	LineWalking_GPIO_Init();			/*巡线传感器GPIO初始化*/
//	IRAvoid_GPIO_Init();				/*红外避障传感器GPIO初始化*/
//	LightSeeking_GPIO_Init();			/*寻光GPIO初始化*/
//	Ultrasonic_GPIO_Init();				/*超声波GPIO初始化*/
//	bsp_Ultrasonic_Timer2_Init();		/*超声波TIM2初始化*/
//	//Uart1_init(9600);					/*调试接口串口 */
//	Uart2_init(9600);					/*协议处理接口 */
//	Adc_Init();	   						/*电压测量ADC初始化*/
//	Buzzer_GPIO_Init();					/*蜂鸣器GPIO初始化*/
//	Buzzer_Control(OFF); 				/*先关闭蜂鸣器*/
//	Fire_GPIO_Init();					/*灭火GPIO初始化*/
//	Fire_Control(OFF);				    /*灭火关闭*/
//	bsp_GS_Init();						/*灰度传感器初始化*/
//	Angle_J1 = 90;						/*初始化舵机云台到向前*/
//	printf("init ok!");
}


/******************* (C) COPYRIGHT 2016 weikk **** END OF FILE ***************/
