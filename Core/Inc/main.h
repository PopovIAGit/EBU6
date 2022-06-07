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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "std.h"
#include "delay.h"
#include "comm.h"
#include "peref.h"
#include "g_Ram.h"
#include "g_Core.h"
#include "stat.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

#define DEVICE_ID		6001	// �?дентификатор 
#define DEVICE_YEAR		2022

// Выбор физики для передачи данных
#define UART_TYPE			0

#define DEF_USER_PASS		1111	// 
#define DEF_FACT_PASS		22222	// 

#define HZ				        18e3		// ??????? ????????? ?????????? RTOS
#define PRD_18KHZ				18e3		// ????????????? ?????? 2000 ??
#define PRD_2KHZ				2000		// ????????????? ?????? 2000 ??
#define PRD_200HZ				200			// ????????????? ?????? 200 ??
#define PRD_50HZ				50			// ????????????? ?????? 50 ??
#define PRD_10HZ				10			// ????????????? ?????? 10 ??
#define PRD_50HZ				50			// ????????????? ?????? 50 ??

#define VERSION                                 1
#define SUBVERSION                              21

#define CONCAT2(First, Second) (First ## Second)
#define CONCAT(First, Second) CONCAT2(First, Second)
#define STATIC_ASSERT(expr) typedef char CONCAT(static_assert_failed_at_line_, __LINE__) [(expr) ? 1 : -1]

#define SIZE(ELEM) (sizeof(ELEM)>> 2)
  
#define DELAY_CYCLES(X) __asm(" RPT #" #X " || NOP");
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_HRTIM_MspPostInit(HRTIM_HandleTypeDef *hhrtim);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_DB4_Pin GPIO_PIN_2
#define LCD_DB4_GPIO_Port GPIOE
#define LCD_DB3_Pin GPIO_PIN_3
#define LCD_DB3_GPIO_Port GPIOE
#define LCD_DB6_Pin GPIO_PIN_4
#define LCD_DB6_GPIO_Port GPIOE
#define LCD_DB5_Pin GPIO_PIN_5
#define LCD_DB5_GPIO_Port GPIOE
#define LCD_RS_Pin GPIO_PIN_6
#define LCD_RS_GPIO_Port GPIOE
#define LCD_DB7_Pin GPIO_PIN_8
#define LCD_DB7_GPIO_Port GPIOI
#define LCD_EN_Pin GPIO_PIN_13
#define LCD_EN_GPIO_Port GPIOC
#define LCD_RW_Pin GPIO_PIN_9
#define LCD_RW_GPIO_Port GPIOI
#define ALARM_SD_Pin GPIO_PIN_10
#define ALARM_SD_GPIO_Port GPIOI
#define OPENED_SD_Pin GPIO_PIN_11
#define OPENED_SD_GPIO_Port GPIOI
#define MU_DU_SD_Pin GPIO_PIN_0
#define MU_DU_SD_GPIO_Port GPIOF
#define CLOSED_SD_Pin GPIO_PIN_1
#define CLOSED_SD_GPIO_Port GPIOF
#define STOP1_HAL_Pin GPIO_PIN_2
#define STOP1_HAL_GPIO_Port GPIOF
#define STOP2_HAL_Pin GPIO_PIN_3
#define STOP2_HAL_GPIO_Port GPIOF
#define PROG1_HAL_Pin GPIO_PIN_4
#define PROG1_HAL_GPIO_Port GPIOF
#define PROG2_HAL_Pin GPIO_PIN_5
#define PROG2_HAL_GPIO_Port GPIOF
#define OPEN_HAL_Pin GPIO_PIN_6
#define OPEN_HAL_GPIO_Port GPIOF
#define CLOSE_HAL_Pin GPIO_PIN_7
#define CLOSE_HAL_GPIO_Port GPIOF
#define VDC_ADC_Pin GPIO_PIN_2
#define VDC_ADC_GPIO_Port GPIOH
#define IU_ADC_Pin GPIO_PIN_3
#define IU_ADC_GPIO_Port GPIOH
#define IV_ADC_Pin GPIO_PIN_4
#define IV_ADC_GPIO_Port GPIOH
#define IW_ADC_Pin GPIO_PIN_5
#define IW_ADC_GPIO_Port GPIOH
#define REF1_5_ON_OFF_Pin GPIO_PIN_3
#define REF1_5_ON_OFF_GPIO_Port GPIOA
#define UL1_ADC_Pin GPIO_PIN_6
#define UL1_ADC_GPIO_Port GPIOA
#define UL2_ADC_Pin GPIO_PIN_7
#define UL2_ADC_GPIO_Port GPIOA
#define UL3_ADC_Pin GPIO_PIN_4
#define UL3_ADC_GPIO_Port GPIOC
#define TMP_DV_A_Pin GPIO_PIN_5
#define TMP_DV_A_GPIO_Port GPIOC
#define TempModule_A_Pin GPIO_PIN_0
#define TempModule_A_GPIO_Port GPIOB
#define I_brake_A_Pin GPIO_PIN_1
#define I_brake_A_GPIO_Port GPIOB
#define TEN_OFF_Pin GPIO_PIN_13
#define TEN_OFF_GPIO_Port GPIOF
#define VOLT_ON_Pin GPIO_PIN_14
#define VOLT_ON_GPIO_Port GPIOF
#define EBU_220_380_MCU_Pin GPIO_PIN_15
#define EBU_220_380_MCU_GPIO_Port GPIOF
#define BREAKE_Pin GPIO_PIN_0
#define BREAKE_GPIO_Port GPIOG
#define Module_Foult_Pin GPIO_PIN_1
#define Module_Foult_GPIO_Port GPIOG
#define Module_OFF_Pin GPIO_PIN_7
#define Module_OFF_GPIO_Port GPIOE
#define BLT_ON_Pin GPIO_PIN_12
#define BLT_ON_GPIO_Port GPIOB
#define USART3_CTS_BLT_Pin GPIO_PIN_13
#define USART3_CTS_BLT_GPIO_Port GPIOB
#define USART3_RTS_BLT_Pin GPIO_PIN_14
#define USART3_RTS_BLT_GPIO_Port GPIOB
#define RES_BLT_Pin GPIO_PIN_15
#define RES_BLT_GPIO_Port GPIOB
#define USART3_TX_BLT_Pin GPIO_PIN_8
#define USART3_TX_BLT_GPIO_Port GPIOD
#define USART3_RX_BLT_Pin GPIO_PIN_9
#define USART3_RX_BLT_GPIO_Port GPIOD
#define CS_TEMP_Pin GPIO_PIN_0
#define CS_TEMP_GPIO_Port GPIOI
#define CS_EEPROM_Pin GPIO_PIN_1
#define CS_EEPROM_GPIO_Port GPIOI
#define LED_CPU_Pin GPIO_PIN_2
#define LED_CPU_GPIO_Port GPIOI
#define UART4_TX_ASU_Pin GPIO_PIN_10
#define UART4_TX_ASU_GPIO_Port GPIOC
#define UART4_RX_ASU_Pin GPIO_PIN_11
#define UART4_RX_ASU_GPIO_Port GPIOC
#define UART5_TX_BRP_Pin GPIO_PIN_12
#define UART5_TX_BRP_GPIO_Port GPIOC
#define RX485DE_ASU_Pin GPIO_PIN_0
#define RX485DE_ASU_GPIO_Port GPIOD
#define RX485DE_BRP_Pin GPIO_PIN_1
#define RX485DE_BRP_GPIO_Port GPIOD
#define UART5_RX_BRP_Pin GPIO_PIN_2
#define UART5_RX_BRP_GPIO_Port GPIOD
#define SPI1_MOSI_PP_Pin GPIO_PIN_7
#define SPI1_MOSI_PP_GPIO_Port GPIOD
#define SPI1_MISO_PP_Pin GPIO_PIN_9
#define SPI1_MISO_PP_GPIO_Port GPIOG
#define CS_TC_Pin GPIO_PIN_10
#define CS_TC_GPIO_Port GPIOG
#define SPI1_SCK_PP_Pin GPIO_PIN_11
#define SPI1_SCK_PP_GPIO_Port GPIOG
#define SPI6_MISO_MI_Pin GPIO_PIN_12
#define SPI6_MISO_MI_GPIO_Port GPIOG
#define SPI6_SCK_MI_Pin GPIO_PIN_13
#define SPI6_SCK_MI_GPIO_Port GPIOG
#define SPI6_MOSI_MI_Pin GPIO_PIN_14
#define SPI6_MOSI_MI_GPIO_Port GPIOG
#define ENB_TC_Pin GPIO_PIN_15
#define ENB_TC_GPIO_Port GPIOG
#define CS_TU_Pin GPIO_PIN_5
#define CS_TU_GPIO_Port GPIOB
#define CS_Iout_Pin GPIO_PIN_6
#define CS_Iout_GPIO_Port GPIOB
#define CS_Iin_Pin GPIO_PIN_7
#define CS_Iin_GPIO_Port GPIOB
#define RX485DE_ENC_Pin GPIO_PIN_9
#define RX485DE_ENC_GPIO_Port GPIOB
#define UART8_RX_ENC_Pin GPIO_PIN_0
#define UART8_RX_ENC_GPIO_Port GPIOE
#define UART8_TX_ENC_Pin GPIO_PIN_1
#define UART8_TX_ENC_GPIO_Port GPIOE
#define LCD_DB0_Pin GPIO_PIN_4
#define LCD_DB0_GPIO_Port GPIOI
#define LCD_ON_Pin GPIO_PIN_5
#define LCD_ON_GPIO_Port GPIOI
#define LCD_DB2_Pin GPIO_PIN_6
#define LCD_DB2_GPIO_Port GPIOI
#define LCD_DB1_Pin GPIO_PIN_7
#define LCD_DB1_GPIO_Port GPIOI
/* USER CODE BEGIN Private defines */
void DELAY_MS(uint32_t ms);
void DELAY_US(uint32_t us);

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi6;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern HRTIM_HandleTypeDef hhrtim;
extern UART_HandleTypeDef huart4;
extern FDCAN_HandleTypeDef hfdcan1;
extern Uns PauseModbus;


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
