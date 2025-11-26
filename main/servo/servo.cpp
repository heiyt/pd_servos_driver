#include "servo.h"

void Servo::Servo_Angle(uint8_t number, int16_t angle) {
    if (angle >= 0) {
        setAngle(number >> 4 & 0x0f, number & 0x0f, angle);
    }
}
Servo::Servo() {
    servo_iic_init(0);
    servo_iic_init(1);
    PCA9685_Init(0, 120, 0);
    PCA9685_Init(1, 120, 0);

}
void Servo::servo_iic_init(const uint8_t c) {
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    conf.clk_flags = 0;
    if (c == 0) {
        conf.sda_io_num = GPIO_NUM_17,
        conf.scl_io_num = GPIO_NUM_18,
        i2c_param_config(I2C_NUM_0, &conf);
        i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    }
    if (c == 1) {
        conf.sda_io_num = GPIO_NUM_26,
        conf.scl_io_num = GPIO_NUM_21,
        i2c_param_config(I2C_NUM_1, &conf);
        i2c_driver_install(I2C_NUM_1, conf.mode, 0, 0, 0);
    }
}
void Servo::servo_write_byte(const uint8_t c, const uint8_t addr, const uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, PCA_Addr, true);
    i2c_master_write_byte(cmd, addr, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    if (c == 0) {
        i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    }
    if (c == 1) {
        i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_PERIOD_MS);
    }
    i2c_cmd_link_delete(cmd);
}
uint8_t Servo::servo_read_byte(const uint8_t c, const uint8_t addr) {
    uint8_t data;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, PCA_Addr, true);
    i2c_master_write_byte(cmd, addr, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, PCA_Addr | 0x01, true);
    i2c_master_read_byte(cmd, &data, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    if (c == 0) {
        i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    }
    if (c == 1) {
        i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_PERIOD_MS);
    }
    i2c_cmd_link_delete(cmd);
    return data;
}
void Servo::PCA9685_setPWM(const uint8_t c, const uint8_t num, const uint32_t on, const uint32_t off) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, PCA_Addr, true);
    i2c_master_write_byte(cmd, LED0_ON_L + 4 * num, true);
    i2c_master_write_byte(cmd, on & 0xFF, true);
    i2c_master_write_byte(cmd, on >> 8, true);
    i2c_master_write_byte(cmd, off & 0xFF, true);
    i2c_master_write_byte(cmd, off >> 8, true);
    i2c_master_stop(cmd);
    const i2c_port_t port = (c == 0) ? I2C_NUM_0 : I2C_NUM_1;
    const esp_err_t ret = i2c_master_cmd_begin(port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        printf("I2C setPWM error: %d\n", ret);
    }
}
void Servo::PCA9685_setFreq(const uint8_t c, const uint16_t freq) {
    const uint8_t oldmode = servo_read_byte(c,PCA_Model);
    const uint8_t newmode = (oldmode & 0x7F) | 0x10;
    servo_write_byte(c,PCA_Model, newmode);
    servo_write_byte(c,PCA_Pre, freq);
    servo_write_byte(c,PCA_Model, oldmode);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    servo_write_byte(c,PCA_Model, oldmode | 0xa1);
}
void Servo::setAngle(const uint8_t c, const uint8_t num, const uint8_t angle) {
    uint32_t off = 0;
    off = static_cast<uint32_t>(158 + angle * 2.2);
    PCA9685_setPWM(c, num, 0, off);
}
void Servo::PCA9685_Init(const uint8_t c, const uint16_t hz, const uint8_t angle) {
    servo_write_byte(c,PCA_Model, 0x00);
    PCA9685_setFreq(c, hz);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
