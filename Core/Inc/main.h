/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define T5_Pin GPIO_PIN_2
#define T5_GPIO_Port GPIOE
#define T6_Pin GPIO_PIN_3
#define T6_GPIO_Port GPIOE
#define T7_Pin GPIO_PIN_4
#define T7_GPIO_Port GPIOE
#define T8_Pin GPIO_PIN_5
#define T8_GPIO_Port GPIOE
#define T1_Pin GPIO_PIN_0
#define T1_GPIO_Port GPIOC
#define T2_Pin GPIO_PIN_1
#define T2_GPIO_Port GPIOC
#define T3_Pin GPIO_PIN_2
#define T3_GPIO_Port GPIOC
#define T4_Pin GPIO_PIN_3
#define T4_GPIO_Port GPIOC
#define LED_2_Pin GPIO_PIN_6
#define LED_2_GPIO_Port GPIOA
#define FRD1_Pin GPIO_PIN_12
#define FRD1_GPIO_Port GPIOA
#define FLD1_Pin GPIO_PIN_0
#define FLD1_GPIO_Port GPIOD
#define FLD0_Pin GPIO_PIN_1
#define FLD0_GPIO_Port GPIOD
#define FRD0_Pin GPIO_PIN_3
#define FRD0_GPIO_Port GPIOD
#define RLD1_Pin GPIO_PIN_4
#define RLD1_GPIO_Port GPIOD
#define RLD0_Pin GPIO_PIN_5
#define RLD0_GPIO_Port GPIOD
#define RRD1_Pin GPIO_PIN_6
#define RRD1_GPIO_Port GPIOD
#define RRD0_Pin GPIO_PIN_7
#define RRD0_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
