from CommandCenter.SwarmData import SwarmData as S
import time
import paho.mqtt.client as mqtt

class MQTTClient:

    client = mqtt.Client()
    ip = "192.168.137.219"
    port = 1883
    refresh = 60

    @staticmethod
    def onMessage(client, userdata, msg):
        print(msg.topic + "  " + str(msg.payload))

    @staticmethod
    def onConnect(__self__, userdata, flags, rc):
        print("Connected with result code: " + str(rc))
        __self__.client.subscribe("esp32/#")

    @staticmethod
    def connectMQTT(__self__):
        __self__.client.connect(__self__.ip, __self__.port, __self__.refresh)
        __self__.client.loop_start()

    @staticmethod
    def publish(__self__, topic, msg):
        __self__.client.publish(topic, msg)