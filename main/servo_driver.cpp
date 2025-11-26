#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "power.h"
#include "servo.h"
#include "debug.h"

extern "C" void app_main(void)
{
    Power power{};
    Servo servo{};
    xTaskCreatePinnedToCore(Debug::debug_task, "debug", 8 * 1024, nullptr, 10, nullptr, 0);
}
