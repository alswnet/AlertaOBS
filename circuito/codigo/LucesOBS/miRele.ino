
void configurarReles() {
  for (int i = 0; i < CantidadAparatos; i++) {
    pinMode(Aparatos[i].pin, OUTPUT);
  }
  actualizarRele();
}

void actualizarRele() {
  for (int i = 0; i < CantidadAparatos; i++) {
    if (Aparatos[i].Estado != Aparatos[i].Estado_Anterior) {
      digitalWrite(Aparatos[i].pin, Aparatos[i].Invertido ? !Aparatos[i].Estado : Aparatos[i].Estado);
    }
  }

  for (int i = 0; i < CantidadAparatos; i++) {
    if (Aparatos[i].Estado != Aparatos[i].Estado_Anterior) {
      Serial << "Rele " << i << " " << Aparatos[i].nombre << ": " << (Aparatos[i].Estado ? "encencido" : "apagado") << "\n";
      TelnetStream << "Rele " << i << " " << Aparatos[i].nombre << ": " << (Aparatos[i].Estado ? "encencido" : "apagado") << "\n";

      Aparatos[i].Estado_Anterior = Aparatos[i].Estado;
      escrivirArchivo(i, Aparatos[i].Estado ? "encendido" : "apagado");
    }
  }
}

void estadoRele() {
  for (int i = 0; i < CantidadAparatos; i++) {
    Serial << "Rele " << i << " " << Aparatos[i].nombre << ": " << (Aparatos[i].Estado ? "encencido" : "apagado") << " - " << leerArchivo(i) << "\n";
    TelnetStream << "Rele " << i << " " << Aparatos[i].nombre << ": " << (Aparatos[i].Estado ? "encencido" : "apagado") << " - " << leerArchivo(i) << "\n";
  }
}
