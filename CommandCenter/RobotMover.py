from CommandCenter.SwarmData import SwarmData as S
import time
import paho.mqtt.client as mqtt

class MQTTClient:

    client = mqtt.Client()
    ip = "192.168.137.219"
    port = 1883
    refresh = 60
    if client.reconnect()

    @staticmethod
    def setupMQTT(__self__):
        __self__.client.on_connect = __self__.onConnect
        __self__.client.on_message = __self__.onMessage

    @staticmethod
    def connectMQTT(__self__):
        __self__.setupMQTT
        __self__.client.connect(__self__.ip, __self__.port, __self__.refresh)