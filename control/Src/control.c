#include "cmsis_os.h"
#include "handle_config.h"

void StartDefaultTask(void *argument);

void StartDefaultTask(void *argument)
{
  gyro_start(&hgyro);
  pwm_init(&hpca);
  display_init(&hdisp);

  osDelay(2000);
  gyro_set_zero(&hgyro);
  all_wheels_start_pid(&hawhl);

  all_wheels_set_speed(&hawhl, 0, 20, 0);
  osDelay(3000);
  all_wheels_set_speed(&hawhl, 0, 0, 0);

  // all_wheels_set_speed(&hawhl, 20, 25, 0);
  // osDelay(2000);
  // all_wheels_set_speed(&hawhl, 0, 0, 0);
  // osDelay(500);
  // all_wheels_set_speed(&hawhl, 20, 0, 0);
  // osDelay(1500);
  // all_wheels_set_speed(&hawhl, 0, 0, 0);

  for (;;) {
    osDelay(2000);
  }
}
