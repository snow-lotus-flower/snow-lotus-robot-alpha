#ifndef __DRIVER_HANDLE_H__
#define __DRIVER_HANDLE_H__

#include "code_scanner.h"
#include "encoder.h"
#include "gyroscope.h"
#include "motor.h"
#include "openmv.h"
#include "pca9685.h"
#include "pid.h"
#include "pwm_driver.h"
#include "seg_display.h"
#include "wheels.h"

extern Scanner_HandleTypeDef hscan;
extern Gyro_HandleTypeDef hgyro;
extern Display_HandleTypeDef hdisp;
extern PCA9685_HandleTypeDef hpca;
extern AllWheels_HandleTypeDef hawhl;
extern Openmv_HandleTypeDef hopmv;

#endif  // !__DRIVER_HANDLE_H__