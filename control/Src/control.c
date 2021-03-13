#include "cmsis_os.h"
#include "handle_config.h"

void StartDefaultTask(void *argument);
void GyroTimerCallback(void *argument);

void StartDefaultTask(void *argument)
{
  gyro_start(&hgyro);
  pwm_init(&hpca);
  display_init(&hdisp);

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
  for (;;) {
    all_wheels_set_main_speed(&hawhl, 0, 20);
    osDelay(2000);
    all_wheels_set_main_speed(&hawhl, -20, 0);
    osDelay(2000);
    all_wheels_set_main_speed(&hawhl, 0, -20);
    osDelay(2000);
    all_wheels_set_main_speed(&hawhl, 20, 0);
    osDelay(2000);
    all_wheels_set_main_speed(&hawhl, -20, 20);
    osDelay(2000);
    all_wheels_set_main_speed(&hawhl, -20, -20);
    osDelay(2000);
    all_wheels_set_main_speed(&hawhl, 20, -20);
    osDelay(2000);
    all_wheels_set_main_speed(&hawhl, 20, 20);
    osDelay(2000);
  }
}

void GyroTimerCallback(void *argument)
{
  AllWheels_HandleTypeDef *hawhl = (AllWheels_HandleTypeDef *)argument;
  float yaw;
  float degree = hawhl->hgyro->degree;
  yaw = (degree < 0 ? 1.0 : -1.0) * degree * degree * 0.02;
  if (yaw < -0.15) {
    yaw = -0.15;
  } else if (yaw > 0.15) {
    yaw = 0.15;
  }
  hawhl->speed_components.gyro_yaw = yaw;
}