
void configurarReles() {
  for (int i = 0; i < CantidadLampara; i++) {
    pinMode(PinLampara[i], OUTPUT);
  }

  actualizarRele();
}

void actualizarRele() {
  for (int i = 0; i < CantidadLampara; i++) {
    if (EstadosLampara[i] != EstadosLamparaAnterior[i]) {

      Serial.print("Rele ");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(EstadosLampara[i] ? "encencido" : "apagado");
      Serial.println();

      TelnetStream.print("Rele ");
      TelnetStream.print(i + 1);
      TelnetStream.print(": ");
      TelnetStream.print(EstadosLampara[i] ? "encencido" : "apagado");
      TelnetStream.println();
      EstadosLamparaAnterior[i] = EstadosLampara[i];
      digitalWrite(PinLampara[i], EstadosLampara[i]);
    }
  }
}

void estadoRele() {
  for (int i = 0; i < CantidadLampara; i++) {
    TelnetStream.print("Rele ");
    TelnetStream.print(i);
    TelnetStream.print(": ");
    TelnetStream.print(EstadosLampara[i] ? "encencido" : "apagado");
    TelnetStream.print(" - ");
    TelnetStream.print(leerArchivo(i));
    TelnetStream.println();
  }
}
