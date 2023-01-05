let TopicBase = ["alsw/notificacion/#"];

function ConectarMQTT() {
  console.log(`Intentando conectar a MQTT ${DataMQTT.broker}`);

  // let BrokerMQTT = "wss://public:public@public.cloud.shiftr.io";
  let BrokerMQTT = `${DataMQTT.protocolo}://${DataMQTT.user}:${DataMQTT.pass}@${DataMQTT.broker}:${DataMQTT.puerto}`;

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

  let listaTopic = topic.split("/");
  let segmento = listaTopic.shift();
  print(`Topic[${topic}]: ${message}`);
  mensaje = message.toString();

  switch (segmento) {
    case "fondo":
      segmento = listaTopic.shift();
      switch (segmento) {
        case "color":
          print("color cambio");
          break;
      }
      break;
    case "alsw":
      segmento = listaTopic.shift();
      switch (segmento) {
        case "notificacion":
          MensajesNotificaciones(listaTopic, mensaje);
          break;
      }
      break;
    default:
      print(`No se encontro que hacer con ${segmento}`)
      break;

  }
}


function MensajesNotificaciones(listaTopic, mensaje) {
  segmento = listaTopic.shift();
  switch (segmento) {
    case "data":
      data = JSON.parse(mensaje);
      NombreMQTT = data.nombre;
      MensajeMQTT = data.mensaje;
      imagenMQTT = data.imagen;
      print(`Mensaje[${NombreMQTT}]: ${MensajeMQTT}`);
      // cargarImagen(imagenMQTT);
      Mensaje.actualizarMensaje(NombreMQTT, MensajeMQTT, imagenMQTT, false);
      break;
    case "dibujar":
      print(`Dibujar[${mensaje}]`);
      inicioConteo = -duracionMensaje;
      dibujarMensaje = (mensaje.toLowerCase() == "true");
      break;
    case "presente":
      data = JSON.parse(mensaje);
      NombreMQTT = data.nombre;
      Miembro = data.miembro;
      ID = data.id_youtube;
      imagenMQTT = data.imagen;
      print(`Presente ${NombreMQTT} - ${Miembro}`);
      Presente.agregar(ID, NombreMQTT, Miembro, imagenMQTT);
      break;
    case "presente_mostar":
      Presente.Activo = (mensaje.toLowerCase() == "true");
      print(`Presente Mostar[${Presente.Activo}]`);
      break;
    case "comando":
      segmento = listaTopic.shift();
      switch (segmento) {
        case "color":

          break;

      }

      break;
  }
}