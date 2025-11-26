
#ifndef ESP32_DEBUG_H
#define ESP32_DEBUG_H

#include "esp_log.h"

class Debug {
public:
    static void debug_task(void *pvParameters);
private:
    static int servo_cmd_handler(int argc, char **argv) ;
    static int pd_set_cmd_handler(int argc, char **argv);
    static int pd_get_cmd_handler(int argc, char **argv);
    static void register_servo_cmd();
    static void register_power_set_cmd();
    static void register_power_get_cmd();
};

#endif