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
void move_right_to_mid(AllWheels_HandleTypeDef *hawhl);

void debug_encoder(AllWheels_HandleTypeDef *hawhl);
void grab_up_ingredient_bottom(AllWheels_HandleTypeDef *hawhl);
void put_rough_top(AllWheels_HandleTypeDef *hawhl);

void stop()
{
  for (;;) {
    osDelay(10000);
  }
}

void test_func(int i)
{
  switch (i) {
    case 0:
      for (;;) osDelay(10000);
      break;
    case 1:
      HAL_UART_AbortReceive(hgyro.huart);
      break;
    case 2:
      move_to_edge(&hawhl, 10, 10);
      all_wheels_move_xy_delta(&hawhl, 4, 0, 10);
      break;
    case 3:
      hawhl.hsrv_shoulder->pos = 0;
      hawhl.hsrv_elbow->pos = 120;
      hawhl.hsrv_hand->pos = -150;
      osDelay(1000);
      hawhl.hsrv_waist->pos = 0;
      break;
    case 4:
      test_func(3);
      move_right_to_mid(&hawhl);
      all_wheels_move_xy_delta(&hawhl, 4, 0, 10);
      grab_up_ingredient_bottom(&hawhl);
      test_func(1);
      test_func(0);
    default:
      break;
  }
}

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
                    hsrv2 = {.hpca = &hpca, .channel = 2},
                    hsrv3 = {.hpca = &hpca, .channel = 3};

  // pwm_set_off_time(&hsrv0, 203);
  // pwm_set_off_time(&hsrv1, 300);
  // pwm_set_off_time(&hsrv2, 260);
  // pwm_set_off_time(&hsrv3, 275);

  // pwm_set_duty_cycle(&hsrv2, 205 + 102);

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

void grab_up_ingredient_bottom(AllWheels_HandleTypeDef *hawhl)
{
  hawhl->hsrv_hand->pos = -150;
  osDelay(1000);
  hawhl->hsrv_shoulder->pos = 0;
  hawhl->hsrv_elbow->pos = 120;
  osDelay(1000);
  hawhl->hsrv_waist->pos = 0;
  osDelay(1000);
  hawhl->hsrv_hand->pos = 0;
  osDelay(1000);
  hawhl->hsrv_shoulder->pos = 70;
  osDelay(1000);
}

void put_rough_top(AllWheels_HandleTypeDef *hawhl)
{
  hawhl->hsrv_shoulder->pos = 70;
  hawhl->hsrv_elbow->pos = 120;
  osDelay(1000);
  hawhl->hsrv_waist->pos = 0;
  osDelay(1000);
  hawhl->hsrv_shoulder->pos = -48;
  hawhl->hsrv_shoulder->pos = -90;
  osDelay(1000);
  hawhl->hsrv_hand->pos = -150;
  osDelay(1000);
  hawhl->hsrv_shoulder->pos = 70;
  hawhl->hsrv_elbow->pos = 120;
  osDelay(1000);
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

void move_right_to_left(AllWheels_HandleTypeDef *hawhl)
{
  all_wheels_move_xy_delta(hawhl, -5, 20, 40);
  move_to_edge(hawhl, 10, 10);
}

void move_left_to_right(AllWheels_HandleTypeDef *hawhl)
{
  all_wheels_move_xy_delta(hawhl, -5, -35, 40);
  move_to_edge(hawhl, 10, 10);
}

void move_mid_to_left(AllWheels_HandleTypeDef *hawhl)
{
  all_wheels_move_xy_delta(hawhl, -5, 5, 40);
  move_to_edge(hawhl, 10, 10);
}

void move_mid_to_right(AllWheels_HandleTypeDef *hawhl)
{
  all_wheels_move_xy_delta(hawhl, -5, -15, 40);
  move_to_edge(hawhl, 10, 10);
}

void move_left_to_mid(AllWheels_HandleTypeDef *hawhl)
{
  all_wheels_move_xy_delta(hawhl, -5, -5, 40);
  all_wheels_set_main_speed(hawhl, 0, -10);
  while (HAL_GPIO_ReadPin(T1_GPIO_Port, T1_Pin) == GPIO_PIN_SET) osDelay(20);
  all_wheels_set_main_speed(hawhl, 10, 0);
  while (HAL_GPIO_ReadPin(T3_GPIO_Port, T3_Pin) == GPIO_PIN_SET) osDelay(20);
  all_wheels_set_main_speed(hawhl, 0, 0);
}

void move_right_to_mid(AllWheels_HandleTypeDef *hawhl)
{
  all_wheels_move_xy_delta(hawhl, -5, 5, 40);
  all_wheels_set_main_speed(hawhl, 0, 10);
  while (HAL_GPIO_ReadPin(T1_GPIO_Port, T1_Pin) == GPIO_PIN_SET) osDelay(20);
  all_wheels_set_main_speed(hawhl, 10, 0);
  while (HAL_GPIO_ReadPin(T3_GPIO_Port, T3_Pin) == GPIO_PIN_SET) osDelay(20);
  all_wheels_set_main_speed(hawhl, 0, 0);
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
  int ingredient_order_up[3], ingredient_order_down[3];
  int order_up[3], order_down[3];
  int pos_main[3] = {1, 2, 0}, pos_up[3], pos_down[3];

  // 机械臂归位
  hawhl.hsrv_shoulder->pos = 70;
  hawhl.hsrv_elbow->pos = 120;
  hawhl.hsrv_hand->pos = -150;
  osDelay(500);
  hawhl.hsrv_waist->pos = 0;

  // test_func(4);
  move_to_edge(&hawhl, 10, 10);
  all_wheels_move_xy_delta(&hawhl, 4, -2, 10);
  put_rough_top(&hawhl);
  all_wheels_move_xy_delta(&hawhl, -4, 2, 10);
  test_func(0);

  // 去扫码
  // scanner_start(&hscan);
  all_wheels_move_xy_delta(&hawhl, 50, 25, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(500);
  strcpy(hscan.result, "213+321");
  // while (!hscan.new_data) {
  //   osDelay(1000);
  // }
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

  for (int i = 0; i < 3; ++i) {
    pos_up[i] = hawhl.hopmv->result[i] - '0';
    pos_down[i] = hawhl.hopmv->result[i + 4] - '0';
  }

  for (int i = 0; i < 3; ++i) {
    order_up[i] = pos_main[hawhl.hscan->result[i] - '1'];
    order_down[i] = pos_main[hawhl.hscan->result[i + 4] - '1'];
    ingredient_order_up[i] = pos_up[hawhl.hscan->result[i] - '1'];
    ingredient_order_down[i] = pos_down[hawhl.hscan->result[i + 4] - '1'];
  }

  // 回到原料区
  all_wheels_move_xy_delta(&hawhl, -20, -20, 40);
  move_to_edge(&hawhl, 10, 10);
  // 右转
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw - 16384);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -3, -3, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(2000);

  // test
  for (int i = 0, cur_state = 2; i <= 3; ++i) {
    int nxt = i == 3 ? 2 : ingredient_order_up[i];
    switch (cur_state * 3 + nxt) {
      case 1:
        move_left_to_mid(&hawhl);
        break;
      case 2:
        move_left_to_right(&hawhl);
        break;
      case 3:
        move_mid_to_left(&hawhl);
        break;
      case 5:
        move_mid_to_right(&hawhl);
        break;
      case 6:
        move_right_to_left(&hawhl);
        break;
      case 7:
        move_right_to_mid(&hawhl);
        break;
    }
    // test_func(2);
    // test_func(1);
    // test_func(0);
    all_wheels_move_xy_delta(&hawhl, 3, 0, 10);
    grab_up_ingredient_bottom(&hawhl);

    all_wheels_move_xy_delta(&hawhl, -3, 0, 10);
    cur_state = nxt;
    osDelay(2000);
  }

  // // test
  // move_right_to_mid(&hawhl);
  // osDelay(2000);
  // move_mid_to_left(&hawhl);
  // osDelay(2000);
  // move_left_to_right(&hawhl);
  // osDelay(2000);
  // move_right_to_left(&hawhl);
  // osDelay(2000);
  // move_left_to_mid(&hawhl);
  // osDelay(2000);
  // move_mid_to_right(&hawhl);
  // osDelay(2000);

  // 左转
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw + 16384);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -3, -3, 40);
  move_to_edge(&hawhl, 10, 10);
  // 去粗加工区
  all_wheels_move_xy_delta(&hawhl, 25, 50, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(2000);

  // test
  for (int i = 0, cur_state = 2; i <= 3; ++i) {
    int nxt = i == 3 ? 2 : order_up[i];
    switch (cur_state * 3 + nxt) {
      case 1:
        move_left_to_mid(&hawhl);
        break;
      case 2:
        move_left_to_right(&hawhl);
        break;
      case 3:
        move_mid_to_left(&hawhl);
        break;
      case 5:
        move_mid_to_right(&hawhl);
        break;
      case 6:
        move_right_to_left(&hawhl);
        break;
      case 7:
        move_right_to_mid(&hawhl);
        break;
    }
    cur_state = nxt;
    osDelay(2000);
  }

  // 左转
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw + 16384);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -3, -3, 40);
  move_to_edge(&hawhl, 10, 10);
  // 去半成品区
  all_wheels_move_xy_delta(&hawhl, 80, 55, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(2000);

  // test
  for (int i = 0, cur_state = 2; i <= 3; ++i) {
    int nxt = i == 3 ? 2 : order_up[i];
    switch (cur_state * 3 + nxt) {
      case 1:
        move_left_to_mid(&hawhl);
        break;
      case 2:
        move_left_to_right(&hawhl);
        break;
      case 3:
        move_mid_to_left(&hawhl);
        break;
      case 5:
        move_mid_to_right(&hawhl);
        break;
      case 6:
        move_right_to_left(&hawhl);
        break;
      case 7:
        move_right_to_mid(&hawhl);
        break;
    }
    cur_state = nxt;
    osDelay(2000);
  }

  // 去原料区
  all_wheels_move_xy_delta(&hawhl, -155, -40, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(2000);
  // 右转掉头
  turn_left(&hgyro, false);
  osDelay(300);
  turn_left(&hgyro, false);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -3, -3, 40);
  move_to_edge(&hawhl, 10, 10);

  // test
  for (int i = 0, cur_state = 2; i <= 3; ++i) {
    int nxt = i == 3 ? 2 : ingredient_order_down[i];
    switch (cur_state * 3 + nxt) {
      case 1:
        move_left_to_mid(&hawhl);
        break;
      case 2:
        move_left_to_right(&hawhl);
        break;
      case 3:
        move_mid_to_left(&hawhl);
        break;
      case 5:
        move_mid_to_right(&hawhl);
        break;
      case 6:
        move_right_to_left(&hawhl);
        break;
      case 7:
        move_right_to_mid(&hawhl);
        break;
    }
    cur_state = nxt;
    osDelay(2000);
  }

  // 重复
  // 左转
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw + 16384);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -3, -3, 40);
  move_to_edge(&hawhl, 10, 10);
  // 去粗加工区
  all_wheels_move_xy_delta(&hawhl, 25, 50, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(2000);

  // test
  for (int i = 0, cur_state = 2; i <= 3; ++i) {
    int nxt = i == 3 ? 2 : order_down[i];
    switch (cur_state * 3 + nxt) {
      case 1:
        move_left_to_mid(&hawhl);
        break;
      case 2:
        move_left_to_right(&hawhl);
        break;
      case 3:
        move_mid_to_left(&hawhl);
        break;
      case 5:
        move_mid_to_right(&hawhl);
        break;
      case 6:
        move_right_to_left(&hawhl);
        break;
      case 7:
        move_right_to_mid(&hawhl);
        break;
    }
    cur_state = nxt;
    osDelay(2000);
  }
  // 左转
  gyro_set_logic_zero_as(&hgyro, hgyro.logic_degree_zero_raw + 16384);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -3, -3, 40);
  move_to_edge(&hawhl, 10, 10);
  // 去半成品区
  all_wheels_move_xy_delta(&hawhl, 80, 55, 40);
  move_to_edge(&hawhl, 10, 10);
  osDelay(2000);

  // test
  for (int i = 0, cur_state = 2; i <= 3; ++i) {
    int nxt = i == 3 ? 2 : order_down[i];
    switch (cur_state * 3 + nxt) {
      case 1:
        move_left_to_mid(&hawhl);
        break;
      case 2:
        move_left_to_right(&hawhl);
        break;
      case 3:
        move_mid_to_left(&hawhl);
        break;
      case 5:
        move_mid_to_right(&hawhl);
        break;
      case 6:
        move_right_to_left(&hawhl);
        break;
      case 7:
        move_right_to_mid(&hawhl);
        break;
    }
    cur_state = nxt;
    osDelay(2000);
  }
  // 走三格
  all_wheels_move_xy_delta(&hawhl, -5, 80, 40);
  move_to_edge(&hawhl, 10, 10);
  // 右转掉头
  turn_left(&hgyro, false);
  osDelay(300);
  turn_left(&hgyro, false);
  wait_until_turn_finished(&hgyro);
  all_wheels_move_xy_delta(&hawhl, -3, -3, 40);
  move_to_edge(&hawhl, 10, 10);
  // // 移到终点
  all_wheels_move_xy_delta(&hawhl, -20, -20, 40);
  move_to_edge(&hawhl, -10, -10);
  all_wheels_move_xy_delta(&hawhl, -3, -3, 20);
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
