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

// ���뵽����Ƶ�ʵ�ӳ�亯��
void distance_to_beep(const float filtered_distance) {
    music_frequency_t frequency;

    // ���ݾ��뷶Χӳ�䵽��Ӧ����
    if (filtered_distance < 50) {
        frequency = MID_DO;  // С��50ʱΪ�����
    }
    else if (filtered_distance > 300) {
        frequency = MID_DO_HIGH;  // ����300ʱΪ�����
    }
    else {
        // ��50-300��Χ�ڣ�������ӳ�䵽8������
        // ÿ��������ӦԼ35.7�ľ��뷶Χ (250/7 �� 35.7)
        int relative_distance = filtered_distance - 50;  // ת��Ϊ0-250�ķ�Χ
        int note_index = relative_distance * 7 / 250;    // ӳ�䵽0-6������

        // ȷ����������Ч��Χ��
        if (note_index > 6) note_index = 6;

        // ��������ѡ���Ӧ������
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

    // ���÷���������
    set_beep_Hz(frequency, true);
}
