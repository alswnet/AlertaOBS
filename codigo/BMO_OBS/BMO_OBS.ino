// ESP32 Wrover Module

template<class T> inline Print &operator<<(Print &obj, T arg) {
  obj.print(arg);
  return obj;
}

#if defined(ESP32)
//Librerias para ESP32
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>
WiFiMulti wifiMulti;

#elif defined(ESP8266)
//Librerias para ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
ESP8266WiFiMulti wifiMulti;

#endif

#include <Ticker.h>
#include <ArduinoOTA.h>
#include <MQTT.h>
#include <TelnetStream.h>
#include <Espalexa.h>
#include "data.h"

#define noWifi 0
#define noMQTT 1
#define conectado 2

struct estado {
  bool actual;
  bool anterior;
};

estado estadoDespierto = { true, true };
estado estadoWifi = { false, false };
estado estadoAlexa = { false, false };

Ticker cambiarLed;

int ledEstado = 2;
boolean EstadoLed = false;
int estado = noWifi;
int estadoAnterior = -1;

struct Audio {
  String nombre;
  int nivel;
  int nivel_mostar;
};

#define candidadAudios 6
Audio Audios[candidadAudios] = {
  { "1_Mic_Limpio", -200, -201 },
  { "2_Mic_Audiencia", -200, -201 },
  { "3_UmaruRed", -200, -201 },
  { "4_Firefox", -200, -201 },
  { "5_ElGatoALSW", -200, -201 },
  { "6_SoloOBS", -200, -201 }
};

boolean ConectadoOBS = false;
boolean ConectadoPC = false;

void funcionLed() {
  EstadoLed = !EstadoLed;
  digitalWrite(ledEstado, EstadoLed ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(ledEstado, OUTPUT);

  configurarPantalla();
  actualizarEstado();
  conectarWifi();

  xTaskCreatePinnedToCore(
    procesoAlexa,   /* Nombre de la funcion */
    "procesoAlexa", /* Nombre del proceso  */
    10000,          /* Tamano de palabra */
    NULL,           /* parametros de entrada */
    9,              /* Prioridas del proceso */
    NULL,           /* Manejo del proceso  */
    1);
}

void loop() {
  actualizarWifi();
  actualizarEstado();
  actualizarPantalla();
  actualizarSerial();
  LeerTelnet();
}

void actualizarSerial() {
  while (Serial.available()) {
    char Letra = Serial.read();
    mensajeSerial(Letra, Serial);
  }
}

void mensajeSerial(char mensaje, Stream &miSerial) {
  switch (mensaje) {
    case 'e':
    case 'E':
      miSerial << "Estado: ";
      switch (estado) {
        case noWifi:
          miSerial << "No Wifi";
          break;
        case noMQTT:
          miSerial << "No MQTT";
          break;
        case conectado:
          miSerial << "Conectado MQTT";
          break;
      }
      miSerial << ", OBS: " << (ConectadoOBS ? "Conectado" : "Desconectado") << "\n";
      break;
  }
}

void actualizarEstado() {
  if (estado != estadoAnterior) {
    Serial.print("Cambiando Estado ");
    Serial.println(estado);

    TelnetStream.print("Cambiando Estado ");
    TelnetStream.println(estado);

    estadoAnterior = estado;

    switch (estado) {
      case noWifi:
        cambiarLed.attach(0.1, funcionLed);
        break;
      case noMQTT:
        cambiarLed.attach(1, funcionLed);
        break;
      case conectado:
        cambiarLed.attach(2, funcionLed);
        break;
    }
  }
}
