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
#include "data.h"

#define noWifi 0
#define noMQTT 1
#define conectado 2

Ticker cambiarLed;

#define CantidadLampara 3
const int PinLampara[CantidadLampara] = { 4, 5, 13 };
boolean EstadoLampara = false;
boolean EstadosLampara[CantidadLampara] = { false, false, false };
boolean EstadosLamparaAnterior[CantidadLampara] = { true, true, true };

int ledEstado = 2;
boolean EstadoLed = false;
int estado = noWifi;
int estadoAnterior = -1;

void funcionLed() {
  EstadoLed = !EstadoLed;
  digitalWrite(ledEstado, EstadoLed ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(ledEstado, OUTPUT);

  cargarEstado();
  actualizarEstado();
  configurarReles();
  conectarWifi();
}

void loop() {
  actualizarWifi();
  actualizarRele();
  actualizarEstado();
  LeerTelnet();
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
