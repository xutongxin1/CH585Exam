//
// Created by xtx on 25-3-3.
//

#ifndef WS2812_H
#define WS2812_H

#include "CH58x_common.h"
#include <stdbool.h>

//�ߵ�ռ��
#define CODE_1 68 //97PSR *0.7
#define CODE_0 29 //97PSR *0.3

#define RESET_LEN    (300)
#define  LED_NUM 1
#define COLOR_BUFFER_LEN ((LED_NUM*(3*8))+RESET_LEN)
#define MAX_STEP (200)
#define LIST_SIZE(list) (sizeof(list)/sizeof(list[0]))
#define hex2rgb(c) (((c)>>16)&0xff),(((c)>>8)&0xff),((c)&0xff)

extern __attribute__ ((aligned (4))) uint32_t color_buf[COLOR_BUFFER_LEN];
// ʱ��������壨��λ����������
#define FLASH_FAST_ON_TIME      5   // ������ʱ��
#define FLASH_FAST_OFF_TIME     5   // ������ʱ��
#define FLASH_FAST_INTERVAL     30   // �������ʱ��

#define FLASH_SLOW_ON_TIME      75   // ������ʱ��
#define FLASH_SLOW_OFF_TIME     75   // ������ʱ��

#define BREATHE_FAST_STEP       10    // ���ٺ�������
#define BREATHE_SLOW_STEP       7    // ���ٺ�������

// LEDģʽ����
typedef enum {
    LED_MODE_DISABLE = 0,      // ����ģʽ
    LED_MODE_FLASH_FAST_1,     // ����1��
    LED_MODE_FLASH_FAST_2,     // ����2��
    LED_MODE_FLASH_FAST_3,     // ����3��
    LED_MODE_FLASH_SLOW,       // ����
    LED_MODE_BREATHE_FAST,     // ���ٺ���
    LED_MODE_BREATHE_SLOW,     // ���ٺ���
    LED_MODE_STATIC,           // ��̬��ʾ
    LED_MODE_MAX
} led_mode_t;

// LED״̬�ṹ��
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    led_mode_t mode;
    uint16_t timer;           // �ڲ���ʱ��
    uint8_t flash_count;      // ��˸����
    uint8_t brightness;       // ��ǰ���ȣ����ں���Ч����
    bool increasing;          // �������ӱ�־�����ں���Ч����
} led_state_t;

void setPixelColor(uint16_t id, uint8_t r, uint8_t g, uint8_t b);
// void w2812_sync();
void WS2812Init();
void wheelLed();
void ws2812_update(void);
void ws2812_effects_init();
void ws2812_set_led_hex(uint16_t led_id, uint32_t hex_color, led_mode_t mode);
void ws2812_set_led(uint16_t led_id, uint8_t r, uint8_t g, uint8_t b, led_mode_t mode);
void ws2812_set_all_color(uint8_t r, uint8_t g, uint8_t b);
void ws2812_set_all_mode(led_mode_t mode);
void ws2812_get_led_state(uint16_t led_id, uint8_t *r, uint8_t *g, uint8_t *b, led_mode_t *mode);
#endif //WS2812_H
