#include "stm32f10x.h"
#include "uart4.h"

//串口5 PC12 PD2 ,APB1时钟
void uart5_init(u32 bound) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);    //使能UART5，GPIOC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);    //使能UART5，GPIOC时钟

    //UART5_TX   GPIOD.12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PD.12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD.12

    //UART5_RX	  GPIOD.2初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PD2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD.2

    //Usart5 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化VIC寄存器

    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式

    USART_Init(UART5, &USART_InitStructure); //初始化串口5
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启串口接受中断
    USART_Cmd(UART5, ENABLE);                    //使能串口5
}

void u5_printf(char *fmt, ...) {
    int i;
    int len;
    char buffer[50]; //足够容纳才可以，可以搞大点
    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);

    len = strlen(buffer);

    for (i = 0; i < len; i++) {
        while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET); //等待发送完毕
        USART_SendData(UART5, buffer[i]); //发送一个字符
    }
}

void UART5_IRQHandler(void)                    //串口5中断服务程序
{
    u8 Res;
    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res = USART_ReceiveData(UART5);    //读取接收到的数据

        u5_printf("收到了一个字符: %c\r\n", Res);
    }

}
