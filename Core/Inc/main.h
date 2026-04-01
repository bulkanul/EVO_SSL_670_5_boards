/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#define Yellow_Tower_Pin GPIO_PIN_2
#define Yellow_Tower_GPIO_Port GPIOE
#define Green_Tower_Pin GPIO_PIN_3
#define Green_Tower_GPIO_Port GPIOE
#define Red_Tower_Pin GPIO_PIN_4
#define Red_Tower_GPIO_Port GPIOE
#define Relay_Pin GPIO_PIN_2
#define Relay_GPIO_Port GPIOB
#define Interlock1_Pin GPIO_PIN_0
#define Interlock1_GPIO_Port GPIOG
#define Interlock2_Pin GPIO_PIN_1
#define Interlock2_GPIO_Port GPIOG
#define Phase_OK_Pin GPIO_PIN_7
#define Phase_OK_GPIO_Port GPIOE
#define EMERGENCY_Pin GPIO_PIN_13
#define EMERGENCY_GPIO_Port GPIOE
#define Keylock_Pin GPIO_PIN_14
#define Keylock_GPIO_Port GPIOE
#define LED_ERROR_Pin GPIO_PIN_12
#define LED_ERROR_GPIO_Port GPIOB
#define LED_EMISSION_Pin GPIO_PIN_13
#define LED_EMISSION_GPIO_Port GPIOB
#define LED_POWER_Pin GPIO_PIN_14
#define LED_POWER_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_10
#define LED_RED_GPIO_Port GPIOD
#define LED_YELLOW_Pin GPIO_PIN_11
#define LED_YELLOW_GPIO_Port GPIOD
#define LED_GREEN_Pin GPIO_PIN_12
#define LED_GREEN_GPIO_Port GPIOD
#define STOP_Pin GPIO_PIN_3
#define STOP_GPIO_Port GPIOG
#define Protection_ON_OFF_Pin GPIO_PIN_7
#define Protection_ON_OFF_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
