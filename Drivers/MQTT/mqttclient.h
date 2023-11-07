#ifndef __MQTTCLIENT_H__
#define __MQTTCLIENT_H__

#include "main.h"
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

void str_to_ip(char *str, uint8_t *ip);

err_t MQTT_Connect(mqtt_client_t *client, char *hostIP, uint16_t hostPort, char *clientID, char *clientUser, char *clientPass, mqtt_connection_cb_t cb);

err_t MQTT_Publish(mqtt_client_t *client, char *topic, char *data, uint8_t qos, uint8_t retain, mqtt_request_cb_t cb);

err_t MQTT_Subscribe(mqtt_client_t *client, char *topic, uint8_t qos, mqtt_request_cb_t cb);

void MQTT_Disconnect(mqtt_client_t *client);

void MQTT_Connect_Request_Callback(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);

void MQTT_Publish_Request_Callback(void *arg, err_t result);

void MQTT_Subscribe_Request_Callback(void *arg, err_t result);

void MQTT_Topic_Callback(void *arg, const char *topic, u32_t tot_len);

void MQTT_Data_Callback(void *arg, const u8_t *data, u16_t len, u8_t flags);

#endif