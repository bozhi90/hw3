import paho.mqtt.client as paho
import serial
import time

# https://os.mbed.com/teams/mqtt/wiki/Using-MQTT#python-client

# MQTT broker hosted on local machine
mqttc = paho.Client()
serdev = '/dev/ttyACM0'
s = serial.Serial(serdev, 9600)

# Settings for connection
# TODO: revise host to your IP
host = "172.20.10.2"
topic1= "Gesture"
topic2= "Angle"

# Callbacks
def on_connect(self, mosq, obj, rc):
    print("Connected rc: " + str(rc))

def on_message(mosq, obj, msg):
    print("[Received] Topic: " + msg.topic + ", Message: " + str(msg.payload) + "\n");
    if msg.topic  == topic1:
        s.write(bytes("/Gesture_UI_stop/run\r\n", 'UTF-8'))
        time.sleep(0.5)
        s.write(bytes("/Tilt_angle_detection/run\r\n", 'UTF-8'))
    elif msg.topic  == topic2:
        s.write(bytes("/Tilt_angle_detection_stop/run\r\n", 'UTF-8'))
        time.sleep(0.5)
        s.write(bytes("/Gesture_UI/run\r\n", 'UTF-8'))

def on_subscribe(mosq, obj, mid, granted_qos):
    print("Subscribed OK")

def on_unsubscribe(mosq, obj, mid, granted_qos):
    print("Unsubscribed OK")

# Set callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_subscribe = on_subscribe
mqttc.on_unsubscribe = on_unsubscribe

# Connect and subscribe
print("Connecting to " + host + "/" + topic1)
mqttc.connect(host, port=1883, keepalive=60)
mqttc.subscribe(topic1, 0)
print("Connecting to " + host + "/" + topic2)
mqttc.subscribe(topic2, 0)


s.write(bytes("/Gesture_UI/run\r\n", 'UTF-8'))
# Loop forever, receiving messages
mqttc.loop_forever()