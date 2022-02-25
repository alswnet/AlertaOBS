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
  var TextNombre = select("#TextNombre").value().toLowerCase();
  var TextMensaje = select("#TextMensaje").value().toLowerCase();
  console.log(`Enviando: ${TextNombre} - ${TextMensaje}`);
  var data = {
    nombre: TextNombre,
    mensaje: TextMensaje,
  };

  clientMQTT.publish(`${temaBase}/data`, JSON.stringify(data));
}
