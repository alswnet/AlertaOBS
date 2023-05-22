class AlertaComando {
    constructor() {
        print("Empezando sistema de Mensajes");
        this.Activo = false;

    }

    dibujar() {
        push();
        this.x = width - width / 20;
        this.y = height / 80;
        translate(this.x, this.y);
        fill(255, 0, 0);
        rect(0, 0, 100, 100)
        pop();

    }
}
