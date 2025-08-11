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
// ȫ�ֱ������洢�˲����
float filtered_distance = 0.0;

// ��̬���������ڴ洢�������ݺͼ���
static float sample_buffer[5] = {0};
static int sample_count = 0;

int median_filter(float input) {
    // �洢�������ݵ�������
    sample_buffer[sample_count] = input;
    sample_count++;

    // ����Ƿ����ռ���5������
    if (sample_count >= 5) {
        float max_val = 0;
        float min_val = 1000;
        float sum = 0.0;
        int max_index = 0;
        int min_index = 0;

        // �ҳ����ֵ����Сֵ��������
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

        // ����ȥ�������Сֵ��ĺ�
        for (int i = 0; i < 5; i++) {
            sum += sample_buffer[i];
        }
        sum = sum - max_val - min_val;

        // ����ƽ��ֵ��3������ƽ����
        filtered_distance = sum / 3.0;

        // ���ü�������׼����һ�ֲ���
        sample_count = 0;

        return 1; // ��ʾ�����һ���˲�
    }

    return 0; // ��ʾ�����ռ�����
}

int min = 0, sec = 0;
bool isSpeed2X = false;
bool isBreath = false, isStop = false;
extern int TIM1_Times, TIM0_Times;
int nowBeepHz = 1;
/*********************************************************************
 * @fn      main
 *
 * @brief   ������
 *
 * @return  none
 */
int main() {
    uint8_t len;

    HSECFG_Capacitance(HSECap_18p);
    SetSysClock(CLK_SOURCE_HSE_PLL_78MHz);

    // uart0������
    GPIOB_SetBits(GPIO_Pin_7);
    GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_PU); // RXD-������������
    GPIOB_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA); // TXD-�������������ע������IO������ߵ�ƽ
    UART0_DefInit();
    UART0_ByteTrigCfg(UART_7BYTE_TRIG);
    UART0_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART0_IRQn);

    // uart1������
    GPIOB_SetBits(GPIO_Pin_13);
    GPIOPinRemap(ENABLE, RB_PIN_UART1);
    GPIOB_ModeCfg(GPIO_Pin_12, GPIO_ModeIN_PU); // RXD-������������
    GPIOB_ModeCfg(GPIO_Pin_13, GPIO_ModeOut_PP_5mA); // TXD-�������������ע������IO������ߵ�ƽ
    UART1_DefInit();
    UART1_ByteTrigCfg(UART_7BYTE_TRIG);
    UART1_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART1_IRQn);

    // uart2�ӿ�2 printf
    GPIOA_SetBits(GPIO_Pin_7);
    GPIOA_ModeCfg(GPIO_Pin_6, GPIO_ModeIN_PU); // RXD-������������
    GPIOA_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA); // TXD-�������������ע������IO������ߵ�ƽ
    UART2_DefInit();
    UART2_ByteTrigCfg(UART_7BYTE_TRIG);
    UART2_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART2_IRQn);

    // uart3�ӿ�3
    GPIOB_SetBits(GPIO_Pin_21);
    GPIOPinRemap(ENABLE, RB_PIN_UART3);
    GPIOB_ModeCfg(GPIO_Pin_20, GPIO_ModeIN_PU); // RXD-������������
    GPIOB_ModeCfg(GPIO_Pin_21, GPIO_ModeOut_PP_5mA); // TXD-�������������ע������IO������ߵ�ƽ
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
    OLED_ColorTurn(0); // 0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(0); // 0������ʾ 1 ��Ļ��ת��ʾ

    // Beep
    GPIOA_ModeCfg(GPIO_Pin_12, GPIO_ModeOut_PP_5mA); // PA12 - PWM4
    PWMX_CLKCfg(78); // ����������1M��
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
    //��ʱ
    TMR0_TimerInit(FREQ_SYS / 10); // ���ö�ʱʱ�� 100ms
    TMR0_ITCfg(ENABLE, TMR0_3_IT_CYC_END);
    PFIC_EnableIRQ(TMR0_IRQn);

    //��
    TMR1_TimerInit(FREQ_SYS / 10); // ���ö�ʱʱ�� 100ms
    TMR1_ITCfg(ENABLE, TMR0_3_IT_CYC_END);
    PFIC_EnableIRQ(TMR1_IRQn);

    TMR2_TimerInit(FREQ_SYS / 40);
    TMR2_ITCfg(ENABLE, TMR0_3_IT_CYC_END);
    PFIC_EnableIRQ(TMR2_IRQn);

    set_led(LED_RED, false);
    set_led(LED_GREEN, false);
    set_led(LED_BLUE, false);

    OLED_Display_GB2312_string(0, 2, (uint8_t *) "�ߺ�������������");
    OLED_Display_GB2312_string(0, 4, (uint8_t *) "��ǰ���룺");
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
