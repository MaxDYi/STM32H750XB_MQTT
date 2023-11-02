/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// MQTT Settings

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
osThreadId task_DefaultHandle;
osThreadId task_LEDHandle;
osThreadId task_MQTT_RecvHandle;
osThreadId task_MQTT_SendHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Task_Default(void const *argument);
void Task_LED(void const *argument);
void Task_MQTT_Receiver(void const *argument);
void Task_MQTT_Send(void const *argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

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
    /* definition and creation of task_Default */
    osThreadDef(task_Default, Task_Default, osPriorityNormal, 0, 512);
    task_DefaultHandle = osThreadCreate(osThread(task_Default), NULL);

    /* definition and creation of task_LED */
    osThreadDef(task_LED, Task_LED, osPriorityIdle, 0, 128);
    task_LEDHandle = osThreadCreate(osThread(task_LED), NULL);

    /* definition and creation of task_MQTT_Recv */
    osThreadDef(task_MQTT_Recv, Task_MQTT_Receiver, osPriorityIdle, 0, 512);
    task_MQTT_RecvHandle = osThreadCreate(osThread(task_MQTT_Recv), NULL);

    /* definition and creation of task_MQTT_Send */
    osThreadDef(task_MQTT_Send, Task_MQTT_Send, osPriorityIdle, 0, 512);
    task_MQTT_SendHandle = osThreadCreate(osThread(task_MQTT_Send), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_Task_Default */
/**
 * @brief  Function implementing the task_Default thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_Task_Default */
void Task_Default(void const *argument)
{
    /* init code for LWIP */
    MX_LWIP_Init();
    /* USER CODE BEGIN Task_Default */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
    /* USER CODE END Task_Default */
}

/* USER CODE BEGIN Header_Task_LED */
/**
 * @brief Function implementing the task_LED thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Task_LED */
void Task_LED(void const *argument)
{
    /* USER CODE BEGIN Task_LED */
    /* Infinite loop */
    for (;;)
    {
        // HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
        osDelay(500);
    }
    /* USER CODE END Task_LED */
}

/* USER CODE BEGIN Header_Task_MQTT_Receiver */
/**
 * @brief Function implementing the task_MQTT_Recv thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Task_MQTT_Receiver */
void Task_MQTT_Receiver(void const *argument)
{
    /* USER CODE BEGIN Task_MQTT_Receiver */

    /* Infinite loop */
    for (;;)
    {
        osDelay(100);
    }
    /* USER CODE END Task_MQTT_Receiver */
}

/* USER CODE BEGIN Header_Task_MQTT_Send */
/**
 * @brief Function implementing the task_MQTT_Send thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Task_MQTT_Send */
void Task_MQTT_Send(void const *argument)
{
    /* USER CODE BEGIN Task_MQTT_Send */

    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
    /* USER CODE END Task_MQTT_Send */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
