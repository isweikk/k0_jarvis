/*
 * @Descripttion: The motion controller
 * @version: v1.0.0
 * @Author: Kevin
 * @Email: kkcoding@qq.com
 * @Date: 2018-01-3 14:39:08
 * @LastEditors: Kevin
 * @LastEditTime: 2019-07-10 16:57:06
 * @Copyright (C): 2016-2020, weikk
 */
/*
 *the Peripherals use to communicate data.
 */
#include "pub_serial.h"
#include <stdlib.h>
#include <string.h>


#define Uart1PrintUsage(a,b) PUB_LOG("usage:%s %s", a, b)
typedef struct {
    u8 num;
    u8 cmd[16];
    u8 info[48];
    u8 prm_num;
}StConsoleCmd;

const StConsoleCmd kConsoleCmdTab[] = {
    {0, "help", "[cmd][parm1][parm1]...", 1},
    
};

u8 kUart1RxBuf[UART1_BUFFER_SIZE];    //
//recieve status
//bit15,	recieve ok flag,0x0d
//bit14,	recieve 0x0a
//bit13~0,	the data length
u16 kUart1RxFlag = 0;
u16 kUart1EchoPtr = 0;

int String2Int(u8 *string) {
    u32 val = 0;
    u8  ptr = 0;
    while (1) {
        if (*(string + ptr) == NULL) {
            return val;
        } else if ((*(string + ptr) >= '0') && (*(string + ptr) <= '9')) {
            val *= 10;
            val += (*(string + ptr)) - '0';
            ptr++;
        } else {
            return EVN_ERROR;
        }
    }
}

int Uart1ConsoleStrHdl(const u8 *string)
{
    u8 cmd_num = 0, cmd_ptr = 0;
    u8 is_cmd_ok = 0;
    u8 prm_num, prm_list[5][12] = {0}; //5 parameter, 11 byte mostest int every one.
    u8 i;
    
    if ((*string < 'a') || (*string > 'z'))  //limit the first word.
        return EVN_ERROR;
    for (; cmd_num < 100; cmd_num++) {
        if (*string != kConsoleCmdTab[cmd_num].cmd[0]) {
            continue;
        } else {
            if ((strncmp((char *)string, (char *)kConsoleCmdTab[cmd_num].cmd, strlen((char *)kConsoleCmdTab[cmd_num].cmd)) == NULL)) {
                if ((*(string + strlen((char *)kConsoleCmdTab[cmd_num].cmd)) != ' ')  //only partial same. not right
                    &&(*(string + strlen((char *)kConsoleCmdTab[cmd_num].cmd)) != NULL)) {
                    continue;
                } else {
                    is_cmd_ok = 1;
                    break;
                }
            } else {
                continue;
            }
        }
    }
    if (is_cmd_ok == 0)
        return EVN_ERROR;
    cmd_ptr += strlen((char *)kConsoleCmdTab[cmd_num].cmd);
    for (; prm_num < 5; ) {
        while (*(string + cmd_ptr) == ' '){   //ignore ' '
            cmd_ptr++;
        }
        i = 0;
        while ((*(string + cmd_ptr) != ' ') && (*(string + cmd_ptr) != NULL)) {
            prm_list[prm_num][i++] = *(string + cmd_ptr);
            cmd_ptr++;
            if (i > 12) {
                Uart1PrintUsage(kConsoleCmdTab[cmd_num].cmd, kConsoleCmdTab[cmd_num].info);
                return EVN_ERROR;
            }
        }
        if (i > 0)
            prm_num++;
        if (*(string + cmd_ptr) == NULL)
            break;
    }
    if (prm_num != kConsoleCmdTab[cmd_num].prm_num) {
        Uart1PrintUsage(kConsoleCmdTab[cmd_num].cmd, kConsoleCmdTab[cmd_num].info);
        return EVN_ERROR;
    }
    
    switch (cmd_num) {
        case 0: 
            //tmp_m0 = String2Int(prm_list[0]);
            //MotorSetPos(0, tmp_m0);
            break;
        case 1:
            break;
        default: break;
    }
    
    return EVN_OK;
}

void Uart1ConsoleTask(void) {                       
    if (kUart1EchoPtr < (kUart1RxFlag & 0x3fff)) {   //have not echoed over.
        for (; kUart1EchoPtr < (kUart1RxFlag & 0x3fff); kUart1EchoPtr++) {
            Uart1SendByte(kUart1RxBuf[kUart1EchoPtr]);
        }
    } else if ((kUart1RxFlag & 0x8000) > 0) {  //receive ok
        printf("\n");
        if(Uart1ConsoleStrHdl(kUart1RxBuf) == EVN_OK) {
            printf(" <ok>\n");
        } else if ((kUart1RxFlag & 0x3fff) > 0){
            printf(" <error>\n");
        }
        printf("stm32: ");
        kUart1RxFlag = 0;
        kUart1EchoPtr = 0;
    }
}

void Uart1Init(u32 bound) {
   //GPIO config
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//enable clock of USART1
    USART_DeInit(USART1);
	
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   //USART1 config
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//stop 1bit
	USART_InitStructure.USART_Parity = USART_Parity_No;//no parity
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//none hardware control
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

/*为确保没有从 C 库链接使用半主机的函数，因为不使用半主机，标准C库stdio.h中
有些使用半主机的函数要重新写,您必须为这些函数提供自己的实现*/
//support for using the function of "printf",then no need to use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)    //确保没有从C库链接使用半主机的函数
struct __FILE {   //标准库需要的支持函数
	int handle; 
}; 

FILE __stdout;       
int _sys_exit(int x) {    //避免使用半主机模式
	x = x;
	return 0;
} 
//modifing define the fputc
int fputc(int ch, FILE *f) { 	
    Uart1SendByte(ch);
	return ch;
}
#endif

void USART1_IRQHandler(void) {
	u8 recv_byte;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {  //数据必须是0x0d 0x0a结尾，linux以0x0a结尾
        recv_byte = USART_ReceiveData(USART1);//(USART1->DR)
        //kUart1RxBuf[kUart1RxFlag & 0X3FFF] = recv_byte ;
        //kUart1RxFlag++;
        if (recv_byte == 0x08) { //backspace
            if ((kUart1RxFlag & 0x3FFF) > 0) {  //receive one byte at least.
                kUart1RxFlag--;
                kUart1EchoPtr--;
                kUart1RxBuf[kUart1RxFlag & 0X3FFF] = 0;
                Uart1SendByte(0x08);
            }
            //goto USART1_IRQ_END;
        } else if ((recv_byte == 0x0D) || (recv_byte == 0x0A) || ((recv_byte >= ' ') && (recv_byte <= '~'))) {
            //兼容linux以\n结尾，Windows以\r\n结尾，\r和\n都不会放入buffer中。
            if ((kUart1RxFlag & 0x8000) == 0) {  //接收未完成
                if (kUart1RxFlag & 0x4000) { //接收到了0x0d
                    if (recv_byte != 0x0a) {
                        kUart1RxFlag = 0;//接收错误,重新开始
                    } else {
                        kUart1RxBuf[kUart1RxFlag & 0X3FFF] = 0;    //end char
                        kUart1RxFlag |= 0x8000;	//接收完成了 
                    }
                } else {
                    //还没收到0X0D
                    if (recv_byte == 0x0d) {
                        kUart1RxFlag |= 0x4000;  //当前收到0X0D
                    }else if(recv_byte == 0x0a) {
                        kUart1RxBuf[kUart1RxFlag & 0X3FFF] = 0;    //end char
                        kUart1RxFlag |= 0x8000;  //收到0a，完成接收
                    } else {
                        kUart1RxBuf[kUart1RxFlag & 0X3FFF] = recv_byte ;
                        kUart1RxFlag++;
                        if (kUart1RxFlag > (UART1_BUFFER_SIZE-1)) { //超出最大size
                            kUart1RxFlag = 0;
                        }
                    }
                }            
            }
        }
    } 
}

void Uart1SendByte(u8 data) {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);    //TX BUFFER empty,if no this line,we will lost first char
    USART_SendData(USART1, data);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); //tx over,if use TXE,we will lost last char.
}

//uart2
#define U2_RX_LEN    128
u8 kNewCmdFlag = 0; //cmd is ok
u16 kU2RxStat = 0;  //receive status
u8 kU2RxBuf[U2_RX_LEN] = {0};
u16 kU2RxLen = 0;

void Uart2Init(u32 bound)
{
  	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	 
	//USART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2
	
	//USART2_RX	  GPIOA.3初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.3 
	
	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	//USART 初始化设置
	
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	
	USART_Init(USART2, &USART_InitStructure); //初始化串口2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接收中断
	USART_Cmd(USART2, ENABLE);                    //使能串口2

}

void Uart2SendByte(u8 data) {
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);    //TX BUFFER empty,if no this line,we will lost first char
    USART_SendData(USART2, data);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); //tx over,if use TXE,we will lost last char.
}

/**
* @brief         串口1中断服务程序
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/
void USART2_IRQHandler(void) {
    static u8 start_flag = 0;
    static u16 tmp_len = 0;
	u8 data = 0;

	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET) {//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断  
		USART_ClearFlag(USART2, USART_FLAG_ORE); //读SR其实就是清除标志
       	USART_ReceiveData(USART2);
    }
		
	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET) {
    	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		data = USART_ReceiveData(USART2);
        if (start_flag == 0) {
            if (data == '$') {
                start_flag = 1;
                kU2RxBuf[0] = data;
                tmp_len = 1;
            }
	    } else {
            kU2RxBuf[tmp_len] = data;
            tmp_len++;
            if (data == '#') {
                kNewCmdFlag = 1; 
                start_flag = 0;
                kU2RxLen = tmp_len;	
                kU2RxBuf[tmp_len] = '\0';
                tmp_len = 0;
            }
	    }
	    
		if (tmp_len >= U2_RX_LEN) {
			tmp_len = 0;
			start_flag = 0;
            kNewCmdFlag = 0;
		}
	}
}

u8 U2GetCmdFlag(void){
    return kNewCmdFlag;
}

void U2ClearCmdFlag(void){
    kNewCmdFlag = 0;
}

u16 U2GetCmdData(u8 *buf) {
    if (kNewCmdFlag) {
        memcpy(buf, kU2RxBuf, kU2RxLen + 1);
        kNewCmdFlag = 0;
        return kU2RxLen;
    }
    return 0;
}

/**************************** iic driver *****************************/
//SDA direction:
#define I2C_SDA_IN()   {GPIOB->CRL &= ~(3<<(7*2));}	//PB7输入模式
#define I2C_SDA_OUT()  {GPIOB->CRL = (GPIOB->CRL & (~(3<<(7*2)))) | (1<<7*2);} //PB7输出模式
#define I2C_SCL_SET(a) {if(a==0) GPIOB->BRR = GPIO_Pin_6; \
                    	else GPIOB->BSRR = GPIO_Pin_6;}
#define I2C_SDA_SET(a) {if(a==0) GPIOB->BRR = GPIO_Pin_7; \
                    	else GPIOB->BSRR = GPIO_Pin_7;}
#define I2C_SDA_GET  (GPIOB->IDR & GPIO_Pin_7)
#define I2C_DELAY(a)    {UsDelay(2*a);}


void I2cInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	//GPIOB7 - SDA, B6 - SCL  初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
}

void I2cStart(void)
{
	I2C_SDA_OUT();
	I2C_SDA_SET(1);
	I2C_SCL_SET(1);
	I2C_DELAY(1);
	I2C_SDA_SET(0);
	I2C_DELAY(1);
	I2C_SCL_SET(0);
	I2C_DELAY(1);
}

void I2cStop(void)
{
	I2C_SDA_OUT();
	I2C_SDA_SET(0);
	I2C_SCL_SET(1);
	I2C_DELAY(1);
	I2C_SDA_SET(1);
	I2C_DELAY(1);
	I2C_SCL_SET(0);
	I2C_DELAY(1);
}

//ack:1=ACK,0=NACK
void I2cSendAck(u8 ack)
{
	I2C_SDA_OUT();
	if(ack){
		I2C_SDA_SET(0);
	}else{
		I2C_SDA_SET(1);
    }
	I2C_DELAY(1);
	I2C_SCL_SET(1);
	I2C_DELAY(1);
	I2C_SCL_SET(0);
	I2C_DELAY(1);
}

//wait ack
//return :EVN_OK=recieve ack TRUE, EVN_ERROR=recieve ack false
int I2cWaitAck(void)
{
	u16 waitTimeout = 0;

	I2C_SDA_IN();
	I2C_SDA_SET(1);	//no this line,we can't get the data
	I2C_DELAY(1);
	I2C_SCL_SET(1);
	I2C_DELAY(1); 
	while(I2C_SDA_GET)
	{
		waitTimeout++;
		if(waitTimeout > 250)
		{
			I2C_SCL_SET(0);
			I2C_DELAY(1);
			I2cStop();
			return EVN_ERROR;
		}
	}
	I2C_SCL_SET(0);
	I2C_DELAY(1); 
	return EVN_OK;
}

void I2cSendByte(u8 data)
{                        
    u8 cnt = 0;   
	I2C_SDA_OUT();
    for(cnt=0; cnt<8; cnt++)
    {
        I2C_SDA_SET((data & 0x80)>>7);
		I2C_DELAY(1);
		I2C_SCL_SET(1);
		I2C_DELAY(1);
		data <<= 1;
		I2C_SCL_SET(0);	//set scl free.
    	I2C_DELAY(1);
    }	 
} 

//recieve one byte ,ack:0=NACK,1=ACK
//get data from high bit to low bit
u8 I2cRecvByte(u8 ack)
{
	u8 cnt = 0, recvData = 0;

	I2C_SDA_IN();
	I2C_SDA_SET(1);	//no this line,we can't get the data
	for(cnt=0; cnt<8; cnt++)
	{
		I2C_DELAY(1);
		I2C_SCL_SET(1);
		I2C_DELAY(1);
		recvData <<= 1;
		if(I2C_SDA_GET)
			recvData++;
		I2C_SCL_SET(0);
		I2C_DELAY(1);
	}
	I2cSendAck(ack);
	return recvData;
}

int I2cWriteSingle(u8 devAdd, u8 regAdd, u8 data)
{
    I2cStart();
    I2cSendByte((devAdd<<1) | 0x00);  //send device address + write
    if(I2cWaitAck() != EVN_OK)
        return EVN_ERROR;
    
    I2cSendByte(regAdd);              //send register address
    if(I2cWaitAck() != EVN_OK)
        return EVN_ERROR;
    
    I2cSendByte(data);  //send device address + read
    if(I2cWaitAck() != EVN_OK)
        return EVN_ERROR;
    I2cStop();
    
    return  EVN_OK;
}

int I2cWriteMultiple(u8 devAdd, u8 regAdd, u32 len, const u8 *dataBuffer)
{
    u32 tmpCnt = 0;
    
    I2cStart();
    I2cSendByte((devAdd<<1) | 0x00);  //send device address + write
    if(I2cWaitAck() != EVN_OK)
        return EVN_ERROR;
    
    I2cSendByte(regAdd);              //send register address
    if(I2cWaitAck() != EVN_OK)
        return EVN_ERROR;
    
    for(tmpCnt=0; tmpCnt< len; tmpCnt++)
    {
    	I2cSendByte(dataBuffer[tmpCnt]);  //send device address + read
	    if(I2cWaitAck() != EVN_OK)
	        return EVN_ERROR;
	}
    I2cStop();
    
    return  EVN_OK;
}

int I2cReadSingle(u8 devAdd, u8 regAdd, u8 *data)
{
    I2cStart();
    I2cSendByte((devAdd<<1) | 0x00);  //send device address + write
    if(I2cWaitAck() != EVN_OK)
        return EVN_ERROR;
    
    I2cSendByte(regAdd);              //send register address
    if(I2cWaitAck() != EVN_OK)
        return EVN_ERROR;
    
    I2cStart();
    I2cSendByte((devAdd<<1) | 0x01);  //send device address + read
    if(I2cWaitAck() != EVN_OK)
        return EVN_ERROR;

    *data = I2cRecvByte(0);
    I2cStop();
    
    return  EVN_OK;
}

int I2cReadMultiple(u8 devAdd, u8 regAdd, u32 len, u8 *dataBuffer)
{
	u32 tmpCnt = 0;

    I2cStart();
    I2cSendByte((devAdd<<1) | 0x00);  //send device address + write
    if(I2cWaitAck() != EVN_OK)
        return EVN_ERROR;
    
    I2cSendByte(regAdd);              //send register address
    if(I2cWaitAck() != EVN_OK)
        return EVN_ERROR;
    
    I2cStart();
    I2cSendByte((devAdd<<1) | 0x01);  //send device address + read
    if(I2cWaitAck() != EVN_OK)
        return EVN_ERROR;

    for(tmpCnt=0; tmpCnt< len-1; tmpCnt++)
    {
    	dataBuffer[tmpCnt] = I2cRecvByte(1);
    }
    dataBuffer[tmpCnt] = I2cRecvByte(0);
    I2cStop();
    
    return  EVN_OK;
}

/**************************** iic end ********************************/

