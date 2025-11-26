
#ifndef SHANGCHI_ESP32_SERVO_H
#define SHANGCHI_ESP32_SERVO_H

using namespace std;

#include "driver/i2c.h"
#include "esp_log.h"

class Servo {
private:
    #define PCA_Addr              0x80
    #define PCA_Model             0x00
    #define LED0_ON_L             0x06
    #define LED0_ON_H             0x07
    #define LED0_OFF_L            0x08
    #define LED0_OFF_H            0x09
    #define PCA_Pre               0xFE

    static void servo_iic_init(uint8_t c);
    static void servo_write_byte(uint8_t c, uint8_t addr, uint8_t data);
    static uint8_t servo_read_byte(uint8_t c, uint8_t addr);
    static void PCA9685_Init(uint8_t c, uint16_t hz, uint8_t angle);
    static void PCA9685_setPWM(uint8_t c, uint8_t num, uint32_t on, uint32_t off);
    static void PCA9685_setFreq(uint8_t c, uint16_t freq);
    static void setAngle(uint8_t c, uint8_t num, uint8_t angle);

public:
    Servo();
    static void Servo_Angle(uint8_t number, int16_t angle);
};

#endif //SHANGCHI_ESP32_SERVO_H