/*
 *the Peripherals use to communicate data.
 */

#ifndef __PUB_SERIAL_H__
#define __PUB_SERIAL_H__

#include "pub_sys.h"

#define UART1_LINUX_MODE        1
#define UART1_BUFFER_SIZE       256
#define UART1_ECHO   1

#define PUB_LOG(format, ...) printf("> %-4d: "format"\r\n", __LINE__, ##__VA_ARGS__)

extern u8 kUart1RxBuf[UART1_BUFFER_SIZE];
extern u16 kUart1RxFlag;

int Uart1ConsoleStrHdl(const u8 *string);
void Uart1ConsoleTask(void);
void Uart1Init(u32 bound);
void Uart1SendByte(u8 data);

//uart2
void Uart2Init(u32 bound);
void Uart2SendByte(u8 data);
u8 U2GetCmdFlag(void);
void U2ClearCmdFlag(void);
u16 U2GetCmdData(u8 *buf);

//iic func
int I2cWriteSingle(u8 devAdd, u8 regAdd, u8 data);
int I2cWriteMultiple(u8 devAdd, u8 regAdd, u32 len, const u8 *dataBuffer);
int I2cReadSingle(u8 devAdd, u8 regAdd, u8 *data);
int I2cReadMultiple(u8 devAdd, u8 regAdd, u32 len, u8 *dataBuffer);


#endif
