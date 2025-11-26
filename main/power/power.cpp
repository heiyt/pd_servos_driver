//
// Created by heiyt on 2025/11/24.
//

#include "power.h"

#include "driver/gpio.h"

uint8_t Power::pd_mode = 0;

Power::Power() { // gpio5读取双通道二极管，gpio6控制20v输入
    gpio_config_t io_conf_5 = {
        .pin_bit_mask = (1ULL << GPIO_NUM_5),  // 选择GPIO5
        .mode = GPIO_MODE_INPUT,               // 设置为输入模式
        .pull_up_en = GPIO_PULLUP_DISABLE,      // 禁用内部上拉电阻
        .pull_down_en = GPIO_PULLDOWN_DISABLE, // 禁用内部下拉电阻
        .intr_type = GPIO_INTR_DISABLE         // 禁用中断
    };
    gpio_config(&io_conf_5); // 应用配置
    gpio_config_t io_conf_6 = {
        .pin_bit_mask = (1ULL << GPIO_NUM_6),  // 选择GPIO6
        .mode = GPIO_MODE_OUTPUT,               // 设置为输入模式
        .pull_up_en = GPIO_PULLUP_DISABLE,      // 禁用内部上拉电阻
        .pull_down_en = GPIO_PULLDOWN_DISABLE, // 禁用内部下拉电阻
        .intr_type = GPIO_INTR_DISABLE         // 禁用中断
    };
    gpio_config(&io_conf_6); // 应用配置
    if (get_power_mode() == 0) {
        pd_mode = 0;
    } else {
        pd_mode = 1;
    }
}

uint8_t Power::get_power_mode() {
    return gpio_get_level(GPIO_NUM_5);
}

void Power::open_pd() {
    pd_mode = 1;
    gpio_set_level(GPIO_NUM_6, 1);
}

void Power::close_pd() {
    pd_mode = 0;
    gpio_set_level(GPIO_NUM_6, 0);
}
