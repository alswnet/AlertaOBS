console.log("Meno oculto, no le digas a nadie");

let temaBase = "alsw/notificacion";
let clientMQTT;
let duracionMensaje = 6000;

let ArchivoMQTT = "../token/mqtt.json";
let DataMQTT;

function preload() {
  DataMQTT = loadJSON(ArchivoMQTT);
  print(DataMQTT);
}

function setup() {
  noCanvas();

  ConectarMQTT();

  let BotonEnviar = select("#BotonEnviar");
  BotonEnviar.mousePressed(FuncionEnviar);

  let BotonMostar = select("#BotonMostar");
  BotonMostar.mousePressed(FuncionMostar);

  let BotonOcultar = select("#BotonOcultar");
  BotonOcultar.mousePressed(FuncionOcultar);

  let BotonLimpiar = select("#BotonLimpiar");
  BotonLimpiar.mousePressed(FuncionLimpiar);

  let BotonPresenteMostar = select("#BotonPresenteMostar");
  BotonPresenteMostar.mousePressed(FuncionPresenteMostar);

  let BotonPresenteOcultar = select("#BotonPresenteOcultar");
  BotonPresenteOcultar.mousePressed(FuncionPresenteOcultar);
}

function FuncionMostar() {
  print(`Enviando: True`);
  clientMQTT.publish(`${temaBase}/dibujar`, "true");
}

function FuncionOcultar() {
  print(`Enviando: False`);
  clientMQTT.publish(`${temaBase}/dibujar`, "false");
}

function FuncionEnviar() {
  var TextNombre = select("#TextNombre").value();
  var TextMensaje = select("#TextMensaje").value();

  console.log(`Enviando: ${TextNombre} - ${TextMensaje}`);
  var data = {
    nombre: TextNombre,
    mensaje: TextMensaje,
    imagen: "no",
    miembro: false,
  };

  clientMQTT.publish(`${temaBase}/data`, JSON.stringify(data));
  data = {
    nombre: TextNombre,
    texto: TextMensaje,
    imagen: "no",
    miembro: false,
  };
  clientMQTT.publish(`alsw/chat/mensajes`, JSON.stringify(data));
}

function FuncionPresenteMostar() {
  print(`Presente: True`);
  clientMQTT.publish(`${temaBase}/presente_mostar`, "true");
}

function FuncionPresenteOcultar() {
  print(`Presente: True`);
  clientMQTT.publish(`${temaBase}/presente_mostar`, "false");
}

function agregarMensaje(padre, nombre, mensaje, imagen, miembro, canal=null) {
  // let cajaChat = select("#cajaChat");
  // let mensajeHTML = createDiv(`${nombre} - ${mensaje}`);
  let mensajeHTML = createDiv();
  mensajeHTML.addClass("mensaje");
  if (miembro) {
    mensajeHTML.addClass("miembro");
  }

  if (canal == "youtube") {
    mensajeHTML.addClass("youtube");
  } else if (canal == "tiktok") {
    mensajeHTML.addClass("tiktok");
  }

  let mensajeNombre = createDiv(`${nombre}`);
  mensajeNombre.addClass("nombre");
  let mensajeTexto = createDiv(`${mensaje}`);
  mensajeTexto.addClass("texto");
  mensajeHTML.child(mensajeNombre);
  mensajeHTML.child(mensajeTexto);

  mensajeHTML.mousePressed(enviarMensaje);
  mensajeHTML.nombreMQTT = nombre;
  mensajeHTML.mensajeMQTT = mensaje;
  mensajeHTML.imagenMQTT = imagen;
  mensajeHTML.canalMQTT = canal;
  mensajeHTML.parent(padre);

  let pollo = document.getElementById('cajaChat');
  pollo.scrollTop = pollo.scrollHeight;
  // cajaChat.child(mensajeHTML);
}

function enviarMensaje() {
  print(`Enviando por MQTT ${this.nombreMQTT} - ${this.mensajeMQTT}`);
  let data = {
    nombre: this.nombreMQTT,
    mensaje: this.mensajeMQTT,
    imagen: this.imagenMQTT,
    canal: this.canalMQTT,
  };
  clientMQTT.publish(`${temaBase}/data`, JSON.stringify(data));
}

function FuncionLimpiar() {
  print("Borrar mensajes");
  Mensajes = selectAll(".mensaje");
  Mensajes.forEach((element) => {
    element.remove();
  });
}
