console.log("Iniciando sistema");

let colorFondo;
let BrokerMQTT = "wss://public:public@public.cloud.shiftr.io";
let NombreMQTT = "Carlos";
let MensajeMQTT = "Mensaje oculto";
let dibujarMensaje = false;
let duracionMensaje = 6000;
let inicioConteo = 0;

function setup() {
  colorFondo = color(0, 255, 0);
  createCanvas(windowWidth, windowHeight);
  ConectarMQTT();
  inicioConteo = millis();
}

function draw() {
  background(colorFondo);
  if (millis() - inicioConteo < duracionMensaje) {
    textSize(32);
    fill(200);
    rect(10, 30, 500, 100);
    fill(0);
    text(`Nombre: ${NombreMQTT} `, 20, 60);
    text(`Mensaje: ${MensajeMQTT} `, 20, 110);
  }
}
