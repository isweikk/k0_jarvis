/*
 * @Descripttion: The main API of parsing protocol
 * @version: v1.0.0
 * @Author: Kevin
 * @Email: kkcoding@qq.com
 * @Date: 2018-01-03 14:39:08
 * @LastEditors: Kevin
 * @LastEditTime: 2019-07-10 16:39:30
 * @Copyright (C): 2016-2020, weikk
 */

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__



typedef struct {
    int type;   //0= unvalid cmd, 1 = common command, 2= key-value
    int mode;
    int spin;
    int steer;
    int gearbox;
    int ptz;
    int color;
}StCmdParam;

int ParseString(const char *str, StCmdParam * prm);

#endif
