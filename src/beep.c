//
// Created by xtx on 2025/8/11.
//

#include "beep.h"
#include "CH58x_common.h"
void set_beep_Hz(const int Hz,const bool status) {
    if (!status) {
        PWMX_16bit_ACTOUT(CH_PWM4,1,Low_Level,false);
        return;
    }
    const int cyc=1000000/Hz;
    PWMX_16bit_CycleCfg (cyc-1);
    PWMX_16bit_ACTOUT(CH_PWM4, cyc/2, Low_Level, true);
}

// 距离到音符频率的映射函数
void distance_to_beep(const float filtered_distance) {
    music_frequency_t frequency;

    // 根据距离范围映射到对应音符
    if (filtered_distance < 50) {
        frequency = MID_DO;  // 小于50时为最低音
    }
    else if (filtered_distance > 300) {
        frequency = MID_DO_HIGH;  // 大于300时为最高音
    }
    else {
        // 在50-300范围内，将距离映射到8个音符
        // 每个音符对应约35.7的距离范围 (250/7 ≈ 35.7)
        int relative_distance = filtered_distance - 50;  // 转换为0-250的范围
        int note_index = relative_distance * 7 / 250;    // 映射到0-6的索引

        // 确保索引在有效范围内
        if (note_index > 6) note_index = 6;

        // 根据索引选择对应的音符
        switch (note_index) {
            case 0: frequency = MID_DO; break;      // 50-85
            case 1: frequency = MID_RE; break;      // 86-121
            case 2: frequency = MID_MI; break;      // 122-157
            case 3: frequency = MID_FA; break;      // 158-193
            case 4: frequency = MID_SOL; break;     // 194-229
            case 5: frequency = MID_LA; break;      // 230-265
            case 6: frequency = MID_SI; break;      // 266-300
            default: frequency = MID_DO; break;
        }
    }

    // 调用蜂鸣器函数
    set_beep_Hz(frequency, true);
}
