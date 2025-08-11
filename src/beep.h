//
// Created by xtx on 2025/8/11.
//

#ifndef CH585_EXAM_BEEP_H
#define CH585_EXAM_BEEP_H
#include <stdbool.h>
typedef enum {
    // µÕ8“Ù (Low Octave)
    LOW_DO = 262,     // C4
    LOW_RE = 294,     // D4
    LOW_MI = 330,     // E4
    LOW_FA = 349,     // F4
    LOW_SOL = 392,    // G4
    LOW_LA = 440,     // A4
    LOW_SI = 494,     // B4
    LOW_DO_HIGH = 523, // C5

    // ÷–8“Ù (Middle Octave)
    MID_DO = 523,     // C5
    MID_RE = 587,     // D5
    MID_MI = 659,     // E5
    MID_FA = 698,     // F5
    MID_SOL = 784,    // G5
    MID_LA = 880,     // A5
    MID_SI = 988,     // B5
    MID_DO_HIGH = 1047, // C6

    // ∏ﬂ8“Ù (High Octave)
    HIGH_DO = 1047,   // C6
    HIGH_RE = 1175,   // D6
    HIGH_MI = 1319,   // E6
    HIGH_FA = 1397,   // F6
    HIGH_SOL = 1568,  // G6
    HIGH_LA = 1760,   // A6
    HIGH_SI = 1976,   // B6
    HIGH_DO_HIGH = 2093 // C7
} music_frequency_t;
void set_beep_Hz(int Hz,bool status);
void distance_to_beep(float filtered_distance);
#endif //CH585_EXAM_BEEP_H
