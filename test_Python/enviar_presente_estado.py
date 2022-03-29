# Creado ChepeCarlos de ALSW
# Tutorial Completo en https://nocheprogramacion.com
# Canal Youtube https://youtube.com/alswnet?sub_confirmation=1
import json
import random

import paho.mqtt.client as mqtt
from decouple import config

Estado = random.choice([True, False])
IdRandom = random.randrange(10, 100, 1)
mensaje = {"estado": Estado}
print(mensaje)

client = mqtt.Client()
client.username_pw_set(config("user"), password=config("pass"))
client.connect(config("broker"), port=int(config("puerto")), keepalive=60)
client.publish("alsw/notificacion/presente_mostar", json.dumps(mensaje))
