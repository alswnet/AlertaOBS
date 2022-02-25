console.log("Iniciando sistema");

let colorFondo;
let BrokerMQTT = "wss://public:public@public.cloud.shiftr.io";
let NombreMQTT = "ChepeCarlos";
let MensajeMQTT = "Amo a Lila <3 💕";
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
  if (millis() - inicioConteo < duracionMensaje || dibujarMensaje) {
    textSize(32);
    fill(200);
    rect(10, 30, 500, 100, 30);
    fill(0);
    text(`${NombreMQTT} `, 25, 60);
    text(`${MensajeMQTT} `, 25, 110);
  }
}
