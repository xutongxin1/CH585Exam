/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        : 串口1收发演示
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "CH58x_common.h"
#include "oled.h"
#include "stdio.h"
#include "TJCScreen.h"
#include "WS2812.h"
uint8_t TxBuff[] = "Hello\r\n";
extern float distance;
#define B1 GPIOB_ReadPortPin(GPIO_Pin_16)
#define B2 GPIOB_ReadPortPin(GPIO_Pin_17)
#define B3 GPIOB_ReadPortPin(GPIO_Pin_8)
#define B4 GPIOB_ReadPortPin(GPIO_Pin_9)
uint8_t ScanKey() {
    if (B1 == 0) {
        mDelaymS(10);
        if (B1 == 0) {
            return 1;
        }
    }
    if (B2 == 0) {
        mDelaymS(10);
        if (B2 == 0) {
            return 2;
        }
    }
    if (B3 == 0) {
        mDelaymS(10);
        if (B3 == 0) {
            return 3;
        }
    }
    if (B4 == 0) {
        mDelaymS(10);
        if (B4 == 0) {
            return 4;
        }
    }
    return 0;
}
uint8_t nowKey = 0, lastKey = 0;
/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @return  none
 */
int main() {
    uint8_t len;

    HSECFG_Capacitance(HSECap_18p);
    SetSysClock(CLK_SOURCE_HSE_PLL_78MHz);

    //uart0串口屏
    GPIOB_SetBits(GPIO_Pin_7);
    GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_PU); // RXD-配置上拉输入
    GPIOB_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
    UART0_DefInit();
    UART0_ByteTrigCfg(UART_7BYTE_TRIG);
    UART0_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART0_IRQn);

    //uart1超声波
    GPIOB_SetBits(GPIO_Pin_13);
    GPIOPinRemap(ENABLE, RB_PIN_UART1);
    GPIOB_ModeCfg(GPIO_Pin_12, GPIO_ModeIN_PU); // RXD-配置上拉输入
    GPIOB_ModeCfg(GPIO_Pin_13, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
    UART1_DefInit();
    UART1_ByteTrigCfg(UART_7BYTE_TRIG);
    UART1_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART1_IRQn);

    //uart2接口2 printf
    GPIOA_SetBits(GPIO_Pin_7);
    GPIOA_ModeCfg(GPIO_Pin_6, GPIO_ModeIN_PU); // RXD-配置上拉输入
    GPIOA_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
    UART2_DefInit();
    UART2_ByteTrigCfg(UART_7BYTE_TRIG);
    UART2_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART2_IRQn);

    //uart3接口3
    GPIOB_SetBits(GPIO_Pin_21);
    GPIOPinRemap(ENABLE, RB_PIN_UART3);
    GPIOB_ModeCfg(GPIO_Pin_20, GPIO_ModeIN_PU); // RXD-配置上拉输入
    GPIOB_ModeCfg(GPIO_Pin_21, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
    UART3_DefInit();
    UART3_ByteTrigCfg(UART_7BYTE_TRIG);
    UART3_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART3_IRQn);

    UART2_SendString(TxBuff, sizeof(TxBuff));
    UART3_SendString(TxBuff, sizeof(TxBuff));
    printf("xtxChino!\r\n");
    TJCScreenInit();
    TJCSendTxt("data", "Hello");

    //OLED
    OLED_Init();
    OLED_ColorTurn(0); //0正常显示，1 反色显示
    OLED_DisplayTurn(0); //0正常显示 1 屏幕翻转显示

    //Beep
    GPIOA_ModeCfg(GPIO_Pin_12, GPIO_ModeOut_PP_5mA); // PA12 - PWM4
    PWMX_CLKCfg(78);
    PWMX_16bit_CycleCfg(1516 - 1);
    // PWMX_16bit_ACTOUT(CH_PWM4, 1516/2, Low_Level, ENABLE);

    //RGB
    GPIOB_ResetBits(GPIO_Pin_0);
    GPIOB_ResetBits(GPIO_Pin_1);
    GPIOB_ResetBits(GPIO_Pin_2);
    GPIOB_ModeCfg(GPIO_Pin_0, GPIO_ModeOut_PP_20mA);
    GPIOB_ModeCfg(GPIO_Pin_1, GPIO_ModeOut_PP_20mA);
    GPIOB_ModeCfg(GPIO_Pin_2, GPIO_ModeOut_PP_20mA);

    //key
    GPIOB_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_9, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_17, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_16, GPIO_ModeIN_PU);

    //ws2812
    WS2812Init();

    UART1_SendByte(0xA0);

    while (1) {
        mDelaymS(50);
        if (distance != 0) {
            printf("distance: %f\r\n", distance);
            UART1_SendByte(0xA0);
            distance=0;
        }
        OLED_Display_GB2312_string(0, 0, "12864,带中文字库"); /*在第1页，第1列，显示一串16x16点阵汉字或8x16的ASCII字*/
        nowKey=ScanKey();
        if (nowKey!=lastKey) {
            lastKey=nowKey;
            printf("nowKey = %d\r\n", nowKey);
            if (nowKey==1) {

            }
            else if (nowKey==2) {

            }
            else if (nowKey==3) {

            }
            else if (nowKey==4) {

            }
        }
    }
}
