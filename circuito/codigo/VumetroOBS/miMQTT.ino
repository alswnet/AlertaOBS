WiFiClient net;
MQTTClient client;

void ConfigurarMQTT() {
  client.begin(BrokerMQTT, net);
  client.onMessage(mensajeMQTT);
}

void mensajeMQTT(String &topic, String &payload) {
  if (topic.indexOf("audio_obs") > 0) {
    int UltimoPleca = topic.lastIndexOf('/');
    int LongitudTopic = topic.length();
    String Mensaje = topic.substring(UltimoPleca + 1, LongitudTopic);
    for (int i = 0; i < candidadAudios; i++) {
      if (Mensaje.equals(Audios[i].nombre)) {
        Audios[i].nivel = payload.toInt();
        //        Serial << "Audio: " << Audios[i].nombre << " Nivel: " << Audios[i].nivel << "\n";
        //        TelnetStream << "Audio: " << Audios[i].nombre << " Nivel: " << Audios[i].nivel << "\n";
        return;
      }
    }
  } else if (topic.indexOf("monitor_esp") > 0) {
    payload.toLowerCase();
    Serial << topic << " - " << payload << "\n";
    if (payload.equals("obs-conectado") || payload.equals("obs-ya-conectado")) {
      ConectadoOBS = true;
    } else if (payload.equals("obs-no-conectado") || payload.equals("obs-no-encontrado")) {
      ConectadoOBS = false;
    }
  }
}
