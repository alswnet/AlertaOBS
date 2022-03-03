console.log("Iniciando sistema");

let colorFondo;
let BrokerMQTT = "wss://public:public@public.cloud.shiftr.io";
let NombreMQTT = "ChepeCarlos";
let MensajeMQTT = "Amo a Lila <3 💕";
let dibujarMensaje = false;
let duracionMensaje = 6000;
let inicioConteo = -duracionMensaje;

function setup() {
  colorFondo = color(0, 255, 0);
  createCanvas(windowWidth, windowHeight);
  ConectarMQTT();
  inicioConteo = millis();
}

function draw() {
  background(colorFondo);
  if (millis() - inicioConteo < duracionMensaje || dibujarMensaje) {
    mostrarTexto(NombreMQTT, MensajeMQTT);
    // textSize(32);
    // fill(200);
    // rect(10, 30, 500, 100, 30);
    // fill(0);
    // text(`${NombreMQTT} `, 25, 60, 400);
    // text(`${MensajeMQTT} `, 25, 110, 400);
  }
  // mostrarTexto("ChepeCarlos", "Hola Mundo");
}

function mostrarTexto(Nombre, Mensaje) {
  let ColorTexto = color(200);
  let ColorFondo = color(50);
  let altoMensaje = 28;
  let altoNombre = 20;
  push();
  translate(width / 2, height * 0.2);
  rectMode(CENTER, CENTER);
  textAlign(CENTER, CENTER);

  textSize(altoMensaje);
  let anchoMensaje = textWidth(Mensaje);

  fill(ColorTexto);
  rect(0, 0, anchoMensaje + 30, altoMensaje + 10, 5);
  fill(ColorFondo);
  text(Mensaje, 0, 0);

  textSize(altoNombre);
  let anchoNombre = textWidth(Nombre);
  let xNombre = -anchoMensaje / 2 + anchoNombre / 2;
  let yNombre = -20 - altoMensaje / 2 - altoNombre / 2;

  fill(ColorTexto);
  rect(xNombre, yNombre, anchoNombre + 30, altoNombre + 10, 5);
  fill(ColorFondo);
  text(Nombre, xNombre, yNombre);

  pop();
}
