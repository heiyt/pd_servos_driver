//
// Created by heiyt on 2025/11/24.
//

#ifndef HEAD_2_ESP32_POWER_H
#define HEAD_2_ESP32_POWER_H
#include <cstdint>
#include <sys/types.h>

class Power {
public:
    static uint8_t pd_mode;
    Power();
    static uint8_t get_power_mode();
    static void open_pd();
    static void close_pd();
};

#endif //HEAD_2_ESP32_POWER_H