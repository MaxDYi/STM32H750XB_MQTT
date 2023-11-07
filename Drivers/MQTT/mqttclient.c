#include "mqttclient.h"

#define MQTT_DEBUG 1

#define MQTT_CLIENT_ID "MQTT_CLIENT"
#define MQTT_HOST_IP "152.136.55.68"
#define MQTT_HOST_PORT 1883
#define MQTT_HOST_USER "gywang"
#define MQTT_HOST_PASS "wgy123456!"

mqtt_client_t *MqttClient;

void MQTT_Init(mqtt_client_t *client)
{

}

void MQTT_Connect(mqtt_client_t *client)
{

}

void MQTT_PrintError(int err)
{
    switch (err)
    {
    case (ERR_OK):
    {
        printf("MQTT OK\r\n");
        break;
    }
    case (ERR_MEM):
    {
        printf("MQTT ERR_MEM\r\n");
        break;
    }
    case (ERR_BUF):
    {
        printf("MQTT ERR_BUF\r\n");
        break;
    }
    case (ERR_TIMEOUT):
    {
        printf("MQTT ERR_TIMEOUT\r\n");
        break;
    }
    case (ERR_RTE):
    {
        printf("MQTT ERR_RTE\r\n");
        break;
    }
    case (ERR_INPROGRESS):
    {
        printf("MQTT ERR_INPROGRESS\r\n");
        break;
    }
    case (ERR_VAL):
    {
        printf("MQTT ERR_VAL\r\n");
        break;
    }
    default:
    {
        printf("MQTT ERR_UNKNOWN\r\n");
        break;
    }
    }
}
