# Creado ChepeCarlos de ALSW
# Tutorial Completo en https://nocheprogramacion.com
# Canal Youtube https://youtube.com/alswnet?sub_confirmation=1

import paho.mqtt.client as mqtt
from decouple import config


def on_connect(client, userdata, flags, rc):
    print("Se conecto con mqtt " + str(rc))
    client.subscribe("alsw/#")


def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set(config("user"), password=config("pass"))
client.connect(config("broker"), port=int(config("puerto")), keepalive=60)

client.loop_forever()
