template<class T> inline Print &operator <<(Print &obj, T arg) {
  obj.print(arg);
  return obj;
}

#if defined(ESP32)
//Librerias para ESP32
#include <WiFi.h>
#include <WiFiMulti.h>
WiFiMulti wifiMulti;

#elif defined(ESP8266)
//Librerias para ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;

#endif

#include <MQTTPubSubClient.h>
#include <Ticker.h>

#include "data.h"

Ticker cambiarLed;
int LedEstado = 2;
boolean EstadoLed = false;

const uint32_t TiempoEsperaWifi = 5000;

WiFiClient client;
MQTTPubSubClient mqtt;



#define CantidadLampara 3
const int PinLampara[CantidadLampara] = {4, 5, 15};
boolean EstadoLampara = false;
boolean EstadosLampara[CantidadLampara] = {false,  false, false};


void FuncionLed() {
  EstadoLed = !EstadoLed;
  digitalWrite(LedEstado, EstadoLed);
}

void MensajeEstado1(const String & payload, const size_t size) {
  MensajeEstadoIndividual(0, payload );
}

void MensajeEstado2(const String & payload, const size_t size) {
  MensajeEstadoIndividual(1, payload);
}

void MensajeEstado3(const String & payload, const size_t size) {
  MensajeEstadoIndividual(2, payload);
}

void MensajeEstadoIndividual(int i, const String payload) {
  if (payload.equals("cambiar")) {
    EstadosLampara[i] = !EstadosLampara[i];
  } else if (payload.equals("encender")) {
    EstadosLampara[i] = true;
  } else {
    EstadosLampara[i] = false;
  }
  Serial << "Cambiar Lampara[" << i << "]: " << (EstadosLampara[i] ? "Encender" : "Apagado") << "\n";
  digitalWrite(PinLampara[i], EstadosLampara[i]);
}



void MensajeEstado(const String & payload, const size_t size) {
  if (payload.equals("cambiar")) {
    boolean Igual = true;
    for (int i = 1; i < CantidadLampara - 1; i++) {
      if (EstadosLampara[0] != EstadosLampara[i]) {
        Igual = false;
      }
    }

    if (Igual) {
      EstadoLampara = !EstadosLampara[0];
    } else {
      EstadoLampara = !EstadoLampara;
    }
  } else if (payload.equals("encender")) {
    EstadoLampara = true;
  } else {
    EstadoLampara = false;
  }

  Serial << "Cambiando Lamparas: " << (EstadoLampara ? "Encender" : "Apagado") << "\n";
  for (int i = 0; i < CantidadLampara; i++) {
    digitalWrite(PinLampara[i], EstadoLampara);
    EstadosLampara[i] = EstadoLampara;
  }
}

void setup() {
  Serial.begin(115200);

  Serial.println("\nIniciando multi Wifi");

  pinMode(LedEstado, OUTPUT);
  for (int i = 0; i < CantidadLampara; i++) {
    pinMode(PinLampara[i], OUTPUT);
    digitalWrite(PinLampara[i], LOW);
  }

  cambiarLed.attach(5, FuncionLed);

  wifiMulti.addAP(ssid_1, password_1);
  wifiMulti.addAP(ssid_2, password_2);

  WiFi.mode(WIFI_STA);
  Serial.print("Conectando a Wifi ***");

  int Contador = 0;
  while (wifiMulti.run(TiempoEsperaWifi) != WL_CONNECTED) {
    Serial.print("*");
    delay(500);
  }
  Serial.println(".. Conectado");
  Serial.print("SSID:");
  Serial.print(WiFi.SSID());
  Serial.print(" ID:");
  Serial.println(WiFi.localIP());

  Serial.print("Conectanco con MQTT...");
  cambiarLed.attach(3, FuncionLed);
  while (!client.connect(BrokerMQTT, 1883)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println(" Conectado!");

  mqtt.begin(client);

  Serial.print("Iniciando Secion con MQTT...");
  while (!mqtt.connect("ElESP")) {
    //  while (!mqtt.connect("arduino", "public", "public")) {
    Serial.print(".");
    delay(100);
  }
  Serial.println(" Conectado!");

  mqtt.subscribe("alsw/estudio/estado", MensajeEstado);
  mqtt.subscribe("alsw/estudio/estado1", MensajeEstado1);
  mqtt.subscribe("alsw/estudio/estado2", MensajeEstado2);
  mqtt.subscribe("alsw/estudio/estado3", MensajeEstado3);
  cambiarLed.attach(0.5, FuncionLed);
}

void loop() {
  mqtt.update();
  delay(100);
}
