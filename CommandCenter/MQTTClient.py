from CommandCenter.SwarmData import SwarmData as S
import time
import paho.mqtt.client as mqtt


class MQTTClient:

    client = mqtt.Client(client_id="CC", clean_session=True)
    ip = "192.168.137.195"
    port = 1883
    refresh = 60
    last_status = None

    #################################################################################
    # onMessage implements a method to be called by the MQTT client library
    # it is linked to the MQTT client later on and is called upon receipt of a msg
    @staticmethod
    def onMessage(self, client, userdata, msg):
        if msg.topic == "esp32/s":
            self.last_status = msg.payload
        print(msg.topic + "  " + str(msg.payload))
    #################################################################################

    #################################################################################
    # onConnect implements a method to be called by the MQTT client library
    # it is linked to the client hereafter and is called when the client connects
    @staticmethod
    def onConnect(__self__, userdata, flags, rc):
        __self__.client.publish("esp32/c", "CC")
        print("MQTT!")
    #################################################################################

    #################################################################################
    # connectMQTT uses the ip, port, and refresh variables and connects the client
    # it is a wrapper around the built-in connect function
    @staticmethod
    def connectMQTT(__self__):
        __self__.client.on_connect = __self__.onConnect
        __self__.client.on_message = __self__.onMessage
        __self__.client.connect_async(__self__.ip, __self__.port, __self__.refresh)
        __self__.client.loop_start()
        # __self__.subscribe("esp32/#")
    #################################################################################

    #################################################################################
    # publish uses the built-in client publish function
    # it published a message to a topic, both of which are input as strings
    @staticmethod
    def publish(__self__, topic, msg):
        __self__.client.publish(topic=topic, qos=1, payload=msg)
    #################################################################################

    #################################################################################
    # publish uses the built-in client publish function
    # it published a message to a topic, both of which are input as strings
    @staticmethod
    def subscribe(__self__, topic):
        __self__.client.subscribe(topic=topic, qos=1)
    #################################################################################
