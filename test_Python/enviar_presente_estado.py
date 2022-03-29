# Creado ChepeCarlos de ALSW
# Tutorial Completo en https://nocheprogramacion.com
# Canal Youtube https://youtube.com/alswnet?sub_confirmation=1

import json
import random

import paho.mqtt.client as mqtt

Estado = random.choice([True, False])
IdRandom = random.randrange(10, 100, 1)
mensaje = {"estado": Estado}
print(mensaje)

client = mqtt.Client()
client.username_pw_set("public", password="public")
# client.connect("test.mosquitto.org", port=1883, keepalive=60)
client.connect("public.cloud.shiftr.io", port=1883, keepalive=60)
client.publish("alsw/notificacion/presente_mostar", json.dumps(mensaje))
