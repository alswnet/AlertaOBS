# Creado ChepeCarlos de ALSW
# Tutorial Completo en https://nocheprogramacion.com
# Canal Youtube https://youtube.com/alswnet?sub_confirmation=1

import json
import random

from decouple import config
import paho.mqtt.client as mqtt

topic = "alsw/chat/comando"
miembro = random.choice([True, False])
mensaje = {"nombre": "CarlosCarlos", "texto": "color rojo", "imagen": "no", "miembro": miembro}
print(topic, mensaje)

client = mqtt.Client()
client.username_pw_set(config("user"), password=config("pass"))
client.connect(config("broker"), port=int(config("puerto")), keepalive=60)
client.publish(topic, json.dumps(mensaje))
