
void conectarWifi() {

  wifiMulti.addAP(ssid_1, password_1);
  wifiMulti.addAP(ssid_2, password_2);

  Serial.println("Conectando con Wifi...");
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi Conectado");
    Serial.println("Mi IP es: ");
    Serial.println(WiFi.localIP());
    estado = noMQTT;
  }

  MDNS.begin(nombre);
  configurarOTA();
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
  ArduinoOTA.handle();
  client.loop();
  delay(10);

  if (!client.connected()) {
    Serial.println("MQTT - No Conectada!");
    if (!client.connect(nombre)) {
      delay(500);
      return;
    }
    Serial.println("topic:");
    for (int i = 0; i < CantidadTopic; i++) {
      Serial.println(TopicMQTT[i]);
      client.subscribe(TopicMQTT[i]);
    }
    Serial.println("MQTT - Conectada!");
  }
  estado = conectado;
}


void LeerTelnet() {
  if (TelnetStream.available()) {
    char Letra = TelnetStream.read();
    mensajeSerial(Letra, TelnetStream);
  }
}
