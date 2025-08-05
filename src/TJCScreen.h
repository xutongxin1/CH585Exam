//
// Created by xtx on 2023/7/14.
//

#ifndef TJCSCREEN_H_
#define TJCSCREEN_H_

#include "string.h"
#include "stdio.h"
void TJCScreenInit();

void TJCSendEnd();

void TJCSendValue(char *name, int value);

void TJCSendTxt(char *name, char *value);

void TJCSendAnyProperty(char *object_name, char *property, char *value);

void TJCSendAny(char *any);

void TJCSendTxtWithFloat(char *name, float value);

#endif //TJCSCREEN_H_
