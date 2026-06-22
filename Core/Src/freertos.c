/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for MiniPC_UART_Tas */
osThreadId_t MiniPC_UART_TasHandle;
const osThreadAttr_t MiniPC_UART_Tas_attributes = {
  .name = "MiniPC_UART_Tas",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for LXFC_UART_Task */
osThreadId_t LXFC_UART_TaskHandle;
const osThreadAttr_t LXFC_UART_Task_attributes = {
  .name = "LXFC_UART_Task",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for LX_UART_Binary_Semaphore */
osSemaphoreId_t LX_UART_Binary_SemaphoreHandle;
const osSemaphoreAttr_t LX_UART_Binary_Semaphore_attributes = {
  .name = "LX_UART_Binary_Semaphore"
};
/* Definitions for JN_UART_Binary_Semaphore */
osSemaphoreId_t JN_UART_Binary_SemaphoreHandle;
const osSemaphoreAttr_t JN_UART_Binary_Semaphore_attributes = {
  .name = "JN_UART_Binary_Semaphore"
};
/* Definitions for GD_UART_Binary_Semaphore */
osSemaphoreId_t GD_UART_Binary_SemaphoreHandle;
const osSemaphoreAttr_t GD_UART_Binary_Semaphore_attributes = {
  .name = "GD_UART_Binary_Semaphore"
};
/* Definitions for OLED_i2c_Binary_Semaphore */
osSemaphoreId_t OLED_i2c_Binary_SemaphoreHandle;
const osSemaphoreAttr_t OLED_i2c_Binary_Semaphore_attributes = {
  .name = "OLED_i2c_Binary_Semaphore"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Staru_MiniPC_UART_Task(void *argument);
void Start_LXFC_UART_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of LX_UART_Binary_Semaphore */
  LX_UART_Binary_SemaphoreHandle = osSemaphoreNew(1, 0, &LX_UART_Binary_Semaphore_attributes);

  /* creation of JN_UART_Binary_Semaphore */
  JN_UART_Binary_SemaphoreHandle = osSemaphoreNew(1, 0, &JN_UART_Binary_Semaphore_attributes);

  /* creation of GD_UART_Binary_Semaphore */
  GD_UART_Binary_SemaphoreHandle = osSemaphoreNew(1, 0, &GD_UART_Binary_Semaphore_attributes);

  /* creation of OLED_i2c_Binary_Semaphore */
  OLED_i2c_Binary_SemaphoreHandle = osSemaphoreNew(1, 0, &OLED_i2c_Binary_Semaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of MiniPC_UART_Tas */
  MiniPC_UART_TasHandle = osThreadNew(Staru_MiniPC_UART_Task, NULL, &MiniPC_UART_Tas_attributes);

  /* creation of LXFC_UART_Task */
  LXFC_UART_TaskHandle = osThreadNew(Start_LXFC_UART_Task, NULL, &LXFC_UART_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Staru_MiniPC_UART_Task */
/**
  * @brief  Function implementing the MiniPC_UART_Tas thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Staru_MiniPC_UART_Task */
__weak void Staru_MiniPC_UART_Task(void *argument)
{
  /* USER CODE BEGIN Staru_MiniPC_UART_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Staru_MiniPC_UART_Task */
}

/* USER CODE BEGIN Header_Start_LXFC_UART_Task */
/**
* @brief Function implementing the LXFC_UART_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_LXFC_UART_Task */
__weak void Start_LXFC_UART_Task(void *argument)
{
  /* USER CODE BEGIN Start_LXFC_UART_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_LXFC_UART_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

