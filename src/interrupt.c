#include "beep.h"
#include "CH58x_common.h"
#include "WS2812.h"
uint8_t Rx0Buff[100] = {0};
uint8_t Rx1Buff[100] = {0};
uint8_t Rx2Buff[100] = {0};
uint8_t Rx3Buff[100] = {0};
float distance = 0.0f;

__INTERRUPT
__HIGH_CODE
void UART0_IRQHandler(void) {
    volatile uint8_t i;

    switch (UART0_GetITFlag()) {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART0_GetLinSTA();
            break;
        }

        case UART_II_RECV_RDY: // 数据达到设置触发点
            for (i = 0; i != 7; i++) {
                Rx0Buff[i] = UART0_RecvByte();
                // UART0_SendByte(Rx0Buff[i]);
            }
            break;

        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
            i = UART0_RecvString(Rx0Buff);
            // UART0_SendString(Rx0Buff, i);
            break;

        case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
            break;
        default:
            break;
    }
}

__INTERRUPT
__HIGH_CODE
void UART1_IRQHandler(void) {
    volatile uint8_t i;

    switch (UART1_GetITFlag()) {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART1_GetLinSTA();
            break;
        }

        case UART_II_RECV_RDY: // 数据达到设置触发点
            for (i = 0; i != 7; i++) {
                Rx1Buff[i] = UART1_RecvByte();
                // UART1_SendByte(Rx1Buff[i]);
            }
            break;

        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
            i = UART1_RecvString(Rx1Buff);
            if (i == 3) {
                uint8_t byte_h = Rx1Buff[0];
                uint8_t byte_m = Rx1Buff[1];
                uint8_t byte_l = Rx1Buff[2];

                uint32_t raw_distance = ((uint32_t) byte_h << 16) +
                    ((uint32_t) byte_m << 8) +
                    (uint32_t) byte_l;

                distance = (float) raw_distance / 1000.0f;
            }
            // 你会在这里收到数据
            break;

        case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
            break;
        default:
            break;
    }
}

__INTERRUPT
__HIGH_CODE
void UART2_IRQHandler(void) {
    volatile uint8_t i;

    switch (UART2_GetITFlag()) {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART2_GetLinSTA();
            break;
        }

        case UART_II_RECV_RDY: // 数据达到设置触发点
            for (i = 0; i != 7; i++) {
                Rx2Buff[i] = UART2_RecvByte();
                UART2_SendByte(Rx2Buff[i]);
            }
            break;

        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
            i = UART2_RecvString(Rx2Buff);
            UART2_SendString(Rx2Buff, i);
            break;

        case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
            break;
        default:
            break;
    }
}

__INTERRUPT
__HIGH_CODE
void UART3_IRQHandler(void) {
    volatile uint8_t i;

    switch (UART3_GetITFlag()) {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART3_GetLinSTA();
            break;
        }

        case UART_II_RECV_RDY: // 数据达到设置触发点
            for (i = 0; i != 7; i++) {
                Rx3Buff[i] = UART3_RecvByte();
                UART3_SendByte(Rx3Buff[i]);
            }
            break;

        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
            i = UART3_RecvString(Rx3Buff);
            UART3_SendString(Rx3Buff, i);
            break;

        case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
            break;
        default:
            break;
    }
}
extern int min, sec;
int TIM0_Times = 0, TIM1_Times = 0, TIM2_Times = 0;
extern bool isSpeed2X, isBreath;
/*********************************************************************
 * @fn      TMR0_IRQHandler
 *
 * @brief   TMR0中断函数
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void TMR0_IRQHandler(void) // TMR0 定时中断
{
    if (TMR0_GetITFlag(TMR0_3_IT_CYC_END)) {
        TMR0_ClearITFlag(TMR0_3_IT_CYC_END); // 清除中断标志
        // UART1_SendByte (0xA0);
        TIM0_Times++;
        if (isSpeed2X) {
            if (TIM0_Times == 5) {
                if (sec == 59) {
                    sec = 0;
                    min++;
                } else {
                    sec++;
                }
                TIM0_Times = 0;
            }
        } else {
            if (TIM0_Times == 10) {
                if (sec == 59) {
                    sec = 0;
                    min++;
                } else {
                    sec++;
                }
                TIM0_Times = 0;
            }
        }
    }
}

/*********************************************************************
 * @fn      TMR1_IRQHandler
 *
 * @brief   TMR1中断函数
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void TMR1_IRQHandler(void) // TMR0 定时中断
{
    if (TMR1_GetITFlag(TMR0_3_IT_CYC_END)) {
        TMR1_ClearITFlag(TMR0_3_IT_CYC_END); // 清除中断标志
        TIM1_Times++;
        if (!isBreath) {
            if (TIM1_Times == 10) {
                ws2812_set_led_hex(0, 0x00ff00, LED_MODE_STATIC);
            } else if (TIM1_Times == 20) {
                ws2812_set_led_hex(0, 0x0000ff, LED_MODE_STATIC);
            } else if (TIM1_Times == 30) {
                ws2812_set_led_hex(0, 0xffffff, LED_MODE_STATIC);
            } else if (TIM1_Times == 40) {
                ws2812_set_led_hex(0, 0xff0000, LED_MODE_STATIC);
                TIM1_Times = 0;
            }
        } else {
        }
        UART1_SendByte(0xA0);
        ws2812_update();
    }
}
int lastBeepHz = 1;
extern int nowBeepHz;
bool isBeepON = false;
/*********************************************************************
 * @fn      TMR2_IRQHandler
 *
 * @brief   TMR2中断函数
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void TMR2_IRQHandler(void) // TMR0 定时中断
{
    if (TMR2_GetITFlag(TMR0_3_IT_CYC_END)) {
        TMR2_ClearITFlag(TMR0_3_IT_CYC_END); // 清除中断标志
        TIM2_Times++;
        if (lastBeepHz != nowBeepHz) {
            lastBeepHz = nowBeepHz;
            TIM2_Times = 0;
        } else {
            set_beep_Hz(MID_DO,false);
            if (TIM2_Times == 40 && nowBeepHz == 1) {
                isBeepON = true;
                set_beep_Hz(MID_DO,true);
                TIM2_Times = 0;
            } else if (TIM2_Times == 20 && nowBeepHz == 2) {
                isBeepON = true;
                set_beep_Hz(MID_DO,true);
                TIM2_Times = 0;
            } else if (TIM2_Times == 10 && nowBeepHz == 4) {
                isBeepON = true;
                set_beep_Hz(MID_DO,true);
                TIM2_Times = 0;
            }
        }
    }
}
