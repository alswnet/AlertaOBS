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

void configurarPantalla() {
  if (!pantalla.begin(SSD1306_SWITCHCAPVCC, Direccion_Pantalla)) {
    Serial.println(F("Error en pantalla OLED"));
    for (;;)
      ;
  }
}

void actualizarPantalla() {
  if ( estado == noWifi) {
    dibujarTriste();
  }
  else if (ConectadoOBS) {
    dibujarAudio();
  } else {
    dibujarBMO();
  }

  delay(50);
}

void dibujarTriste() {
  pantalla.clearDisplay();

  pantalla.fillCircle(Ancho_Pantalla / 2, Alto_Pantalla , 20, WHITE);
  pantalla.fillCircle(Ancho_Pantalla / 2, Alto_Pantalla + 2, 18, BLACK);
//  pantalla.fillRect(0, 0, Ancho_Pantalla, Alto_Pantalla / 2 , BLACK);

  pantalla.fillCircle(Ancho_Pantalla / 2 + 20, 20, 5, WHITE);
  pantalla.fillCircle(Ancho_Pantalla / 2 - 20, 20, 5, WHITE);

  pantalla.display();
}


void dibujarBMO() {
  pantalla.clearDisplay();

  pantalla.fillCircle(Ancho_Pantalla / 2, Alto_Pantalla / 2, 20, WHITE);
  pantalla.fillCircle(Ancho_Pantalla / 2, Alto_Pantalla / 2 - 2, 18, BLACK);
  pantalla.fillRect(0, 0, Ancho_Pantalla, Alto_Pantalla / 2 , BLACK);

  pantalla.fillCircle(Ancho_Pantalla / 2 + 20, 20, 5, WHITE);
  pantalla.fillCircle(Ancho_Pantalla / 2 - 20, 20, 5, WHITE);

  pantalla.display();
}

void dibujarAudio() {
  pantalla.clearDisplay();
  for (int i = 0; i < candidadAudios; i++) {
    dibujarBarra(i, Audios[i].nivel_mostar);
    if (Audios[i].nivel_mostar < Audios[i].nivel) {
      Audios[i].nivel_mostar = Audios[i].nivel;
    } else if (Audios[i].nivel_mostar > -200) {
      Audios[i].nivel_mostar = Audios[i].nivel_mostar - 4;
    }
  }
  pantalla.display();
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

  pantalla.drawRoundRect(PosicionX, 1, AnchoX, Alto_Pantalla - 1, 4, WHITE);

  if (AltoBarra > 0) {
    pantalla.fillRoundRect(PosicionX + BordeX, BordeY + PosicionY, AnchoX - 2 * BordeX, AltoBarra, 2, WHITE);
  }

  for (int i = 25; i <= 75; i = i + 25) {
    int Alto = map(i, 0, 100, 0, AltoMaximo) + BordeY;
    pantalla.drawLine(PosicionX + 1, Alto, PosicionX + AnchoX - 2, Alto, INVERSE);
  }
}
