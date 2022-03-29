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

  dibujar() {
    push();
    let Acua = "#008080";
    let Naranja = "#ff3a20";
    let AzulOscuro = "#001427";
    let ColorTexto = "#fff";
    let Borde = 20;
    let DesfaceFondo = 4;
    let altoNombre = 30;
    this.x = width / 20;
    this.y = height / 5;
    this.ancho = width / 3;
    this.alto = height - (1 / 5) * height - (1 / 20) * height;
    translate(this.x, this.y);
    strokeWeight(3);
    textFont(FuenteRoboto);

    fill(AzulOscuro);
    rect(DesfaceFondo + 2, DesfaceFondo, this.ancho, this.alto, 5);
    fill(Acua);
    stroke(AzulOscuro);
    rect(0, 0, this.ancho, this.alto, 5);

    textSize(altoNombre);

    let lista = Object.entries(this.presentes);
    fill(ColorTexto);
    if (lista.length <= 0) {
      text("Usa !presente en chat", 10, (altoNombre + 5) * (0 + 1));
    } else {
      print("Cantidad " + lista.length);
      for (let i = 0; i < lista.length; i++) {
        let Presente = lista[i];
        text(Presente[1].nombre, 10, (altoNombre + 5) * (i + 1));
      }
      fill(Naranja);
      text("#" + lista.length, this.ancho - 60, (altoNombre + 5) * (0 + 1));
    }

    pop();
  }
}
