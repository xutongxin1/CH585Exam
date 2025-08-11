//
// Created by xtx on 2025/8/11.
//

#include "led.h"

#include <stdbool.h>

#include "CH58x_common.h"
void set_led(led_color_t led,bool status) {
    if (led == LED_GREEN) {
        if (status) {
            GPIOB_ResetBits(GPIO_Pin_1);
        }
        else {
            GPIOB_SetBits(GPIO_Pin_1);
        }
    }
    else if (led == LED_BLUE) {
        if (status) {
            GPIOB_ResetBits(GPIO_Pin_2);
        }
        else {
            GPIOB_SetBits(GPIO_Pin_2);
        }
    }
    else {
        if (status) {
            GPIOB_ResetBits(GPIO_Pin_0);
        }
        else {
            GPIOB_SetBits(GPIO_Pin_0);
        }
    }
}
