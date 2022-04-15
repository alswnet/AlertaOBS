# Creado ChepeCarlos de ALSW
# Tutorial Completo en https://nocheprogramacion.com
# Canal Youtube https://youtube.com/alswnet?sub_confirmation=1

import json
import random

import paho.mqtt.client as mqtt
from decouple import config

miembro = random.choice([True, False])
IdRandom = random.randrange(10, 100, 1)
mensaje = {"nombre": "CarlosCarlos", "id_youtube": f"ID_{IdRandom}", "miembro": miembro, "imagen": "no"}
print(mensaje)

client = mqtt.Client()
client.username_pw_set(config("user"), password=config("pass"))
client.connect(config("broker"), port=int(config("puerto")), keepalive=60)
client.publish("alsw/notificacion/presente", json.dumps(mensaje))
