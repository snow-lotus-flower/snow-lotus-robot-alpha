#include "driver_handle.h"

#include "i2c.h"
#include "main.h"
#include "spi.h"
#include "usart.h"

Scanner_HandleTypeDef hscan = {.huart = &huart2};
Gyro_HandleTypeDef hgyro = {.huart = &huart1};
PCA9685_HandleTypeDef hpca = {
    .i2c_handle = &hi2c1,
    .device_address = PCA9865_I2C_DEFAULT_DEVICE_ADDRESS,
    .inverted = false};
Display_HandleTypeDef hdisp = {
    .hspi = &hspi1, .cs_port = CS_GPIO_Port, .cs_pin = CS_Pin};

PWM_HandleTypeDef hpwmFL = {.hpca = &hpca, .channel = 8};
PWM_HandleTypeDef hpwmFR = {.hpca = &hpca, .channel = 9};
PWM_HandleTypeDef hpwmRL = {.hpca = &hpca, .channel = 10};
PWM_HandleTypeDef hpwmRR = {.hpca = &hpca, .channel = 11};

PID_HandleTypeDef hpidFL, hpidFR, hpidRL, hpidRR;

Motor_HandleTypeDef hmtrFL = {.hpwm = {.hpca = &hpca, .channel = 8},
                              .dir1_pin = FLD0_Pin};