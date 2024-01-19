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

#include <ArduinoOTA.h>

#include <TelnetStream.h>
#include "data.h"

#define noWifi 0
#define noMQTT 1
#define conectado 2


struct Rele {
  String nombre;
  int pin;
  boolean Estado;
  boolean Estado_Anterior;
  boolean Invertido;
};

#define CantidadAparatos 3


Rele Aparatos[CantidadAparatos] = {
  { "lampara izquierda", 4, false, true, false },
  { "lampara centro", 5, false, true, false },
  { "lampara derecha", 15, false, true, true }
};

boolean EstadoAparato = false;

int estado = noWifi;
int estadoAnterior = -1;

void setup() {
  Serial.begin(115200);
  configurarLed();
  cargarEstado();
  conectarWifi();

  configurarAlexa();
  ConfigurarMQTT();
  actualizarEstado();
  configurarReles();
}

void loop() {
  actualizarAlexa();
  actualizarMQTT();
  delay(10);

  actualizarWifi();
  actualizarRele();
  actualizarEstado();
  LeerTelnet();
}
