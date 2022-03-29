console.log("Iniciando sistema");

let NombreMQTT = "ChepeCarlos";
let MensajeMQTT = "Amo a Lila <3";
// MensajeMQTT =
//   "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus a neque vel odio luctus tincidunt sit amet ut neque. Praesent quis leo feugiat elit accumsan accumsan. Donec accumsan luctus est, ut posuere libero vehicula";
// MensajeMQTT =
//   "Hola mundo los quiero mucho este lindo dia, quiero estar con ustedes , Les quiero compartir algo muy interesante . sobre mqtt y Arduino es especial algo que puede ayudarte en proyectos de MQTT 😀 en el futuro";
let ImagenMQTT;
let imagenCargada = false;
let noImagen = false;
let Imagen =
  "https://yt4.ggpht.com/hyfbUP1SmorOKU6Rk0vMTHyz1CBrAc8dPSlg3Z3aMgiAcENaqF_EQxIr2eZryXPjB08cIzXIRQ=s64-c-k-c0x00ffffff-no-rj";

let dibujarMensaje = false;
let duracionMensaje = 6000;
let inicioConteo = -duracionMensaje;
// let ImagenFondo;
let ArchivoMQTT = "token/mqtt.json";
let DataMQTT;
let Presente;

let FuenteRoboto;

function preload() {
  // ImagenFondo = loadImage("Fondo.png");
  FuenteRoboto = loadFont("fuente/Roboto-Black.ttf");
  DataMQTT = loadJSON(ArchivoMQTT);
}

function setup() {
  createCanvas(windowWidth, windowHeight);
  ConectarMQTT();
  inicioConteo = millis();
  cargarImagen(Imagen);
  Presente = new AlertaPresente();
}

function draw() {
  clear();
  // image(ImagenFondo, 0, 0, width, height);

  if (!imagenCargada) return;

  if (millis() - inicioConteo < duracionMensaje || dibujarMensaje) {
    let Posicion = createVector(width / 15, height / 80);
    mostrarTexto(Posicion, NombreMQTT, MensajeMQTT, ImagenMQTT);
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

function mostrarTexto(Posicion, Nombre, Mensaje, Perfil) {
  let AzulOscuro = "#001427";
  let Naranja = "#ff3a20";
  let Acua = "#008080";
  let ColorTexto = "#fff";
  let ColorFondo = color(50);
  let altoNombre = 20;
  let altoMensaje = altoNombre + 7;
  let Borde = altoNombre;
  let DesfaceFondo = 4;
  let AnchoMaximo = width - 2 * Posicion.x;
  let AltoMaximo = 75;
  let Separador = 10;

  push();
  translate(Posicion.x, Posicion.y);
  textFont(FuenteRoboto);

  stroke(AzulOscuro);
  strokeWeight(3);
  fill(AzulOscuro);
  if (noImagen) {
    rect(DesfaceFondo, DesfaceFondo, AltoMaximo, AltoMaximo);
    image(Perfil, 0, 0, AltoMaximo, AltoMaximo);
    AnchoMaximo = AnchoMaximo - AltoMaximo;
    translate(AltoMaximo + Separador, 0);
  }

  textSize(altoNombre);
  let anchoNombre = textWidth(Nombre);

  fill(AzulOscuro);
  rect(
    DesfaceFondo * 2,
    DesfaceFondo,
    anchoNombre + Borde,
    altoNombre + Borde / 2,
    Borde / 4
  );
  fill(Naranja);
  rect(0, 0, anchoNombre + Borde, altoNombre + Borde / 2, Borde / 4);
  noStroke();
  fill(ColorTexto);
  text(Nombre, Borde / 2, altoNombre + Borde / 8);
  textSize(altoMensaje);
  let anchoMensaje = textWidth(Mensaje);
  let Lineas = int(anchoMensaje / AnchoMaximo);
  translate(0, altoNombre + Borde / 2 + Separador);
  if (anchoMensaje > AnchoMaximo) {
    anchoMensaje = AnchoMaximo;
  }
  stroke(AzulOscuro);
  strokeWeight(3);
  fill(AzulOscuro);
  rect(
    DesfaceFondo + 2,
    DesfaceFondo,
    anchoMensaje + Borde,
    (Lineas + 1) * altoMensaje + 10,
    5
  );
  fill(Acua);
  rect(0, 0, anchoMensaje + Borde, (Lineas + 1) * altoMensaje + 10, 5);
  fill(ColorTexto);
  textLeading(altoMensaje);
  text(Mensaje, Borde / 2, altoMensaje + Borde / 8, AnchoMaximo);

  pop();
}

function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
}
