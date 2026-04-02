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
#ifndef FDCAN
  #include "can.h"
  #define CAN_TYPE CAN_HandleTypeDef
#else
  #include "fdcan.h"
  #define CAN_TYPE FDCAN_HandleTypeDef
#endif

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
#define LED_ERROR_Pin GPIO_PIN_0
#define LED_ERROR_GPIO_Port GPIOB
#define LED_EMISSION_Pin GPIO_PIN_1
#define LED_EMISSION_GPIO_Port GPIOB
#define LED_POWER_Pin GPIO_PIN_2
#define LED_POWER_GPIO_Port GPIOB
#define EMERGENCY_Pin GPIO_PIN_13
#define EMERGENCY_GPIO_Port GPIOE
#define Keylock_Pin GPIO_PIN_14
#define Keylock_GPIO_Port GPIOE
#define LED_RED_Pin GPIO_PIN_7
#define LED_RED_GPIO_Port GPIOC
#define LED_YELLOW_Pin GPIO_PIN_8
#define LED_YELLOW_GPIO_Port GPIOC
#define LED_GREEN_Pin GPIO_PIN_9
#define LED_GREEN_GPIO_Port GPIOC
#define RS485_RSE_Pin GPIO_PIN_12
#define RS485_RSE_GPIO_Port GPIOC
#define CHILLER_INTERLOCK_Pin GPIO_PIN_10
#define CHILLER_INTERLOCK_GPIO_Port GPIOG
#define PS_ENABLE_Pin GPIO_PIN_11
#define PS_ENABLE_GPIO_Port GPIOG
#define INTERLOCK_Pin GPIO_PIN_12
#define INTERLOCK_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
