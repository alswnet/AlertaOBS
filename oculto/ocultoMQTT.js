let TopicBase = ["alsw/chat/#"];

function ConectarMQTT() {
  console.log(`Intentando conectar a MQTT ${DataMQTT.broker}`);

  // let BrokerMQTT = "wss://public:public@public.cloud.shiftr.io";
  let BrokerMQTT = `${DataMQTT.protocolo}://${DataMQTT.user}:${DataMQTT.pass}@${DataMQTT.broker}:${DataMQTT.puerto}`;

  clientMQTT = mqtt.connect(BrokerMQTT, {
    clientId: "Oculto_" + floor(random(10000)),
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
  message = message.toString();
  print(topic, message);
  if (topic == "alsw/chat/mensajes") {
    // TODO: Trata antes de explote
    try {
      const data = JSON.parse(message);
      nombreMQTT = data.nombre;
      mensajeMQTT = data.texto;
      imagenMQTT = data.imagen;
      miembro = data.miembro;
      canal = data.canal;

      print(
        `Mensaje: ${nombreMQTT} - ${mensajeMQTT} -  ${miembro} - ${imagenMQTT} - ${canal}`
      );
      agregarMensaje("#cajaChat", nombreMQTT, mensajeMQTT, imagenMQTT, miembro, canal);
    } catch (error) {
      console.error(error);
      // print("Error con Json");
    }
    // dibujarMensaje = false;
    // inicioConteo = millis();
  } else if (topic == "alsw/chat/comando") {
    const data = JSON.parse(message);
    nombreMQTT = data.nombre;
    mensajeMQTT = data.texto;
    imagenMQTT = data.imagen;
    miembro = data.miembro;
    canal = data.canal;
    print(
      `Mensaje: ${nombreMQTT} - ${mensajeMQTT} -  ${miembro} - ${imagenMQTT} - ${canal}`
    );
    agregarMensaje("cajaComando", nombreMQTT, mensajeMQTT, imagenMQTT, miembro, canal);
  } else if (topic == "alsw/chat/donar") {
    const data = JSON.parse(message);
    nombreMQTT = data.nombre;
    mensajeMQTT = data.texto;
    imagenMQTT = data.imagen;
    miembro = data.miembro;
    canal = data.canal;
    print(
      `Mensaje: ${nombreMQTT} - ${mensajeMQTT} -  ${miembro} - ${imagenMQTT} - ${canal}`
    );
    agregarMensaje(
      "cajaDonaciones",
      nombreMQTT,
      mensajeMQTT,
      imagenMQTT,
      miembro
    );
  } else {
    print("Error no se pudo clasificar");
  }
}
