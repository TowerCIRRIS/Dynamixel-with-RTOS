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
#include "stm32l4xx_hal.h"

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
#define IN_BUTTON_1_Pin GPIO_PIN_13
#define IN_BUTTON_1_GPIO_Port GPIOC
#define IN_BUTTON_1_EXTI_IRQn EXTI15_10_IRQn
#define IN_BUTTON_3_Pin GPIO_PIN_2
#define IN_BUTTON_3_GPIO_Port GPIOF
#define IN_BUTTON_4_Pin GPIO_PIN_3
#define IN_BUTTON_4_GPIO_Port GPIOF
#define OUT_IMU1_CS_Pin GPIO_PIN_4
#define OUT_IMU1_CS_GPIO_Port GPIOF
#define OUT_IMU2_CS_Pin GPIO_PIN_5
#define OUT_IMU2_CS_GPIO_Port GPIOF
#define ENCODER3_2_Pin GPIO_PIN_6
#define ENCODER3_2_GPIO_Port GPIOF
#define ENCODER3_1_Pin GPIO_PIN_7
#define ENCODER3_1_GPIO_Port GPIOF
#define PWM_MOTOR_2_Pin GPIO_PIN_10
#define PWM_MOTOR_2_GPIO_Port GPIOF
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define IN_ADC_EMG_Pin GPIO_PIN_0
#define IN_ADC_EMG_GPIO_Port GPIOC
#define IN_ADC_POT_1_Pin GPIO_PIN_1
#define IN_ADC_POT_1_GPIO_Port GPIOC
#define IN_ADC_BAT_VOLTAGE_Pin GPIO_PIN_2
#define IN_ADC_BAT_VOLTAGE_GPIO_Port GPIOC
#define IN_ADC_4_Pin GPIO_PIN_3
#define IN_ADC_4_GPIO_Port GPIOC
#define IN_ADC_5_Pin GPIO_PIN_0
#define IN_ADC_5_GPIO_Port GPIOA
#define PWM_SERVO_1_Pin GPIO_PIN_1
#define PWM_SERVO_1_GPIO_Port GPIOA
#define PWM_MOTOR_1_Pin GPIO_PIN_2
#define PWM_MOTOR_1_GPIO_Port GPIOA
#define PWM_SERVO_2_Pin GPIO_PIN_3
#define PWM_SERVO_2_GPIO_Port GPIOA
#define OUT_MOTOR1_DIR_Pin GPIO_PIN_6
#define OUT_MOTOR1_DIR_GPIO_Port GPIOA
#define ENCODER1_2_Pin GPIO_PIN_7
#define ENCODER1_2_GPIO_Port GPIOA
#define OUT_MOTOR2_DIR_Pin GPIO_PIN_4
#define OUT_MOTOR2_DIR_GPIO_Port GPIOC
#define IN_SDCARD_DETECT_Pin GPIO_PIN_2
#define IN_SDCARD_DETECT_GPIO_Port GPIOB
#define OUT_OLED_DC_Pin GPIO_PIN_13
#define OUT_OLED_DC_GPIO_Port GPIOF
#define OUT_OLED_RESET_Pin GPIO_PIN_14
#define OUT_OLED_RESET_GPIO_Port GPIOF
#define IN_BUTTON_2_Pin GPIO_PIN_1
#define IN_BUTTON_2_GPIO_Port GPIOG
#define ENCODER4_1_Pin GPIO_PIN_9
#define ENCODER4_1_GPIO_Port GPIOE
#define ENCODER4_2_Pin GPIO_PIN_11
#define ENCODER4_2_GPIO_Port GPIOE
#define OUT_OLED_CS_Pin GPIO_PIN_12
#define OUT_OLED_CS_GPIO_Port GPIOE
#define PWM_SERVO_3_Pin GPIO_PIN_10
#define PWM_SERVO_3_GPIO_Port GPIOB
#define OUT_LED_1_Pin GPIO_PIN_14
#define OUT_LED_1_GPIO_Port GPIOB
#define OUT_GPS_SB_Pin GPIO_PIN_9
#define OUT_GPS_SB_GPIO_Port GPIOD
#define OUT_GPS_INT_Pin GPIO_PIN_10
#define OUT_GPS_INT_GPIO_Port GPIOD
#define OUT_GPS_RESET_Pin GPIO_PIN_11
#define OUT_GPS_RESET_GPIO_Port GPIOD
#define ENCODER2_1_Pin GPIO_PIN_12
#define ENCODER2_1_GPIO_Port GPIOD
#define ENCODER2_2_Pin GPIO_PIN_13
#define ENCODER2_2_GPIO_Port GPIOD
#define OUT_TEMPSENSOR_CS_Pin GPIO_PIN_4
#define OUT_TEMPSENSOR_CS_GPIO_Port GPIOG
#define STLK_RX_Pin GPIO_PIN_7
#define STLK_RX_GPIO_Port GPIOG
#define STLK_TX_Pin GPIO_PIN_8
#define STLK_TX_GPIO_Port GPIOG
#define ENCODER1_1_Pin GPIO_PIN_6
#define ENCODER1_1_GPIO_Port GPIOC
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define PWM_SERVO_4_Pin GPIO_PIN_15
#define PWM_SERVO_4_GPIO_Port GPIOA
#define OUT_WIFIBT_ENABLE_Pin GPIO_PIN_4
#define OUT_WIFIBT_ENABLE_GPIO_Port GPIOD
#define EXP_CS_1_Pin GPIO_PIN_7
#define EXP_CS_1_GPIO_Port GPIOD
#define SMPS_V1_Pin GPIO_PIN_10
#define SMPS_V1_GPIO_Port GPIOG
#define SMPS_EN_Pin GPIO_PIN_11
#define SMPS_EN_GPIO_Port GPIOG
#define SMPS_PG_Pin GPIO_PIN_12
#define SMPS_PG_GPIO_Port GPIOG
#define SMPS_SW_Pin GPIO_PIN_13
#define SMPS_SW_GPIO_Port GPIOG
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define OUT_LED_2_Pin GPIO_PIN_7
#define OUT_LED_2_GPIO_Port GPIOB
#define IN_SPECT_INTERRUPT_Pin GPIO_PIN_3
#define IN_SPECT_INTERRUPT_GPIO_Port GPIOH
#define IN_HB_INTERRUPT_Pin GPIO_PIN_1
#define IN_HB_INTERRUPT_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
