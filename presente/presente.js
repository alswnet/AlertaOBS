class AlertaPresente {
  constructor() {
    print("Empezando sistema de Presentes");
    this.presentes = Object();
    this.Activo = false;
  }

  agregar(id, nombre, miembro, url) {
    this.presentes[id] = { nombre: nombre, miembro: miembro, url: url };
    this.mostrar();
  }

  mostrar() {
    let lista = Object.entries(this.presentes);
    print("Cantidad " + lista.length);
    lista.forEach((element) => {
      print(element[0] + "-" + element[1].nombre);
    });
    print("\n");
  }
}
