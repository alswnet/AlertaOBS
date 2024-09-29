
WiFiClient net;
MQTTClient client;

void ConfigurarMQTT() {
  client.begin(BrokerMQTT, net);
  client.onMessage(MensajeMQTT);
}

void MensajeMQTT(String topic, String payload) {
  if (topic.indexOf("monitor_esp") > 0) {
    payload.toLowerCase();
    Serial << "MQTT[" << topic << "] " << payload << "\n";
    TelnetStream << "MQTT[" << topic << "] " << payload << "\n";
    if (payload.equals("obs-conectado") || payload.equals("obs-ya-conectado")) {
      Indicadores[obs].Estado = Encendido;
      ConectadoOBS = true;
    } else if (payload.equals("obs-no-conectado") || payload.equals("obs-no-encontrado")) {
      Indicadores[obs].Estado = Apagado;
      Indicadores[grabar].Estado = Apagado;
      Indicadores[envivo].Estado = Apagado;
      ConectadoOBS = false;
    } else if (payload.equals("obs-grabando") || payload.equals("obs-grabando-vertival")) {
      Indicadores[grabar].Estado = Encendido;
    } else if (payload.equals("obs-no-grabando") || payload.equals("obs-no-grabando-vertival")) {
      Indicadores[grabar].Estado = Apagado;
    } else if (payload.equals("obs-envivo")) {
      Indicadores[envivo].Estado = Encendido;
    } else if (payload.equals("obs-no-envivo")) {
      Indicadores[envivo].Estado = Apagado;
    } else {
      Serial.println("[Error] No comando");
      TelnetStream.println("[Error] No comando");
    }
    actualizarIndocadores();

  } else if (topic.indexOf("audio_obs") > 0) {
    int UltimoPleca = topic.lastIndexOf('/');
    int LongitudTopic = topic.length();
    String Mensaje = topic.substring(UltimoPleca + 1, LongitudTopic);
    for (int i = 0; i < candidadAudios; i++) {
      if (Mensaje.equals(Audios[i].nombre)) {
        Audios[i].nivel = payload.toInt();
        // Serial << "Audio: " << Audios[i].nombre << " Nivel: " << Audios[i].nivel << "\n";
        // TelnetStream << "Audio: " << Audios[i].nombre << " Nivel: " << Audios[i].nivel << "\n";
        return;
      }
    }
  } else if (topic.indexOf("tiempo_obs") >= 0) {
    tiempoOBS = payload;
  }
}

void SolisitarEstado() {
  EnviarMQTT(TopicControl, MensajeEstado);
}

void EnviarMQTT(const char* Topic, const char* Mensaje) {
  Serial << "Enviando Topic:" << Topic << " - " << Mensaje << "\n";
  TelnetStream << "Enviando Topic:" << Topic << " - " << Mensaje << "\n";
  client.publish(Topic, Mensaje);
}
