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
#include "stm32u5xx_hal.h"

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
#define LCD_DISP_EN_Pin GPIO_PIN_4
#define LCD_DISP_EN_GPIO_Port GPIOE
#define TP_IRQ_Pin GPIO_PIN_5
#define TP_IRQ_GPIO_Port GPIOE
#define TP_IRQ_EXTI_IRQn EXTI5_IRQn
#define LCD_BL_CTRL_Pin GPIO_PIN_6
#define LCD_BL_CTRL_GPIO_Port GPIOE
#define USER_BUTTON_Pin GPIO_PIN_13
#define USER_BUTTON_GPIO_Port GPIOC
#define Channel_1_Volume_Pin GPIO_PIN_0
#define Channel_1_Volume_GPIO_Port GPIOC
#define Channel_2_Volume_Pin GPIO_PIN_1
#define Channel_2_Volume_GPIO_Port GPIOC
#define Channel_3_Volume_Pin GPIO_PIN_2
#define Channel_3_Volume_GPIO_Port GPIOC
#define Channel_4_Volume_Pin GPIO_PIN_3
#define Channel_4_Volume_GPIO_Port GPIOC
#define Channel_5_Volume_Pin GPIO_PIN_0
#define Channel_5_Volume_GPIO_Port GPIOA
#define Channel_6_Volume_Pin GPIO_PIN_2
#define Channel_6_Volume_GPIO_Port GPIOA
#define Master_Volume_Pin GPIO_PIN_5
#define Master_Volume_GPIO_Port GPIOA
#define Channel_1_LR_Pin GPIO_PIN_6
#define Channel_1_LR_GPIO_Port GPIOA
#define Channel_2_LR_Pin GPIO_PIN_7
#define Channel_2_LR_GPIO_Port GPIOA
#define Channel_3_LR_Pin GPIO_PIN_0
#define Channel_3_LR_GPIO_Port GPIOB
#define Channel_4_LR_Pin GPIO_PIN_1
#define Channel_4_LR_GPIO_Port GPIOB
#define Channel_5_LR_Pin GPIO_PIN_0
#define Channel_5_LR_GPIO_Port GPIOG
#define Channel_6_LR_Pin GPIO_PIN_1
#define Channel_6_LR_GPIO_Port GPIOG
#define USER_LD2_RED_Pin GPIO_PIN_2
#define USER_LD2_RED_GPIO_Port GPIOD
#define USER_LD3_GREEN_Pin GPIO_PIN_4
#define USER_LD3_GREEN_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
