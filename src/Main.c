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

#include "beep.h"
#include "CH58x_common.h"
#include "led.h"
#include "oled.h"
#include "stdio.h"
#include "TJCScreen.h"
#include "WS2812.h"
uint8_t TxBuff[] = "Hello\r\n";
extern float distance;
#define B1 GPIOB_ReadPortPin (GPIO_Pin_16)
#define B2 GPIOB_ReadPortPin (GPIO_Pin_17)
#define B3 GPIOB_ReadPortPin (GPIO_Pin_8)
#define B4 GPIOB_ReadPortPin (GPIO_Pin_9)

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

bool isWork = 0;
uint8_t nowKey = 0, lastKey = 0;
// 全局变量，存储滤波结果
float filtered_distance = 0.0;

// 静态变量，用于存储采样数据和计数
static float sample_buffer[5] = {0};
static int sample_count = 0;

int median_filter(float input) {
    // 存储输入数据到缓冲区
    sample_buffer[sample_count] = input;
    sample_count++;

    // 检查是否已收集满5个数据
    if (sample_count >= 5) {
        float max_val = 0;
        float min_val = 1000;
        float sum = 0.0;
        int max_index = 0;
        int min_index = 0;

        // 找出最大值和最小值及其索引
        for (int i = 0; i < 5; i++) {
            if (sample_buffer[i] > max_val) {
                max_val = sample_buffer[i];
                max_index = i;
            }
            if (sample_buffer[i] < min_val) {
                min_val = sample_buffer[i];
                min_index = i;
            }
        }

        // 计算去除最大最小值后的和
        for (int i = 0; i < 5; i++) {
            sum += sample_buffer[i];
        }
        sum = sum - max_val - min_val;

        // 计算平均值（3个数的平均）
        filtered_distance = sum / 3.0;

        // 重置计数器，准备下一轮采样
        sample_count = 0;

        return 1; // 表示已完成一次滤波
    }

    return 0; // 表示还在收集数据
}

int min = 0, sec = 0;
bool isSpeed2X = false;
bool isBreath = false, isStop = false;
extern int TIM1_Times, TIM0_Times;
int nowBeepHz = 1;
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

    // uart0串口屏
    GPIOB_SetBits(GPIO_Pin_7);
    GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_PU); // RXD-配置上拉输入
    GPIOB_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
    UART0_DefInit();
    UART0_ByteTrigCfg(UART_7BYTE_TRIG);
    UART0_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART0_IRQn);

    // uart1超声波
    GPIOB_SetBits(GPIO_Pin_13);
    GPIOPinRemap(ENABLE, RB_PIN_UART1);
    GPIOB_ModeCfg(GPIO_Pin_12, GPIO_ModeIN_PU); // RXD-配置上拉输入
    GPIOB_ModeCfg(GPIO_Pin_13, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
    UART1_DefInit();
    UART1_ByteTrigCfg(UART_7BYTE_TRIG);
    UART1_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART1_IRQn);

    // uart2接口2 printf
    GPIOA_SetBits(GPIO_Pin_7);
    GPIOA_ModeCfg(GPIO_Pin_6, GPIO_ModeIN_PU); // RXD-配置上拉输入
    GPIOA_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
    UART2_DefInit();
    UART2_ByteTrigCfg(UART_7BYTE_TRIG);
    UART2_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART2_IRQn);

    // uart3接口3
    GPIOB_SetBits(GPIO_Pin_21);
    GPIOPinRemap(ENABLE, RB_PIN_UART3);
    GPIOB_ModeCfg(GPIO_Pin_20, GPIO_ModeIN_PU); // RXD-配置上拉输入
    GPIOB_ModeCfg(GPIO_Pin_21, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
    UART3_DefInit();
    UART3_ByteTrigCfg(UART_7BYTE_TRIG);
    UART3_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART3_IRQn);

    UART2_SendString(TxBuff, sizeof (TxBuff));
    UART3_SendString(TxBuff, sizeof (TxBuff));
    printf("xtxChino!\r\n");
    TJCScreenInit();

    // OLED
    OLED_Init();
    OLED_ColorTurn(0); // 0正常显示，1 反色显示
    OLED_DisplayTurn(0); // 0正常显示 1 屏幕翻转显示

    // Beep
    GPIOA_ModeCfg(GPIO_Pin_12, GPIO_ModeOut_PP_5mA); // PA12 - PWM4
    PWMX_CLKCfg(78); // 分下来就是1M了
    set_beep_Hz(MID_DO, false);

    // RGB
    GPIOB_ResetBits(GPIO_Pin_0);
    GPIOB_ResetBits(GPIO_Pin_1);
    GPIOB_ResetBits(GPIO_Pin_2);
    GPIOB_ModeCfg(GPIO_Pin_0, GPIO_ModeOut_PP_5mA);
    GPIOB_ModeCfg(GPIO_Pin_1, GPIO_ModeOut_PP_5mA);
    GPIOB_ModeCfg(GPIO_Pin_2, GPIO_ModeOut_PP_5mA);

    // key
    GPIOB_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_9, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_17, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_16, GPIO_ModeIN_PU);

    // ws2812
    WS2812Init();

    // TMR0
    //计时
    TMR0_TimerInit(FREQ_SYS / 10); // 设置定时时间 100ms
    TMR0_ITCfg(ENABLE, TMR0_3_IT_CYC_END);
    PFIC_EnableIRQ(TMR0_IRQn);

    //灯
    TMR1_TimerInit(FREQ_SYS / 10); // 设置定时时间 100ms
    TMR1_ITCfg(ENABLE, TMR0_3_IT_CYC_END);
    PFIC_EnableIRQ(TMR1_IRQn);

    TMR2_TimerInit(FREQ_SYS / 40);
    TMR2_ITCfg(ENABLE, TMR0_3_IT_CYC_END);
    PFIC_EnableIRQ(TMR2_IRQn);

    set_led(LED_RED, false);
    set_led(LED_GREEN, false);
    set_led(LED_BLUE, false);

    OLED_Display_GB2312_string(0, 2, (uint8_t *) "沁恒赛道能力测评");
    OLED_Display_GB2312_string(0, 4, (uint8_t *) "当前距离：");
    char tmp[20] = {0};
    while (1) {
        // mDelaymS (200);
        if (distance != 0) {
            if (median_filter(distance)) {
                sprintf(tmp, "%.02fmm", filtered_distance);
                OLED_Display_GB2312_string(0, 6, (uint8_t *) tmp);
                TJCSendTxt("data", tmp);
                float filtered_distance_cm = filtered_distance / 10.0f;
                int filtered_distance_cm_int = filtered_distance_cm;
                if (filtered_distance_cm > 25.f) {
                    TJCSendAnyProperty("pic", "pco", "63488");
                    sprintf(tmp, "R:%dcm", filtered_distance_cm_int);
                    TJCSendTxt("nowColor", tmp);
                    nowBeepHz = 1;
                    // set_beep_Hz(1,true);
                } else if (filtered_distance_cm >= 15.f) {
                    nowBeepHz = 2;
                    TJCSendAnyProperty("pic", "pco", "6677");
                    sprintf(tmp, "B:%dcm", filtered_distance_cm_int);
                    TJCSendTxt("nowColor", tmp);
                    // set_beep_Hz(2,true);
                } else if (filtered_distance_cm < 15.f) {
                    nowBeepHz = 4;
                    TJCSendAnyProperty("pic", "pco", "2016");
                    sprintf(tmp, "G:%dcm", filtered_distance_cm_int);
                    TJCSendTxt("nowColor", tmp);
                    // set_beep_Hz(4,true);
                }
                int present;
                if (filtered_distance_cm <= 5.f) {
                    present = 0;
                } else if (filtered_distance_cm >= 50.f) {
                    present = 100;
                } else {
                    present = (filtered_distance_cm - 5.f) / 45.0f * 100.0f;
                }
                sprintf(tmp, "%d", present);
                TJCSendAnyProperty("pic", "val", tmp);
                int x = 0;
                if (present <= 25) {
                    x = 0;
                } else {
                    x = (present - 25) * 8;
                }
                sprintf(tmp, "%d", x);
                TJCSendAnyProperty("nowColor", "x", tmp);
            }
            // UART1_SendByte (0xA0);
            distance = 0;
        }

        // key
        nowKey = ScanKey();
        if (nowKey != lastKey) {
            lastKey = nowKey;
            printf("nowKey = %d\r\n", nowKey);
            if (nowKey == 1) {
                if (isSpeed2X) {
                    isSpeed2X = false;
                    TIM0_Times = 0;
                } else {
                    isSpeed2X = true;
                    TIM0_Times = 0;
                }
            } else if (nowKey == 2) {
                if (isBreath) {
                    TIM1_Times = 0;
                    ws2812_set_led_hex(0, 0xff0000, LED_MODE_STATIC);
                    isBreath = false;
                } else {
                    TIM1_Times = 0;
                    ws2812_set_led_hex(0, 0xff0000, LED_MODE_BREATHE_SLOW);
                    isBreath = true;
                }
            } else if (nowKey == 3) {
            } else if (nowKey == 4) {
                if (isStop) {
                    isStop = false;
                } else {
                    isStop = true;
                }
            }
        }

        // oled
        sprintf(tmp, "2025    %02d.%02d", min, sec);
        OLED_Display_GB2312_string(0, 0, (uint8_t *) tmp);
        sprintf(tmp, "%02d.%02d", min, sec);
        TJCSendTxt("time", tmp);
    }
}
