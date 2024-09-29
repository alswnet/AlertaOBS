// Creado ChepeCarlos de ALSW
// Tutorial Completo en https://nocheprogramacion.com
// Canal Youtube https://youtube.com/alswnet?sub_confirmation=1

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

#include "token.h"

#include <Ticker.h>
#include <MQTT.h>
#include <TelnetStream.h>
#include <WiFiUdp.h>

#define Apagado 0
#define Encendido 1

#define noWifi 0
#define noMQTT 1
#define conectado 2

struct estado {
  bool actual;
  bool anterior;
};

estado estadoGrabando = { false, false };


int estado = noWifi;
int estadoAnterior = -1;

float WifiTiempo = 0;
float WifiEspera = 500;

float tiempoPasado = 0;
float tiempoEspera = 500;

const uint32_t TiempoEsperaWifi = 3000;

struct Indicador {
  String Nombre;
  int Led;
  int Estado;
  int Estado_Anterior;
  boolean Activo;
};

#define obs 0
#define grabar 1
#define envivo 2

#define cantidadLed 3
Indicador Indicadores[cantidadLed] = {
  { "OBS", 0, Apagado, Encendido, HIGH },        // D3 - OBS
  { "Grabando", 15, Apagado, Encendido, HIGH },  // D5 - Rojo
  { "EnVivo", 14, Apagado, Encendido, HIGH }     // D8 - Verde
};

#define grabando 0
#define pausar 1
#define trasmitir 2

#define cantidadBotones 3
const int Boton[3] = {
  13,  // grabando
  16,  // pausar
  12,  // trasmitir
};


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

String tiempoOBS = "--:--:--";

void setup() {
  Serial.begin(115200);
  Serial.println("\nIniciando El Monitor de OBS");
  Serial.println();

  configurarLed();
  for (int i = 0; i < cantidadLed; i++) {
    pinMode(Indicadores[i].Led, OUTPUT);
    digitalWrite(Indicadores[i].Led, Indicadores[i].Activo);
    delay(350);
    digitalWrite(Indicadores[i].Led, !Indicadores[i].Activo);
  }

  for (int i = 0; i < cantidadBotones; i++) {
    pinMode(Boton[i], INPUT);
  }

  actualizarIndocadores();

  ConfigurarWifi();
  configurarPantalla();
}

void loop() {
  actualizarWifi();
  actualizarIndocadores();
  actualizarSerial();
  actualizarTelnet();
  actualizarLed();
  actualizarPantalla();

  if (Indicadores[obs].Estado || estado == conectado) {
    ActualizarBotones();
  } else {
    ErrorBotones();
  }
}

void ActualizarBotones() {
  int BotonGrabar = digitalRead(Boton[grabando]);
  int BotonEnvivo = digitalRead(Boton[trasmitir]);
  int BotonPausar = digitalRead(Boton[pausar]);

  if (millis() - tiempoPasado > tiempoEspera) {
    if (Indicadores[obs].Estado) {
      if (BotonGrabar) {
        Serial.println("Cambiando Grabacion");
        TelnetStream.println("Cambiando Grabacion");
        EnviarMQTT(TopicControl, MensajeGrabacion);
        EsperarBoton(Boton[grabando]);
      } else if (BotonEnvivo) {
        Serial.println("Cambiando Envivo");
        TelnetStream.println("Cambiando Envivo");
        EnviarMQTT(TopicControl, MensajeEnvivo);
        EsperarBoton(Boton[trasmitir]);
      } else if (BotonPausar) {
        Serial.println("Cambiando Pausa");
        TelnetStream.println("Cambiando Pausa");
        EnviarMQTT(TopicControl, MensajePausar);
        EsperarBoton(Boton[pausar]);
      }
    } else {
      if (BotonGrabar) {
        Serial.println("Abriendo OBS");
        TelnetStream.println("Abriendo OBS");
        EnviarMQTT(TopicControl, MensajeOBS);
        EsperarBoton(Boton[grabando]);
      } else if (BotonEnvivo) {
        Serial.println("Conectando OBS");
        TelnetStream.println("Conectando OBS");
        EnviarMQTT(TopicControl, MensajeConectar);
        EsperarBoton(Boton[trasmitir]);
      }
    }
  }
}

void EsperarBoton(int Boton) {
  int Estado = 0;
  do {
    Serial.print("-");
    Estado = digitalRead(Boton);
    actualizarIndocadores();
    delay(250);
  } while (Estado);
  tiempoPasado = millis();
}


void ErrorBotones() {
  int BotonGrabar = digitalRead(Boton[grabando]);
  int BotonEnvivo = digitalRead(Boton[trasmitir]);

  if (BotonGrabar) {
    for (int i = 0; i < 6; i++) {
      Indicadores[grabar].Estado = Encendido;
      actualizarIndocadores();
      delay(200);
      Indicadores[grabar].Estado = Apagado;
      actualizarIndocadores();
      delay(200);
    }
  }
  if (BotonEnvivo) {
    for (int i = 0; i < 6; i++) {
      Indicadores[envivo].Estado = Encendido;
      actualizarIndocadores();
      delay(200);
      Indicadores[envivo].Estado = Apagado;
      actualizarIndocadores();
      delay(200);
    }
  }
}

void actualizarIndocadores() {
  boolean Cambio = false;
  for (int i = 0; i < cantidadLed; i++) {
    if (Indicadores[i].Estado != Indicadores[i].Estado_Anterior) {
      Serial << Indicadores[i].Nombre << "[" << (Indicadores[i].Estado ? "Encendido" : "Apagado") << "] ";
      TelnetStream << Indicadores[i].Nombre << "[" << (Indicadores[i].Estado ? "Encendido" : "Apagado") << "] ";
      Indicadores[i].Estado_Anterior = Indicadores[i].Estado;
      if (Indicadores[i].Estado) {
        digitalWrite(Indicadores[i].Led, Indicadores[i].Activo);
      } else {
        digitalWrite(Indicadores[i].Led, !Indicadores[i].Activo);
      }
      Cambio = true;
    }
  }
  if (Cambio) {
    Serial.println();
    TelnetStream.println();
  }
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
      EstadoSistema(miSerial);
      break;
  }
}

void EstadoSistema(Stream &miSerial) {
  for (int i = 0; i < cantidadLed; i++) {
    miSerial << Indicadores[i].Nombre << "[" << (Indicadores[i].Estado ? "Encendido" : "Apagado") << "] "
             << "\n";
  }
  miSerial << "Estado: ";
  switch (estado) {
    case noWifi:
      miSerial << "No Wifi ";
      break;
    case noMQTT:
      miSerial << "No MQTT ";
      break;
    case conectado:
      miSerial << "Conectado MQTT ";
      break;
  }
  miSerial << "\n";
}
