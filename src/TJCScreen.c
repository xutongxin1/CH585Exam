//
// Created by xtx on 2023/7/14.
//

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "TJCScreen.h"

#include "CH58x_common.h"

void TJCScreenInit() {
    TJCSendEnd();
    TJCSendAny("rest");
    mDelaymS(1000);
    TJCSendEnd();
}

void TJCSendEnd() {
    UART0_SendString( (uint8_t *) "\xff\xff\xff", 3);
}

void TJCSendValue(char *name, int value) {
    char tmp[256]={0};
    sprintf(tmp, "%s.val=%d", name, value);
    UART0_SendString( (uint8_t *) tmp, strlen(tmp));
    TJCSendEnd();
}

void TJCSendTxt(char *name, char *value) {
    char tmp[256]={0};
    sprintf(tmp, "%s.txt=\"%s\"", name, value);
    UART0_SendString((uint8_t *) tmp, strlen(tmp));
    TJCSendEnd();
}

void TJCSendAnyProperty(char *object_name, char *property, char *value) {
    char tmp[256]={0};
    sprintf(tmp, "%s.%s=%s", object_name, property, value);
    UART0_SendString( (uint8_t *) tmp, strlen(tmp));
    TJCSendEnd();
}

void TJCSendAny(char *any) {
    UART0_SendString( (uint8_t *) any, strlen(any));
    TJCSendEnd();
}
void TJCSendTxtWithFloat(char *name, const float value) {
    char tmp[256]={0};
    sprintf(tmp, "%s.txt=\"%.02f\"", name, value);
    UART0_SendString((uint8_t *) tmp, strlen(tmp));
    TJCSendEnd();
}
// void TJCSendTxtFormatted(char *name, const char *format, ...) {
//     char tmp[256] = {0};
//     char value_str[200] = {0};
//     va_list args;
//
//     // 处理可变参数，格式化value部分
//     va_start(args, format);
//     vsprintf(value_str, format, args);
//     va_end(args);
//
//     // 组装最终的命令字符串
//     sprintf(tmp, "%s.txt=\"%s\"", name, value_str);
//
//     // 发送数据
//     UART0_SendString((uint8_t *) tmp, strlen(tmp));
//     TJCSendEnd();
// }
