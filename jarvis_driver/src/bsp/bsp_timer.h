/*
 * @Descripttion: The timers
 * @version: v1.0.0
 * @Author: Kevin
 * @Email: kkcoding@qq.com
 * @Date: 2018-01-03 14:39:08
 * @LastEditors: Kevin
 * @LastEditTime: 2019-07-10 16:48:34
 * @Copyright (C): 2016-2020, weikk
 */

#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include "stm32f10x.h"


extern long g_count; /*用来上报数据*/

void Timer1Iint(u16 arr,u16 psc);


#endif

