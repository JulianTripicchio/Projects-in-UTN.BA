# Programming Paradigms — Paradigmas de Programación

Trabajos prácticos, guías y parciales resueltos de **Paradigmas de Programación (UTN FRBA)**, organizados por paradigma:

| Carpeta    | Paradigma         | Lenguaje | Herramienta                |
| ---------- | ----------------- | -------- | -------------------------- |
| `Haskell/` | Funcional         | Haskell  | Stack + PdePreludat        |
| `Prolog/`  | Lógico            | Prolog   | SWI-Prolog                 |
| `Wollok/`  | Objetos           | Wollok   | Wollok (VS Code + CLI)     |

---

## Haskell — Paradigma funcional

| Proyecto                                  | Descripción                                           |
| ----------------------------------------- | ----------------------------------------------------- |
| `2025-f-haskelicia-JulianT52`             | Ejercicio individual *Haskelicia*                     |
| `2025-f-haskelicia-2-JulianT52`           | Segunda parte de *Haskelicia*                         |
| `2025-f-tpi-pisteando-junior-engineers`   | TP integrador grupal *Pisteando como un campeón* (autos, carreras y tramos) |

En todos, el código está en `src/Library.hs` y los tests (Hspec) en `src/Spec.hs`. Usan **[PdePreludat](https://github.com/10Pines/pdepreludat)**, un Prelude adaptado para la cursada.

### Instalación

Instalá **Stack** con [GHCup](https://www.haskell.org/ghcup/) (recomendado), que además instala GHC y el language server:

- **Windows** (PowerShell):

  ```powershell
  Set-ExecutionPolicy Bypass -Scope Process -Force;[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; try { & ([ScriptBlock]::Create((Invoke-WebRequest https://www.haskell.org/ghcup/sh/bootstrap-haskell.ps1 -UseBasicParsing))) -Interactive -DisableCurl } catch { Write-Error $_ }
  ```

- **Linux / macOS / WSL**:

  ```bash
  curl --proto '=https' --tlsv1.2 -sSf https://get-ghcup.haskell.org | sh
  ```

Para el editor, la extensión **Haskell** de VS Code.

### Ejecución

Parado en la carpeta del proyecto (por ejemplo `Haskell/2025-f-tpi-pisteando-junior-engineers`):

```bash
stack build      # compila (la primera vez descarga GHC y PdePreludat: tarda varios minutos)
stack test       # corre los tests de src/Spec.hs
stack ghci       # abre la consola interactiva con Library cargado
```

Dentro de `stack ghci` se pueden probar las funciones directamente (ejemplo en `2025-f-haskelicia-JulianT52`):

```haskell
ghci> doble 21
42
ghci> :r        -- recarga los cambios después de editar el código
ghci> :q        -- salir
```

> La versión de GHC la fija `stack.yaml` (`resolver: lts-22.43`), así que Stack descarga la correcta sola.

---

## Prolog — Paradigma lógico

| Proyecto                        | Descripción                                                    |
| ------------------------------- | -------------------------------------------------------------- |
| `2025-l-dundies-JulianT52`      | Ejercicio individual *Dundies* (`programa.pl`)                 |
| `2025-tpl-juniors-engineers`    | TP grupal *Figuritas* (`solucion.pl`)                          |
| `parciales.prolog/`             | Parciales y finales resueltos (31 minutos, Dakar, Hogwarts, Pokémon, Parque de atracciones, etc.) |

### Instalación

Descargá **[SWI-Prolog](https://www.swi-prolog.org/Download.html)**.

- Windows: instalador `.exe` (marcá la opción de agregar `swipl` al `PATH`).
- Ubuntu / WSL: `sudo apt install swi-prolog`.
- macOS: `brew install swi-prolog`.

Para el editor, la extensión **VSC-Prolog** de VS Code.

### Ejecución

Cargá el archivo al abrir la consola:

```bash
cd "Prolog/2025-l-dundies-JulianT52"
swipl programa.pl
```

y hacé consultas:

```prolog
?- consumoDeCafe(dwight, Cantidad).
Cantidad = 5.
```

- `;` → muestra la siguiente solución · `.` o Enter → corta la búsqueda
- `make.` → recarga los archivos después de editarlos
- `halt.` → salir

Para cargar un archivo desde una consola ya abierta:

```prolog
?- consult('parciales.prolog/pokemon.pl').
```

(`parciales.prolog/final18-7-15` no tiene extensión: se carga igual con `consult('parciales.prolog/final18-7-15').`)

**Sin instalar nada:** se puede pegar el código en [SWISH](https://swish.swi-prolog.org/), la versión online de SWI-Prolog.

---

## Wollok — Paradigma de objetos

| Carpeta                      | Contenido                                                                                       |
| ---------------------------- | ----------------------------------------------------------------------------------------------- |
| `Guia Objetos Spigariol/`    | Guía de objetos: alambique viajero, AFA, bebidas, casa pintada, exportación de papa, lobo feroz, planta nuclear, Rolando hechicero, videojuegos, voladores, etc. |
| `Guia Listas Spigariol/`     | Guía de colecciones (`afaColecciones`)                                                          |
| `05- Guia Objetos Integracion/` | Ejercicio integrador *Siete y medio*                                                         |
| `Parciales/`                 | Parciales resueltos: Catar 2022, Danger Zone, El nuevo Miguelito, La feria, Noticias de ayer, PdeP Brawls, Servicio de mensajería, Steim |

Cada ejercicio es un proyecto Wollok independiente: el código en `*.wlk`, los tests en `*.wtest` y los programas ejecutables en `*.wpgm`.

### Instalación

Los proyectos usan **Wollok 4** (la versión basada en TypeScript).

1. Instalá [Node.js 20+](https://nodejs.org/).
2. Instalá la CLI de Wollok:

   ```bash
   npm install -g wollok-ts-cli
   ```

3. En VS Code, instalá la extensión **Wollok** (de *uqbar*). Agrega resaltado, errores en vivo y botones para correr tests y programas.

Más información en [wollok.org](https://www.wollok.org/).

### Ejecución

**Desde VS Code:** abrí la carpeta de un ejercicio (por ejemplo `Wollok/Parciales/catar2022`) y usá los botones *Run* que aparecen sobre cada `test` / `describe` / `program`.

**Desde la terminal**, parado en la carpeta del ejercicio:

```bash
wollok test                   # corre todos los tests (*.wtest)
wollok repl example.wlk       # consola interactiva con los objetos del archivo cargados
```

Dentro del REPL se pueden mandar mensajes a los objetos:

```text
wollok:example> pepita.energia()
```

Para correr un programa (`.wpgm`), por ejemplo en `Guia Objetos Spigariol/voladores`:

```bash
wollok run "mainExample.pepitavoladora"
```

(`wollok run --help` muestra todas las opciones.)

> Las carpetas `log/` las genera Wollok automáticamente y se pueden borrar sin problema.
