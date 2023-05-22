WiFiClient net;
MQTTClient client;

void ConfigurarMQTT() {
  client.begin(BrokerMQTT, net);
  client.onMessage(mensajeMQTT);
}

void mensajeMQTT(String &topic, String &payload) {
  Serial.println("Mensaje: " + topic + " - " + payload);
  TelnetStream.println("Mensaje: " + topic + " - " + payload);

  int UltimoPleca = topic.lastIndexOf('/');
  int LongitudTopic = topic.length();
  String Mensaje = topic.substring(UltimoPleca + 1, LongitudTopic);

  if (Mensaje.equals("t") || Mensaje.equals("T")  ) {
    for (int i = 0; i < CantidadLampara; i++) {
      if (payload.equals("c")  || payload.equals("C")) {
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

        for (int i = 0; i < CantidadLampara; i++) {
          digitalWrite(PinLampara[i], EstadoLampara);
          EstadosLampara[i] = EstadoLampara;
          escrivirArchivo(i, EstadosLampara[i] ? "encendido" : "apagado");
        }
      } else if (payload.equals("e")  || payload.equals("E") ) {
        EstadosLampara[i] = true;
      } else {
        EstadosLampara[i] = false;
      }
    }
  } else {
    int ID = Mensaje.toInt();
    if (ID < 0) return;
    if (payload.equals("c")  || payload.equals("C")) {
      EstadosLampara[ID - 1] = !EstadosLampara[ID - 1];
    } else if (payload.equals("e")  || payload.equals("E") ) {
      EstadosLampara[ID - 1] = true;
    } else {
      EstadosLampara[ID - 1] = false;
    }
    escrivirArchivo(ID - 1, EstadosLampara[ID - 1] ? "encendido" : "apagado");
  }
}
