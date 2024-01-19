// #define ESPALEXA_DEBUG            //Activa depuraccion

#include <Espalexa.h>

Espalexa miAlexa;

void lamparaIzquierda(uint8_t brillo) {
  Serial.print("Cambiando Lampara Izquierda: ");
  TelnetStream.print("Cambiando Lampara Izquierda: ");
  if (brillo) {
    Serial << "Encendido";
    TelnetStream << "Encendido";
    Aparatos[0].Estado = true;
  } else {
    Serial << "Apagado";
    TelnetStream << "Apagado";
    Aparatos[0].Estado = false;
  }
  Serial << ", brillo " << brillo << "\n";
  TelnetStream << ", brillo " << brillo << "\n";
}

void lamparaCentro(uint8_t brillo) {
  Serial.print("Cambiando Lampara Centro: ");
  TelnetStream.print("Cambiando Lampara Centro: ");
  if (brillo) {
    Serial << "Encendido";
    TelnetStream << "Encendido";
    Aparatos[1].Estado = true;
  } else {
    Serial << "Apagado";
    TelnetStream << "Apagado";
    Aparatos[1].Estado = false;
  }
  Serial << ", brillo " << brillo << "\n";
  TelnetStream << ", brillo " << brillo << "\n";
}

void lamparaDerecha(uint8_t brillo) {
  Serial.print("Cambiando Lampara Derecho: ");
  TelnetStream.print("Cambiando Lampara Derecho: ");
  if (brillo) {
    Serial << "Encendido";
    TelnetStream << "Encendido";
    Aparatos[2].Estado = true;
  } else {
    Serial << "Apagado";
    TelnetStream << "Apagado";
    Aparatos[2].Estado = false;
  }
  Serial << ", brillo " << brillo << "\n";
  TelnetStream << ", brillo " << brillo << "\n";
}


void configurarAlexa() {
  miAlexa.addDevice(Aparatos[0].nombre, lamparaIzquierda);
  miAlexa.addDevice(Aparatos[1].nombre, lamparaCentro);
  miAlexa.addDevice(Aparatos[2].nombre, lamparaDerecha);

  miAlexa.begin();
}

void actualizarAlexa() {
  if (estado == noWifi) {
    return;
  }
  miAlexa.loop();
}