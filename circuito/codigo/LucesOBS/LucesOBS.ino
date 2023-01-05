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

#include "data.h"

#define Ancho_Pantalla 128
#define Alto_Pantalla 64
#define Direccion_Pantalla 0x3C
#define Reset_Pantalla -1

const uint32_t TiempoEsperaWifi = 5000;

WiFiClient client;
MQTTPubSubClient mqtt;

#define CantidadLampara 2
const int PinLampara[CantidadLampara] = {LED_BUILTIN, 5};
boolean EstadoLampara = false;
boolean EstadosLampara[CantidadLampara] = {false,  false};


void MensajeEstado1(const String & payload, const size_t size) {
  MensajeEstadoIndividual(0, payload );
}

void MensajeEstado2(const String & payload, const size_t size) {
  MensajeEstadoIndividual(1, payload);
}

void MensajeEstadoIndividual(int i, const String payload) {
  if (payload.equals("cambiar")) {
    EstadosLampara[i] = !EstadosLampara[i];
  } else if (payload.equals("encender")) {
    EstadosLampara[i] = true;
  } else {
    EstadosLampara[i] = false;
  }
  Serial << "Estado Lampara " << i << "-" << (EstadosLampara[i] ? "Encendida" : "Apagado") << "\n";
  digitalWrite(PinLampara[i], EstadosLampara[i]);
}



void MensajeEstado(const String & payload, const size_t size) {
  if (payload.equals("cambiar")) {
    if (EstadosLampara[0] != EstadosLampara[1]) {
      EstadoLampara = !EstadoLampara;
    } else {
      EstadoLampara = !EstadoLampara;
    }
  } else if (payload.equals("encender")) {
    EstadoLampara = true;
  } else {
    EstadoLampara = false;
  }

  Serial << "Estado Lampara " << (EstadoLampara ? "Encendida" : "Apagado") << "\n";
  for (int i = 0; i < CantidadLampara; i++) {
    digitalWrite(PinLampara[i], EstadoLampara);
    EstadosLampara[i] = EstadoLampara;
  }
}

void setup() {
  Serial.begin(115200);

  Serial.println("\nIniciando multi Wifi");

  for (int i = 0; i < CantidadLampara; i++) {
    pinMode(PinLampara[i], OUTPUT);
  }

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
}

void loop() {
  mqtt.update();
  delay(100);
}
