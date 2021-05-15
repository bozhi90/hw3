#include "mbed.h"
#include "MQTTNetwork.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"

// GLOBAL VARIABLES
WiFiInterface *wifi;
volatile int message_num = 0;
volatile int arrivedcount = 0;
volatile bool closed = false;

MQTT::Client<MQTTNetwork, Countdown>* client_pt;
const char* topic1 = "Gesture";
const char* topic2 = "Angle";


Thread mqtt_thread;
EventQueue mqtt_queue;

/******************************************************************************/
void messageArrived(MQTT::MessageData& md) {
    MQTT::Message &message = md.message;
    char msg[300];
    sprintf(msg, "Message arrived: QoS%d, retained %d, dup %d, packetID %d\r\n"
                    , message.qos, message.retained, message.dup, message.id);
    printf(msg);
    ThisThread::sleep_for(1000ms);
    char payload[300];
    sprintf(payload, "Payload %.*s\r\n", message.payloadlen, (char*)message.payload);
    printf(payload);
    ++arrivedcount;
}
/******************************************************************************/
void publish_message(MQTT::Client<MQTTNetwork, Countdown>* client, char *buffer, int select) {
    message_num++;
    MQTT::Message message;
    char buff[100];

    sprintf(buff, "%s", buffer);
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void*) buff;
    message.payloadlen = strlen(buff) + 1;
    int rc;
    if (select == 0) {
        rc = client->publish(topic1, message);
    } else if (select == 1) {
        rc = client->publish(topic2, message);
    }

    printf("rc:  %d\r\n", rc);
    printf("Puslish message: %s\r\n", buff);
}
/******************************************************************************/
void close_mqtt() {
    closed = true;
}

int function_wifi() {
    wifi = WiFiInterface::get_default_instance();
    if (!wifi) {
            printf("ERROR: No WiFiInterface found.\r\n");
            return -1;
    }
    printf("\nConnecting to %s...\r\n", MBED_CONF_APP_WIFI_SSID);
    int ret = wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
            printf("\nConnection error: %d\r\n", ret);
            return -1;
    }
    NetworkInterface* net = wifi;
    MQTTNetwork mqttNetwork(net);
    MQTT::Client<MQTTNetwork, Countdown> client(mqttNetwork);
    //TODO: revise host to your IP
    const char* host = "172.20.10.2";
    printf("Connecting to TCP network...\r\n");
    SocketAddress sockAddr;
    sockAddr.set_ip_address(host);
    sockAddr.set_port(1883);
    printf("address is %s/%d\r\n", (sockAddr.get_ip_address() ? sockAddr.get_ip_address() : "None"),  (sockAddr.get_port() ? sockAddr.get_port() : 0) ); //check setting
    int rc = mqttNetwork.connect(sockAddr);//(host, 1883);
    if (rc != 0) {
            printf("Connection error.");
            return -1;
    }
    printf("Successfully connected!\r\n");
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "Mbed";

    if ((rc = client.connect(data)) != 0){
            printf("Fail to connect MQTT\r\n");
    }
    if (client.subscribe(topic1, MQTT::QOS0, messageArrived) != 0){
            printf("Fail to subscribe\r\n");
    }
    if (client.subscribe(topic2, MQTT::QOS0, messageArrived) != 0){
            printf("Fail to subscribe\r\n");
    }

/******************************************************************************/
    client_pt = &client;//
    mqtt_thread.start(callback(&mqtt_queue, &EventQueue::dispatch_forever));

    while (1) {
        if (closed) break;
        client.yield(500);
        ThisThread::sleep_for(500ms);
    }
/******************************************************************************/
    printf("Ready to close MQTT Network......\n");
    if ((rc = client.unsubscribe(topic1)) != 0) {
            printf("Failed: rc from unsubscribe was %d\n", rc);
    }
    if ((rc = client.unsubscribe(topic2)) != 0) {
            printf("Failed: rc from unsubscribe was %d\n", rc);
    }
    if ((rc = client.disconnect()) != 0) {
    printf("Failed: rc from disconnect was %d\n", rc);
    }

    mqttNetwork.disconnect();
    printf("Successfully closed!\n");

    return 0;
}