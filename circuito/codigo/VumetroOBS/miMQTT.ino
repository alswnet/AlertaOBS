WiFiClient net;
MQTTClient client;

void ConfigurarMQTT() {
  client.begin(BrokerMQTT, net);
  client.onMessage(mensajeMQTT);
}

void mensajeMQTT(String &topic, String &payload) {

  int UltimoPleca = topic.lastIndexOf('/');
  int LongitudTopic = topic.length();
  String Mensaje = topic.substring(UltimoPleca + 1, LongitudTopic);

  for (int i = 0; i < candidadAudios; i++) {
    if (Mensaje.equals(Audios[i].nombre)) {
      Audios[i].nivel = payload.toInt();
      Serial.println("Audio: " + Audios[i].nombre + " Nivel: " + Audios[i].nivel);
      TelnetStream.println("Audio: " + Audios[i].nombre + " Nivel: " + Audios[i].nivel);
      return;
    }
  }
}
