/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define HW_AMP_CTL_Pin GPIO_PIN_5
#define HW_AMP_CTL_GPIO_Port GPIOE
#define RTC_CE_Pin GPIO_PIN_13
#define RTC_CE_GPIO_Port GPIOC
#define HY_CK_Pin GPIO_PIN_0
#define HY_CK_GPIO_Port GPIOF
#define HY_CS_Pin GPIO_PIN_1
#define HY_CS_GPIO_Port GPIOF
#define HY_DI_Pin GPIO_PIN_2
#define HY_DI_GPIO_Port GPIOF
#define HY_DO_Pin GPIO_PIN_3
#define HY_DO_GPIO_Port GPIOF
#define HW_CTL_D_Pin GPIO_PIN_4
#define HW_CTL_D_GPIO_Port GPIOF
#define HW_CTL_E_Pin GPIO_PIN_5
#define HW_CTL_E_GPIO_Port GPIOF
#define HW_ENB_Pin GPIO_PIN_6
#define HW_ENB_GPIO_Port GPIOF
#define HW_THERMISTOR_Pin GPIO_PIN_4
#define HW_THERMISTOR_GPIO_Port GPIOA
#define HW_LOW_BAT_Pin GPIO_PIN_5
#define HW_LOW_BAT_GPIO_Port GPIOA
#define SCH_UNCONNECTED_B_Pin GPIO_PIN_11
#define SCH_UNCONNECTED_B_GPIO_Port GPIOF
#define SCH_UNCONNECTED_C_Pin GPIO_PIN_13
#define SCH_UNCONNECTED_C_GPIO_Port GPIOF
#define SCH_UNCONNECTED_D_Pin GPIO_PIN_14
#define SCH_UNCONNECTED_D_GPIO_Port GPIOF
#define R_mA_A_Pin GPIO_PIN_0
#define R_mA_A_GPIO_Port GPIOG
#define R_uA_Pin GPIO_PIN_1
#define R_uA_GPIO_Port GPIOG
#define SYS_BUZZER_Pin GPIO_PIN_7
#define SYS_BUZZER_GPIO_Port GPIOE
#define SYS_BACKLIGHT_Pin GPIO_PIN_8
#define SYS_BACKLIGHT_GPIO_Port GPIOE
#define HW_PWR_CTL_Pin GPIO_PIN_9
#define HW_PWR_CTL_GPIO_Port GPIOE
#define B_FUSE_mA_Pin GPIO_PIN_10
#define B_FUSE_mA_GPIO_Port GPIOE
#define B_FUSE_A_Pin GPIO_PIN_11
#define B_FUSE_A_GPIO_Port GPIOE
#define HW_SHDN_A_Pin GPIO_PIN_12
#define HW_SHDN_A_GPIO_Port GPIOE
#define SCH_UNCONNECTED_A_Pin GPIO_PIN_13
#define SCH_UNCONNECTED_A_GPIO_Port GPIOE
#define HW_LED_CK_Pin GPIO_PIN_15
#define HW_LED_CK_GPIO_Port GPIOE
#define R_VA_Pin GPIO_PIN_2
#define R_VA_GPIO_Port GPIOG
#define R_O_B_D_C_Pin GPIO_PIN_3
#define R_O_B_D_C_GPIO_Port GPIOG
#define R_Hz_Duty_Pin GPIO_PIN_4
#define R_Hz_Duty_GPIO_Port GPIOG
#define R_mV_TEMP_Pin GPIO_PIN_5
#define R_mV_TEMP_GPIO_Port GPIOG
#define R_VOLTS_Pin GPIO_PIN_6
#define R_VOLTS_GPIO_Port GPIOG
#define R_LowZ_Pin GPIO_PIN_7
#define R_LowZ_GPIO_Port GPIOG
#define B_MODE_Pin GPIO_PIN_8
#define B_MODE_GPIO_Port GPIOG
#define HW_VA_CTL_Pin GPIO_PIN_11
#define HW_VA_CTL_GPIO_Port GPIOA
#define HW_FRE_CTL_Pin GPIO_PIN_12
#define HW_FRE_CTL_GPIO_Port GPIOA
#define HW_TEMP_CTL_Pin GPIO_PIN_2
#define HW_TEMP_CTL_GPIO_Port GPIOH
#define HW_CTL_B_Pin GPIO_PIN_11
#define HW_CTL_B_GPIO_Port GPIOC
#define HW_CTL_A_Pin GPIO_PIN_0
#define HW_CTL_A_GPIO_Port GPIOD
#define HW_PWR_CTL2_Pin GPIO_PIN_1
#define HW_PWR_CTL2_GPIO_Port GPIOD
#define HW_DCmV_CTL_Pin GPIO_PIN_7
#define HW_DCmV_CTL_GPIO_Port GPIOD
#define B_SETUP_Pin GPIO_PIN_9
#define B_SETUP_GPIO_Port GPIOG
#define B_MINMAX_Pin GPIO_PIN_10
#define B_MINMAX_GPIO_Port GPIOG
#define B_MEM_Pin GPIO_PIN_11
#define B_MEM_GPIO_Port GPIOG
#define B_RANGE_Pin GPIO_PIN_12
#define B_RANGE_GPIO_Port GPIOG
#define B_HOLD_Pin GPIO_PIN_13
#define B_HOLD_GPIO_Port GPIOG
#define B_REL_Pin GPIO_PIN_14
#define B_REL_GPIO_Port GPIOG
#define B_PEAK_Pin GPIO_PIN_15
#define B_PEAK_GPIO_Port GPIOG
#define RTC_CLK_Pin GPIO_PIN_6
#define RTC_CLK_GPIO_Port GPIOB
#define RTC_DO_Pin GPIO_PIN_7
#define RTC_DO_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
