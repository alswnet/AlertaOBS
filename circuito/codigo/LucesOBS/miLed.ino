#include <Ticker.h>

int ledEstado = 2;
boolean EstadoLed = false;
Ticker cambiarLed;

void configurarLed() {
  pinMode(ledEstado, OUTPUT);
}

void funcionLed() {
  EstadoLed = !EstadoLed;
  digitalWrite(ledEstado, EstadoLed ? HIGH : LOW);
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
