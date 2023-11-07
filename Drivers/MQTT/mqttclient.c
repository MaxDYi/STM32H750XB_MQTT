#include "mqttclient.h"

// 将字符串格式IPV4地址转变为数组
void str_to_ip(char *str, uint8_t *ip)
{
    char *str_temp = strtok(str, ".");
    int i = 0;
    while (str_temp)
    {
        ip[i] = atoi(str_temp);
        str_temp = strtok(NULL, ".");
        i++;
    }
}

err_t MQTT_Connect(mqtt_client_t *client, char *hostIP, uint16_t hostPort, char *clientID, char *clientUser, char *clientPass, mqtt_connection_cb_t cb)
{
    struct mqtt_connect_client_info_t clientInfo;
    err_t err;
    memset(&clientInfo, 0, sizeof(clientInfo));
    clientInfo.client_id = clientID;
    clientInfo.client_user = clientUser;
    clientInfo.client_pass = clientPass;

    uint8_t IPV4[4];
    str_to_ip(hostIP, IPV4);

    ip_addr_t ip_addr;
    IP4_ADDR(&ip_addr, IPV4[0], IPV4[1], IPV4[2], IPV4[3]);

    err = mqtt_client_connect(client, &ip_addr, MQTT_PORT, cb, 0, &clientInfo);
    return err;
}

err_t MQTT_Publish(mqtt_client_t *client, char *topic, char *data, uint8_t qos, uint8_t retain, mqtt_request_cb_t cb)
{
    err_t err;
    err = mqtt_publish(client, topic, data, strlen(data), qos, retain, cb, (void *)cb);
    return err;
}

err_t MQTT_Subscribe(mqtt_client_t *client, char *topic, uint8_t qos, mqtt_request_cb_t cb)
{
    err_t err;
    err = mqtt_subscribe(client, topic, qos, cb, (void *)cb);
    return err;
}

void MQTT_Disconnect(mqtt_client_t *client)
{
    mqtt_disconnect(client);
}