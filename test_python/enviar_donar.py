# Creado ChepeCarlos de ALSW
# Tutorial Completo en https://nocheprogramacion.com
# Canal Youtube https://youtube.com/alswnet?sub_confirmation=1

import json
import random

from decouple import config
import paho.mqtt.client as mqtt


topic = "alsw/chat/donar"
miembro = random.choice([True, False])
mensaje = {"nombre": "CarlosCarlos", "texto": "$1.2 Dolares", "imagen": "no", "miembro": miembro}

client = mqtt.Client()
client.username_pw_set(config("user"), password=config("pass"))
client.connect(config("broker"), port=int(config("puerto")), keepalive=60)
client.publish(topic, json.dumps(mensaje))
