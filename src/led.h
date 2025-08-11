//
// Created by xtx on 2025/8/11.
//

#ifndef CH585_EXAM_LED_H
#define CH585_EXAM_LED_H
#include <stdbool.h>
typedef enum {
    LED_RED=0,
    LED_GREEN,
    LED_BLUE
}led_color_t;
void set_led(led_color_t led,bool status);
#endif //CH585_EXAM_LED_H