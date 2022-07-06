// Creado ChepeCarlos de ALSW
// Tutorial Completo en https://nocheprogramacion.com
// Canal Youtube https://youtube.com/alswnet?sub_confirmation=1

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

#include "token.h"
#include <MQTT.h>

#define Apagado 0
#define Encendido 1

#define wifi 0
#define conectado 1
#define grabar 2
#define envivo 3

float WifiTiempo = 0;
float WifiEspera = 200;
const uint32_t TiempoEsperaWifi = 3000;

struct Indicador {
  int Led;
  int Estado;
  int Estado_Anterior;
};

#define cantidadLed 4
Indicador Indicadores[cantidadLed] = {
  {2, Apagado, Encendido}, // D4
  {0, Apagado, Encendido}, // D3
  {14, Apagado, Encendido}, // D6
  {16, Apagado, Encendido} // D8
};

#define cantidadBotones 2
const int Boton[2] = {12, 13}; // D6 - D7

WiFiClient net;
MQTTClient client;

void setup() {
  Serial.begin(115200);
  Serial.println("\nIniciando El Monitor de OBS");

  for (int i = 0; i < cantidadLed; i++) {
    pinMode(Indicadores[i].Led, OUTPUT);
  }

  for (int i = 0; i < cantidadBotones; i++) {
    pinMode(Boton[i], INPUT);
  }

  client.begin(BrokerMQTT, net);
  client.onMessage(MensajeMQTT);
  
  ActualizarIndocadores();
  
  ConfigurarWifi();
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    Reconectar();
  }

  ActualizarIndocadores();
}

void ConfigurarWifi() {

  wifiMulti.addAP(ssid_1, password_1);
  //  wifiMulti.addAP(ssid_2, password_2);

  WiFi.mode(WIFI_STA);

  Serial.print("\nConectando a Wifi...");
  int contador = 0;
  while (wifiMulti.run(TiempoEsperaWifi) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    contador++;
    if (contador > 10) {
      Indicadores[wifi].Estado = Apagado;
      return;
    }

  }
  Indicadores[wifi].Estado = Encendido;
  Serial.println(".. Conectado");
  Serial.print("SSID:");
  Serial.print(WiFi.SSID());
  Serial.print(" ID:");
  Serial.println(WiFi.localIP());
}

void MensajeMQTT(String topic, String payload) {
  payload.toLowerCase();
  Serial.println("MQTT[" + topic + "] " + payload);

  if (payload.equals("obs-conectado") || payload.equals("obs-ya-conectado")) {
    Indicadores[conectado].Estado = true;
  } else  if (payload.equals("obs-no-conectado") || payload.equals("obs-no-encontrado") ) {
    Indicadores[conectado].Estado = false;
  }  else if (payload.equals("obs-grabando")) {
    Indicadores[grabar].Estado = true;
  }  else if (payload.equals("obs-no-grabando")) {
    Indicadores[grabar].Estado = false;
  } else if (payload.equals("obs-envivo")) {
    Indicadores[envivo].Estado = true;
  } else if (payload.equals("obs-no-envivo")) {
    Indicadores[envivo].Estado = false;
  } else {
    Serial.println("[Error] No comando");
  }
  ActualizarIndocadores();
}

void ActualizarIndocadores() {
  boolean Cambio = false;
  for (int i = 0; i < cantidadLed; i++) {
    if (Indicadores[i].Estado != Indicadores[i].Estado_Anterior) {
      Serial << i << "-" << Indicadores[i].Estado << " " << Indicadores[i].Estado_Anterior << " | " ;
      Indicadores[i].Estado_Anterior = Indicadores[i].Estado;
      digitalWrite(Indicadores[i].Led, Indicadores[i].Estado);
      Cambio = true;
    }
  }
  if (Cambio) {
    Serial.println();
  }

}

void Reconectar() {
  Serial.print("\nConectando a Wifi ..");
  int contador = 0;
  while (wifiMulti.run(TiempoEsperaWifi) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    contador++;
    if (contador > 10) {
      Indicadores[wifi].Estado = Apagado;
      return;
    }
  }
  Serial.println("..Conectado");
  Serial.print("Conectado a MQTT***");
  contador = 0;
  //  while (!client.connect("arduino", "public", "public")) {/
  while (!client.connect(NombreESP)) {
    Serial.print("*");
    delay(500);
    contador++;
    if (contador > 10) {
      Indicadores[wifi].Estado = Apagado;
      return;
    }
  }
  Serial.println("**Conectado");
  Indicadores[wifi].Estado = Encendido;
  client.subscribe(Topic);

  SolisitarEstado();
  ActualizarIndocadores();
}

void SolisitarEstado() {
  client.publish(TopicSolisitud, MensajeEstado);
}
