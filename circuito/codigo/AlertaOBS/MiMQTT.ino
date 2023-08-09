
WiFiClient net;
MQTTClient client;

void ConfigurarMQTT() {
  client.begin(BrokerMQTT, net);
  client.onMessage(MensajeMQTT);
}

void MensajeMQTT(String topic, String payload) {
  payload.toLowerCase();
  Serial << "MQTT[" << topic << "] " << payload << "\n";
  TelnetStream << "MQTT[" << topic << "] " << payload << "\n";
  if (payload.equals("obs-conectado") || payload.equals("obs-ya-conectado")) {
    Indicadores[obs].Estado = Encendido;
  } else if (payload.equals("obs-no-conectado") || payload.equals("obs-no-encontrado")) {
    Indicadores[obs].Estado = Apagado;
    Indicadores[grabar].Estado = Apagado;
    Indicadores[envivo].Estado = Apagado;
  } else if (payload.equals("obs-grabando")) {
    Indicadores[grabar].Estado = Encendido;
  } else if (payload.equals("obs-no-grabando")) {
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
}

void SolisitarEstado() {
  client.publish(TopicSolisitud, MensajeEstado);
}

void EnviarMQTT(const char* Topic, const char* Mensaje) {
  client.publish(Topic, Mensaje);
}
