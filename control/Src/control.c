#include "cmsis_os.h"
#include "handle_config.h"
#include "i2c.h"
#include "lcd_hd44780_i2c.h"
#include "main.h"
#include "stdio.h"
#include "usart.h"

char debug_buffer[80];

void StartDefaultTask(void *argument);
void GyroTimerCallback(void *argument);
void move_to_edge(AllWheels_HandleTypeDef *hawhl, float speedx, float speedy);
void run_whole_map();

void debug_encoder(AllWheels_HandleTypeDef *hawhl);

void StartDefaultTask(void *argument)
{
  lcdInit(&hi2c2, (uint8_t)0x27, (uint8_t)2, (uint8_t)16);

  // Print text and home position 0,0
  lcdPrintStr((uint8_t *)"Hello,", 6);

  // Set cursor at zero position of line 3
  lcdSetCursorPosition(0, 2);

  // Print text at cursor position
  lcdPrintStr((uint8_t *)"World!", 6);

  // gyro_start(&hgyro);
  pwm_init(&hpca);
  display_init(&hdisp);

  PWM_HandleTypeDef hsrv0 = {.hpca = &hpca, .channel = 0},
                    hsrv1 = {.hpca = &hpca, .channel = 1},
                    hsrv2 = {.hpca = &hpca, .channel = 2};

  pwm_set_duty_cycle(&hsrv0, 0.125);
  pwm_set_duty_cycle(&hsrv1, 0.075);
  pwm_set_duty_cycle(&hsrv2, 0.025);

  osDelay(2000);
  all_wheels_start_encoder(&hawhl);
  // gyro_start(&hgyro);
  // // for (;;) {
  // //   osDelay(500);
  // //   debug_encoder(&hawhl);
  // // }
  // hawhl.FL->hmtr->speed = 0.1;
  // hawhl.FR->hmtr->speed = 0.1;
  // hawhl.RL->hmtr->speed = 0.1;
  // hawhl.RR->hmtr->speed = 0.1;
  // motor_speed_update(hawhl.FL->hmtr);
  // motor_speed_update(hawhl.FR->hmtr);
  // motor_speed_update(hawhl.RL->hmtr);
  // motor_speed_update(hawhl.RR->hmtr);
  // for (;;) {
  //   osDelay(100);
  // }
  all_wheels_start_speed_composition(&hawhl);
  // for (;;) {
  //   osDelay(100);
  // }
  run_whole_map();
  all_wheels_set_main_speed(&hawhl, 20, 0);
  osDelay(100000);
  // osTimerStart(osTimerNew(GyroTimerCallback, osTimerPeriodic, &hawhl,
  //                         &(osTimerAttr_t){.name = "gyroTimer"}),
  //              50);

  // all_wheels_set_speed(&hawhl, 20, 25, 0);
  // osDelay(2000);
  // all_wheels_set_speed(&hawhl, 0, 0, 0);
  // osDelay(500);
  // all_wheels_set_speed(&hawhl, 20, 0, 0);
  // osDelay(1500);
  // all_wheels_set_speed(&hawhl, 0, 0, 0);

  // all_wheels_move_xy_delta(&hawhl, 30, 0, 20);
  // all_wheels_move_xy_delta(&hawhl, 0, 30, 20);
  // all_wheels_move_xy_delta(&hawhl, -30, -30, 20);
  // all_wheels_move_xy_delta(&hawhl, 45, 35, 40);
  osDelay(2000);
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw - 16384);
  osDelay(5000);
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw + 16384);
  osDelay(5000);
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw + 16384);
  osDelay(5000);
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw - 16384);
  osDelay(1000);
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw - 16384);
  osDelay(5000);
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw + 16384);
  osDelay(5000);
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw + 16384);
  // move_to_edge(&hawhl, 10, -10);
  // osDelay(2000);
  // all_wheels_move_xy_delta(&hawhl, 80, 5, 40);
  // osDelay(2000);
  // move_to_edge(&hawhl, 10, -10);
  // osDelay(2000);
  // all_wheels_move_xy_delta(&hawhl, 20, 90, 40);
  // osDelay(2000);
  // move_to_edge(&hawhl, 10, -10);
  // osDelay(2000);
  // all_wheels_move_xy_delta(&hawhl, -65, 55, 40);
  // osDelay(2000);
  // move_to_edge(&hawhl, 10, 10);
  for (;;) {
    // all_wheels_move_xy_delta(&hawhl, -20, 0, 20);
    osDelay(5000);
    gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw + 16384);
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

void move_to_edge_custom(AllWheels_HandleTypeDef *hawhl, float speedx,
                         float speedy, GPIO_TypeDef *xport, uint16_t xpin,
                         GPIO_TypeDef *yport, uint16_t ypin)
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
    if (HAL_GPIO_ReadPin(xport, xpin) == GPIO_PIN_RESET) {
      x_cont = false;
      all_wheels_set_main_speed(hawhl, 0, hawhl->speed_components.main_y);
    }
    if (HAL_GPIO_ReadPin(yport, ypin) == GPIO_PIN_RESET) {
      y_cont = false;
      all_wheels_set_main_speed(hawhl, hawhl->speed_components.main_x, 0);
    }
    osDelay(20);
  } while (x_cont || y_cont);
}

void wait_until_turn_finished(Gyro_HandleTypeDef *hgyro)
{
  do {
    osDelay(500);
  } while (fabs(hgyro->logic_degree) > 1.0);
}

void turn_left(Gyro_HandleTypeDef *hgyro, bool left)
{
  gyro_set_logic_zero_as(
      hgyro, hgyro->logic_degree_zero_raw + (left ? 16384 : -16384));
}

void run_whole_map()
{
  // 去扫码
  scanner_start(&hscan);
  all_wheels_move_xy_delta(&hawhl, 50, 25, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(500);
  while (!hscan.new_data) {
    osDelay(1000);
  }
  lcdSetCursorPosition(0, 0);
  lcdPrintStr((uint8_t *)"Task: ", 6);
  lcdPrintStr(hscan.result, 7);

  // 去识别颜色
  all_wheels_move_xy_delta(&hawhl, 80, -5, 40);
  move_to_edge(&hawhl, 10, 10);
  all_wheels_move_xy_delta(&hawhl, 5, 5, 40);
  move_to_edge_custom(&hawhl, 10, 10, T2_GPIO_Port, T2_Pin, T1_GPIO_Port,
                      T1_Pin);
  openmv_start(&hopmv);
  hopmv.new_data = false;
  while (!hopmv.new_data) osDelay(1000);
  lcdSetCursorPosition(0, 1);
  lcdPrintStr((uint8_t *)"Color: ", 7);
  lcdPrintStr(hopmv.result, 7);
  // 回到原料区
  all_wheels_move_xy_delta(&hawhl, -20, -20, 40);
  move_to_edge(&hawhl, 10, 10);
  // 右转
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw - 16384);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -1, -1, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(2000);
  // 左转
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw + 16384);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -1, -1, 40);
  move_to_edge(&hawhl, 10, 10);
  // 去粗加工区
  all_wheels_move_xy_delta(&hawhl, 25, 50, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(2000);
  // 左转
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw + 16384);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -1, -1, 40);
  move_to_edge(&hawhl, 10, 10);
  // 去半成品区
  all_wheels_move_xy_delta(&hawhl, 80, 55, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(2000);
  // 去原料区
  all_wheels_move_xy_delta(&hawhl, -155, -35, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(2000);
  // 右转掉头
  turn_left(&hgyro, false);
  osDelay(300);
  turn_left(&hgyro, false);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -1, -1, 40);
  move_to_edge(&hawhl, 10, 10);
  // 重复
  // 左转
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw + 16384);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -1, -1, 40);
  move_to_edge(&hawhl, 10, 10);
  // 去粗加工区
  all_wheels_move_xy_delta(&hawhl, 25, 50, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(2000);
  // 左转
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw + 16384);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -1, -1, 40);
  move_to_edge(&hawhl, 10, 10);
  // 去半成品区
  all_wheels_move_xy_delta(&hawhl, 80, 55, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(2000);
  // 走三格
  all_wheels_move_xy_delta(&hawhl, -5, 80, 40);
  move_to_edge(&hawhl, 10, 10);
  // 右转掉头
  turn_left(&hgyro, false);
  osDelay(300);
  turn_left(&hgyro, false);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -1, -1, 40);
  move_to_edge(&hawhl, 10, 10);
  // // 移到终点
  all_wheels_move_xy_delta(&hawhl, -20, -20, 40);
  move_to_edge(&hawhl, -10, -10);
  all_wheels_move_xy_delta(&hawhl, -1, -1, 20);
  for (;;) {
    osDelay(1000);
  }
}

void GyroTimerCallback(void *argument)
{
  AllWheels_HandleTypeDef *hawhl = (AllWheels_HandleTypeDef *)argument;
  float yaw;
  float degree = hawhl->hgyro->logic_degree;
  yaw = (degree < 0 ? 1.0 : -1.0) * degree * degree * 0.05;
  if (yaw < -2) {
    yaw = -2;
  } else if (yaw > 2) {
    yaw = 2;
  }
  hawhl->speed_components.gyro_yaw = yaw;
}

void debug_encoder(AllWheels_HandleTypeDef *hawhl)
{
  size_t size;
  size = snprintf(debug_buffer, 80, "%6hi %6hi %6hi %6hi\r\n",
                  (int16_t)__HAL_TIM_GET_COUNTER(hawhl->FL->henc->htim),
                  (int16_t)__HAL_TIM_GET_COUNTER(hawhl->FR->henc->htim),
                  (int16_t)__HAL_TIM_GET_COUNTER(hawhl->RL->henc->htim),
                  (int16_t)__HAL_TIM_GET_COUNTER(hawhl->RR->henc->htim));

  HAL_UART_Transmit_DMA(&huart4, (uint8_t *)debug_buffer, size);
}
