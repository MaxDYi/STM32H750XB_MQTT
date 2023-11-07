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
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_priv.h"
#include "lwip/timeouts.h"
#include "lwip/ip_addr.h"
#include "lwip/mem.h"
#include "lwip/err.h"
#include "lwip/pbuf.h"
#include "lwip/altcp.h"
#include "lwip/altcp_tcp.h"
#include "lwip/altcp_tls.h"
#include "mqttclient.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define MQTT_CLIENT_ID "MQTT_CLIENT"
#define MQTT_HOST_IP "152.136.55.68"
#define MQTT_HOST_PORT 1883
#define MQTT_HOST_USER "gywang"
#define MQTT_HOST_PASS "wgy123456!"

#define QOS0 0
#define QOS1 1
#define QOS2 2
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

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void MQTT_Connect_Request_Callback(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    if (status == MQTT_CONNECT_ACCEPTED)
    {
        mqtt_set_inpub_callback(client, MQTT_Topic_Callback, MQTT_Data_Callback, arg);
    }
    else
    {
        MQTT_Connect(client, MQTT_HOST_IP, MQTT_HOST_PORT, MQTT_CLIENT_ID,
                     MQTT_HOST_USER, MQTT_HOST_PASS, MQTT_Connect_Request_Callback);
    }
}

void MQTT_Topic_Callback(void *arg, const char *topic, u32_t tot_len)
{
    printf("MQTT_Topic_Callback: %s\r\n", topic);
    if (strcmp(topic, "test") == 0)
    {
    }
    else if (strcmp(topic, "LED") == 0)
    {
    }
}

void MQTT_Data_Callback(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    printf("MQTT_Data_Callback: %s\r\n", data);
}

/* USER CODE END FunctionPrototypes */

void Task_Default(void const *argument);
void Task_LED(void const *argument);

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
    mqtt_client_t *mqttClient = mqtt_client_new();
    if (mqttClient != NULL)
    {
        err_t err;
        err = MQTT_Connect(mqttClient, MQTT_HOST_IP, MQTT_HOST_PORT, MQTT_CLIENT_ID,
                           MQTT_HOST_USER, MQTT_HOST_PASS, MQTT_Connect_Request_Callback);
        if (err == ERR_OK)
        {

            MQTT_Subscribe(mqttClient, "LED", QOS1, NULL);
        }
        else
        {
        }
    }

    /* Infinite loop */
    for (;;)
    {
        MQTT_Publish(mqttClient, "test", "hello", QOS1, 0, NULL);
        osDelay(1000);
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
        HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
        osDelay(500);
    }
    /* USER CODE END Task_LED */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
