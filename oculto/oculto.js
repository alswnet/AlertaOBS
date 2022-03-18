console.log("Meno oculto, no le digas a nadie");

let BrokerMQTT = "wss://public:public@public.cloud.shiftr.io";
let temaBase = "alsw/notificacion";
let clientMQTT;
let duracionMensaje = 6000;

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

  // agregarMensaje(TextNombre, TextMensaje, "hola");

  console.log(`Enviando: ${TextNombre} - ${TextMensaje}`);
  var data = {
    nombre: TextNombre,
    mensaje: TextMensaje,
    imagen: "no",
    miembro: "no",
  };

  clientMQTT.publish(`${temaBase}/data`, JSON.stringify(data));
  data = {
    nombre: TextNombre,
    texto: TextMensaje,
    imagen: "no",
    miembro: "no",
  };
  clientMQTT.publish(`alsw/chat/mensajes`, JSON.stringify(data));
}

function agregarMensaje(padre, nombre, mensaje, imagen, miembro) {
  // let cajaChat = select("#cajaChat");
  // let mensajeHTML = createDiv(`${nombre} - ${mensaje}`);
  let mensajeHTML = createDiv();
  mensajeHTML.addClass("mensaje");
  if (miembro == "si") {
    mensajeHTML.addClass("miembro");
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
  mensajeHTML.parent(padre);
  // cajaChat.child(mensajeHTML);
}

function enviarMensaje() {
  print(`Enviando por MQTT ${this.nombreMQTT} - ${this.mensajeMQTT}`);
  let data = {
    nombre: this.nombreMQTT,
    mensaje: this.mensajeMQTT,
    imagen: this.imagenMQTT,
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
