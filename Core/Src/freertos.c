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
#include "lwip/apps/mqtt.h"
#include "mqttclient.h"
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
osThreadId task_DefaultHandle;
osThreadId task_LEDHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void example_do_connect(mqtt_client_t *client);
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void mqtt_sub_request_cb(void *arg, err_t result);
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len);
static void mqtt_incoming_data_cb(void *arg, const uint8_t *data, uint16_t len, uint8_t flags);
void example_publish(mqtt_client_t *client, void *arg);
static void mqtt_pub_request_cb(void *arg, err_t result);

void example_do_connect(mqtt_client_t *client)
{
    struct mqtt_connect_client_info_t ci;
    err_t err;

    /* Setup an empty client info structure */
    memset(&ci, 0, sizeof(ci));

    /* Minimal amount of information required is client identifier, so set it here */
    ci.client_id = "lwip_test";
    ci.client_user = "gywang";
    ci.client_pass = "wgy123456!";
    /* Initiate client and connect to server, if this fails immediately an error code is returned
       otherwise mqtt_connection_cb will be called with connection result after attempting
       to establish a connection with the server.
       For now MQTT version 3.1.1 is always used */
    ip_addr_t ip_addr;
    IP4_ADDR(&ip_addr, 152, 136, 55, 68);
    // IP4_ADDR(&ip_addr, 192, 168, 68, 188);
    err = mqtt_client_connect(client, &ip_addr, MQTT_PORT, mqtt_connection_cb, 0, &ci);

    /* For now just print the result code if something goes wrong */
    if (err != ERR_OK)
    {
        printf("mqtt_connect return %d\n", err);
    }
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    err_t err;
    if (status == MQTT_CONNECT_ACCEPTED)
    {
        printf("mqtt_connection_cb: Successfully connected\n");

        /* Setup callback for incoming publish requests */
        mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);

        /* Subscribe to a topic named "subtopic" with QoS level 1, call mqtt_sub_request_cb with result */
        err = mqtt_subscribe(client, "test", 1, mqtt_sub_request_cb, arg);

        if (err != ERR_OK)
        {
            printf("mqtt_subscribe return: %d\n", err);
        }
    }
    else
    {
        printf("mqtt_connection_cb: Disconnected, reason: %d\n", status);

        /* Its more nice to be connected, so try to reconnect */
        example_do_connect(client);
    }
}

static void mqtt_sub_request_cb(void *arg, err_t result)
{
    /* Just print the result code here for simplicity,
       normal behaviour would be to take some action if subscribe fails like
       notifying user, retry subscribe or disconnect from server */
    printf("Subscribe result: %d\n", result);
}

static int inpub_id;
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    printf("Incoming publish at topic %s with total length %u\n", topic, (unsigned int)tot_len);

    /* Decode topic string into a user defined reference */
    if (strcmp(topic, "print_payload") == 0)
    {
        inpub_id = 0;
    }
    else if (topic[0] == 'A')
    {
        /* All topics starting with 'A' might be handled at the same way */
        inpub_id = 1;
    }
    else
    {
        /* For all other topics */
        inpub_id = 2;
    }
}

static void mqtt_incoming_data_cb(void *arg, const uint8_t *data, uint16_t len, uint8_t flags)
{
    printf("Incoming publish payload with length %d, flags %u\n", len, (unsigned int)flags);

    if (flags & MQTT_DATA_FLAG_LAST)
    {
        /* Last fragment of payload received (or whole part if payload fits receive buffer
           See MQTT_VAR_HEADER_BUFFER_LEN)  */

        /* Call function or do action depending on reference, in this case inpub_id */
        if (inpub_id == 0)
        {
            /* Don't trust the publisher, check zero termination */
            if (data[len - 1] == 0)
            {
                printf("mqtt_incoming_data_cb: %s\n", (const char *)data);
            }
        }
        else if (inpub_id == 1)
        {
            /* Call an 'A' function... */
        }
        else
        {
            printf("mqtt_incoming_data_cb: Ignoring payload...\n");
        }
    }
    else
    {
        /* Handle fragmented payload, store in buffer, write to file or whatever */
    }
}

void example_publish(mqtt_client_t *client, void *arg)
{
    const char *pub_payload = "PubSubHubLubJub";
    err_t err;
    uint8_t qos = 1;    /* 0 1 or 2, see MQTT specification */
    uint8_t retain = 0; /* No don't retain such crappy payload... */
    err = mqtt_publish(client, "test", pub_payload, strlen(pub_payload), qos, retain, mqtt_pub_request_cb, arg);
    if (err != ERR_OK)
    {
        printf("Publish err: %d\n", err);
    }
}

static void mqtt_pub_request_cb(void *arg, err_t result)
{
    if (result != ERR_OK)
    {
        printf("Publish result: %d\n", result);
    }
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
    mqtt_client_t static_client;
    static_client.conn_state = 0;
    example_do_connect(&static_client);
    /* Infinite loop */
    for (;;)
    {
        example_publish(&static_client, (void *)mqtt_pub_request_cb);
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
