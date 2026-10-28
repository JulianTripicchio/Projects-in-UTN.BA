# Simulador de Sistema Operativo — TP Sistemas Operativos 2025 1C (Junior Engineers)

Simulación distribuida de un sistema operativo escrita en C. Está compuesta por **4 módulos** que corren como procesos separados y se comunican por sockets TCP, más una biblioteca compartida:

| Módulo    | Qué hace                                                                                     | Puerto(s) que escucha            |
| --------- | -------------------------------------------------------------------------------------------- | -------------------------------- |
| `memoria` | Memoria principal con paginación multinivel, SWAP y memory dumps. Lee el pseudocódigo de los procesos. | `8002`                            |
| `kernel`  | Planificación de largo, mediano y corto plazo (FIFO / SJF / SRT), syscalls y gestión de IOs. | `8001` dispatch · `8004` interrupt · `8003` IO |
| `cpu`     | Ciclo de instrucción (fetch / decode / execute / interrupciones), TLB y caché de páginas.   | — (es cliente)                   |
| `io`      | Dispositivo de entrada/salida que atiende pedidos del Kernel.                                | — (es cliente)                   |
| `utils`   | Biblioteca estática compartida: serialización, sockets, logs.                                | —                                |

```
                 ┌──────────┐
                 │ MEMORIA  │ :8002
                 └────▲─────┘
           ┌──────────┴───────────┐
     ┌─────┴────┐            ┌────┴─────┐
     │  KERNEL  │◄──────────►│   CPU    │  (1 o más)
     └─────▲────┘ dispatch/  └──────────┘
           │      interrupt
     ┌─────┴────┐
     │    IO    │  (1 o más)
     └──────────┘
```

> [!IMPORTANT]
> El proyecto **solo compila y corre en Linux** (usa `pthread`, sockets POSIX y la biblioteca *so-commons* de la cátedra). En Windows o macOS hay que usar una **máquina virtual con Ubuntu**. Más abajo están todos los pasos.

---

## Índice

1. [Preparar la máquina virtual](#1-preparar-la-máquina-virtual)
2. [Instalar dependencias dentro de la VM](#2-instalar-dependencias-dentro-de-la-vm)
3. [Clonar y compilar el proyecto](#3-clonar-y-compilar-el-proyecto)
4. [Preparar carpetas y scripts de pseudocódigo](#4-preparar-carpetas-y-scripts-de-pseudocódigo)
5. [Ejecutar los módulos](#5-ejecutar-los-módulos)
6. [Configuración](#6-configuración)
7. [Correr en varias máquinas virtuales](#7-correr-en-varias-máquinas-virtuales)
8. [Logs y resultados](#8-logs-y-resultados)
9. [Problemas comunes](#9-problemas-comunes)

---

## 1. Preparar la máquina virtual

### 1.1 Instalar VirtualBox

Descargá e instalá [VirtualBox](https://www.virtualbox.org/wiki/Downloads) para tu sistema operativo (si usás Windows, también conviene instalar el *Extension Pack*).

### 1.2 Conseguir una VM con Ubuntu

Tenés dos opciones:

**Opción A — VM de la cátedra (recomendada).**
La cátedra de Sistemas Operativos de UTN FRBA publica máquinas virtuales listas para usar (Ubuntu Server, sin interfaz gráfica, y una variante con escritorio) en [docs.utnso.com.ar](https://docs.utnso.com.ar/). Descargá la imagen, abrila con VirtualBox (*Archivo → Importar servicio virtualizado* si es un `.ova`, o *Máquina → Agregar* si es una carpeta con `.vbox`).

- Usuario: `utnso`
- Contraseña: `utnso`

Estas VMs ya traen `gcc`, `make`, `git` y otras herramientas instaladas.

**Opción B — Instalar Ubuntu a mano.**
Descargá [Ubuntu Server 22.04 o 24.04 LTS](https://ubuntu.com/download/server), creá una VM nueva en VirtualBox (2 GB de RAM y 20 GB de disco alcanzan) e instalala. **Creá el usuario `utnso`**: los archivos de configuración usan rutas del tipo `/home/utnso/...`. Si usás otro usuario, tenés que cambiar esas rutas en `memoria/memoria.config`.

### 1.3 Configurar la red de la VM

En VirtualBox: *Configuración de la VM → Red → Adaptador 1*.

- **Una sola VM** (lo más simple): dejá **NAT**. Todos los módulos corren en la misma VM y se conectan por `127.0.0.1`.
- **Para conectarte por SSH desde tu PC** (muy recomendable con Ubuntu Server, que no tiene escritorio): en NAT, abrí *Avanzado → Reenvío de puertos* y agregá una regla:

  | Nombre | Protocolo | Puerto anfitrión | Puerto invitado |
  | ------ | --------- | ---------------- | --------------- |
  | ssh    | TCP       | `2222`           | `22`            |

  Después, desde una terminal de tu PC:

  ```bash
  ssh utnso@localhost -p 2222
  ```

  También podés usar la extensión **Remote - SSH** de VS Code para editar el código directamente dentro de la VM.

- **Varias VMs** (un módulo por máquina): usá **Adaptador puente** o **Red NAT**, para que las VMs se vean entre sí. Ver [sección 7](#7-correr-en-varias-máquinas-virtuales).

Si SSH no está instalado en la VM:

```bash
sudo apt install -y openssh-server
```

---

## 2. Instalar dependencias dentro de la VM

Todo lo que sigue se ejecuta **dentro de la VM**.

### 2.1 Herramientas de compilación

(Si usás la VM de la cátedra probablemente ya estén instaladas; correrlo igual no rompe nada.)

```bash
sudo apt update
sudo apt install -y build-essential git make libreadline-dev
```

### 2.2 so-commons-library

Es la biblioteca de la cátedra (listas, colas, diccionarios, config, logs, etc.). Los módulos linkean contra `-lcommons`, así que **es obligatoria**.

```bash
cd ~
git clone https://github.com/sisoputnfrba/so-commons-library
cd so-commons-library
make debug
sudo make install
```

Esto deja los headers en `/usr/include/commons` y la biblioteca en `/usr/lib`.

---

## 3. Clonar y compilar el proyecto

```bash
cd ~
git clone https://github.com/JulianT52/Projects-in-UTN.BA.git
cd Projects-in-UTN.BA/tp-2025-1c-Junior-Engineers
```

Cada módulo tiene su propio `Makefile`. Al compilar cualquier módulo, también se compila automáticamente la biblioteca `utils`. Para compilar todo de una:

```bash
for m in utils memoria kernel cpu io; do make -C "$m"; done
```

O de a uno:

```bash
cd kernel
make
```

Los ejecutables quedan en `<módulo>/bin/<módulo>` (por ejemplo `kernel/bin/kernel`).

| Comando        | Qué hace                                          |
| -------------- | ------------------------------------------------- |
| `make`         | Compila en modo debug (`-g`, recomendado)         |
| `make release` | Compila optimizado (`-O3`)                        |
| `make clean`   | Borra `bin/` y `obj/` del módulo y de `utils`     |

---

## 4. Preparar carpetas y scripts de pseudocódigo

La Memoria lee los programas de los procesos desde `PATH_INSTRUCCIONES` y escribe los dumps en `DUMP_PATH`. Con la configuración por defecto hay que crear estas carpetas:

```bash
mkdir -p /home/utnso/scripts /home/utnso/dump_files
```

El archivo de SWAP (`/home/utnso/swapfile.bin`) lo crea la Memoria sola al iniciar.

### 4.1 Formato del pseudocódigo

Cada archivo es texto plano con **una instrucción por línea** y los parámetros separados por espacios. Las instrucciones soportadas son:

| Instrucción                    | Descripción                                                                  |
| ------------------------------ | ---------------------------------------------------------------------------- |
| `NOOP`                         | No hace nada (consume un ciclo).                                             |
| `WRITE <dirección> <texto>`    | Escribe `<texto>` en la dirección lógica indicada.                           |
| `READ <dirección> <tamaño>`    | Lee `<tamaño>` bytes desde la dirección lógica indicada.                     |
| `GOTO <nro_instrucción>`       | Salta a la instrucción indicada (el PC empieza en 0).                        |
| `IO <dispositivo> <tiempo_ms>` | Syscall: bloquea el proceso usando el IO `<dispositivo>` durante `<tiempo_ms>`. |
| `INIT_PROC <archivo> <tamaño>` | Syscall: crea un nuevo proceso con ese pseudocódigo y tamaño en bytes.       |
| `DUMP_MEMORY`                  | Syscall: vuelca la memoria del proceso a un archivo `.dmp` en `DUMP_PATH`.   |
| `EXIT`                         | Syscall: finaliza el proceso.                                                |

### 4.2 Ejemplo

Creá `/home/utnso/scripts/PROCESO_INICIAL`:

```text
NOOP
WRITE 0 HOLA_MUNDO
READ 0 10
IO DISCO 3000
INIT_PROC PROCESO_HIJO 64
DUMP_MEMORY
EXIT
```

y `/home/utnso/scripts/PROCESO_HIJO`:

```text
NOOP
WRITE 16 SOY_EL_HIJO
READ 16 11
EXIT
```

> [!NOTE]
> El nombre del dispositivo en `IO <dispositivo> ...` tiene que coincidir con el nombre con el que levantes un módulo IO (en el ejemplo, `DISCO`).

---

## 5. Ejecutar los módulos

Cada módulo lee su `.config` **desde la carpeta actual**, así que hay que ejecutarlos **parado en la carpeta del módulo**. Se necesita **una terminal por módulo** (podés abrir varias sesiones SSH o usar `tmux`).

Respetá este orden:

**Terminal 1 — Memoria** (tiene que estar levantada antes que todo):

```bash
cd ~/Projects-in-UTN.BA/tp-2025-1c-Junior-Engineers/memoria
./bin/memoria
```

**Terminal 2 — Kernel**, pasando el pseudocódigo del primer proceso (relativo a `PATH_INSTRUCCIONES`) y su tamaño en bytes:

```bash
cd ~/Projects-in-UTN.BA/tp-2025-1c-Junior-Engineers/kernel
./bin/kernel PROCESO_INICIAL 256
```

**Terminal 3 — CPU**, con un identificador (se usa para el nombre del log). Podés levantar varias CPUs con identificadores distintos:

```bash
cd ~/Projects-in-UTN.BA/tp-2025-1c-Junior-Engineers/cpu
./bin/cpu CPU1
```

**Terminal 4 — IO**, con el nombre del dispositivo. Podés levantar varios IOs (con el mismo o con distintos nombres):

```bash
cd ~/Projects-in-UTN.BA/tp-2025-1c-Junior-Engineers/io
./bin/io DISCO
```

**Por último**, volvé a la terminal del **Kernel** y presioná **ENTER**: recién ahí arranca la planificación de largo plazo y el proceso inicial empieza a ejecutarse.

Para cortar la ejecución, `Ctrl + C` en cada terminal.

### Con tmux (opcional)

Si no querés abrir 4 sesiones SSH:

```bash
sudo apt install -y tmux
tmux
```

- `Ctrl+b` y después `%` → divide la pantalla verticalmente
- `Ctrl+b` y después `"` → divide horizontalmente
- `Ctrl+b` y después una flecha → moverse entre paneles

---

## 6. Configuración

Cada módulo tiene un archivo `<módulo>.config` en su carpeta. Los valores por defecto sirven para correr todo en una sola máquina.

### `kernel/kernel.config`

| Clave                       | Default     | Descripción                                                      |
| --------------------------- | ----------- | ---------------------------------------------------------------- |
| `IP_MEMORIA`                | `127.0.0.1` | IP de la máquina donde corre la Memoria                          |
| `PUERTO_MEMORIA`            | `8002`      | Puerto de la Memoria                                             |
| `PUERTO_ESCUCHA_DISPATCH`   | `8001`      | Puerto donde el Kernel espera la conexión dispatch de la CPU     |
| `PUERTO_ESCUCHA_INTERRUPT`  | `8004`      | Puerto donde el Kernel espera la conexión interrupt de la CPU    |
| `PUERTO_ESCUCHA_IO`         | `8003`      | Puerto donde el Kernel espera a los IOs                          |
| `ALGORITMO_CORTO_PLAZO`     | `FIFO`      | `FIFO`, `SJF` o `SRT`                                            |
| `ALGORITMO_INGRESO_A_READY` | `FIFO`      | `FIFO` o `PMCP` (más chico primero)                              |
| `ALFA`                      | `1`         | Factor de la estimación de ráfaga (SJF/SRT)                      |
| `ESTIMACION_INICIAL`        | `10000`     | Estimación inicial de ráfaga en ms                               |
| `TIEMPO_SUSPENSION`         | `3000`      | ms que un proceso puede estar bloqueado antes de pasar a SWAP    |
| `LOG_LEVEL`                 | `TRACE`     | Nivel de log                                                     |

### `cpu/cpu.config`

| Clave                     | Default     | Descripción                                    |
| ------------------------- | ----------- | ---------------------------------------------- |
| `IP_MEMORIA`              | `127.0.0.1` | IP de la Memoria                               |
| `PUERTO_MEMORIA`          | `8002`      | Puerto de la Memoria                           |
| `IP_KERNEL`               | `127.0.0.1` | IP del Kernel                                  |
| `PUERTO_KERNEL_DISPATCH`  | `8001`      | Puerto dispatch del Kernel                     |
| `PUERTO_KERNEL_INTERRUPT` | `8004`      | Puerto interrupt del Kernel                    |
| `ENTRADAS_TLB`            | `0`         | Cantidad de entradas de la TLB (`0` = deshabilitada) |
| `REEMPLAZO_TLB`           | `FIFO`      | `FIFO` o `LRU`                                 |
| `ENTRADAS_CACHE`          | `2`         | Cantidad de entradas de la caché de páginas (`0` = deshabilitada) |
| `REEMPLAZO_CACHE`         | `CLOCK-M`   | `CLOCK` o `CLOCK-M`                            |
| `RETARDO_CACHE`           | `250`       | Retardo de acceso a caché en ms                |
| `LOG_LEVEL`               | `TRACE`     | Nivel de log                                   |

### `memoria/memoria.config`

| Clave                | Default                      | Descripción                                   |
| -------------------- | ---------------------------- | --------------------------------------------- |
| `PUERTO_ESCUCHA`     | `8002`                       | Puerto donde escucha la Memoria               |
| `TAM_MEMORIA`        | `2048`                       | Tamaño total de la memoria en bytes           |
| `TAM_PAGINA`         | `32`                         | Tamaño de página / marco en bytes             |
| `ENTRADAS_POR_TABLA` | `4`                          | Entradas por tabla de páginas                 |
| `CANTIDAD_NIVELES`   | `3`                          | Niveles de la tabla de páginas                |
| `RETARDO_MEMORIA`    | `500`                        | Retardo de acceso a memoria en ms             |
| `PATH_SWAPFILE`      | `/home/utnso/swapfile.bin`   | Archivo de SWAP                               |
| `RETARDO_SWAP`       | `5000`                       | Retardo de acceso a SWAP en ms                |
| `DUMP_PATH`          | `/home/utnso/dump_files`     | Carpeta donde se guardan los memory dumps     |
| `PATH_INSTRUCCIONES` | `/home/utnso/scripts/`       | Carpeta con los archivos de pseudocódigo (**con `/` al final**) |
| `LOG_LEVEL`          | `TRACE`                      | Nivel de log                                  |

### `io/io.config`

| Clave           | Default     | Descripción               |
| --------------- | ----------- | ------------------------- |
| `IP_KERNEL`     | `127.0.0.1` | IP del Kernel             |
| `PUERTO_KERNEL` | `8003`      | Puerto de IO del Kernel   |
| `LOG_LEVEL`     | `TRACE`     | Nivel de log              |

---

## 7. Correr en varias máquinas virtuales

Para simular un entorno distribuido (como en la entrega) podés poner cada módulo en una VM distinta:

1. Clonar la VM en VirtualBox (*clic derecho → Clonar*, eligiendo **"Generar nuevas direcciones MAC"**), o repetir los pasos 1–3 en cada una.
2. Poner todas en **Adaptador puente** (o en la misma **Red NAT**).
3. Ver la IP de cada VM con:

   ```bash
   ip a
   ```

4. Editar los `.config` con las IPs reales:
   - `kernel.config` → `IP_MEMORIA`
   - `cpu.config` → `IP_MEMORIA` e `IP_KERNEL`
   - `io.config` → `IP_KERNEL`
5. Verificar que se ven entre sí (`ping <ip>`) y levantar los módulos en el mismo orden de la [sección 5](#5-ejecutar-los-módulos).

---

## 8. Logs y resultados

Los logs se generan en la carpeta desde donde se ejecuta cada módulo:

| Módulo  | Archivos                                      |
| ------- | --------------------------------------------- |
| Kernel  | `kernel.log`, `kernelDebug.log`               |
| CPU     | `<identificador>.log` (ej. `CPU1.log`)        |
| Memoria | `memoria.log`                                 |
| IO      | `io.log`                                      |

Los memory dumps quedan en `DUMP_PATH` con el formato `<PID>-<hora>.dmp`.

---

## 9. Problemas comunes

| Error                                                | Solución                                                                                               |
| ---------------------------------------------------- | ------------------------------------------------------------------------------------------------------ |
| `cannot find -lcommons` / `commons/log.h: No such file` | No está instalada la so-commons-library. Ver [2.2](#22-so-commons-library).                          |
| `cannot find -lreadline`                             | `sudo apt install -y libreadline-dev`                                                                 |
| `Address already in use` al levantar un módulo       | Quedó un proceso viejo corriendo. Matalo con `pkill -f bin/kernel` (o `bin/memoria`, etc.) o esperá unos segundos. |
| El Kernel se cierra apenas arranca (`Segmentation fault`) | Faltan los argumentos: `./bin/kernel <archivo_pseudocodigo> <tamaño>`.                            |
| La CPU / el IO no se conectan                        | Revisá que la Memoria y el Kernel estén levantados **antes**, y que las IPs y puertos de los `.config` coincidan. |
| `Error al abrir el archivo: /home/utnso/scripts/...` | El archivo de pseudocódigo no existe en `PATH_INSTRUCCIONES` o el nombre no coincide con el pasado al Kernel / a `INIT_PROC`. |
| `Error al abrir archivo para dump`                   | No existe la carpeta `DUMP_PATH`: `mkdir -p /home/utnso/dump_files`.                                 |
| Un proceso queda bloqueado para siempre en IO        | No hay ningún módulo IO levantado con el nombre que usa la instrucción `IO`.                           |
| No pasa nada después de levantar todo                | Falta presionar **ENTER** en la terminal del Kernel.                                                   |

---

## Desarrollo en VS Code

Para abrir los 5 módulos como workspace (por ejemplo dentro de la VM vía Remote - SSH):

```bash
code tp.code-workspace
```

Guías útiles de la cátedra:

- [Cómo interpretar errores de compilación](https://docs.utnso.com.ar/primeros-pasos/primer-proyecto-c#errores-de-compilacion)
- [Cómo utilizar el debugger](https://docs.utnso.com.ar/guias/herramientas/debugger)
- [Cómo configuramos Visual Studio Code](https://docs.utnso.com.ar/guias/herramientas/code)
