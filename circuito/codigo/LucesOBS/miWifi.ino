
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
    client.subscribe("alsw/estudio/estado/#");
    Serial.println("MQTT - Conectada!");
  }
}


void LeerTelnet() {
  if (TelnetStream.available()) {
    char Letra = TelnetStream.read();
    switch (Letra) {
      case 'R':
        TelnetStream.stop();
        delay(100);
        ESP.restart();
        break;
      case 'e':
      case 'E':
        estadoRele();
        break;
      case 'f':
      case 'F':
        if (!LittleFS.format()) {
          TelnetStream.println("Error formatiando");
          return;
        } else {
          TelnetStream.println("Se boro todo");
        }
        break;
    }
  }
}
