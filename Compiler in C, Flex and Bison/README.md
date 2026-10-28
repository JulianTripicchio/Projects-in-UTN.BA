# Compilador de C con Flex y Bison — Sintaxis y Semántica de los Lenguajes

Trabajos prácticos de **Sintaxis y Semántica de los Lenguajes (UTN FRBA)**. Construyen, etapa por etapa, el front-end de un compilador para un subconjunto del lenguaje C:

| Carpeta               | Etapa                     | Herramientas     | Qué hace                                                                                                 |
| --------------------- | ------------------------- | ---------------- | -------------------------------------------------------------------------------------------------------- |
| `Symbol's table/`     | Autómata finito           | C                | Reconoce constantes enteras (decimal, octal, hexadecimal) con una tabla de transiciones.                 |
| `Lexical Analyzer/`   | Análisis léxico (TP2)     | C + Flex         | Reconoce los tokens de un archivo C y lista identificadores, literales, palabras reservadas, constantes, puntuación y cadenas no reconocidas. |
| `Syntactic Analyzer/` | Análisis sintáctico (TP3) | C + Flex + Bison | Valida la gramática y reporta variables y funciones declaradas, sentencias y errores sintácticos.        |
| `Semantic Analyzer/`  | Análisis semántico (TP4)  | C + Flex + Bison | Agrega tabla de símbolos y chequeos semánticos (tipos incompatibles, redeclaraciones, etc.).             |

Cada etapa es un programa independiente que recibe un archivo de entrada por parámetro e imprime el reporte por pantalla.

---

## Requisitos

- `gcc`
- `flex`
- `bison`

### Linux / WSL

```bash
sudo apt update
sudo apt install -y gcc flex bison
```

### Windows

Opciones:

- **WSL** (recomendado): instalar Ubuntu con `wsl --install` y seguir los pasos de Linux.
- **Nativo**: instalar [MinGW](https://www.mingw-w64.org/) (para `gcc`) y [Flex y Bison de GnuWin32](https://gnuwin32.sourceforge.net/packages.html) (o `winflexbison`), y agregar sus carpetas `bin` al `PATH`.

Verificá la instalación con:

```bash
gcc --version
flex --version
bison --version
```

---

## Compilación y ejecución

Todos los comandos se ejecutan **dentro de la carpeta `src` de cada etapa**. En Windows, el ejecutable se genera como `.exe` y se corre sin el `./`.

### 1. Autómata (`Symbol's table`)

```bash
cd "Symbol's table/src"
gcc main.c -o automata
./automata entrada.txt
```

La entrada es una lista de constantes separadas por comas, **sin espacios**:

```text
0x1F,017,123,9a
```

Salida:

```text
0x1F		HEXADECIMAL
017		OCTAL
123		DECIMAL
9a		NO RECONOCIDA
```

### 2. Analizador léxico (`Lexical Analyzer`)

```bash
cd "Lexical Analyzer/src"
flex scanner.l
gcc lex.yy.c misFunciones.c global.c -o lexico
./lexico prueba.c
```

### 3. Analizador sintáctico (`Syntactic Analyzer`)

```bash
cd "Syntactic Analyzer/src"
bison -d parser.y
flex scanner.l
gcc parser.tab.c lex.yy.c misFunciones.c general.c -o sintactico
./sintactico prueba.c
```

### 4. Analizador semántico (`Semantic Analyzer`)

```bash
cd "Semantic Analyzer/src"
bison -d parser.y
flex scanner.l
gcc parser.tab.c lex.yy.c misFunciones.c general.c -o semantico
./semantico prueba.c
```

> [!NOTE]
> El orden importa: primero `bison -d` (genera `parser.tab.c` y `parser.tab.h`), después `flex` (el scanner incluye `parser.tab.h`) y por último `gcc`.
> Durante la compilación aparecen warnings de Bison (conflictos desplazamiento/reducción) y de `gcc`; son esperables y no impiden generar el ejecutable.

### Archivo de prueba

Cualquier archivo `.c` sirve como entrada. Por ejemplo, `prueba.c`:

```c
int suma(int a, int b);

int main(void) {
    int x = 5;
    float y = 3.14;
    char c = 'a';
    return 0;
}
```

Para guardar el reporte en un archivo en vez de verlo por pantalla:

```bash
./semantico prueba.c > reporte.txt
```

---

## Ejemplo de salida (analizador semántico)

```text
* Listado de variables declaradas (tipo de dato y numero de linea):
y: float, linea 4, columna 11
c: char, linea 5, columna 10

* Listado de funciones declaradas y definidas:
suma: declaracion, input: int a, int b, retorna: int, linea 1

* Listado de errores semanticos:
4:15: Incompatibilidad de tipos al inicializar el tipo 'float' usando el tipo 'double'

* Listado de errores sintacticos:
...
```

---

## Limpieza

Los archivos generados no forman parte del código fuente y se pueden borrar:

```bash
rm -f lex.yy.c parser.tab.c parser.tab.h automata lexico sintactico semantico *.exe
```
