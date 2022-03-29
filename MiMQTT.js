let TopicBase = ["alsw/notificacion/#"];

function ConectarMQTT() {
  console.log(`Intentando conectar a MQTT ${DataMQTT.broker}`);

  // let BrokerMQTT = "wss://public:public@public.cloud.shiftr.io";
  let BrokerMQTT = `${DataMQTT.protocolo}://${DataMQTT.user}:${DataMQTT.pass}@${DataMQTT.broker}:${DataMQTT.puerto}`;
  print(BrokerMQTT);

  clientMQTT = mqtt.connect(BrokerMQTT, {
    clientId: "Alerta_OBS_" + floor(random(10000)),
  });

  clientMQTT.on("connect", ConectadoMQTT);
  clientMQTT.on("message", RecibirMensaje);
  clientMQTT.on("reconnect", ReconectarMQTT);
  clientMQTT.on("disconnect", DesconectandoMQTT);
  clientMQTT.on("error", ErrorMQTT);
}

function DesconectandoMQTT() {
  console.log(`Desconectado a MQTT`);
}

function ReconectarMQTT() {
  console.log(`Reconectando a MQTT ${DataMQTT.broker}`);
}

function ErrorMQTT(error) {
  console.log("Error" + error);
}

function ConectadoMQTT() {
  console.log(`Conectado a MQTT! ${DataMQTT.broker}`);
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
    imagenMQTT = data.imagen;
    print(`Topic[${topic}]: ${NombreMQTT} - ${MensajeMQTT}`);
    cargarImagen(imagenMQTT);
    // dibujarMensaje = false;
    // inicioConteo = millis();
  } else if (topic == "alsw/notificacion/dibujar") {
    let Mensaje = message.toString();
    print(`Mensaje ${Mensaje}`);
    inicioConteo = -duracionMensaje;
    if (Mensaje == "true") {
      dibujarMensaje = true;
    } else {
      dibujarMensaje = false;
    }
  } else if (topic == "alsw/notificacion/presente") {
    const data = JSON.parse(message.toString());
    NombreMQTT = data.nombre;
    Miembro = data.miembro;
    ID = data.id_youtube;
    imagenMQTT = data.imagen;
    print(`Topic[${topic}]: ${NombreMQTT} - ${Miembro}`);
    Presente.agregar(ID, NombreMQTT, Miembro, imagenMQTT);
    // cargarImagen(imagenMQTT);
    // Presente.
  } else if (topic == "alsw/notificacion/presente_mostar") {
    const data = JSON.parse(message.toString());
    Presente.Estado = data.estado;
    print("Estado Presentes " + Presente.Estado);
  } else {
    print("Error no se pudo clasificar");
  }
}
