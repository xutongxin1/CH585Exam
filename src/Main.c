/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        : ����1�շ���ʾ
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "CH58x_common.h"
#include "oled.h"
#include "stdio.h"
#include "TJCScreen.h"
uint8_t TxBuff[] = "Hello\r\n";
extern float distance;
unsigned char lei1[]={/*--  ����:  ?  --*/
    /*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
    0x80,0x80,0x80,0xBF,0xA5,0xA5,0xA5,0x3F,0xA5,0xA5,0xA5,0xBF,0x80,0x80,0x80,0x00,
    0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00,0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00};

/*********************************************************************
 * @fn      main
 *
 * @brief   ������
 *
 * @return  none
 */
int main()
{
    uint8_t len;

    HSECFG_Capacitance(HSECap_18p);
    SetSysClock(CLK_SOURCE_HSE_PLL_78MHz);

    //uart0������
    GPIOB_SetBits(GPIO_Pin_7);
    GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_PU);      // RXD-������������
    GPIOB_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA); // TXD-�������������ע������IO������ߵ�ƽ
    UART0_DefInit();
    UART0_ByteTrigCfg(UART_7BYTE_TRIG);
    UART0_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART0_IRQn);

    //uart1������
    GPIOB_SetBits(GPIO_Pin_13);
    GPIOPinRemap(ENABLE, RB_PIN_UART1);
    GPIOB_ModeCfg(GPIO_Pin_12, GPIO_ModeIN_PU);      // RXD-������������
    GPIOB_ModeCfg(GPIO_Pin_13, GPIO_ModeOut_PP_5mA); // TXD-�������������ע������IO������ߵ�ƽ
    UART1_DefInit();
    UART1_ByteTrigCfg(UART_7BYTE_TRIG);
    UART1_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART1_IRQn);

    //uart2�ӿ�2 printf
    GPIOA_SetBits(GPIO_Pin_7);
    GPIOA_ModeCfg(GPIO_Pin_6, GPIO_ModeIN_PU);      // RXD-������������
    GPIOA_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA); // TXD-�������������ע������IO������ߵ�ƽ
    UART2_DefInit();
    UART2_ByteTrigCfg(UART_7BYTE_TRIG);
    UART2_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART2_IRQn);

    //uart3�ӿ�3
    GPIOB_SetBits(GPIO_Pin_21);
    GPIOPinRemap(ENABLE, RB_PIN_UART3);
    GPIOB_ModeCfg(GPIO_Pin_20, GPIO_ModeIN_PU);      // RXD-������������
    GPIOB_ModeCfg(GPIO_Pin_21, GPIO_ModeOut_PP_5mA); // TXD-�������������ע������IO������ߵ�ƽ
    UART3_DefInit();
    UART3_ByteTrigCfg(UART_7BYTE_TRIG);
    UART3_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART3_IRQn);

    UART2_SendString(TxBuff, sizeof(TxBuff));
    UART3_SendString(TxBuff, sizeof(TxBuff));
    printf("xtxChino!\r\n");
    TJCScreenInit();
    TJCSendTxt("data","Hello");

    OLED_Init();
    OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ

    GPIOA_ModeCfg(GPIO_Pin_12, GPIO_ModeOut_PP_5mA); // PA12 - PWM4
    PWMX_CLKCfg(78);
    PWMX_16bit_CycleCfg(1516-1);
    PWMX_16bit_ACTOUT(CH_PWM4, 1516/2, Low_Level, ENABLE);
    while(1)
    {
        distance=0;
        UART1_SendByte(0xA0);
        mDelaymS(200);
        if (distance!=0) {
            printf("distance: %f\r\n",distance);
        }
        mDelaymS(800);
        OLED_Display_GB2312_string(0,0,"12864,�������ֿ�");	/*�ڵ�1ҳ����1�У���ʾһ��16x16�����ֻ�8x16��ASCII��*/
        // OLED_Display_16x16(113,6,lei1);
    }
}


