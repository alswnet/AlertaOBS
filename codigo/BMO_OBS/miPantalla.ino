#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define Ancho_Pantalla 128
#define Alto_Pantalla 64
#define Direccion_Pantalla 0x3C
#define Reset_Pantalla -1

Adafruit_SSD1306 pantalla(Ancho_Pantalla, Alto_Pantalla, &Wire, Reset_Pantalla);

Ticker cambiarPantalla;

Ticker cambiarRectangulo;
int contadorRectangulo = 0;
int cantidadRectangulo = 8;
float tiempoRectangulo = 0.15;

void actualizarPantalla() {

  if (estadoDespierto.actual != estadoDespierto.anterior) {
    estadoDespierto.anterior = estadoDespierto.actual;
    if (estadoDespierto.actual) {
      Serial.println("BMO[Despierto]");
    } else {
      Serial.println("BMO[Dormido]");
    }
  }

  if (estadoDespierto.actual) {
    if (estado == noWifi) {
      dibujarTriste();
      dibujarVivo();
    } else if (ConectadoOBS) {
      dibujarAudio();
    } else if (ConectadoPC) {
      dibujarBMO();
      dibujarVivo();
    } else {
      dibujarNormal();
      dibujarVivo();
    }
  } else {
    limpiarPantalla();
  }

  pantalla.display();
  delay(50);
}

void dibujarTriste() {
  pantalla.clearDisplay();

  pantalla.fillCircle(Ancho_Pantalla / 2, Alto_Pantalla, 20, WHITE);
  pantalla.fillCircle(Ancho_Pantalla / 2, Alto_Pantalla + 2, 18, BLACK);

  pantalla.fillCircle(Ancho_Pantalla / 2 + 20, 20, 5, WHITE);
  pantalla.fillCircle(Ancho_Pantalla / 2 - 20, 20, 5, WHITE);
}

void dibujarNormal() {
  pantalla.clearDisplay();

  pantalla.fillCircle(Ancho_Pantalla / 2 + 20, 20, 5, WHITE);
  pantalla.fillCircle(Ancho_Pantalla / 2 - 20, 20, 5, WHITE);

  pantalla.fillRect(Ancho_Pantalla / 2 - Ancho_Pantalla / 6, Alto_Pantalla / 2 + 10, Ancho_Pantalla / 3, Alto_Pantalla / 10, WHITE);
}

void dibujarBMO() {
  pantalla.clearDisplay();

  pantalla.fillCircle(Ancho_Pantalla / 2, Alto_Pantalla / 2, 20, WHITE);
  pantalla.fillCircle(Ancho_Pantalla / 2, Alto_Pantalla / 2 - 2, 18, BLACK);
  pantalla.fillRect(0, 0, Ancho_Pantalla, Alto_Pantalla / 2, BLACK);

  pantalla.fillCircle(Ancho_Pantalla / 2 + 20, 20, 5, WHITE);
  pantalla.fillCircle(Ancho_Pantalla / 2 - 20, 20, 5, WHITE);
}

void dibujarVivo() {
  pantalla.fillRect(0, Alto_Pantalla - 3, Ancho_Pantalla, 2, WHITE);
  pantalla.fillRect(0, 0, Ancho_Pantalla, 2, WHITE);

  pantalla.fillRect(0, 0, 2, Alto_Pantalla, WHITE);
  pantalla.fillRect(Ancho_Pantalla - 3, 0, 2, Alto_Pantalla, WHITE);

  int cantidadX = cantidadRectangulo;
  pantalla.fillRect((contadorRectangulo * Ancho_Pantalla) / (cantidadX - 1), 0,
                    Ancho_Pantalla / cantidadX, 2, INVERSE);
  pantalla.fillRect((((cantidadX - 1) - contadorRectangulo) * Ancho_Pantalla) / cantidadX, Alto_Pantalla - 3,
                    Ancho_Pantalla / cantidadX, 2, INVERSE);

  // TODO Mejorar lineas vertiacles
  // int cantidadY = cantidadRectangulo / 4;
  // int iY = contadorRectangulo / 4;
  // pantalla.fillRect(0, (iY * Alto_Pantalla) / (cantidadY - 1),
  //                   2, Alto_Pantalla / cantidadY, INVERSE);
  // pantalla.fillRect(Ancho_Pantalla - 3, (((cantidadY - 1) - iY) * Alto_Pantalla) / (cantidadY - 1),
  //                   2, Alto_Pantalla / cantidadY, INVERSE);
}

void dibujarAudio() {

  if (estadoGrabando.actual != estadoGrabando.anterior) {
    estadoGrabando.anterior = estadoGrabando.actual;
    if (estadoGrabando.actual) {
      Serial.println("BMO[Grabando]");
    } else {
      Serial.println("BMO[NoGrabando]");
    }
  }

  pantalla.clearDisplay();

  for (int i = 0; i < candidadAudios; i++) {
    dibujarBarra(i, Audios[i].nivel_mostar);
    if (Audios[i].nivel_mostar < Audios[i].nivel) {
      Audios[i].nivel_mostar = Audios[i].nivel;
    } else if (Audios[i].nivel_mostar > -200) {
      Audios[i].nivel_mostar = Audios[i].nivel_mostar - 4;
    }
  }
}

void dibujarBarra(int i, int nivel) {
  int PosicionX = i * 21;
  int BordeX = 4;
  int AnchoX = 16;
  int BordeY = 3;

  int AltoMaximo = Alto_Pantalla - BordeY * 2;
  int AltoBarra = map(nivel, -60, 0, 0, AltoMaximo);
  AltoBarra = constrain(AltoBarra, 0, AltoMaximo);
  int PosicionY = AltoMaximo - AltoBarra;

  if (estadoGrabando.actual) {
    pantalla.drawRoundRect(PosicionX, 1, AnchoX, Alto_Pantalla - 1, 4, WHITE);
  }

  if (AltoBarra > 0) {
    pantalla.fillRoundRect(PosicionX + BordeX, BordeY + PosicionY, AnchoX - 2 * BordeX, AltoBarra, 2, WHITE);
  }

  for (int i = 25; i <= 75; i = i + (estadoGrabando.actual ? 25 : 10)) {
    int Alto = map(i, 0, 100, 0, AltoMaximo) + BordeY;
    pantalla.drawLine(PosicionX + 1, Alto, PosicionX + AnchoX - 2, Alto, INVERSE);
  }
}


void configurarPantalla() {
  if (!pantalla.begin(SSD1306_SWITCHCAPVCC, Direccion_Pantalla)) {
    Serial.println(F("Error en pantalla OLED"));
    for (;;)
      ;
  }
  cambiarRectangulo.attach(tiempoRectangulo, actualizarRectangulo);
}

void actualizarRectangulo() {

  contadorRectangulo++;
  if (contadorRectangulo >= cantidadRectangulo) {
    contadorRectangulo = 0;
  }
}

void limpiarPantalla() {
  pantalla.clearDisplay();
}
