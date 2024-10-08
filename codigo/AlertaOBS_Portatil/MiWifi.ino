
void ConfigurarWifi() {

  wifiMulti.addAP(ssid_1, password_1);
  wifiMulti.addAP(ssid_2, password_2);

  WiFi.mode(WIFI_STA);
  Serial.println("Conectando con Wifi...");
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial << "SSID:" << WiFi.SSID() << " IP:" << WiFi.localIP() << "\n";
    estado = noMQTT;
  }

  MDNS.begin(NombreESP);
  ConfigurarMQTT();

  MDNS.addService("telnet", "tcp", 23);
  TelnetStream.begin();
}


void actualizarWifi() {

  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi No Conectada!");
    delay(500);
    estado = noWifi;
    return;
  } else {
    estado = noMQTT;
  }

#if defined(ESP8266)
  MDNS.update();
#endif
  client.loop();
  delay(10);

  if (!client.connected()) {
    Serial.println("MQTT - No Conectada!");
    if (!client.connect(NombreESP)) {
      delay(500);
      return;
    }
    for (int i = 0; i < CantidadTopic ; i++) {
      client.subscribe(TopicSolisitud[i]);
    }
    Serial.println("MQTT - Conectada!");
    SolisitarEstado();
  }
  estado = conectado;
}

void actualizarTelnet() {
  while (TelnetStream.available()) {
    char Letra = TelnetStream.read();
    mensajeSerial(Letra, TelnetStream);
  }
}
