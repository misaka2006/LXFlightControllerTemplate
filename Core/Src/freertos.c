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
/* Definitions for Task_LX */
osThreadId_t Task_LXHandle;
const osThreadAttr_t Task_LX_attributes = {
  .name = "Task_LX",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Task_LXUART */
osThreadId_t Task_LXUARTHandle;
const osThreadAttr_t Task_LXUART_attributes = {
  .name = "Task_LXUART",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_ESC_LED */
osThreadId_t Task_ESC_LEDHandle;
const osThreadAttr_t Task_ESC_LED_attributes = {
  .name = "Task_ESC_LED",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_OLED */
osThreadId_t Task_OLEDHandle;
const osThreadAttr_t Task_OLED_attributes = {
  .name = "Task_OLED",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Task_DT_UART */
osThreadId_t Task_DT_UARTHandle;
const osThreadAttr_t Task_DT_UART_attributes = {
  .name = "Task_DT_UART",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CPU_Task */
osThreadId_t CPU_TaskHandle;
const osThreadAttr_t CPU_Task_attributes = {
  .name = "CPU_Task",
  .stack_size = 512 * 4,
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

void APPTask_LX(void *argument);
void APPTask_LXUART(void *argument);
void APPTask_ESC_LED(void *argument);
void APPTask_OLED(void *argument);
void APPTask_DT_UART(void *argument);
void APPCPU_Task(void *argument);

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
  /* creation of Task_LX */
  Task_LXHandle = osThreadNew(APPTask_LX, NULL, &Task_LX_attributes);

  /* creation of Task_LXUART */
  Task_LXUARTHandle = osThreadNew(APPTask_LXUART, NULL, &Task_LXUART_attributes);

  /* creation of Task_ESC_LED */
  Task_ESC_LEDHandle = osThreadNew(APPTask_ESC_LED, NULL, &Task_ESC_LED_attributes);

  /* creation of Task_OLED */
  Task_OLEDHandle = osThreadNew(APPTask_OLED, NULL, &Task_OLED_attributes);

  /* creation of Task_DT_UART */
  Task_DT_UARTHandle = osThreadNew(APPTask_DT_UART, NULL, &Task_DT_UART_attributes);

  /* creation of CPU_Task */
  CPU_TaskHandle = osThreadNew(APPCPU_Task, NULL, &CPU_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_APPTask_LX */
/**
  * @brief  Function implementing the Task_LX thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_APPTask_LX */
__weak void APPTask_LX(void *argument)
{
  /* USER CODE BEGIN APPTask_LX */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END APPTask_LX */
}

/* USER CODE BEGIN Header_APPTask_LXUART */
/**
* @brief Function implementing the Task_LXUART thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APPTask_LXUART */
__weak void APPTask_LXUART(void *argument)
{
  /* USER CODE BEGIN APPTask_LXUART */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END APPTask_LXUART */
}

/* USER CODE BEGIN Header_APPTask_ESC_LED */
/**
* @brief Function implementing the Task_ESC_LED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APPTask_ESC_LED */
__weak void APPTask_ESC_LED(void *argument)
{
  /* USER CODE BEGIN APPTask_ESC_LED */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END APPTask_ESC_LED */
}

/* USER CODE BEGIN Header_APPTask_OLED */
/**
* @brief Function implementing the Task_OLED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APPTask_OLED */
__weak void APPTask_OLED(void *argument)
{
  /* USER CODE BEGIN APPTask_OLED */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END APPTask_OLED */
}

/* USER CODE BEGIN Header_APPTask_DT_UART */
/**
* @brief Function implementing the Task_DT_UART thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APPTask_DT_UART */
__weak void APPTask_DT_UART(void *argument)
{
  /* USER CODE BEGIN APPTask_DT_UART */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END APPTask_DT_UART */
}

/* USER CODE BEGIN Header_APPCPU_Task */
/**
* @brief Function implementing the CPU_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APPCPU_Task */
__weak void APPCPU_Task(void *argument)
{
  /* USER CODE BEGIN APPCPU_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END APPCPU_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

