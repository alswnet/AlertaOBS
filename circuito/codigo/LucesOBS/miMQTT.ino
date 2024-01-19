#include <MQTT.h>

WiFiClient net_MQTT;
MQTTClient client_MQTT;

void ConfigurarMQTT() {
  client_MQTT.begin(BrokerMQTT, net_MQTT);
  client_MQTT.onMessage(mensajeMQTT);
}

void actualizarMQTT() {
  client_MQTT.loop();
}

void conectarseMQTT() {
  if (!client_MQTT.connected()) {
    Serial.println("MQTT - No Conectada!");
    // TelnetStream.println("MQTT - No Conectada!");
    if (!client_MQTT.connect(nombre)) {
      delay(500);
      return;
    }
    client_MQTT.subscribe(TopicMQTT);
    Serial.println("MQTT - Conectada!");
    // TelnetStream.println("MQTT - Conectada!");
  }
  
}

// estados c - cambiar e - encender a - apagar
// Cambiar todos alsw/estudio/estado/t - c
// Cambiar individual alsw/estudio/estado/2 - c
void mensajeMQTT(String &topic, String &payload) {
  Serial.println("Mensaje: " + topic + " - " + payload);
  TelnetStream.println("Mensaje: " + topic + " - " + payload);

  int UltimoPleca = topic.lastIndexOf('/');
  int LongitudTopic = topic.length();
  String Mensaje = topic.substring(UltimoPleca + 1, LongitudTopic);

  if (Mensaje.equals("t") || Mensaje.equals("T")) {
    boolean EstadoCambiar = true;
    if (payload.equals("c") || payload.equals("C")) {
      boolean Igual = true;
      for (int i = 1; i < CantidadAparatos - 1; i++) {
        if (Aparatos[0].Estado != Aparatos[i].Estado) {
          Igual = false;
        }
      }
      if (Igual) {
        EstadoAparato = !Aparatos[0].Estado;
      } else {
        EstadoAparato = !EstadoAparato;
      }
      EstadoCambiar = EstadoAparato;
    } else if (payload.equals("e") || payload.equals("E")) {
      EstadoCambiar = true;
    } else {
      EstadoCambiar = false;
    }
    for (int i = 0; i < CantidadAparatos; i++) {
      Aparatos[i].Estado = EstadoCambiar;
    
    }
  } else {
    int ID = Mensaje.toInt() - 1;
    if (ID < 0 || ID > CantidadAparatos - 1) return;
    if (payload.equals("c") || payload.equals("C")) {
      Aparatos[ID].Estado = !Aparatos[ID].Estado;
    } else if (payload.equals("e") || payload.equals("E")) {
      Aparatos[ID].Estado = true;
    } else {
      Aparatos[ID].Estado = false;
    }
  }
}
