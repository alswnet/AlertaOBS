console.log("Meno oculto, no le digas a nadie");

let BrokerMQTT = "wss://public:public@public.cloud.shiftr.io";
let temaBase = "alsw/notificacion";
let clientMQTT;

function setup() {
  noCanvas();

  ConectarMQTT();

  let BotonEnviar = select("#BotonEnviar");
  BotonEnviar.mousePressed(BotonTextBox);
}

function BotonTextBox() {
  var TextNombre = select("#TextNombre").value().toLowerCase();
  var TextMensaje = select("#TextMensaje").value().toLowerCase();
  console.log(`Enviando: ${TextNombre} - ${TextMensaje}`);
  var data = {
    nombre: TextNombre,
    mensaje: TextMensaje,
  };

  clientMQTT.publish(`${temaBase}/data`, JSON.stringify(data));
}
