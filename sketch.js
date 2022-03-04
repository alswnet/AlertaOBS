console.log("Iniciando sistema");

let BrokerMQTT = "wss://public:public@public.cloud.shiftr.io";
let NombreMQTT = "ChepeCarlos";
let MensajeMQTT = "Amo a Lila <3 💕";
let ImagenMQTT;
let imagenCargada = false;
let noImagen = false;
let Imagen =
  "https://yt4.ggpht.com/hyfbUP1SmorOKU6Rk0vMTHyz1CBrAc8dPSlg3Z3aMgiAcENaqF_EQxIr2eZryXPjB08cIzXIRQ=s64-c-k-c0x00ffffff-no-rj";

let dibujarMensaje = false;
let duracionMensaje = 6000;
let inicioConteo = -duracionMensaje;

function setup() {
  createCanvas(windowWidth, windowHeight);
  ConectarMQTT();
  inicioConteo = millis();
  cargarImagen(Imagen);
  // mostrarTexto(NombreMQTT, MensajeMQTT, Imagen);
}

function draw() {
  clear();

  if (!imagenCargada) return;

  if (millis() - inicioConteo < duracionMensaje || dibujarMensaje) {
    mostrarTexto(NombreMQTT, MensajeMQTT, ImagenMQTT);
  }
}

function cargarImagen(imageUrl) {
  imagenCargada = false;
  noImagen = true;
  if (imageUrl == "no") {
    imagenCargada = true;
    inicioConteo = millis();
    noImagen = false;
    return;
  }

  loadImage(imageUrl, (img) => {
    print(`Cargando ${imageUrl}`);
    ImagenMQTT = img;
    imagenCargada = true;
    inicioConteo = millis();
  });
}

function mostrarTexto(Nombre, Mensaje, Perfil) {
  let ColorTexto = color(200);
  let ColorFondo = color(50);
  let altoMensaje = 28;
  let altoNombre = 20;
  push();
  translate(width / 2, height * 0.2);
  rectMode(CENTER, CENTER);
  textAlign(CENTER, CENTER);
  imageMode(CENTER);

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

  if (noImagen) {
    let xPerfil = anchoMensaje / 2 + anchoNombre / 2;
    let yPerfil = altoMensaje / 2 + altoNombre / 2;

    image(Perfil, -xPerfil, -yPerfil);
  }

  pop();
}
