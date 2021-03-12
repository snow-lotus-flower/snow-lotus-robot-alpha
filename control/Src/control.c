#include "cmsis_os.h"
#include "driver_handle.h"

void StartDefaultTask(void *argument)
{
  gyro_start(&hgyro);
  pwm_init(&hpca);
  display_init(&hdisp);

  osDelay(2000);
  gyro_set_zero(&hgyro);
  all_wheels_start_pid(&hawhl);

  for (;;) {
    all_wheels_set_speed(&hawhl, 20, 0, 0);
    osDelay(2000);
    all_wheels_set_speed(&hawhl, 20, 20, 0);
    osDelay(2000);
  }
}