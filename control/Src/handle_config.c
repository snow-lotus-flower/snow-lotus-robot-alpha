#include "handle_config.h"

#include "cmsis_os.h"
#include "i2c.h"
#include "main.h"
#include "tim.h"
#include "usart.h"

extern osTimerId_t EncoderTimerHandle, PIDTimerHandle;

Gyro_HandleTypeDef hgyro = {
    .huart = &huart1,
    .drifting_rate = -2.2e-5,
};
Scanner_HandleTypeDef hscan = {
    .huart = &huart2,
};
Openmv_HandleTypeDef hopmv = {
    .huart = &huart3,
};
Laser_HandleTypeDef hlas_x = {
    .huart = &huart4,
};
Laser_HandleTypeDef hlas_y = {
    .huart = &huart5,
};

PCA9685_HandleTypeDef hpca = {
    .i2c_handle = &hi2c1,
    .device_address = PCA9865_I2C_DEFAULT_DEVICE_ADDRESS,
    .inverted = false,
};
// Display_HandleTypeDef hdisp = {
//     .hspi = &hspi1, .cs_port = CS_GPIO_Port, .cs_pin = CS_Pin};

PWM_HandleTypeDef hpwmFL = {
    .hpca = &hpca,
    .channel = 8,
};
PWM_HandleTypeDef hpwmFR = {
    .hpca = &hpca,
    .channel = 9,
};
PWM_HandleTypeDef hpwmRL = {
    .hpca = &hpca,
    .channel = 10,
};
PWM_HandleTypeDef hpwmRR = {
    .hpca = &hpca,
    .channel = 11,
};

PWM_HandleTypeDef hpwm_yaw = {
    .hpca = &hpca,
    .channel = 0,
};
PWM_HandleTypeDef hpwm_arm1 = {
    .hpca = &hpca,
    .channel = 1,
};
PWM_HandleTypeDef hpwm_arm2 = {
    .hpca = &hpca,
    .channel = 2,
};
PWM_HandleTypeDef hpwm_arm3 = {
    .hpca = &hpca,
    .channel = 3,
};
PWM_HandleTypeDef hpwm_paw = {
    .hpca = &hpca,
    .channel = 4,
};
PWM_HandleTypeDef hpwm_plate = {
    .hpca = &hpca,
    .channel = 5,
};

PIDWheel_HandleTypeDef hpidFL, hpidFR, hpidRL, hpidRR;
PIDYaw_HandleTypeDef hpid_yaw;
PIDLaser_HandleTypeDef hpid_las_x, hpid_las_y;

Encoder_HandleTypeDef hencFL = {
    .htim = &htim1,
};
Encoder_HandleTypeDef hencFR = {
    .htim = &htim2,
};
Encoder_HandleTypeDef hencRL = {
    .htim = &htim3,
};
Encoder_HandleTypeDef hencRR = {
    .htim = &htim4,
};

Motor_HandleTypeDef hmtrFL = {
    .hpwm = &hpwmFL,
    .dir1_port = FLD0_GPIO_Port,
    .dir1_pin = FLD0_Pin,
    .dir2_port = FLD1_GPIO_Port,
    .dir2_pin = FLD1_Pin,
};
Motor_HandleTypeDef hmtrFR = {
    .hpwm = &hpwmFR,
    .dir1_port = FRD0_GPIO_Port,
    .dir1_pin = FRD0_Pin,
    .dir2_port = FRD1_GPIO_Port,
    .dir2_pin = FRD1_Pin,
};
Motor_HandleTypeDef hmtrRL = {
    .hpwm = &hpwmRL,
    .dir1_port = RLD0_GPIO_Port,
    .dir1_pin = RLD0_Pin,
    .dir2_port = RLD1_GPIO_Port,
    .dir2_pin = RLD1_Pin,
};
Motor_HandleTypeDef hmtrRR = {
    .hpwm = &hpwmRR,
    .dir1_port = RRD0_GPIO_Port,
    .dir1_pin = RRD0_Pin,
    .dir2_port = RRD1_GPIO_Port,
    .dir2_pin = RRD1_Pin,
};

Servo_HandleTypeDef hsrv_yaw = {
    .hpwm = &hpwm_yaw,
    .base = 385,
    .pos = 1,
};
Servo_HandleTypeDef hsrv_arm1 = {
    .hpwm = &hpwm_arm1,
    .base = 235,
    .pos = 1,
};
Servo_HandleTypeDef hsrv_arm2 = {
    .hpwm = &hpwm_arm2,
    .base = 280,
    .pos = 1,
};
Servo_HandleTypeDef hsrv_arm3 = {
    .hpwm = &hpwm_arm3,
    .base = 250,
    .pos = 1,
};
Servo_HandleTypeDef hsrv_paw = {
    .hpwm = &hpwm_paw,
    .base = 250,
    .pos = 1,
};
Servo_HandleTypeDef hsrv_plate = {
    .hpwm = &hpwm_plate,
    .base = 317,
    .pos = 1,
};

Wheel_HandleTypeDef hwhlFL = {
    .henc = &hencFL,
    .hmtr = &hmtrFL,
    .hpid = &hpidFL,
};
Wheel_HandleTypeDef hwhlFR = {
    .henc = &hencFR,
    .hmtr = &hmtrFR,
    .hpid = &hpidFR,
};
Wheel_HandleTypeDef hwhlRL = {
    .henc = &hencRL,
    .hmtr = &hmtrRL,
    .hpid = &hpidRL,
};
Wheel_HandleTypeDef hwhlRR = {
    .henc = &hencRR,
    .hmtr = &hmtrRR,
    .hpid = &hpidRR,
};

AllWheels_HandleTypeDef hawhl = {
    .FL = &hwhlFL,
    .FR = &hwhlFR,
    .RL = &hwhlRL,
    .RR = &hwhlRR,

    .hsrv_arm1 = &hsrv_arm1,
    .hsrv_yaw = &hsrv_yaw,
    .hsrv_arm2 = &hsrv_arm2,
    .hsrv_arm3 = &hsrv_arm3,
    .hsrv_paw = &hsrv_paw,
    .hsrv_plate = &hsrv_plate,

    .hgyro = &hgyro,
    .hscan = &hscan,
    .hopmv = &hopmv,
    .hlas_x = &hlas_x,
    .hlas_y = &hlas_y,

    .length_separation = 21.0,
    .width_separation = 25.0,
    .perimeter = 20.17,
    .tim_ticks_enc = 20,
    .tim_ticks_pid_wheel = 50,
    .tim_ticks_spd = 50,
    .tim_ticks_pid_yaw = 50,
    .tim_ticks_pwm = 50,
    .tim_ticks_pid_laser = 50,
    .hpid_yaw = &hpid_yaw,
    .hpid_las_x = &hpid_las_x,
    .hpid_las_y = &hpid_las_y,
};

void USART1_IRQHandler(void)
{
  gyro_IRQHandler(&hgyro);
  HAL_UART_IRQHandler(&huart1);
}

void USART2_IRQHandler(void)
{
  scanner_IRQHandler(&hscan);
  HAL_UART_IRQHandler(&huart2);
}

void USART3_IRQHandler(void)
{
  openmv_IRQHandler(&hopmv);
  HAL_UART_IRQHandler(&huart3);
}

void UART4_IRQHandler(void)
{
  laser_IRQHandler(&hlas_x);
  HAL_UART_IRQHandler(&huart4);
}

void UART5_IRQHandler(void)
{
  laser_IRQHandler(&hlas_y);
  HAL_UART_IRQHandler(&huart5);
}
