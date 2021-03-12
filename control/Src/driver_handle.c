#include "driver_handle.h"

#include "cmsis_os.h"
#include "i2c.h"
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

extern osTimerId_t EncoderTimerHandle, PIDTimerHandle;

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

Encoder_HandleTypeDef hencFL = {.htim = &htim1};
Encoder_HandleTypeDef hencFR = {.htim = &htim2};
Encoder_HandleTypeDef hencRL = {.htim = &htim3};
Encoder_HandleTypeDef hencRR = {.htim = &htim4};

Motor_HandleTypeDef hmtrFL = {
    .hpwm = &(PWM_HandleTypeDef){.hpca = &hpca, .channel = 8},
    .dir1_port = FLD0_GPIO_Port,
    .dir1_pin = FLD0_Pin,
    .dir2_port = FLD1_GPIO_Port,
    .dir2_pin = FLD1_Pin};
Motor_HandleTypeDef hmtrFR = {
    .hpwm = &(PWM_HandleTypeDef){.hpca = &hpca, .channel = 8},
    .dir1_port = FRD0_GPIO_Port,
    .dir1_pin = FRD0_Pin,
    .dir2_port = FRD1_GPIO_Port,
    .dir2_pin = FRD1_Pin};
Motor_HandleTypeDef hmtrRL = {
    .hpwm = &(PWM_HandleTypeDef){.hpca = &hpca, .channel = 8},
    .dir1_port = RLD0_GPIO_Port,
    .dir1_pin = RLD0_Pin,
    .dir2_port = RLD1_GPIO_Port,
    .dir2_pin = RLD1_Pin};
Motor_HandleTypeDef hmtrRR = {
    .hpwm = &(PWM_HandleTypeDef){.hpca = &hpca, .channel = 8},
    .dir1_port = RRD0_GPIO_Port,
    .dir1_pin = RRD0_Pin,
    .dir2_port = RRD1_GPIO_Port,
    .dir2_pin = RRD1_Pin};

AllWheels_HandleTypeDef hawhl = {
    .FL = &(
        Wheel_HandleTypeDef){.henc = &hencFL, .hmtr = &hmtrFL, .hpid = &hpidFL},
    .FR = &(
        Wheel_HandleTypeDef){.henc = &hencFR, .hmtr = &hmtrFR, .hpid = &hpidFR},
    .RL = &(
        Wheel_HandleTypeDef){.henc = &hencRL, .hmtr = &hmtrRL, .hpid = &hpidRL},
    .RR = &(
        Wheel_HandleTypeDef){.henc = &hencRR, .hmtr = &hmtrRR, .hpid = &hpidRR},
    .length_separation = 21.0,
    .width_separation = 25.0,
    .perimeter = 20.42,
};

void driver_handle_init()
{
  hawhl.htim_enc = EncoderTimerHandle;
  hawhl.htim_pid = PIDTimerHandle;
  hawhl.tim_ticks_enc = 20;
  hawhl.tim_ticks_pid = 50;
}