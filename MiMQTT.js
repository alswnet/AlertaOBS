let TopicBase = ["alsw/notificacion/#"];

function ConectarMQTT() {
  console.log("Intentando conectar a MQTT");
  clientMQTT = mqtt.connect(BrokerMQTT, {
    clientId: "Fondo_OBS_" + floor(random(10000)),
  });

  clientMQTT.on("connect", ConectadoMQTT);
  clientMQTT.on("message", RecibirMensaje);
}

function ConectadoMQTT() {
  console.log(`Conectado a MQTT!`);
  for (let i = 0; i < TopicBase.length; i++) {
    console.log(`Subscribiéndose a ${TopicBase[i]}`);
    clientMQTT.subscribe(TopicBase[i]);
  }
}

function RecibirMensaje(topic, message) {
  topic = topic.toString();
  if (topic == "alsw/notificacion/data") {
    // TODO: Trata antes de explote
    const data = JSON.parse(message.toString());
    NombreMQTT = data.nombre;
    MensajeMQTT = data.mensaje;
    print(`Topic[${topic}]: ${NombreMQTT} - ${MensajeMQTT}`);
    dibujarMensaje = false;
    inicioConteo = millis();
  } else if (topic == "alsw/notificacion/dibujar") {
    let Mensaje = message.toString();
    print(`Mensaje ${Mensaje}`);
    inicioConteo = -duracionMensaje;
    if (Mensaje == "true") {
      dibujarMensaje = true;
    } else {
      dibujarMensaje = false;
    }
  } else {
    print("Error no se pudo clasificar");
  }
}
