#include "cmsis_os.h"
#include "handle_config.h"
#include "i2c.h"
#include "lcd_hd44780_i2c.h"
#include "main.h"

void StartDefaultTask(void *argument);
void GyroTimerCallback(void *argument);
void move_to_edge(AllWheels_HandleTypeDef *hawhl, float speedx, float speedy);

void StartDefaultTask(void *argument)
{
  lcdInit(&hi2c2, (uint8_t)0x27, (uint8_t)2, (uint8_t)16);

  // Print text and home position 0,0
  lcdPrintStr((uint8_t *)"Hello,", 6);

  // Set cursor at zero position of line 3
  lcdSetCursorPosition(0, 2);

  // Print text at cursor position
  lcdPrintStr((uint8_t *)"World!", 6);

  gyro_start(&hgyro);
  pwm_init(&hpca);
  display_init(&hdisp);

  PWM_HandleTypeDef hsrv0 = {.hpca = &hpca, .channel = 0},
                    hsrv1 = {.hpca = &hpca, .channel = 1},
                    hsrv2 = {.hpca = &hpca, .channel = 2};

  pwm_set_duty_cycle(&hsrv0, 0.125);
  pwm_set_duty_cycle(&hsrv1, 0.075);
  pwm_set_duty_cycle(&hsrv2, 0.025);

  osDelay(2000);
  gyro_set_zero(&hgyro);
  all_wheels_start_speed_composition(&hawhl);
  osTimerStart(osTimerNew(GyroTimerCallback, osTimerPeriodic, &hawhl,
                          &(osTimerAttr_t){.name = "gyroTimer"}),
               50);

  // all_wheels_set_speed(&hawhl, 20, 25, 0);
  // osDelay(2000);
  // all_wheels_set_speed(&hawhl, 0, 0, 0);
  // osDelay(500);
  // all_wheels_set_speed(&hawhl, 20, 0, 0);
  // osDelay(1500);
  // all_wheels_set_speed(&hawhl, 0, 0, 0);

  // for (;;) {
  //   all_wheels_set_main_speed(&hawhl, 0, 20);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, 20, 0);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, 0, -20);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, -20, 0);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, 20, 20);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, 20, -20);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, -20, -20);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, -20, 20);
  //   osDelay(2000);
  // }
  // for (;;) {
  //   all_wheels_set_main_speed(&hawhl, 0, 20);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, -20, 0);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, 0, -20);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, 20, 0);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, -20, 20);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, -20, -20);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, 20, -20);
  //   osDelay(2000);
  //   all_wheels_set_main_speed(&hawhl, 20, 20);
  //   osDelay(2000);
  // }

  // all_wheels_move_xy_delta(&hawhl, 30, 0, 20);
  // all_wheels_move_xy_delta(&hawhl, 0, 30, 20);
  // all_wheels_move_xy_delta(&hawhl, -30, -30, 20);
  all_wheels_move_xy_delta(&hawhl, 45, 35, 40);
  osDelay(2000);
  move_to_edge(&hawhl, 10, -10);
  osDelay(2000);
  all_wheels_move_xy_delta(&hawhl, 80, 5, 40);
  osDelay(2000);
  move_to_edge(&hawhl, 10, -10);
  osDelay(2000);
  all_wheels_move_xy_delta(&hawhl, 20, 90, 40);
  osDelay(2000);
  move_to_edge(&hawhl, 10, -10);
  osDelay(2000);
  all_wheels_move_xy_delta(&hawhl, -65, 55, 40);
  osDelay(2000);
  move_to_edge(&hawhl, 10, 10);
  for (;;) {
    // all_wheels_move_xy_delta(&hawhl, -20, 0, 20);
    osDelay(2000);
  }
}

void move_to_edge(AllWheels_HandleTypeDef *hawhl, float speedx, float speedy)
{
  bool x_cont = true, y_cont = true;
  if (speedx == 0.0) {
    x_cont = false;
  }
  if (speedy == 0.0) {
    y_cont = false;
  }
  all_wheels_set_main_speed(hawhl, speedx, speedy);
  do {
    if (HAL_GPIO_ReadPin(T1_GPIO_Port, T1_Pin) == GPIO_PIN_RESET) {
      x_cont = false;
      all_wheels_set_main_speed(hawhl, 0, hawhl->speed_components.main_y);
    }
    if (HAL_GPIO_ReadPin(T2_GPIO_Port, T2_Pin) == GPIO_PIN_RESET) {
      y_cont = false;
      all_wheels_set_main_speed(hawhl, hawhl->speed_components.main_x, 0);
    }
    osDelay(20);
  } while (x_cont || y_cont);
}

void GyroTimerCallback(void *argument)
{
  AllWheels_HandleTypeDef *hawhl = (AllWheels_HandleTypeDef *)argument;
  float yaw;
  float degree = hawhl->hgyro->degree;
  yaw = (degree < 0 ? 1.0 : -1.0) * degree * degree * 0.05;
  if (yaw < -0.2) {
    yaw = -0.2;
  } else if (yaw > 0.2) {
    yaw = 0.2;
  }
  hawhl->speed_components.gyro_yaw = yaw;
}
