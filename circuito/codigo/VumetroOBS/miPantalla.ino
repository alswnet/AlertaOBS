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
  pantalla.clearDisplay();
  for (int i = 0; i < candidadAudios; i++) {
    // if(Audios[i].nivel != Audios[i].nivel_anterior
    dibujarBarra(i, Audios[i].nivel);
  }
  pantalla.display();
  delay(50);
}

void dibujarBarra(int i, int nivel) {
  int AltoMaximo = Alto_Pantalla - 4;
  int AltoBarra = map(nivel, -60, 0, 0, AltoMaximo);
  AltoBarra = constrain(AltoBarra, 0, AltoMaximo);
  pantalla.drawRoundRect(i * 21, 1, 16, AltoMaximo + 2, 4, WHITE);
  pantalla.fillRoundRect(i * 21 + 4, AltoMaximo - AltoBarra, 8, AltoMaximo, 4, WHITE);
}