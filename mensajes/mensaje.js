class AlertaMensaje {
    constructor() {
        print("Empezando sistema de Comandos");
        this.activo = false;
        this.inicio = millis();
        this.duracion = 6;
        this.colorTexto = "#fff";
        this.colorBorde = "#001427";
        this.colorNombre = "#ff3a20";
        this.colorMensaje = "#008080";
        this.canal = null;
        this.tamanoNombre = 20;
        this.tamanoMensaje = this.tamanoNombre + 7;
        this.tamanoBorde = 20;
        this.tamanoLinea = 3;
        this.tamanoPerfil = 75
        this.desfaceBorde = 4;
        this.tamanoRedondeado = 20;
        this.posicion = createVector(width / 15, height / 20);

        // this.fuente = null;
        // this.posicion = createVector(width / 15, height / 80);
        // this.cargandoImagen = false;
        // this.noImagen = true;
        this.imagen = null;
        this.imagenCargada = false;

        this.nombre = "ChepeCarlos";
        this.mensaje = "Hola Mundo";
        // this.urlImagen = "no"
        this.urlImagen = "https://yt4.ggpht.com/hyfbUP1SmorOKU6Rk0vMTHyz1CBrAc8dPSlg3Z3aMgiAcENaqF_EQxIr2eZryXPjB08cIzXIRQ=s64-c-k-c0x00ffffff-no-rj";

    }

    // cargarFuente(fuente) {
    //     this.fuente = loadFont("fuente/Roboto-Black.ttf");
    // }


    actualizarMensaje(nombre, mensaje, imagen, miembro, canal) {
        print(`Actualizar mensaje ${nombre} - ${mensaje}`)
        this.nombre = nombre;
        this.mensaje = mensaje;
        this.urlImagen = imagen;
        this.miembro = miembro;
        this.imagenCargada = false;
        this.imagen = null;
        this.canal = canal;

        if(this.canal == "youtube"){
            this.colorNombre = "#ff3a20";
            this.colorMensaje = "#ff3a20";
        } else if(this.canal == "tiktok"){
            this.colorNombre = "#0034a3";
            this.colorMensaje = "#0034a3";
        } else {
            this.colorNombre = "#000000";
            this.colorMensaje = "#000000";
        }
    }

    dibujar() {
        if (!this.imagenCargada) {
            this.cargarImagen();
            return;
        }

        if (this.esperarImagen) return;

        if (millis() - this.inicio < this.duracion * 1000 || this.activo) {
            push();
            translate(this.posicion.x, this.posicion.y);

            let x = 0;

            if (this.imagen != null) {
                fill(this.colorBorde);
                rect(this.desfaceBorde, this.desfaceBorde, this.tamanoPerfil, this.tamanoPerfil);
                image(this.imagen, 0, 0, this.tamanoPerfil, this.tamanoPerfil);
                x = this.tamanoPerfil + this.tamanoBorde;
            }

            this.dibujarRectangulo(x, 0, this.nombre, this.tamanoNombre, this.colorNombre, this.colorBorde);
            this.dibujarRectangulo(x, 2 * this.tamanoNombre, this.mensaje, this.tamanoMensaje, this.colorMensaje, this.colorBorde);
            pop();
        }

    }

    dibujarRectangulo(x, y, texto, tamano, color, colorBorde) {
        push();
        translate(x, y);
        stroke(this.colorBorde);
        strokeWeight(this.tamanoLinea);
        textSize(tamano);
        textFont(FuenteRoboto);
        let anchoMaximo = width - 2 * x - this.posicion.x;
        let anchoTexto = textWidth(texto);
        let lineas = int(anchoTexto / anchoMaximo);
        let ancho = anchoTexto + this.tamanoBorde;
        let alto = tamano + this.tamanoBorde / 2;
        let altoMensaje = tamano + 7;
        if (lineas > 0) {
            ancho = anchoMaximo;
            alto = alto * (lineas + 1);
        }
        fill(colorBorde);
        rect(this.desfaceBorde * 2, this.desfaceBorde, ancho, alto, this.tamanoBorde / 4);
        fill(color);
        rect(0, 0, ancho, alto, this.tamanoBorde / 4);
        fill(this.colorTexto);

        textLeading(altoMensaje);
        text(texto, this.tamanoBorde / 2, tamano + this.desfaceBorde / 8, ancho - this.tamanoBorde/2);
        pop();
    }

    cargarImagen() {
        print("Intentando cargar la imagen");
        this.imagenCargada = true;
        this.esperarImagen = true;

        if (this.urlImagen == "no") {
            print("No imagen")
            this.inicio = millis();
            this.esperarImagen = false;
            return;
        }

        loadImage(this.urlImagen, (img) => {
            print(`Imagen cargada ${this.urlImagen}`);
            this.imagen = img;
            this.cargando
            this.inicio = millis();
            this.esperarImagen = false;
        });

    }
}


