# Creado ChepeCarlos de ALSW
# Tutorial Completo en https://nocheprogramacion.com
# Canal Youtube https://youtube.com/alswnet?sub_confirmation=1

import json
import random

import paho.mqtt.client as mqtt

# opciones = ["si", "no"]
# miembro = random.sample(opciones, len(opciones))
miembro = random.choice(["si", "no"])
mensaje = {"nombre": "CarlosCarlos", "texto": "Te amo aura", "imagen": "no", "miembro": miembro}
print(mensaje)

client = mqtt.Client()
client.username_pw_set("public", password="public")
client.connect("public.cloud.shiftr.io", port=1883, keepalive=60)
client.publish("alsw/chat/mensajes", json.dumps(mensaje))
