import paho.mqtt.client as mqtt


def onMessage(client, userdata, msg):
    print(msg.topic + "  " + str(msg.payload))

def onConnect(userdata, flags, rc):
    print("Connected with result code: " + str(rc))
    client.subscribe("esp32/#")


client = mqtt.Client()
client.on_connect = onConnect
client.on_message = onMessage


client.connect("192.58.125.4", 1883, 60)

