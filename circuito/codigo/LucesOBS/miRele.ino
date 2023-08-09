
void configurarReles() {
  for (int i = 0; i < CantidadLampara; i++) {
    pinMode(PinLampara[i], OUTPUT);
  }

  actualizarRele();
}

void actualizarRele() {
  for (int i = 0; i < CantidadLampara; i++) {
    if (EstadosLampara[i] != EstadosLamparaAnterior[i]) {

      Serial << "Rele " << i << ": " << (EstadosLampara[i] ? "encencido" : "apagado") << "\n";
      TelnetStream << "Rele " << i << ": " << (EstadosLampara[i] ? "encencido" : "apagado") << "\n";

      EstadosLamparaAnterior[i] = EstadosLampara[i];
      digitalWrite(PinLampara[i], InvertidoLampara[i] ? !EstadosLampara[i], EstadosLampara[i]);
    }
  }
}

void estadoRele() {
  for (int i = 0; i < CantidadLampara; i++) {
    Serial << "Rele " << i << ": " << (EstadosLampara[i] ? "encencido" : "apagado") << " - " << leerArchivo(i) << "\n";
    TelnetStream << "Rele " << i << ": " << (EstadosLampara[i] ? "encencido" : "apagado") << " - " << leerArchivo(i) << "\n";
  }
}
